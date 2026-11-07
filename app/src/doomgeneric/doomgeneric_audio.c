/*
 * dg_audio.c - software mixer + DMA for doomgeneric on STM32F7 (BSP audio out)
 */
#include "doomgeneric_audio.h"

#include <string.h>

#include "audio_app.h"
#include "stm32f7xx.h" /* CMSIS: PRIMASK, SCB D-cache ops */

#define MIX_CHUNK_FRAMES 128 /* stack use in the IRQ: ~1 KB acc + 0.5 KB music tmp */

typedef struct {
    const uint8_t   *pcm;          /* unsigned 8-bit mono, straight from the WAD lump */
    uint32_t         len;          /* samples */
    uint32_t         idx;          /* integer read position */
    uint32_t         frac;         /* 16-bit fractional part */
    uint32_t         step;         /* 16.16: src_rate / out_rate */
    int32_t          lgain, rgain; /* 0..255 (255 ~ unity) */
    volatile uint8_t active;       /* set by main thread, cleared by mixer at end of sample */
} sfx_voice_t;

static sfx_voice_t        g_voice[DG_AUDIO_MAX_SFX_CHANNELS];
static dg_music_render_fn g_music_fn;
static volatile int       g_music_vol = 127;

/* ------------------------------------------------------------------------- */

static inline uint32_t irq_lock(void) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return primask;
}

static inline void irq_unlock(uint32_t primask) {
    __set_PRIMASK(primask);
}

static inline int16_t clip16(int32_t v) {
    if(v > 32767)
        return 32767;
    if(v < -32768)
        return -32768;
    return (int16_t)v;
}

static void clean_dcache(void *p, uint32_t bytes) {
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    if(SCB->CCR & SCB_CCR_DC_Msk) {
        SCB_CleanDCache_by_Addr((uint32_t *)p, (int32_t)bytes);
    }
#else
    (void)p;
    (void)bytes;
#endif
}

/* Same panning law as Chocolate Doom's SDL port (Mix_SetPanning values 0..255). */
static void calc_gain(int vol, int sep, int32_t *l, int32_t *r) {
    int left  = ((254 - sep) * vol) / 127;
    int right = (sep * vol) / 127;

    *l = left < 0 ? 0 : (left > 255 ? 255 : left);
    *r = right < 0 ? 0 : (right > 255 ? 255 : right);
}

/* ------------------------------------------------------------------------- */
/* Main-thread API                                                           */
/* ------------------------------------------------------------------------- */

int dg_mix_sfx_start(int ch, const uint8_t *pcm, uint32_t len, uint32_t rate, int vol, int sep) {
    if(ch < 0 || ch >= DG_AUDIO_MAX_SFX_CHANNELS || pcm == NULL || len == 0 || rate == 0) {
        return -1;
    }

    sfx_voice_t *v = &g_voice[ch];
    int32_t      lg, rg;
    calc_gain(vol, sep, &lg, &rg);

    uint32_t p = irq_lock();
    v->active  = 0;
    v->pcm     = pcm;
    v->len     = len;
    v->idx     = 0;
    v->frac    = 0;
    v->step    = (uint32_t)(((uint64_t)rate << 16) / DG_AUDIO_RATE);
    v->lgain   = lg;
    v->rgain   = rg;
    v->active  = 1; /* last: mixer only looks at the voice once this is set */
    irq_unlock(p);

    return 0;
}

void dg_mix_sfx_params(int ch, int vol, int sep) {
    if(ch < 0 || ch >= DG_AUDIO_MAX_SFX_CHANNELS) {
        return;
    }
    int32_t lg, rg;
    calc_gain(vol, sep, &lg, &rg);

    uint32_t p        = irq_lock();
    g_voice[ch].lgain = lg;
    g_voice[ch].rgain = rg;
    irq_unlock(p);
}

void dg_mix_sfx_stop(int ch) {
    if(ch < 0 || ch >= DG_AUDIO_MAX_SFX_CHANNELS) {
        return;
    }
    uint32_t p         = irq_lock();
    g_voice[ch].active = 0;
    irq_unlock(p);
}

