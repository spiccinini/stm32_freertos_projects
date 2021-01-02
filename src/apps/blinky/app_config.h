#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#include <assert.h>

enum {
    MSI_CLOCK = 16000000,
    SYSTEM_PLL_PLLM = 1,
    SYSTEM_PLL_PLLN = 10,
    SYSTEM_PLL_PLLR = 2,
    SYSTEM_PLL_PLLP = 0,
    SYSTEM_PLL_PLLQ = 0,
    // 16MHz / PPLM * PLLN / PLLR = 16 / 1 * 10 / 2 = 80 MHz
    MAIN_PLL_CLOCK_HZ = MSI_CLOCK / SYSTEM_PLL_PLLM * SYSTEM_PLL_PLLN / SYSTEM_PLL_PLLR,
    SYSTEM_CORE_CLOCK_HZ = MAIN_PLL_CLOCK_HZ,
    SYSTICK_CLOCK_HZ = SYSTEM_CORE_CLOCK_HZ / 8
};


static_assert(SYSTEM_CORE_CLOCK_HZ == 80000000, "Configured clocks don't match");

#endif
