#ifndef __OLED_FONTS_H
#define __OLED_FONTS_H

#include <stdint.h>

// 字模声明
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

// 汉字结构
typedef struct {
    char Index[4];
    uint8_t Data[32];
} ChineseCell_t;

extern const ChineseCell_t OLED_CF16x16[];

#endif
