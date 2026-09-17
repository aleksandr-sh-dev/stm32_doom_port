#pragma once

#define __SDRAM_MEMORY__ __attribute__((section(".sdram_data")))
#define __ALIGNED_32__   __attribute__((aligned(32)))
