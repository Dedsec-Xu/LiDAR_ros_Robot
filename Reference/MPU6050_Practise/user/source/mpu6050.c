#include "mpu6050.h"
#include "usart1.h"
#include "delay.h"
SIX_AXIS_DATA  six_axis;

#define delay_x_ms   Delay_ms
#define myPrintf printf


void Init_MPU6050(void)
{
	u8 pBuffer[6]={2};
	
	I2C_ByteWrite(MPU6050_ADDR,0x80, PWR_MGMT_1);//复位器件
	delay_x_ms(100);
	I2C_ByteWrite(MPU6050_ADDR,0x00, PWR_MGMT_1);//唤醒器件
	
	I2C_ByteWrite(MPU6050_ADDR,0x00, SMPLRT_DIV); //设置采样率8KHz/1KHz
	I2C_ByteWrite(MPU6050_ADDR,0x00, CONFIG);//数字低通滤波设置 256Hz  //约1ms的延迟，噪声小 8KHz
	
	I2C_ByteWrite(MPU6050_ADDR,0x00 , ACCEL_CONFIG);//Accel +/- 2g
	
#ifdef 	GYRO_250_DIV
	I2C_ByteWrite(MPU6050_ADDR,0x00 , GYRO_CONFIG); //+/- 250度/秒
#endif

#ifdef GYRO_500_DIV
	I2C_ByteWrite(MPU6050_ADDR,0x08 , GYRO_CONFIG);//+/- 500度/秒
#endif

#ifdef GYRO_1000_DIV
	I2C_ByteWrite(MPU6050_ADDR,0x10 , GYRO_CONFIG);//+/- 1000度/秒
#endif

#ifdef GYRO_2000_DIV
	I2C_ByteWrite(MPU6050_ADDR,0x18 , GYRO_CONFIG); //+/- 2000度/秒
#endif
	I2C_ByteWrite(MPU6050_ADDR,0x00, 0x38); //关闭所有中断
	I2C_ByteWrite(MPU6050_ADDR,0x00, 0x6A); //I2C主模式关闭
	I2C_ByteWrite(MPU6050_ADDR,0x00, 0x23); //关闭FIFO
	I2C_ByteWrite(MPU6050_ADDR,0x80, 0x37);	//INT引脚低电平有效
	
	I2C_BufferRead(MPU6050_ADDR, pBuffer, WHO_AM_I, 1);
	if(pBuffer[0]==MPU6050NO){
		
	I2C_ByteWrite(MPU6050_ADDR,0x01, 0x6b); //设置CLKSEL,PLL X轴为参考
	I2C_ByteWrite(MPU6050_ADDR,0x00, 0x6c); 	//加速度与陀螺仪都工作
		
	myPrintf("\r\n MPU6050 Init is ok!\r\n");
	}else {
	myPrintf("\r\n MPU6050 Init is fail!\r\n");
	}

	myPrintf("\r\n MPU6050 Config ok!\r\n");

}

int16_t ReadAccel_X(void)
{
	uint8_t Buffer[2];
	
	I2C_BufferRead(MPU6050_ADDR, Buffer, ACCEL_XOUT_H,2);
	
	return (int16_t)(Buffer[0]<<8 | Buffer[1]);

}

int16_t ReadAccel_Y(void)
{
	uint8_t Buffer[2];
	
	I2C_BufferRead(MPU6050_ADDR, Buffer, ACCEL_YOUT_H,2);
	
	return (int16_t)(Buffer[0]<<8 | Buffer[1]);

}

int16_t ReadAccel_Z(void)
{
	uint8_t Buffer[2];
	
	I2C_BufferRead(MPU6050_ADDR, Buffer, ACCEL_ZOUT_H,2);
	
	return (int16_t)(Buffer[0]<<8 | Buffer[1]);

}

int16_t ReadGyro_X(void)
{
	uint8_t Buffer[2];
	
	I2C_BufferRead(MPU6050_ADDR, Buffer, GYRO_XOUT_H,2);
	
	return (int16_t)(Buffer[0]<<8 | Buffer[1]);

}

int16_t ReadGyro_Y(void)
{
	uint8_t Buffer[2];
	
	I2C_BufferRead(MPU6050_ADDR, Buffer, GYRO_YOUT_H,2);
	
	return (int16_t)(Buffer[0]<<8 | Buffer[1]);

}

int16_t ReadGyro_Z(void)
{
	uint8_t Buffer[2];
	
	I2C_BufferRead(MPU6050_ADDR, Buffer, GYRO_ZOUT_H,2);
	
	return (int16_t)(Buffer[0]<<8 | Buffer[1]);

}

float ReadTemperature(void)
{
	uint8_t Buffer[2];
	int16_t temp;
	
	I2C_BufferRead(MPU6050_ADDR, Buffer, TEMP_OUT_H,2);
	temp=(Buffer[0]<<8 | Buffer[1]);
	
	return (float)(((float)temp)/340.0f + 36.53f);

}


void get_Accel_Gyro_Temp(SIX_AXIS_DATA *six_axis)
{

	six_axis->accel_x=ReadAccel_X();
	six_axis->accel_y=ReadAccel_Y();
	six_axis->accel_z=ReadAccel_Z();
	
	six_axis->gyro_x=ReadGyro_X();
	six_axis->gyro_y=ReadGyro_Y();
	six_axis->gyro_z=ReadGyro_Z();
	
	six_axis->Temperature=ReadTemperature();
}

void get_Accel(SIX_AXIS_DATA *six_axis)
{
	six_axis->accel_x=ReadAccel_X();
	six_axis->accel_y=ReadAccel_Y();
	six_axis->accel_z=ReadAccel_Z();
}

void get_Gyro(SIX_AXIS_DATA *six_axis)
{
	six_axis->gyro_x=ReadGyro_X();
	six_axis->gyro_y=ReadGyro_Y();
	six_axis->gyro_z=ReadGyro_Z();
}

void get_Temp(SIX_AXIS_DATA *six_axis)
{
	six_axis->Temperature=ReadTemperature();
}


uint8_t MPU6xxx_Read_Len(u8 DEV_No,u8 ReadAddr,u16 NumByteToRead,u8* pBuffer)
{
	I2C_BufferRead( DEV_No,  pBuffer,  ReadAddr,  NumByteToRead);
	return 0;
}


u8 MPU6xxx_Write_Len(u8 DEV_No,u8 WriteAddr,u8 NumByteToWrite,u8 *pBuffer)
{
	I2C_BufferWrite( DEV_No, WriteAddr, NumByteToWrite, pBuffer);

	return 0;
}

