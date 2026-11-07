/*
 * dg_audio.h - doomgeneric audio back-end for STM32F7 (DMA ring buffer + software mixer)
 *
 * Data flow:
 *
 *   S_StartSound() -> I_StartSound() -> i_stm32sound.c -> dg_mix_sfx_start()
 *                                                              |
 *   BSP half/complete DMA IRQ -> DG_FillAudioBuffer() -> mixer (SFX + music) -> int16 stereo
 *                                                              |
 *   SCB_CleanDCache_by_Addr() -> SAI DMA (circular) <- audio_app_play(audio_buffer)
 */
#ifndef DG_AUDIO_H
#define DG_AUDIO_H

#include <stdint.h>
#include <stdbool.h>

#include "audio_app.h" /* DEFAULT_AUDIO_IN_FREQ, audio_app_play() */

/* Output sample rate of the SAI stream. Must match what audio_app_init() was given. */
#ifndef DG_AUDIO_RATE
#define DG_AUDIO_RATE DEFAULT_AUDIO_IN_FREQ
#endif

/* Doom's default is 8 channels (snd_channels); the SDL port allows up to 16. */
#define DG_AUDIO_MAX_SFX_CHANNELS 16

/*
 * Start circular DMA playback.
 *   buf       - the DMA buffer (32-byte aligned, NOT in a cacheable-and-unmanaged region,
 *               i.e. internal SRAM is fine as long as we clean the D-cache - done for you)
 *   n_samples - number of int16 values in buf (L,R,L,R...), multiple of 32
 * Call once from DG_Init() after audio_app_init().
 */
int DG_AudioStart(uint16_t *buf, uint32_t n_samples);

/*
 * Called from BSP_AUDIO_OUT_HalfTransfer_CallBack / TransferComplete_CallBack.
 * Fills n_samples int16 values (interleaved stereo) and cleans the D-cache for that range.
 * Runs in DMA IRQ context.
 */
void DG_FillAudioBuffer(uint16_t *buf, uint32_t n_samples);

/* ---- SFX voices (used by i_stm32sound.c, main-thread API) ---- */
int  dg_mix_sfx_start(int            ch,
                      const uint8_t *pcm_u8,
                      uint32_t       len,
                      uint32_t       rate_hz,
                      int            vol /*0..127*/,
                      int            sep /*0..254*/);
void dg_mix_sfx_params(int ch, int vol, int sep);
void dg_mix_sfx_stop(int ch);
bool dg_mix_sfx_playing(int ch);

/* ---- Music source (stage 2: OPL/MUS player plugs in here) ----
 * render() must write `frames` stereo int16 frames at DG_AUDIO_RATE. It is called from the
 * DMA IRQ, so keep it short or move the heavy synth to a lower-priority context.
 */
typedef void (*dg_music_render_fn)(int16_t *out_stereo, uint32_t frames);
void dg_mix_set_music(dg_music_render_fn fn);
void dg_mix_set_music_volume(int vol /*0..127*/);

#endif /* DG_AUDIO_H */
