  /*
  ******************************************************************************
  * @file     : ControlCNT.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 9:58
  * @brief    : ControlCNT HAL
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#ifndef _CONTROLCNT_H_
#define _CONTROLCNT_H_

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
// Define Counter
#define CNT_USBCNT	1

// Define Counter channel
#define	CNT_CH0		(1<<0)	//CNT_CH0	
#define	CNT_CH1		(1<<1)	//CNT_CH1	
#define	CNT_CH2		(1<<2)	//CNT_CH2	
#define	CNT_CH3		(1<<3)	//CNT_CH3	
#define	CNT_ALL		(0x0F)	//CNT_CH_ALL	


typedef struct _CNT_INIT_CONFIG{
	uint8_t 	CounterMode;		//Counter Mode: 0-Up,1-Down
	uint8_t		CounterPolarity;	//Counter Polarity:0-Rising,1-Falling,2-BothEdge
	uint8_t		CounterBitWide;		//Counter Bit Wide:16-16bit,32-32bit
}CNT_INIT_CONFIG,*PCNT_INIT_CONFIG;

#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI CNT_ScanDevice(uint8_t NeedInit);
int32_t WINAPI CNT_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI CNT_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI CNT_InitCounter(int32_t DevType, int32_t DevIndex, uint8_t Channel, PCNT_INIT_CONFIG pInitConfig);
int32_t WINAPI CNT_SetCounter(int32_t DevType, int32_t DevIndex, uint8_t Channel, uint32_t *pCounterValue);
int32_t WINAPI CNT_GetCounter(int32_t DevType, int32_t DevIndex, uint8_t Channel, uint32_t *pCounterValue);
int32_t WINAPI CNT_StartCounter(int32_t DevType, int32_t DevIndex, uint8_t Channel);
int32_t WINAPI CNT_StopCounter(int32_t DevType, int32_t DevIndex, uint8_t Channel);

#ifdef __cplusplus
}
#endif


#endif