bool dg_mix_sfx_playing(int ch) {
    if(ch < 0 || ch >= DG_AUDIO_MAX_SFX_CHANNELS) {
        return false;
    }
    return g_voice[ch].active != 0;
}

void dg_mix_set_music(dg_music_render_fn fn) {
    uint32_t p = irq_lock();
    g_music_fn = fn;
    irq_unlock(p);
}

void dg_mix_set_music_volume(int vol) {
    g_music_vol = vol < 0 ? 0 : (vol > 127 ? 127 : vol);
}

/* ------------------------------------------------------------------------- */
/* Mixer (IRQ context)                                                       */
/* ------------------------------------------------------------------------- */

static void mix_music(int32_t *acc, uint32_t frames) {
    dg_music_render_fn fn  = g_music_fn;
    int                vol = g_music_vol;

    if(fn == NULL || vol == 0) {
        return;
    }

    int16_t tmp[MIX_CHUNK_FRAMES * 2];
    fn(tmp, frames);

    for(uint32_t i = 0; i < frames * 2; i++) {
        acc[i] += (tmp[i] * vol) >> 7;
    }
}

static void mix_sfx(int32_t *acc, uint32_t frames) {
    for(int c = 0; c < DG_AUDIO_MAX_SFX_CHANNELS; c++) {
        sfx_voice_t *v = &g_voice[c];
        if(!v->active) {
            continue;
        }

        const uint8_t *pcm = v->pcm;
        uint32_t       len = v->len, idx = v->idx, frac = v->frac, step = v->step;
        int32_t        lg = v->lgain, rg = v->rgain;
        uint32_t       f;

        for(f = 0; f < frames; f++) {
            if(idx >= len) {
                v->active = 0; /* finished: main thread releases the lump in Update() */
                break;
            }

            int32_t s0 = pcm[idx];
            int32_t s1 = (idx + 1 < len) ? pcm[idx + 1] : s0;

            /* linear interpolation, u8 -> s16: ((s0 + (s1-s0)*frac/65536) - 128) * 256 */
            int32_t s = (s0 << 8) + (((s1 - s0) * (int32_t)frac) >> 8) - 32768;

            acc[2 * f] += (s * lg) >> 8;
            acc[2 * f + 1] += (s * rg) >> 8;

            frac += step;
            idx += frac >> 16;
            frac &= 0xFFFFu;
        }

        v->idx  = idx;
        v->frac = frac;
    }
}

void DG_FillAudioBuffer(uint16_t *buf, uint32_t n_samples) {
    int16_t *out    = (int16_t *)buf;
    uint32_t frames = n_samples / 2;

    while(frames > 0) {
        uint32_t chunk = frames > MIX_CHUNK_FRAMES ? MIX_CHUNK_FRAMES : frames;
        int32_t  acc[MIX_CHUNK_FRAMES * 2];

        memset(acc, 0, chunk * 2 * sizeof(acc[0]));
        mix_music(acc, chunk);
        mix_sfx(acc, chunk);

        for(uint32_t i = 0; i < chunk * 2; i++) {
            out[i] = clip16(acc[i]);
        }

        out += chunk * 2;
        frames -= chunk;
    }

    /* The DMA reads straight from SRAM: push what the CPU just wrote out of the D-cache. */
    clean_dcache(buf, n_samples * sizeof(int16_t));
}

/* ------------------------------------------------------------------------- */

int DG_AudioStart(uint16_t *buf, uint32_t n_samples) {
    /* Each half must be a whole number of 32-byte cache lines: (n/2)*2 bytes % 32 == 0. */
    if(buf == NULL || n_samples < 64 || (n_samples % 32) != 0 || ((uintptr_t)buf % 32) != 0) {
        return -1;
    }

    /* Pre-fill both halves (silence until the first sound starts) so DMA never plays garbage. */
    DG_FillAudioBuffer(buf, n_samples / 2);
    DG_FillAudioBuffer(buf + n_samples / 2, n_samples / 2);

    /* BSP_AUDIO_OUT_Play() takes the size in BYTES and runs the SAI DMA in circular mode. */
    int res = audio_app_play(buf, n_samples * (uint32_t)sizeof(uint16_t));
    return (res == AUDIO_APP_OK) ? 0 : -2;
}
