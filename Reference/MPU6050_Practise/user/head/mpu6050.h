#ifndef _MPU6050_H_
#define _MPU6050_H_
#include "stm32f10x.h"
#include "I2C.H"


#define MPU6050_ADDR 0XD0 
#define WHO_AM_I 		 0x75

#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define FIFO_EN      0x23
#define INT_PIN_CFG  0x37
#define INT_ENABLE   0x38

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48


#define SIGNAL_PATH_RESET 0x68 //RESET
#define USER_CTRL    0x6A      //RESET
#define	PWR_MGMT_1   0x6B    //电源管理，0x00典型值 ，正常使用

#define FIFO_COUNTH  0x72
#define FIFO_COUNTL  0x73
#define FIFO_R_W     0x74

#define MPU6050NO	 0x68


//#define GYRO_250_DIV
//#define GYRO_500_DIV
//#define GYRO_1000_DIV
#define GYRO_2000_DIV


typedef struct accel_gyro {

	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
	
	float Temperature;
	
}SIX_AXIS_DATA;

extern SIX_AXIS_DATA  six_axis;

void Init_MPU6050(void);

int16_t ReadAccel_X(void);
int16_t ReadAccel_Y(void);
int16_t ReadAccel_Z(void);

int16_t ReadGyro_X(void);
int16_t ReadGyro_Y(void);
int16_t ReadGyro_Z(void);

float ReadTemperature(void);

void get_Accel_Gyro_Temp(SIX_AXIS_DATA *six_axis);
void get_Accel(SIX_AXIS_DATA *six_axis);
void get_Gyro(SIX_AXIS_DATA *six_axis);
void get_Temp(SIX_AXIS_DATA *six_axis);


u8 MPU6xxx_Write_Len(u8 DEV_No,u8 WriteAddr,u8 NumByteToWrite,u8 *pBuffer);
uint8_t MPU6xxx_Read_Len(u8 DEV_No,u8 ReadAddr,u16 NumByteToRead,u8* pBuffer);
#endif

