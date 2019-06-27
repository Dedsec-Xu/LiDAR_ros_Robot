#include "user_gpio_i2c.h"
GPIO_TypeDef *gpio_i2c_scl_gpiox;
uint16_t gpio_i2c_scl_pin;
GPIO_TypeDef* gpio_i2c_sda_gpiox;
uint16_t gpio_i2c_sda_pin;

#define IIC_SCL_LOW     (gpio_i2c_scl_gpiox->BRR = gpio_i2c_scl_pin)
#define IIC_SCL_HIGH    (gpio_i2c_scl_gpiox->BSRR = gpio_i2c_scl_pin)
#define IIC_SDA_LOW     (gpio_i2c_sda_gpiox->BRR = gpio_i2c_sda_pin)
#define IIC_SDA_HIGH    (gpio_i2c_sda_gpiox->BSRR = gpio_i2c_sda_pin)

#define READ_SDA        (gpio_i2c_sda_gpiox->IDR & gpio_i2c_sda_pin)

#define SDA_IN() {SDA_GPIO_Port->CRH &= 0XFFFF0FFF; SDA_GPIO_Port->CRH|=8<<12;}

#define SDA_OUT() {SCL_GPIO_Port->CRH &= 0XFFFF0FFF; SCL_GPIO_Port->CRH|=3<<12;}


void IIC_Init()
{
    gpio_i2c_scl_gpiox = SCL_GPIO_Port;
    gpio_i2c_scl_pin = SCL_Pin;
    gpio_i2c_sda_gpiox = SDA_GPIO_Port;
    gpio_i2c_sda_pin = SDA_Pin;
		
}

void delay_us(uint32_t us)
{
    while(us--)
    {
        __nop();
    }
}


int IIC_Start(void){
    SDA_OUT();
    IIC_SDA_HIGH;
    if(!READ_SDA) return 0;
    IIC_SCL_HIGH;
    delay_us(1);
    IIC_SDA_LOW;
    if(READ_SDA) return 0;
    delay_us(1);
    IIC_SCL_LOW;
    return 1;
}

void IIC_Stop(void){
    SDA_OUT();
    IIC_SCL_LOW;
    IIC_SDA_LOW;
    delay_us(1);
    IIC_SCL_HIGH;
    IIC_SDA_HIGH;
    delay_us(1);
}

int IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA_IN();
    IIC_SDA_HIGH;
    delay_us(1);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>50)
        {
            IIC_Stop();
            return 0;
        }
        delay_us(1);
    }
    IIC_SCL_LOW;
    return 1;
}

void IIC_Ack(void)
{
    IIC_SCL_LOW;
    SDA_OUT();
    IIC_SDA_LOW;
    delay_us(1);
    IIC_SCL_HIGH;
    delay_us(1);
    IIC_SCL_LOW;
}
