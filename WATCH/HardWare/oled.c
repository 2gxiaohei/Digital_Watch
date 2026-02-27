#include "oled.h"
#include "oled_fonts.h"
#include "myi2c_hal.h"
#include <string.h>
#include <stdio.h>

uint8_t OLED_DisplayBuf[8][128];

//==============================================================================
// 基础操作函数（原有函数）
//==============================================================================

// OLED_WriteCommand - 写命令到OLED
void OLED_WriteCommand(uint8_t Command)
{
	  MyI2C_Start();//起始
  	MyI2C_SendByte(0x78);//发送从机地址
    MyI2C_ReceiveAck();//应答
  	MyI2C_SendByte(0x00);//Control Byte（非连续模式写命令）
    MyI2C_ReceiveAck();//应答	  
	  MyI2C_SendByte(Command);//Data Byte
    MyI2C_ReceiveAck();//应答	  
	  MyI2C_Stop();//终止
}

// OLED_WriteData - 写数据到OLED
void OLED_WriteData(uint8_t *Data,uint8_t Count)
{
	  MyI2C_Start();//起始
  	MyI2C_SendByte(0x78);//发送从机地址
    MyI2C_ReceiveAck();//应答
  	MyI2C_SendByte(0x40);//Control Byte（非连续模式写数据）
    MyI2C_ReceiveAck();//应答	  
	  for(uint8_t i=0;i<Count;i++)
	  {
			MyI2C_SendByte(Data[i]);//要写入GDDRAM的显示数据
			MyI2C_ReceiveAck();//应答	  
		}
		MyI2C_Stop();//终止
}

// OLED_Init - OLED初始化
void OLED_Init(void)
{
	MyI2C_Init();
	OLED_WriteCommand(0xAE);
	
	OLED_WriteCommand(0xD5);
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);
	
	OLED_WriteCommand(0xA1);
	
	OLED_WriteCommand(0xC8);

	OLED_WriteCommand(0xDA);
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);

	OLED_WriteCommand(0xA6);

	OLED_WriteCommand(0x8D);
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);
}

// OLED_SetCursor - 设置光标位置
void OLED_SetCursor(uint8_t X,uint8_t Page)//列0~127，页0~7
{
	OLED_WriteCommand(0x00 | (X&0x0F));//取出X低四位或上低四位命令码
	OLED_WriteCommand(0x10 | ((X&0xF0)>>4));//取出X高四位或上高四位命令码
	OLED_WriteCommand(0xB0 | Page);//页地址
}

// OLED_Update - 更新显示（将显存内容发送到OLED）
void OLED_Update(void)
{
	for(uint8_t j=0;j<8;j++){
		OLED_SetCursor(0,j);
		OLED_WriteData(OLED_DisplayBuf[j],128);
	}
}

// OLED_Clear - 清空整个屏幕
void OLED_Clear(void)
{
	for(uint8_t j=0;j<8;j++){
		for(uint8_t i=0;i<128;i++){
			OLED_DisplayBuf[j][i]=0x00;
		}
	}
}

// OLED_ClearArea - 清空指定区域
void OLED_ClearArea(uint8_t X,uint8_t Y,uint8_t Width,uint8_t Height)
{
	for(uint8_t j=Y;j<Y+Height;j++)
	{
		for(uint8_t i=X;i<X+Width;i++)
	  {
			 //在 OLED 缓冲区的指定坐标 (i,j) 处，把对应的那个 bit 设为 0，而不影响同一字节里的其他 7 个 bit。
			 OLED_DisplayBuf[j/8][i] &= ~(0x01<<(j%8));
		}
	}
}

// OLED_Reverse - 整个屏幕反色
void OLED_Reverse(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				
	{
		for (i = 0; i < 128; i ++)			
		{
			OLED_DisplayBuf[j][i] ^= 0xFF;	
		}
	}
}

// OLED_ReverseArea - 指定区域反色
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
	int16_t i, j;
	
	for (j = Y; j < Y + Height; j ++)		//遍历指定页
	{
		for (i = X; i < X + Width; i ++)	//遍历指定列
		{
			if (i >= 0 && i <= 127 && j >=0 && j <= 63)			//超出屏幕的内容不显示
			{
				OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8);	//将显存数组指定数据取反
			}
		}
	}
}

// OLED_ShowChar - 显示单个字符
void OLED_ShowChar(uint8_t X,uint8_t Y,char Char,uint8_t Fontsize)
{
	if(Fontsize==6){
		OLED_ShowImage(X,Y,6,8,OLED_F6x8[Char-' ']);
	}else if(Fontsize==8){
		OLED_ShowImage(X,Y,8,15,OLED_F8x16[Char-' ']);
	}
}

// OLED_ShowString - 显示字符串
void OLED_ShowString(uint8_t X,uint8_t Y,char *String,uint8_t FontSize)
{
	for(uint8_t i=0;String[i]!='\0';i++)
	{
		OLED_ShowChar(X+i*FontSize,Y,String[i],FontSize);
	}
}

