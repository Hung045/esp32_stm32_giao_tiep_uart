#ifndef VL53L0X_H
#define VL53L0X_H

#include "stm32f10x.h"

// Ð?a ch? I2C m?c d?nh c?a VL53L0X
#define VL53L0X_ADDR 0x29

// Các hàm giao ti?p v?i c?m bi?n
void VL53L0X_Init_Full(void);
uint16_t VL53L0X_GetDist(void);

#endif
