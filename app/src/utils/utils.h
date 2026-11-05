#pragma once

#define __SDRAM_MEMORY__ __attribute__((section(".sdram_data")))
#define __ALIGNED_32__   __attribute__((aligned(32)))

uint32_t utils_get_ticks_ms(void);
void utils_sleep_ms(uint32_t ms);
