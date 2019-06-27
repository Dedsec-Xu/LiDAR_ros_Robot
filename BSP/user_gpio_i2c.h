#ifndef __USER_GPIO_I2C_H_
#define __USER_GPIO_I2C_H_
#include "stm32h7xx_hal.h"
#include "main.h"

void IIC_Init(void);

int IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(uint8_t ack);
int IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

uint8_t IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data);
uint8_t IICWriteByte(uint8_t dev, uint8_t reg, uint8_t data);
uint8_t I2C_ReadOneByte(uint8_t I2C_Addr, uint8_t addr);
uint8_t IICWriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data);

uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t IICwriteBits(uint8_t dev, uint8_t reg, uint8_t bigStart, uint8_t length, uint8_t data);
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);

#endif