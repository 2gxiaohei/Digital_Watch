#ifndef __STARTUP_ANIMATION_H
#define __STARTUP_ANIMATION_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

// 动画任务函数
void vTaskStartupAnimation(void *pvParameters);

// 动画子函数
void vAnimation_Typewriter(void);
void vAnimation_LoadingBar(void);

// 创建动画任务（供main调用）
BaseType_t xCreateStartupAnimationTask(UBaseType_t uxPriority);

// 等待动画完成的标志（可选，用于同步）
extern volatile BaseType_t xAnimationCompleted;

#endif /* __STARTUP_ANIMATION_H */