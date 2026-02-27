/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */
  RTC_TimeTypeDef time;   // 时间结构体参数
  RTC_DateTypeDef datebuff;   // 日期结构体参数
  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */
  __HAL_RCC_BKP_CLK_ENABLE();       // 开启后备区域时钟
  __HAL_RCC_PWR_CLK_ENABLE();       // 开启电源时钟
  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  // 检查备份寄存器，判断是否为首次上电
  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0xA5A5)
  {
    /* USER CODE END Check_RTC_BKUP */

    /** Initialize RTC and set the Time and Date
    */
    // 首次上电，设置初始时间
    sTime.Hours = 21;       // 21点
    sTime.Minutes = 32;     // 32分
    sTime.Seconds = 0;      // 0秒

    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
    
    DateToUpdate.WeekDay = 4;    // 星期四 (通常1=星期一, 2=星期二, 3=星期三, 4=星期四)
    DateToUpdate.Month = 2;       // 二月
    DateToUpdate.Date = 26;       // 26日
    DateToUpdate.Year = 26;       // 2026年 (表示年份的后两位)

    if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* USER CODE BEGIN RTC_Init 2 */
    // 开启RTC时钟秒中断
    __HAL_RTC_SECOND_ENABLE_IT(&hrtc, RTC_IT_SEC);
    
    // 保存日期数据到备份寄存器
    datebuff = DateToUpdate;  // 把日期数据拷贝到自己定义的data中
    
    // 向后备区域寄存器写入数据，标记已初始化
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0xA5A5);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, (uint16_t)datebuff.Year);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, (uint16_t)datebuff.Month);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, (uint16_t)datebuff.Date);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, (uint16_t)datebuff.WeekDay);
    /* USER CODE END RTC_Init 2 */
  }
  else
  {
    /* USER CODE BEGIN RTC_Init 2 */
    // 非首次上电，从备份寄存器读取之前保存的日期数据
    datebuff.Year    = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2);
    datebuff.Month   = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3);
    datebuff.Date    = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4);
    datebuff.WeekDay = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5);
    
    // 恢复日期设置
    DateToUpdate = datebuff;
    if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
    
    // 开启RTC时钟秒中断
    __HAL_RTC_SECOND_ENABLE_IT(&hrtc, RTC_IT_SEC);
    /* USER CODE END RTC_Init 2 */
  }
  
/* USER CODE BEGIN RTC_Init 2 */
	
/* USER CODE END RTC_Init 2 */
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
