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
#ifndef OS_UNIX
#include <Windows.h>

#else
#ifndef WINAPI

#define WINAPI

#include <unistd.h>
#endif
#endif

//适配器类型定义
#define VPI_USBPWM		(2)

//PWM通道定义
#define	VPI_PWM_CH0		(1<<0)	//PWM_CH0
#define	VPI_PWM_CH1		(1<<1)	//PWM_CH1
#define	VPI_PWM_CH2		(1<<2)	//PWM_CH2
#define	VPI_PWM_CH3		(1<<3)	//PWM_CH3
#define	VPI_PWM_CH4		(1<<4)	//PWM_CH4
#define	VPI_PWM_CH5		(1<<5)	//PWM_CH5
#define	VPI_PWM_CH6		(1<<6)	//PWM_CH6
#define	VPI_PWM_CH7		(1<<7)	//PWM_CH7
#define	VPI_PWM_ALL		(0XFF)	//PWM_ALL

typedef struct _VPI_INIT_CONFIG{
	uint8_t		PWM_ChannelMask;//PWM索引号，每个bit对应一个通道
    uint8_t     PWM_Mode;		//PWM模式，取值0或者1
	uint8_t		PWM_Pulse;		//PWM占空比,0到100之间取值
	uint8_t		PWM_Polarity;	//PWM输出极性，取值0或者1
    uint32_t    PWM_Frequency;	//PWM频率,1到1000000之间取值
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
int32_t	WINAPI VPI_SetPWMPeriod(int32_t DevType, int32_t DevIndex, uint8_t ChannelMask,uint32_t *Frequency);
#ifdef __cplusplus
}
#endif

#endif
