#include "MYI2C_HAL.h"

// 定义引脚
#define OLED_SCL_PIN GPIO_PIN_8
#define OLED_SDA_PIN GPIO_PIN_9
#define OLED_GPIO_PORT GPIOB

// 写SCL引脚电平 (HAL库版本)
void MyI2C_W_SCL(uint8_t BitValue)
{
    HAL_GPIO_WritePin(OLED_GPIO_PORT, OLED_SCL_PIN, 
                      (GPIO_PinState)BitValue);
}

// 写SDA引脚电平 (HAL库版本)
void MyI2C_W_SDA(uint8_t BitValue)
{
    HAL_GPIO_WritePin(OLED_GPIO_PORT, OLED_SDA_PIN, 
                      (GPIO_PinState)BitValue);
}

// 读SDA引脚电平 (HAL库版本)
uint8_t MyI2C_R_SDA(void)
{
    return HAL_GPIO_ReadPin(OLED_GPIO_PORT, OLED_SDA_PIN);
}

// I2C初始化 - CubeMX已配置，此函数可以简化或删除
void MyI2C_Init(void)
{
    // CubeMX已经初始化了GPIO
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

// 起始信号：SCL高电平时，SDA从高变低
void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

// 停止信号：SCL高电平时，SDA从低变高
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

// 发送字节
void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(!!(Byte & (0x80 >> i)));
        MyI2C_W_SCL(1);
        // 添加短延时（可选）
        // for(uint8_t j=0; j<5; j++);
        MyI2C_W_SCL(0);
    }
}

// 接收字节
uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA()) {Byte |= (0x80 >> i);}
        MyI2C_W_SCL(0);
    }
    return Byte;
}

// 发送应答
void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

// 接收应答
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}