// OLED_ShowImage - 显示图像
void OLED_ShowImage(uint8_t X,uint8_t Y,uint8_t Width,uint8_t Height,const uint8_t *Image)
{
	OLED_ClearArea(X,Y,Width,Height);//先清空指定区域
	for(uint8_t j=0;j<(Height-1)/8+1;j++)
	{
		for(uint8_t i=0;i<Width;i++)
		{
			OLED_DisplayBuf[Y/8+j][X+i]|=Image[j*Width+i]<<(Y%8);
		  OLED_DisplayBuf[Y/8+j+1][X+i]|=Image[j*Width+i]>>(8-Y%8);
		}
	}
}

// OLED_ShowChinese - 显示汉字
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese)
{
	char SigleChinese[4] = {0};//存储当前的汉字
	uint8_t pChinese = 0;//指示当前存储到第几个字节了
	uint8_t pIndex;//存放汉字索引
	
	for (uint8_t i = 0; Chinese[i] != '\0'; i ++)
	{
		SigleChinese[pChinese] = Chinese[i];
		pChinese ++;
		
		if (pChinese >= 3)//一个汉字一般三个字节+一个结束字节
		{
			pChinese = 0;
			
			for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex ++)
			{
				if (strcmp(OLED_CF16x16[pIndex].Index, SigleChinese) == 0)
				{
					break;
				}
			}
			OLED_ShowImage(X + ((i + 1) / 3 - 1) * 16, Y, 16, 16, OLED_CF16x16[pIndex].Data);
		}
	}
}

// OLED_DrawPoint - 画点
void OLED_DrawPoint(uint8_t X,uint8_t Y)
{
	OLED_DisplayBuf[Y/8][X]=0X01<<(Y%8);
}

// OLED_GetPoint - 获取点状态（是否点亮）
uint8_t OLED_GetPoint(uint8_t X,uint8_t Y)
{
	if(OLED_DisplayBuf[Y/8][X]&0X01<<(Y%8))
	{
		return 1;
	}
	return 0;
}

//==============================================================================
// 新增封装函数
//==============================================================================

// OLED_DisplayOn - 打开OLED显示
void OLED_DisplayOn(void)
{
    OLED_WriteCommand(0xAF);
}

// OLED_DisplayOff - 关闭OLED显示
void OLED_DisplayOff(void)
{
    OLED_WriteCommand(0xAE);
}

// OLED_SetContrast - 设置对比度
void OLED_SetContrast(uint8_t contrast)
{
    OLED_WriteCommand(0x81);
    OLED_WriteCommand(contrast);
}

// OLED_SetDisplayMode - 设置显示模式（正常/反色）
void OLED_SetDisplayMode(uint8_t isInverse)
{
    if (isInverse)
        OLED_WriteCommand(0xA7);  // 反色显示
    else
        OLED_WriteCommand(0xA6);  // 正常显示
}

// OLED_FillArea - 填充指定区域
void OLED_FillArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t data)
{
    for (uint8_t j = Y; j < Y + Height; j++)
    {
        for (uint8_t i = X; i < X + Width; i++)
        {
            if (i <= 127 && j <= 63)
            {
                if (data)
                    OLED_DisplayBuf[j / 8][i] |= (0x01 << (j % 8));
                else
                    OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));
            }
        }
    }
}

// OLED_DrawRectangle - 绘制矩形（空心/填充）
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t isFill)
{
    if (isFill)
    {
        // 填充矩形
        OLED_FillArea(X, Y, Width, Height, 1);
    }
    else
    {
        // 空心矩形
        for (uint8_t i = X; i < X + Width; i++)
        {
            OLED_DrawPoint(i, Y);
            OLED_DrawPoint(i, Y + Height - 1);
        }
        for (uint8_t j = Y; j < Y + Height; j++)
        {
            OLED_DrawPoint(X, j);
            OLED_DrawPoint(X + Width - 1, j);
        }
    }
}

// OLED_DrawLine - 绘制直线
void OLED_DrawLine(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
{
    int16_t dx = X2 > X1 ? X2 - X1 : X1 - X2;
    int16_t dy = Y2 > Y1 ? Y2 - Y1 : Y1 - Y2;
    int16_t sx = X1 < X2 ? 1 : -1;
    int16_t sy = Y1 < Y2 ? 1 : -1;
    int16_t err = dx - dy;
    
    while (1)
    {
        OLED_DrawPoint(X1, Y1);
        
        if (X1 == X2 && Y1 == Y2) break;
        
        int16_t e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            X1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            Y1 += sy;
        }
    }
}

// OLED_ShowString_Center - 居中显示字符串
void OLED_ShowString_Center(uint8_t Y, char *String, uint8_t FontSize)
{
    uint8_t len = strlen(String);
    uint8_t X = (128 - len * FontSize) / 2;
    OLED_ShowString(X, Y, String, FontSize);
}

