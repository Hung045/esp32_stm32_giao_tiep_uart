#ifndef I2C_REG_H
#define I2C_REG_H
#include "stm32f10x.h"

void I2C1_Init(void);
void I2C1_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t data);
uint8_t I2C1_ReadByte(uint8_t slave_addr, uint8_t reg_addr);

#endif
