#include "led.h"

void LED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. B?t Clock cho Port C
    RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);

    // 2. C?u hình chân PC13 là Output Push-Pull
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    // 3. M?c d?nh ban d?u là t?t LED
    LED_Off();
}

void LED_On(void) {
    GPIO_ResetBits(LED_PORT, LED_PIN); // M?c th?p = Sáng
}

void LED_Off(void) {
    GPIO_SetBits(LED_PORT, LED_PIN);   // M?c cao = T?t
}

void LED_Toggle(void) {
    // Ð?o tr?ng thái chân LED
    if(GPIO_ReadOutputDataBit(LED_PORT, LED_PIN)) {
        LED_On();
    } else {
        LED_Off();
    }
}