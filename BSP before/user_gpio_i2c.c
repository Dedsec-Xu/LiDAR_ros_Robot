#include "user_gpio_i2c.h"

GPIO_TypeDef *gpio_i2c_scl_gpiox;
uint16_t gpio_i2c_scl_pin;
GPIO_TypeDef *gpio_i2c_sda_gpiox;
uint16_t gpio_i2c_sda_pin;

//#define IIC_SCL_LOW     (gpio_i2c_scl_gpiox->BRR = gpio_i2c_scl_pin) 
#define IIC_SCL_LOW     (gpio_i2c_scl_gpiox->BSRR = gpio_i2c_scl_pin<<16)//xjb cai de 
#define IIC_SCL_HIGH    (gpio_i2c_scl_gpiox->BSRR = gpio_i2c_scl_pin)
#define IIC_SDA_LOW     (gpio_i2c_sda_gpiox->BSRR = gpio_i2c_sda_pin<<16)
#define IIC_SDA_HIGH    (gpio_i2c_sda_gpiox->BSRR = gpio_i2c_sda_pin)


#define READ_SDA        (gpio_i2c_sda_gpiox->IDR &gpio_i2c_sda_pin)


// #define SDA_IN() {SDA_GPIO_Port->CRH &= 0XFFFF0FFF;SDA_GPIO_Port->OTYPER |= 8 << 12;}//定义为输入模式


// #define SDA_OUT() {SCL_GPIO_Port->CRH &= 0XFFFF0FFF;SCL_GPIO_Port->OTYPER |= 3 << 12;}//推挽输出

// #define SDA_IN() {SDA_GPIO_Port->OTYPER &= 0XFFFF0FFF;SDA_GPIO_Port->OTYPER |= 8 << 12;}


// #define SDA_OUT() {SCL_GPIO_Port->OTYPER &= 0XFFFF0FFF;SCL_GPIO_Port->OTYPER |= 3 << 12;}

#define SDA_IN() {SDA_GPIO_Port->regs->CRH &= 0XFFFF0FFF;SDA_GPIO_Port->OTYPER |= 8 << 12;}//定义为输入模式


#define SDA_OUT() {SCL_GPIO_Port->regs->CRH &= 0XFFFF0FFF;SCL_GPIO_Port->OTYPER |= 3 << 12;}//推挽输出

void IIC_Init()
{
    gpio_i2c_scl_gpiox = SCL_GPIO_Port;
    gpio_i2c_scl_pin = SCL_Pin;
    gpio_i2c_sda_gpiox = SDA_GPIO_Port;
    gpio_i2c_sda_pin = SDA_Pin;

}

void delay_us(uint32_t us)
{
    while (us--)
    {
        __nop();
    }
}

int IIC_Start(void)
{
    SDA_OUT();
    IIC_SDA_HIGH;
    if (!READ_SDA) return 0;
    IIC_SCL_HIGH;
    delay_us(1);
    IIC_SDA_LOW;
    if (READ_SDA) return 0;
    delay_us(1);
    IIC_SCL_LOW;
    return 1;
}

void IIC_Stop(void)
{
    SDA_OUT();
    IIC_SCL_LOW;
    IIC_SDA_LOW;
    delay_us(1);
    IIC_SCL_HIGH;
    IIC_SDA_HIGH;
    delay_us(1);
}

void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL_LOW; //拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        if(((txd &0x80)>>7)==1){
            IIC_SDA_HIGH;
        }
        else if(((txd &0x80)>>7)==0){
            IIC_SCL_LOW;
        }
        txd <<= 1;
        delay_us(1);
        IIC_SCL_HIGH;
        delay_us(1);
        IIC_SCL_LOW;
        delay_us(1);
    }
}

uint8_t IIC_Read_Byte(uint8_t ack)
{
    unsigned char i, receive = 0;
    SDA_IN(); //SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        IIC_SCL_LOW;
        delay_us(2);
        IIC_SCL_HIGH;
        receive <<= 1;
        if (READ_SDA) receive++;
        delay_us(2);
    }
    if (ack)
        IIC_Ack(); //发送ACK 
    else
        IIC_NAck(); //发送nACK  
    return receive;
}

int IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN();
    IIC_SDA_HIGH;
    delay_us(1);
    while (READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 50)
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

void IIC_NAck(void)
{
    SDA_OUT();
    IIC_SCL_LOW; // SCL=0
    IIC_SDA_HIGH; // SDA=1
    delay_us(1);
    IIC_SCL_HIGH; // SCL=1
    delay_us(1);
    IIC_SCL_LOW; // SCL=0
    delay_us(1);
}

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    int i;
    if (!IIC_Start())
    {
        return 1;
    }
        
    IIC_Send_Byte(addr << 1);
    if (!IIC_Wait_Ack())
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    for (i = 0; i < len; i++)
    {
        IIC_Send_Byte(data[i]);
        if (!IIC_Wait_Ack())
        {
            IIC_Stop();
            return 0;
        }
    }
    IIC_Stop();
    return 0;
}

int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!IIC_Start())
        return 1;
    IIC_Send_Byte(addr << 1);
    if (!IIC_Wait_Ack())
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((addr << 1) + 1);
    IIC_Wait_Ack();
    while (len)
    {
        if (len == 1)
            *
            buf = IIC_Read_Byte(0);
        else
            *buf = IIC_Read_Byte(1);
        buf++;
        len--;
    }
    IIC_Stop();
    return 0;
}

uint8_t IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data)
{
    *data = I2C_ReadOneByte(dev, reg);
    return 1;
}

uint8_t IICWriteByte(uint8_t dev, uint8_t reg, uint8_t data)
{
    return IICwriteBytes(dev, reg, 1, &data);
}

uint8_t I2C_ReadOneByte(uint8_t I2C_Addr, uint8_t addr)
{
    unsigned char res = 0;

    IIC_Start();
    IIC_Send_Byte(I2C_Addr); //发送写命令
    res++;
    IIC_Wait_Ack();
    IIC_Send_Byte(addr);
    res++; //发送地址
    IIC_Wait_Ack();
    //IIC_Stop();//产生一个停止条件	
    IIC_Start();
    IIC_Send_Byte(I2C_Addr + 1);
    res++; //进入接收模式			   
    IIC_Wait_Ack();
    res = IIC_Read_Byte(0);
    IIC_Stop(); //产生一个停止条件

    return res;
}

uint8_t IICWriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data)
{
    uint8_t b;
    IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b &~(1 << bitNum));
    return IICWriteByte(dev, reg, b);
}

uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
    uint8_t count = 0;
    IIC_Start();
    IIC_Send_Byte(dev); //发送写命令
    IIC_Wait_Ack();
    IIC_Send_Byte(reg); //发送地址
    IIC_Wait_Ack();
    for (count = 0; count < length; count++)
    {
        IIC_Send_Byte(data[count]);
        IIC_Wait_Ack();
    }
    IIC_Stop(); //产生一个停止条件

    return 1; //status == 0;
}

uint8_t IICwriteBits(uint8_t dev, uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t data)
{
    uint8_t b;
    if (IICreadByte(dev, reg, &b) != 0)
    {
        uint8_t mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return IICWriteByte(dev, reg, b);
    }
    else
    {
        return 0;
    }
}
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
    uint8_t count = 0;

    IIC_Start();
    IIC_Send_Byte(dev); //发送写命令
    IIC_Wait_Ack();
    IIC_Send_Byte(reg); //发送地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(dev + 1); //进入接收模式	
    IIC_Wait_Ack();

    for (count = 0; count < length; count++)
    {
        if (count != length - 1)
        {
            data[count] = IIC_Read_Byte(1); //带ACK的读数据
        }
        else
        {
            data[count] = IIC_Read_Byte(0); //最后一个字节NACK
        }

    }
    IIC_Stop(); //产生一个停止条件
    return count;
}
