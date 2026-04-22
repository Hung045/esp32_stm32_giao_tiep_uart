#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

// Ð?nh nghia chân LED d? d? thay d?i n?u sau này dùng board khác
#define LED_PORT   GPIOC
#define LED_PIN    GPIO_Pin_13
#define LED_RCC    RCC_APB2Periph_GPIOC

void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

#endif
