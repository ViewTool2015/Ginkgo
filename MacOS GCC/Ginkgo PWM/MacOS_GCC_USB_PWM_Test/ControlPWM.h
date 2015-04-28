  /*
  ******************************************************************************
  * @file     : ControlPWM.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 10:16
  * @brief    : ControlPWM HAL API
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
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
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

// Define adapter type
#define VPI_USBPWM		(2)

// Define PWM channel
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
	uint8_t		PWM_ChannelMask;//PWM index, Each bit corresponds to a channel
    uint8_t     PWM_Mode;		//PWM mode (0 or 1)
	uint8_t		PWM_Pulse;		//PWM duty ratio, integer in range 0..100
	uint8_t		PWM_Polarity;	//PWM Polarity (0 or 1)
    uint32_t    PWM_Frequency;	//PWM Frequency, integer in range 0..10M
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
