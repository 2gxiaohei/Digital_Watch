#include "Clock.h"
#include "myrtc.h"    
#include "oled.h"

/* 定义任务句柄 */
TaskHandle_t RTC_Display_Handle = NULL;

//动画完成标志的外部引用(用于同步）
extern volatile uint8_t xAnimationCompleted;

//RTC显示任务函数
void Task_RTC_Display(void *argument)
{
    /* 等待开机动画完成 */
    while (xAnimationCompleted == pdFALSE)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    /* 无限循环 - 显示时间 */
    while (1)
    {
        // 读取当前RTC时间
        MyRTC_ReadTime();
        
        // 清屏
        OLED_Clear();
        
        // 第1行 (Y=1) - 显示年月日
        OLED_ShowString(1, 1, "Date:", 8);                    // 列1: "Date:"
        OLED_ShowNum(1 + 6 * 8, 1, MyRTC_Time[0], 4, 8);      // 列49: 年
        OLED_ShowString(1 + 10 * 8, 1, "-", 8);               // 列81: "-"
        OLED_ShowNum(1 + 11 * 8, 1, MyRTC_Time[1], 2, 8);     // 列89: 月
        OLED_ShowString(1 + 13 * 8, 1, "-", 8);               // 列105: "-"
        OLED_ShowNum(1 + 14 * 8, 1, MyRTC_Time[2], 2, 8);     // 列113: 日
        
        // 第2行 (Y=1+16=17) - 显示时分秒
        OLED_ShowString(1, 17, "Time:", 8);                   // 列1: "Time:"
        OLED_ShowNum(1 + 6 * 8, 17, MyRTC_Time[3], 2, 8);     // 列49: 时
        OLED_ShowString(1 + 8 * 8, 17, ":", 8);               // 列65: ":"
        OLED_ShowNum(1 + 9 * 8, 17, MyRTC_Time[4], 2, 8);     // 列73: 分
        OLED_ShowString(1 + 11 * 8, 17, ":", 8);              // 列89: ":"
        OLED_ShowNum(1 + 12 * 8, 17, MyRTC_Time[5], 2, 8);    // 列97: 秒
        
        // 更新OLED显示
        OLED_Update();
        
        // 延时1秒
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//时钟任务初始化函数
void Clock_Task_Init(void)
{
    /* 创建RTC显示任务 */
    xTaskCreate(
        Task_RTC_Display,       // 任务函数
        "RTC_Display",          // 任务名称
        256,                    // 任务堆栈大小（根据实际需求调整）
        NULL,                   // 任务参数
        1,                      // 任务优先级（与LED相同）
        &RTC_Display_Handle     // 任务句柄
    );
}