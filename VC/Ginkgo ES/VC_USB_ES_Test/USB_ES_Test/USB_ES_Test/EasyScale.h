  /*
  ******************************************************************************
  * @file     : EasyScale.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 10:08
  * @brief    : EasyScale HAL API
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#ifndef	__EASYSCALE_H__
#define __EASYSCALE_H__

#include <stdint.h>
#include "ErrorType.h"
#ifndef OS_UNIX
#include <Windows.h>
#endif

//EasyScale bus data type
typedef struct
{
	uint8_t	ESMode;
	uint8_t	DevAddr;		//Device address
	uint8_t	RFA:1;
	uint8_t	SubAddr:2;
	uint8_t	Data:5;
}VEI_ES_OBJ,*PVEI_ES_OBJ;

//EasyScale bus initialize data type
typedef struct
{
	uint16_t	TShutDown;
	uint16_t	ESDelay;
	uint16_t	ESDetect;
	uint16_t	ESWin;
	uint16_t	TStart;
	uint16_t	TEos;
	uint16_t	TValACK;
	uint16_t	TACKN;
	uint16_t	HighBit_THigh;
	uint16_t	HighBit_TLow;
	uint16_t	LowBit_THigh;
	uint16_t	LowBit_TLow;
}VEI_ES_TIMING,*PVEI_ES_TIMING;

#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI VEI_ScanDevice(uint8_t NeedInit=1);
int32_t WINAPI VEI_OpenDevice(int32_t DevType, int32_t DevIndex,int32_t Reserved);
int32_t WINAPI VEI_CloseDevice(int32_t DevType, int32_t DevIndex);
int32_t WINAPI VEI_InitES(int32_t DevType, int32_t DevIndex, int32_t ESChannel, int32_t InitPinValue, PVEI_ES_TIMING pESTiming);
int32_t WINAPI VEI_Transmit(int32_t DevType, int32_t DevIndex, int32_t ESChannel, PVEI_ES_OBJ pESData, int32_t Len);

#ifdef __cplusplus
}
#endif
#endif
