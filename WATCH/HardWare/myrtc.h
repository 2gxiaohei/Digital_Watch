#ifndef __MYRTC_H
#define __MYRTC_H

#include "stm32f1xx_hal.h"  // 修改标准库→HAL库头文件

extern uint16_t MyRTC_Time[];//年月日时分秒

void MyRTC_SetTime(void);
void MyRTC_ReadTime(void);

#endif