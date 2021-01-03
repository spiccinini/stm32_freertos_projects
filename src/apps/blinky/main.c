#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>

#include <app_config.h>

#define LED_GREEN_PORT GPIOB
#define LED_GREEN_PIN GPIO13
#define USART_CONSOLE USART2

static void clock_setup(void) {
    /* FIXME - this should eventually become a clock struct helper setup */
    rcc_osc_on(RCC_HSI16);
    rcc_osc_on(RCC_HSI48);
    rcc_set_clock48_source(RCC_CCIPR_CLK48SEL_HSI48);
    
    flash_prefetch_enable();
    flash_set_ws(4);
    flash_dcache_enable();
    flash_icache_enable();

    scb_set_priority_grouping(SCB_AIRCR_PRIGROUP_GROUP16_NOSUB);

    rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, SYSTEM_PLL_PLLM, SYSTEM_PLL_PLLN, SYSTEM_PLL_PLLP,
                         SYSTEM_PLL_PLLQ, RCC_PLLCFGR_PLLR_DIV2);
    static_assert(SYSTEM_PLL_PLLR == 2, "RCC_PLLCFGR_PLLR_DIV2 in the line above.");

    rcc_osc_on(RCC_PLL);
    /* either rcc_wait_for_osc_ready() or do other things */

    /* Enable clocks for the ports we need */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Enable clocks for peripherals we need */
    rcc_periph_clock_enable(RCC_USART2);
    //rcc_periph_clock_enable(RCC_TIM7);
    rcc_periph_clock_enable(RCC_SYSCFG);

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    rcc_set_sysclk_source(RCC_CFGR_SW_PLL); /* careful with the param here! */
    rcc_wait_for_sysclk_status(RCC_PLL);
    /* FIXME - eventually handled internally */
    rcc_ahb_frequency = SYSTEM_CORE_CLOCK_HZ;
    rcc_apb1_frequency = SYSTEM_CORE_CLOCK_HZ;
    rcc_apb2_frequency = SYSTEM_CORE_CLOCK_HZ;
}

static void usart2_setup(void) {
    /* USART2 GPIO Configuration
    PA0   ------> USART2_CTS
    PA1   ------> USART2_RTS
    PA2   ------> USART2_TX
    PA3   ------> USART2_RX
    */
    // Setup USART2 TX pin as alternate function.
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2|GPIO3);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2|GPIO3);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO2);

    usart_set_baudrate(USART_CONSOLE, 230400);
    usart_set_databits(USART_CONSOLE, 8);
    usart_set_stopbits(USART_CONSOLE, USART_STOPBITS_1);
    usart_set_mode(USART_CONSOLE, USART_MODE_TX_RX);
    usart_set_parity(USART_CONSOLE, USART_PARITY_NONE);
    usart_set_flow_control(USART_CONSOLE, USART_FLOWCONTROL_NONE);

    // It is essential that interrupt handlers that make use of the FreeRTOS API have a logical
    // priority equal to or below (with a bigger number) that set by the configMAX_SYSCALL_INTERRUPT_PRIORITY.
    // Here we use minimal logical priority
    nvic_set_priority(NVIC_USART2_IRQ, configKERNEL_INTERRUPT_PRIORITY);
    nvic_enable_irq(NVIC_USART2_IRQ);
    usart_enable(USART_CONSOLE);
}

int k = 0;

void task_blink(void *args __attribute__((unused))) {
    while (1) {
        gpio_toggle(LED_GREEN_PORT, LED_GREEN_PIN);
        usart_send_blocking(USART_CONSOLE, 'x');
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void vApplicationStackOverflowHook( __attribute__((unused)) TaskHandle_t xTask,
                                    __attribute__((unused)) char * pcTaskName ) {
  for ( ;; ) {
    gpio_toggle(GPIOB, GPIO13);
    vTaskDelay(pdMS_TO_TICKS( 100 ));
  }
}

int main(void) {
    clock_setup();

    gpio_mode_setup(LED_GREEN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GREEN_PIN);
    gpio_set(LED_GREEN_PORT, LED_GREEN_PIN); // Turn off

    usart2_setup();

    xTaskCreate(task_blink, "blink", 100, NULL, PRIORITY_LOW, NULL);
    vTaskStartScheduler();

    while (1);
    return 0;
}
