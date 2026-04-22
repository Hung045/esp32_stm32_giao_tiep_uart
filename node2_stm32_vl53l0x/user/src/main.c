#include "uart_reg.h"
#include "i2c_reg.h"
#include "vl53l0x.h"
#include "delay.h"
#include "led.h"
#include <stdio.h>
#include <string.h>

// Bien quan sat Debug KeilC
volatile uint16_t current_dist = 0; 
// Bien dem thoi gian tu luc khoi dong
uint32_t total_ms = 0; 

int main(void) {
    char log_buffer[200];    // Buffer in log len Hercules (UART2)
    char reply_buffer[100];  // Buffer phan hoi sang ESP32 (UART1)
    
    // Khoi tao cac module
    Delay_Init();
    UART1_Init(115200); // Ket noi ESP32
    UART2_Init(9600);   // Ket noi Hercules
    I2C1_Init();
    VL53L0X_Init_Full();
    LED_Init();

    UART2_SendString("\r\nSTM32 (NODE 2) SAN SANG\r\n");

    while (1) {
        // 1. Doc cam bien VL53L0X
        uint16_t temp = VL53L0X_GetDist();
        if (temp < 8000) current_dist = temp;

        // 2. Dieu khien LED PC13
        if (current_dist > 0 && current_dist < 250) LED_On();
        else LED_Off();

        // Tinh thoi gian thuc [Phut:Giay]
        uint32_t sec = total_ms / 1000;
        uint32_t mn = sec / 60;
				uint32_t hour = mn / 60;
        sec %= 60;
				hour %= 60;
			
			  // 3. In Log len Hercules qua UART2
        sprintf(log_buffer, "[%02d:%02d:%02d] [STM32 LOG] Khoang cach do duoc: %d mm\r\n",hour, mn, sec, current_dist);
        UART2_SendString(log_buffer);

        // 3. Kiem tra du lieu tu ESP32 (Node 1)
        if (rx_flag == 1) {
            // In log nhan tin kem Timestamp ra Hercules
            sprintf(log_buffer, "[ESP32 LOG] NHAN: %s\r\n", rx_buffer);
            UART2_SendString(log_buffer);

            // 4. Tao va gui phan hoi sang Node 1
            memset(reply_buffer, 0, sizeof(reply_buffer));
            sprintf(reply_buffer, "Phan hoi tu node 2 ve node 1 : %d mm\n", current_dist);
            UART1_SendString(reply_buffer);

            // In log xac nhan da phan hoi
            sprintf(log_buffer, "[STM32 LOG] DA PHAN HOI SANG NODE 1\r\n\r\n");
            UART2_SendString(log_buffer);

            rx_flag = 0; // Xoa co nhan
        }

        Delay_ms(20); 
        total_ms += 20;
    }
}