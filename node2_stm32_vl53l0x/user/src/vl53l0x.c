#include "vl53l0x.h"
#include "i2c_reg.h"
#include "delay.h"

void VL53L0X_Init_Full(void) {
    // Các l?nh dánh th?c Laser
    I2C1_Write(VL53L0X_ADDR, 0x88, 0x00);
    I2C1_Write(VL53L0X_ADDR, 0x80, 0x01);
    I2C1_Write(VL53L0X_ADDR, 0xFF, 0x01);
    I2C1_Write(VL53L0X_ADDR, 0x00, 0x00);
    
    // Ð?c và ghi l?i d? c?u hình di?n áp
    uint8_t temp = I2C1_ReadByte(VL53L0X_ADDR, 0x91);
    I2C1_Write(VL53L0X_ADDR, 0x91, temp);
    
    I2C1_Write(VL53L0X_ADDR, 0x00, 0x01);
    I2C1_Write(VL53L0X_ADDR, 0xFF, 0x00);
    I2C1_Write(VL53L0X_ADDR, 0x80, 0x00);

    // Kích ho?t ch? d? do liên t?c
    I2C1_Write(VL53L0X_ADDR, 0x00, 0x02); 
    
    Delay_ms(100);
}

uint16_t VL53L0X_GetDist(void) {
    uint16_t dist = 0;
    uint32_t timeout = 10000;
    uint8_t h, l;

    // Ð?i d? li?u Ready (bit 0 c?a thanh ghi 0x13)
    while (!(I2C1_ReadByte(VL53L0X_ADDR, 0x13) & 0x01) && --timeout);

    // Ð?c byte cao và byte th?p
    h = I2C1_ReadByte(VL53L0X_ADDR, 0x1E);
    l = I2C1_ReadByte(VL53L0X_ADDR, 0x1F);
    
    dist = (uint16_t)((h << 8) | l);

    // Xóa ng?t d? c?m bi?n do ti?p
    I2C1_Write(VL53L0X_ADDR, 0x0B, 0x01); 
    
    return dist;
}