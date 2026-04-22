#ifndef __UART_REG_H
#define __UART_REG_H

#include "stm32f10x.h"

// Khai báo bi?n dùng chung (extern) d? main.c có th? truy c?p
extern char rx_buffer[128];
extern uint8_t rx_flag;

// Các hàm cho UART1 (K?t n?i ESP32 - Chân PA9, PA10)
void UART1_Init(uint32_t baudrate);
void UART1_SendChar(char c);
void UART1_SendString(char *str);

// Các hàm cho UART2 (K?t n?i Hercules - Chân PA2, PA3)
void UART2_Init(uint32_t baudrate);
void UART2_SendChar(char c);
void UART2_SendString(char *str);

#endif
