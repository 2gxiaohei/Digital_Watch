#include "myrtc.h"      
#include <time.h>

extern RTC_HandleTypeDef hrtc;  // 添加CubeMX生成的RTC句柄
uint16_t MyRTC_Time[6];

//初始化代码在CUBEMX生成的rtc.c文件的MX_RTC_Init()函数中

void MyRTC_SetTime(void)
{
    time_t time_cnt;
    struct tm time_date;
    RTC_TimeTypeDef sTime = {0};   // HAL库时间结构体，用于设置时分秒
    RTC_DateTypeDef sDate = {0};   // HAL库日期结构体，用于设置年月日
    
    // 从MyRTC_Time数组获取时间数据
    time_date.tm_year = MyRTC_Time[0]-1900;
    time_date.tm_mon = MyRTC_Time[1]-1;
    time_date.tm_mday = MyRTC_Time[2];
    time_date.tm_hour = MyRTC_Time[3];
    time_date.tm_min = MyRTC_Time[4];
    time_date.tm_sec = MyRTC_Time[5];
    
    // 转换为时间戳
    time_cnt = mktime(&time_date);
    
    // 将UTC时间戳转换回tm结构体
    time_date = *localtime(&time_cnt);
    
    // 填充HAL库时间结构体
    sTime.Hours = time_date.tm_hour;
    sTime.Minutes = time_date.tm_min;
    sTime.Seconds = time_date.tm_sec;
    
    // 填充HAL库日期结构体（HAL年份=tm_year-100，因为HAL以2000年为基准）
    sDate.Year = time_date.tm_year - 100;
    sDate.Month = time_date.tm_mon + 1;
    sDate.Date = time_date.tm_mday;
    
    // HAL库函数：设置RTC时间和日期（Binary格式，与CubeMX配置一致）
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    
    // 删除：HAL库函数内部已包含等待操作完成的机制
    // RTC_WaitForLastTask();
}

void MyRTC_ReadTime(void)
{
    time_t time_cnt;
    struct tm time_date;
    RTC_TimeTypeDef sTime = {0};   // HAL库时间结构体，用于接收时分秒
    RTC_DateTypeDef sDate = {0};   // HAL库日期结构体，用于接收年月日
    
    // HAL库函数：从RTC读取时间和日期（Binary格式）
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    
    // 将HAL库结构体转换为tm结构体
    time_date.tm_year = sDate.Year + 100;  // HAL年份+100 = tm_year
    time_date.tm_mon = sDate.Month - 1;
    time_date.tm_mday = sDate.Date;
    time_date.tm_hour = sTime.Hours;
    time_date.tm_min = sTime.Minutes;
    time_date.tm_sec = sTime.Seconds;
    
    // 转换为时间戳
    time_cnt = mktime(&time_date);
    time_date = *localtime(&time_cnt);
    
    // 更新MyRTC_Time数组供应用程序使用
    MyRTC_Time[0] = time_date.tm_year + 1900;
    MyRTC_Time[1] = time_date.tm_mon + 1;
    MyRTC_Time[2] = time_date.tm_mday;
    MyRTC_Time[3] = time_date.tm_hour;
    MyRTC_Time[4] = time_date.tm_min;
    MyRTC_Time[5] = time_date.tm_sec;
}