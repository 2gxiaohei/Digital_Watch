#ifndef __CLOCK_H
#define __CLOCK_H

/* 包含头文件 */
#include "FreeRTOS.h"
#include "task.h"

//任务句柄声明
extern TaskHandle_t RTC_Display_Handle;

/* 任务函数声明 */
void Task_RTC_Display(void *argument);

/* 任务创建函数声明 */
void Clock_Task_Init(void);

#endif /* __CLOCK_H */