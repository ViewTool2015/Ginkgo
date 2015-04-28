/**
  ******************************************************************************
  * @file    ControlCNT.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   Ginkgo USB-CNT适配器底层控制相关API函数定义.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef _CONTROLCNT_H_
#define _CONTROLCNT_H_

#include <stdint.h>
#include "ErrorType.h"
#ifndef OS_LINUX
#include <Windows.h>
#endif
//
#define CNT_USBCNT	1

//定义Counter通道
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
