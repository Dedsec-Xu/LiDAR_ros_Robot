#ifndef _I2C_H_
#define _I2C_H_

#include "stm32f10x.h"
#include "mpu6050.h"


#define I2C_Speed              200000
#define I2C1_OWN_ADDRESS7    0x0A

void bsp_I2c_Init(void);

void I2C_ByteWrite(u8 DEV_No,u8 data, u8 WriteAddr);
void I2C_BufferRead(u8 DEV_No, u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
u8 I2C_BufferWrite(u8 DEV_No,u8 WriteAddr,u8 NumByteToWrite,u8 *pBuffer);

#endif
