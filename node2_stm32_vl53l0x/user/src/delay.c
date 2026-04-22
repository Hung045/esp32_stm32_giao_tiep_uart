#include "delay.h"
void Delay_Init(void) {
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    // C?u hình Systick ng?t m?i 1ms (72MHz / 1000)
    SysTick->LOAD = 72000 - 1; 
    SysTick->VAL = 0;
}
void Delay_ms(uint32_t ms) {
    uint32_t i;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    for (i = 0; i < ms; i++) {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}