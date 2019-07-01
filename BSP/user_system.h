#ifndef __USER_SYSTEM_H_
#define __USER_SYSTEM_H_

#include "stm32h7xx_hal.h"
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>


#include "user_protcol.h"
#include "user_comm.h"
#include "driver_motor.h"
#include "user_robot_base_2.h"
#include "user_gpio_i2c.h"

#define MPU6050
#define EMPL_TARGET_STM32F4
#include "imu_mpu6050.h"
//#include "imu.h"
#include "DMP/imu_dmp.h"
#define SYSTEM_VER 0x1708 //maybe need to change
#define PI 3.1415926535898
#define DEBUG

extern uint8_t cmd_flag;
extern float cmd_liner_vel_x;
extern float cmd_liner_vel_y;
extern float cmd_angular_rad_z;

extern float set_p;
extern float set_i;
extern float set_d;

void user_test();
void user_system_init();
void user_system_thread_0();

void user_delay_us(uint32_t us);

void user_delay_ms_start(uint32_t *tick);
void user_delay_ms_end(uint32_t *tick,uint32_t Delay);




#endif /*__USER_SYSTEM_H_*/
