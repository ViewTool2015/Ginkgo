  /*
  ******************************************************************************
  * @file     : ControlGPIO.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 12:20
  * @brief    : ControlGPIO demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
#ifndef _CONTROLGPIO_H_
#define _CONTROLGPIO_H_

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

// The adapter type definition
#define VGI_USBGPIO			(1)	

// Define GPIO pin
#define	VGI_GPIO_PIN0		(1<<0)	//GPIO_0
#define	VGI_GPIO_PIN1		(1<<1)	//GPIO_1
#define	VGI_GPIO_PIN2		(1<<2)	//GPIO_2
#define	VGI_GPIO_PIN3		(1<<3)	//GPIO_3
#define	VGI_GPIO_PIN4		(1<<4)	//GPIO_4
#define	VGI_GPIO_PIN5		(1<<5)	//GPIO_5
#define	VGI_GPIO_PIN6		(1<<6)	//GPIO_6
#define	VGI_GPIO_PIN7		(1<<7)	//GPIO_7
#define	VGI_GPIO_PIN8		(1<<8)	//GPIO_8
#define	VGI_GPIO_PIN9		(1<<9)	//GPIO_9
#define	VGI_GPIO_PIN10		(1<<10)	//GPIO_10
#define	VGI_GPIO_PIN11		(1<<11)	//GPIO_11
#define	VGI_GPIO_PIN12		(1<<12)	//GPIO_12
#define	VGI_GPIO_PIN13		(1<<13)	//GPIO_13
#define	VGI_GPIO_PIN14		(1<<14)	//GPIO_14
#define	VGI_GPIO_PIN15		(1<<15)	//GPIO_15
#define	VGI_GPIO_PIN_ALL	(0xFFFF)//ALL PIN

//1.Data types of Ginkgo series adapter information.
typedef  struct  _VGI_BOARD_INFO{
	uint8_t		ProductName[32];	// Product name e.g. "Ginkgo-I2C-Adapter"(include '\0')
	uint8_t		FirmwareVersion[4];	// Firmware version
	uint8_t		HardwareVersion[4];	// Firmware version
	uint8_t		SerialNumber[12];	// The adapter serial number
} VGI_BOARD_INFO,*PVGI_BOARD_INFO; 



#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI VGI_ScanDevice(uint8_t NeedInit=1);
int32_t WINAPI VGI_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI VGI_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI VGI_ReadBoardInfo(int32_t DevIndex,PVGI_BOARD_INFO pInfo);
int32_t WINAPI VGI_InitGPIO(int32_t DevType, int32_t DevIndex, uint16_t PinMask, uint32_t PinMode);
int32_t WINAPI VGI_WriteDatas(int32_t DevType,int32_t DevIndex, uint16_t PinMask,uint16_t Data);
int32_t WINAPI VGI_ReadDatas(int32_t DevType,int32_t DevIndex,uint16_t PinMask,uint16_t *pData);
int32_t	WINAPI VGI_SetPins(int32_t DevType,int32_t DevIndex,uint16_t PinMask);
int32_t	WINAPI VGI_ResetPins(int32_t DevType,int32_t DevIndex,uint16_t PinMask);
int32_t	WINAPI VGI_SetInput(int32_t DevType,int32_t DevIndex,uint16_t PinMask);
int32_t	WINAPI VGI_SetOutput(int32_t DevType,int32_t DevIndex,uint16_t PinMask);
int32_t	WINAPI VGI_SetOpenDrain(int32_t DevType,int32_t DevIndex,uint16_t PinMask);

#ifdef __cplusplus
}
#endif


#endif
