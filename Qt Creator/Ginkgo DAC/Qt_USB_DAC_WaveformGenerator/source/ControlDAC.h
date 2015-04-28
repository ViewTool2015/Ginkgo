/**
  ******************************************************************************
  * @file    ControlDAC.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   Ginkgo USB-DAC适配器底层控制相关API函数定义.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef _CONTROLDAC_H_
#define _CONTROLDAC_H_

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

//The adapter type definition
#define VDA_USBDAC		(1)		//Device type

//Define the DAC channels
#define	VDA_DAC_CH0		(0)	//DAC_CH0	
#define	VDA_DAC_CH1		(1)	//DAC_CH1		



#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI VDA_ScanDevice(uint8_t NeedInit=1);
int32_t WINAPI VDA_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI VDA_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI VDA_InitDAC(int32_t DevType, int32_t DevIndex, uint8_t Channel, uint16_t *pWaveData,uint16_t WaveDataNum,uint16_t Period);
int32_t WINAPI VDA_StartDAC(int32_t DevType, int32_t DevIndex, uint8_t Channel);
int32_t WINAPI VDA_StopDAC(int32_t DevType, int32_t DevIndex, uint8_t Channel);

#ifdef __cplusplus
}
#endif


#endif
