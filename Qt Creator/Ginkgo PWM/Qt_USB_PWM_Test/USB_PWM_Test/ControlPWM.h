/**
  ******************************************************************************
  * @file    ControlPWM.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   Ginkgo USB-PWM适配器底层控制相关API函数定义.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef _CONTROLPWM_H_
#define _CONTROLPWM_H_

#include <stdint.h>
#include "ErrorType.h"
#ifndef OS_LINUX
#include <Windows.h>
#endif

//适配器类型定义
#define VPI_USBPWM		(2)

typedef struct _VPI_INIT_CONFIG{
	uint8_t		PWM_ChannelMask;//PWM索引号
    uint8_t     PWM_Mode;		//PWM模式
	uint8_t		PWM_Pulse;		//PWM占空比
	uint8_t		PWM_Polarity;	//PWM输出极性
    uint32_t    PWM_Frequency;	//PWM频率
}VPI_INIT_CONFIG,*PVPI_INIT_CONFIG;

#ifdef __cplusplus
extern "C"
{
#endif
int32_t WINAPI VPI_ScanDevice(uint8_t NeedInit=1);
int32_t WINAPI VPI_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI VPI_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI VPI_InitPWM(int32_t DevType, int32_t DevIndex, PVPI_INIT_CONFIG pInitConfig);
int32_t	WINAPI VPI_StartPWM(int32_t DevType, int32_t DevIndex, uint8_t ChannelMask);
int32_t	WINAPI VPI_StopPWM(int32_t DevType, int32_t DevIndex, uint8_t ChannelMask);
int32_t	WINAPI VPI_SetPWMPulse(int32_t DevType, int32_t DevIndex, uint8_t ChannelMask,uint8_t *pPulse);
int32_t	WINAPI VPI_SetPWMPeriod(int32_t DevType, int32_t DevIndex, uint8_t ChannelMask,uint32_t *pFrequency);
#ifdef __cplusplus
}
#endif

#endif
