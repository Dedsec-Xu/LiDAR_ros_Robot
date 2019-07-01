/**********模板**********
 编写：
 硬件平台：
 功能概述：
 日期：
 修改记录：
 **********    **********/
//--头文件包含--//
#include "stm32f10x.h"
#include "i2c.h"
#include "usart1.h"
#include "inv_mpu.h"
#include "delay.h"

//--宏定义--//
//--全局变量声明--//
//--函数预定义--//
//--主函数--//
int main (void)
{
	
	RCC_Configuration();
	Delay_Init();
	USART1_Config();
	bsp_I2c_Init();
	Init_MPU6050();
	while(mpu_dmp_init())
 	{
		printf("mpu_dmp_init Exeing\r\n");
	}
	printf("mpu_dmp_init Exeing ok\r\n");
	Delay_ms(1000);
	mpu_EXTI_Configuration();
	while(1)
	{	
		;
	}
	return 0;
}	
//--函数定义--//
/*******************************************************************************
* Function Name  : None
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
