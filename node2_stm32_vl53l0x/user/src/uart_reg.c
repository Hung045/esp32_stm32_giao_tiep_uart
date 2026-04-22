#include "uart_reg.h"
#include <string.h>

// Ð?nh nghia bi?n
char rx_buffer[128];
uint8_t rx_index = 0;
uint8_t rx_flag = 0;

// --- C?u hình UART 1 (PA9-TX, PA10-RX) ---
void UART1_Init(uint32_t baudrate) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
    GPIOA->CRH &= ~((0xF << 4) | (0xF << 8)); 
    GPIOA->CRH |= (0xB << 4) | (0x4 << 8); // PA9: AF PP, PA10: Float Input
    
    // Baudrate 115200 (72MHz): 72M/(16*115200) = 39.0625 -> 39(0x27), 0.0625*16=1(0x1)
    USART1->BRR = 0x271; 
    
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART1_IRQn);
}

void UART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

void UART1_SendString(char *str) {
    while (*str) UART1_SendChar(*str++);
}

// --- C?u hình UART 2 (PA2-TX, PA3-RX) ---
void UART2_Init(uint32_t baudrate) {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // UART2 n?m trên APB1
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    
    GPIOA->CRL &= ~((0xF << 8) | (0xF << 12)); 
    GPIOA->CRL |= (0xB << 8) | (0x4 << 12); // PA2: AF PP, PA3: Float Input

    // Baudrate 115200 (36MHz): 36M/(16*115200) = 19.53125 -> 19(0x13), 0.53125*16=8.5(~9: 0x9)
    USART2->BRR = 0x139; 

    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void UART2_SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

void UART2_SendString(char *str) {
    while (*str) UART2_SendChar(*str++);
}

// --- Ng?t nh?n UART 1 ---
void USART1_IRQHandler(void) {
    if (USART1->SR & USART_SR_RXNE) {
        char c = (char)(USART1->DR & 0xFF);
        if (c == '\n' || c == '\r') {
            if (rx_index > 0) {
                rx_buffer[rx_index] = '\0';
                rx_flag = 1;
                rx_index = 0;
            }
        } else {
            if (rx_index < 127) {
                rx_buffer[rx_index++] = c;
            }
        }
    }
}
