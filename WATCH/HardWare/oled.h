#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>


// 显示缓存
extern uint8_t OLED_DisplayBuf[8][128];

//==============================================================================
// 基础操作函数
//==============================================================================

// OLED_Init - OLED初始化
void OLED_Init(void);

// OLED_WriteCommand - 写命令到OLED
void OLED_WriteCommand(uint8_t Command);

// OLED_WriteData - 写数据到OLED
void OLED_WriteData(uint8_t *Data, uint8_t Count);

// OLED_SetCursor - 设置光标位置
void OLED_SetCursor(uint8_t X, uint8_t Page);

// OLED_Update - 更新显示（将显存内容发送到OLED）
void OLED_Update(void);

// OLED_Clear - 清空整个屏幕
void OLED_Clear(void);

// OLED_ClearArea - 清空指定区域
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);

// OLED_Reverse - 整个屏幕反色
void OLED_Reverse(void);

// OLED_ReverseArea - 指定区域反色
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

// OLED_ShowChar - 显示单个字符
void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t Fontsize);

// OLED_ShowString - 显示字符串
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize);

// OLED_ShowImage - 显示图像
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);

// OLED_ShowChinese - 显示汉字
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese);

// OLED_DrawPoint - 画点
void OLED_DrawPoint(uint8_t X, uint8_t Y);

// OLED_GetPoint - 获取点状态（是否点亮）
uint8_t OLED_GetPoint(uint8_t X, uint8_t Y);

//==============================================================================
// 显示设置函数
//==============================================================================

// OLED_DisplayOn - 打开OLED显示
void OLED_DisplayOn(void);

// OLED_DisplayOff - 关闭OLED显示
void OLED_DisplayOff(void);

// OLED_SetContrast - 设置对比度
void OLED_SetContrast(uint8_t contrast);

// OLED_SetDisplayMode - 设置显示模式（正常/反色）
void OLED_SetDisplayMode(uint8_t isInverse);

//==============================================================================
// 区域操作函数
//==============================================================================

// OLED_FillArea - 填充指定区域
void OLED_FillArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t data);

// OLED_DrawRectangle - 绘制矩形（空心/填充）
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t isFill);

// OLED_DrawLine - 绘制直线
void OLED_DrawLine(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);

//==============================================================================
// 字符串操作增强函数
//==============================================================================

// OLED_ShowString_Center - 居中显示字符串
void OLED_ShowString_Center(uint8_t Y, char *String, uint8_t FontSize);

// OLED_ShowString_Right - 右对齐显示字符串
void OLED_ShowString_Right(uint8_t X, uint8_t Y, char *String, uint8_t FontSize);

//==============================================================================
// 数字显示函数
//==============================================================================

// OLED_ShowNum - 显示无符号整数
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t num, uint8_t len, uint8_t FontSize);

// OLED_ShowSignedNum - 显示有符号整数
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t num, uint8_t len, uint8_t FontSize);

// OLED_ShowHexNum - 显示十六进制数
void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t num, uint8_t len, uint8_t FontSize);

// OLED_ShowFloatNum - 显示浮点数
void OLED_ShowFloatNum(uint8_t X, uint8_t Y, float num, uint8_t intLen, uint8_t floatLen, uint8_t FontSize);

//==============================================================================
// 滚动效果函数
//==============================================================================

// OLED_ScrollString - 滚动显示字符串
void OLED_ScrollString(uint8_t Y, char *String, uint8_t FontSize, uint16_t delay, uint8_t direction);

// pow_int函数声明 - 辅助函数
double pow_int(double x, int y);

#endif  
