#include "delay.h"

void Delay_Init (void)
{
	SystemInit();
	SysTick->CTRL &= ~BIT(0);      //ʧ�ܶ�ʱ��
  SysTick->CTRL &= ~BIT(1);			 //ʧ���ж�
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

void Delay_us (u32 time_us)
{
	if (SysTick_Config(time_us*9))	//װ��LOADֵ 
	{ 
		/* Capture error */ 
		while (1);
	}
	SysTick->CTRL |= BIT(0);
  while(!(SysTick->CTRL&BIT(16))); //�ȴ�ʱ�䵽�� 
	SysTick->CTRL &= ~BIT(0);      //ʧ�ܶ�ʱ��
  SysTick->VAL=0X00000000;         //��ռ�����        
}

void Delay_ms(u32 time_ms)
{
	while(time_ms--)
	{
		Delay_us(1000);
	}
}

void RCC_Configuration (void)
{
	//----------ʹ���ⲿRC����-----------//
	RCC_DeInit();			//��ʼ��Ϊȱʡֵ
	RCC_HSEConfig(RCC_HSE_ON);	//ʹ���ⲿ�ĸ���ʱ�� 
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);	//�ȴ��ⲿ����ʱ��ʹ�ܾ���
	
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//Enable Prefetch Buffer
	FLASH_SetLatency(FLASH_Latency_2);		//Flash 2 wait state
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);		//HCLK = SYSCLK
	RCC_PCLK2Config(RCC_HCLK_Div1);			//PCLK2 =  HCLK
	RCC_PCLK1Config(RCC_HCLK_Div2);			//PCLK1 = HCLK/2
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);	//PLLCLK = 8MHZ * 9 =72MHZ
	RCC_PLLCmd(ENABLE);			//Enable PLLCLK

	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		;//Wait till PLLCLK is ready
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//Select PLL as system clock
	while(RCC_GetSYSCLKSource()!=0x08);		//Wait till PLL is used as system clock source
	RCC_ClockSecuritySystemCmd (ENABLE);//����ʱ�Ӽ�����
	
	//---------����Ӧ����ʱ��--------------------//
}
