#include "startup_animation.h"
#include "oled.h"
#include "oled_fonts.h"
#include <string.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

// 动画完成标志（可选）
volatile BaseType_t xAnimationCompleted = pdFALSE;

//开机动画任务函数

void vTaskStartupAnimation(void *pvParameters)
{
    // 先显示测试信息，确认任务运行
    OLED_Clear();
    OLED_ShowString_Center(24, "Animation Start", 8);
    OLED_Update();
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // 1. 打字机效果显示 "2G XIAOHEI"
    vAnimation_Typewriter();
    
    // 2. 加载进度条动画
    vAnimation_LoadingBar();
    
    // 设置完成标志
    xAnimationCompleted = pdTRUE;
    
    // 动画完成后删除自己
    vTaskDelete(NULL); 
}

//打字机效果显示文字 "2G XIAOHEI"

void vAnimation_Typewriter(void)
{
    char *text = "2G XIAOHEI";
    uint8_t len = strlen(text);
    uint8_t i;
    uint8_t startX;
    uint8_t startY;
    
    OLED_Clear();
    OLED_Update();
    
    // 使用8x16字体
    startX = (128 - (len * 8)) / 2;
    startY = 24;
    
    // 逐个字符显示，实现打字机效果
    for (i = 0; i < len; i++)
    {
        OLED_ShowChar(startX + i * 8, startY, text[i], 8);
        OLED_Update();
        vTaskDelay(pdMS_TO_TICKS(200));
    }
    
    vTaskDelay(pdMS_TO_TICKS(500));
}

//加载进度条动画

void vAnimation_LoadingBar(void)
{
    uint8_t progress;
    char percent[4];
    uint8_t loadingX, percentX;
    
    OLED_Clear();
    OLED_Update();
    
    // 显示"LOADING"文字
    loadingX = (128 - (7 * 8)) / 2;  // "LOADING"共7个字符
    OLED_ShowString(loadingX, 16, "LOADING", 8);
    OLED_Update();
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // 进度条外框
    OLED_DrawRectangle(24, 36, 80, 10, 0);
    OLED_Update();
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // 进度条动画
    for (progress = 0; progress <= 100; progress += 5)
    {
        // 显示百分比
        sprintf(percent, "%d%%", progress);
        percentX = (128 - (strlen(percent) * 8)) / 2;
        OLED_ShowString(percentX, 50, percent, 8);
        
        // 绘制进度
        if (progress > 0)
        {
            OLED_FillArea(26, 38, (76 * progress) / 100, 6, 1);
        }
        
        OLED_Update();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    vTaskDelay(pdMS_TO_TICKS(500));
}

//创建开机动画任务

BaseType_t xCreateStartupAnimationTask(UBaseType_t uxPriority)
{
    BaseType_t xReturn;
    TaskHandle_t xHandle = NULL;
    
    xReturn = xTaskCreate(
        vTaskStartupAnimation,      // 任务函数
        "Animation",                // 任务名称
        256,                        // 堆栈大小（字）- 根据实际需要调整
        NULL,                       // 任务参数
        uxPriority,                 // 优先级
        &xHandle                    // 任务句柄
    );
    
    return xReturn;
}