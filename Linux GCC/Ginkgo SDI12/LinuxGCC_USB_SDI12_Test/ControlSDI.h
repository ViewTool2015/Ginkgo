  /*
  ******************************************************************************
  * @file     : ControlSDI.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 10:57
  * @brief    : USB-SDI12 HAL API
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
#ifndef _CONTROLSDI_H_
#define _CONTROLSDI_H_

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

// Defien adapter type 
#define SDI_USBSDI			(1)	
#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI SDI_ScanDevice(uint8_t NeedInit=1);
int32_t WINAPI SDI_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI SDI_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI SDI_InitSDI(int32_t DevType, int32_t DevIndex, uint8_t SDIIndex);
int32_t WINAPI SDI_SendCmd(int32_t DevType, int32_t DevIndex, uint8_t SDIIndex,uint8_t *pCommand,uint8_t *pRespond,uint8_t CheckCRC);

#ifdef __cplusplus
}
#endif

#endif