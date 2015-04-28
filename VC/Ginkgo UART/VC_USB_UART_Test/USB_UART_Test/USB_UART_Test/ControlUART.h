  /*
  ******************************************************************************
  * @file     : ControlUART.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 11:01
  * @brief    : ControlUART demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#ifndef _CONTROLUART_H_
#define _CONTROLUART_H_

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
// Define device type
#define UART_USBUART		(2)

//1.Data types of Ginkgo series adapter information.
typedef  struct  _UART_BOARD_INFO{
	uint8_t		ProductName[32];	// Product name, e.g."Ginkgo-I2C-Adaptor"
	uint8_t		FirmwareVersion[4];	// Firmware version
	uint8_t		HardwareVersion[4];	// Hardware version
    uint8_t		SerialNumber[12];	// The adapter serial number
} UART_BOARD_INFO,*PUART_BOARD_INFO; 


//2.Defien initialization data types
typedef struct _UART_INIT_CONFIG{
	uint8_t		StopBits;		// Stop bit，0-1bit,1-1.5bit,2-2bit,3-1.5bit
	uint8_t		WordLength;		// 8
	uint8_t		Parity;			// 0-No,4-Even,6-Odd
	uint16_t	RS485Mode;		// 485-RS485模式，232-RS232模式
	uint32_t	BaudRate;		// Baud Rate
}UART_INIT_CONFIG,*PUART_INIT_CONFIG;

#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI UART_ScanDevice(uint8_t NeedInit);
int32_t WINAPI UART_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI UART_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI UART_ReadBoardInfo(int32_t DevType,int32_t DevIndex,PUART_BOARD_INFO pInfo);

int32_t WINAPI UART_InitDevice(int32_t DevType,int32_t DevIndex,int32_t UARTIndex, PUART_INIT_CONFIG pInitConfig);

int32_t WINAPI UART_WriteBytes(int32_t DevType,int32_t DevIndex,int32_t UARTIndex,uint8_t* pWriteData,uint16_t Len);
int32_t WINAPI UART_ReadBytes(int32_t DevType,int32_t DevIndex,int32_t UARTIndex,uint8_t *pReadData,uint16_t *pLen);

#ifdef __cplusplus
}
#endif

#endif