// OLED_ShowString_Right - 右对齐显示字符串
void OLED_ShowString_Right(uint8_t X, uint8_t Y, char *String, uint8_t FontSize)
{
    uint8_t len = strlen(String);
    X = X - len * FontSize;
    OLED_ShowString(X, Y, String, FontSize);
}

// OLED_ShowNum - 显示无符号整数
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t num, uint8_t len, uint8_t FontSize)
{
    char str[len + 1];
    uint8_t i;
    
    for (i = 0; i < len; i++)
    {
        str[len - 1 - i] = '0' + (num % 10);
        num /= 10;
    }
    str[len] = '\0';
    
    OLED_ShowString(X, Y, str, FontSize);
}

// OLED_ShowSignedNum - 显示有符号整数
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t num, uint8_t len, uint8_t FontSize)
{
    char str[len + 2];
    uint8_t i;
    uint8_t isNegative = 0;
    
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
        len--;
    }
    
    for (i = 0; i < len; i++)
    {
        str[len - 1 - i] = '0' + (num % 10);
        num /= 10;
    }
    
    if (isNegative)
    {
        for (i = len; i > 0; i--)
        {
            str[i] = str[i - 1];
        }
        str[0] = '-';
        len++;
    }
    
    str[len] = '\0';
    OLED_ShowString(X, Y, str, FontSize);
}

// OLED_ShowHexNum - 显示十六进制数
void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t num, uint8_t len, uint8_t FontSize)
{
    char str[len + 1];
    uint8_t i;
    
    for (i = 0; i < len; i++)
    {
        uint8_t nibble = (num >> (4 * (len - 1 - i))) & 0x0F;
        str[i] = nibble < 10 ? '0' + nibble : 'A' + nibble - 10;
    }
    str[len] = '\0';
    
    OLED_ShowString(X, Y, str, FontSize);
}

// OLED_ShowFloatNum - 显示浮点数
void OLED_ShowFloatNum(uint8_t X, uint8_t Y, float num, uint8_t intLen, uint8_t floatLen, uint8_t FontSize)
{
    char str[intLen + floatLen + 2];
    int32_t intPart = (int32_t)num;
    int32_t floatPart = (int32_t)((num - intPart) * pow_int(10, floatLen));
    
    if (floatPart < 0) floatPart = -floatPart;
    
    // 转换整数部分
    for (int i = 0; i < intLen; i++)
    {
        str[intLen - 1 - i] = '0' + (intPart % 10);
        intPart /= 10;
    }
    
    str[intLen] = '.';
    
    // 转换小数部分
    for (int i = 0; i < floatLen; i++)
    {
        str[intLen + 1 + i] = '0' + (floatPart / (int32_t)pow_int(10, floatLen - 1 - i)) % 10;
    }
    
    str[intLen + floatLen + 1] = '\0';
    OLED_ShowString(X, Y, str, FontSize);
}

// OLED_ScrollString - 滚动显示字符串
void OLED_ScrollString(uint8_t Y, char *String, uint8_t FontSize, uint16_t delay, uint8_t direction)
{
    uint8_t len = strlen(String);
    uint8_t strWidth = len * FontSize;
    uint8_t i, j;
    
    if (direction == 0)  // 向左滚动
    {
        for (i = 0; i <= strWidth; i++)
        {
            OLED_ClearArea(0, Y, 128, FontSize == 6 ? 8 : 16);
            
            for (j = 0; j < len; j++)
            {
                if (FontSize == 6)
                {
                    OLED_ShowImage(128 - strWidth + i + j * 6, Y, 6, 8, OLED_F6x8[String[j] - ' ']);
                }
                else
                {
                    OLED_ShowImage(128 - strWidth + i + j * 8, Y, 8, 16, OLED_F8x16[String[j] - ' ']);
                }
            }
            
            OLED_Update();
            for (j = 0; j < delay; j++);
        }
    }
    else  // 向右滚动
    {
        for (i = 0; i <= strWidth; i++)
        {
            OLED_ClearArea(0, Y, 128, FontSize == 6 ? 8 : 16);
            
            for (j = 0; j < len; j++)
            {
                if (FontSize == 6)
                {
                    OLED_ShowImage(0 - i + j * 6, Y, 6, 8, OLED_F6x8[String[j] - ' ']);
                }
                else
                {
                    OLED_ShowImage(0 - i + j * 8, Y, 8, 16, OLED_F8x16[String[j] - ' ']);
                }
            }
            
            OLED_Update();
            for (j = 0; j < delay; j++);
        }
    }
}

// pow_int - 辅助函数：计算10的幂
static double pow_int(double x, int y)
{
    double result = 1.0;
    for (int i = 0; i < y; i++)
    {
        result *= x;
    }
    return result;
}