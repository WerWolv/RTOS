#include <hw/stm32f7/uart.hpp>

#include <hw/stm32f7/stm32f769xx.h>

namespace hw::uart {

    // UART4_RX: PD0
    // UART4_TX: PH13

    void init() {

        // Enable UART4 Clock
        RCC->APB1ENR |= RCC_APB1ENR_UART4EN;

        // Use System clock as UART4 Clock
        RCC->DCKCFGR2 |= RCC_DCKCFGR2_UART4SEL_0;

        // Enable GPIOD and GPIOH Clock
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;

        // Put UART4_RX on PD0
        GPIOD->MODER |= (0b10 << GPIO_MODER_MODER0_Pos);
        GPIOD->AFR[0] |= (0b1000 << GPIO_AFRL_AFRL0_Pos);

        // Put UART4_TX on PH13
        GPIOH->MODER |= (0b10 << GPIO_MODER_MODER13_Pos);
        GPIOH->AFR[1] |= (0b1000 << GPIO_AFRH_AFRH5_Pos);

        UART4->CR1 |= USART_CR1_TE;
        
        u16 div = 16000000 / 115200;
        UART4->BRR = (((div / 16) << USART_BRR_DIV_MANTISSA_Pos) | ((div % 16) << USART_BRR_DIV_FRACTION_Pos));

        UART4->CR1 |= USART_CR1_UE;
    }

    void write(char c) {
        while (!(UART4->ISR & USART_ISR_TXE));

        UART4->TDR = c;
    }

    void write(const char *string) {
        while (*string != 0x00) {
            write(*string);
            string++;
        }
    }

}