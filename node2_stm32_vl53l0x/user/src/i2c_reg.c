#include "i2c_reg.h"

void I2C1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;   // Clock Port B
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;   // Clock I2C1
    
    // PB6, PB7: Alternate Function Open-Drain
    GPIOB->CRL &= ~0xFF000000;
    GPIOB->CRL |=  0xEE000000; 

    I2C1->CR2 = 36;          // T?n s? PCLK1 (36MHz)
    I2C1->CCR = 180;         // T?c d? 100kHz
    I2C1->TRISE = 37;        // Th?i gian tang t?i da
    I2C1->CR1 |= I2C_CR1_PE; // Enable I2C
}

void I2C1_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t data) {
    while (I2C1->SR2 & I2C_SR2_BUSY);      // Ch? bus r?nh
    I2C1->CR1 |= I2C_CR1_START;            // T?o Start
    while (!(I2C1->SR1 & I2C_SR1_SB));
    
    I2C1->DR = slave_addr << 1;            // G?i d?a ch? Slave (Write)
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;                       // Clear ADDR flag
    
    I2C1->DR = reg_addr;                   // G?i d?a ch? thanh ghi
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    
    I2C1->DR = data;                       // G?i d? li?u
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    
    I2C1->CR1 |= I2C_CR1_STOP;             // T?o Stop
}

uint8_t I2C1_ReadByte(uint8_t slave_addr, uint8_t reg_addr) {
    uint32_t timeout = 100000;
    uint8_t data = 0;

    // 1. T?o di?u ki?n Start
    I2C1->CR1 |= (1 << 8); 
    while (!(I2C1->SR1 & (1 << 0)) && --timeout);
    if (timeout == 0) return 0xFF; // Thoát n?u l?i

    // 2. G?i d?a ch? ghi (d? ch?n thanh ghi)
    I2C1->DR = (slave_addr << 1);
    timeout = 100000;
    while (!(I2C1->SR1 & (1 << 1)) && --timeout);
    (void)I2C1->SR2;

    // 3. G?i d?a ch? thanh ghi n?i b?
    I2C1->DR = reg_addr;
    timeout = 100000;
    while (!(I2C1->SR1 & (1 << 7)) && --timeout);

    // 4. T?o Restart d? d?c
    I2C1->CR1 |= (1 << 8);
    timeout = 100000;
    while (!(I2C1->SR1 & (1 << 0)) && --timeout);

    // 5. G?i d?a ch? d?c
    I2C1->DR = (slave_addr << 1) | 1;
    timeout = 100000;
    while (!(I2C1->SR1 & (1 << 1)) && --timeout);
    (void)I2C1->SR2;

    // 6. Nh?n d? li?u và k?t thúc
    I2C1->CR1 &= ~(1 << 10); // NACK
    I2C1->CR1 |= (1 << 9);  // STOP
    timeout = 100000;
    while (!(I2C1->SR1 & (1 << 6)) && --timeout);
    data = I2C1->DR;

    return data;
}