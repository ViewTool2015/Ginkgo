  /*
  ******************************************************************************
  * @file     : ControlADC.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 9:35
  * @brief    : ControlADC demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
  
#ifndef _CONTROLADC_H_
#define _CONTROLADC_H_

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
#define VAI_USBADC		(1)		//device type

// define ADC channel
#define	VAI_ADC_CH0		(1<<0)	// ADC_CH0	
#define	VAI_ADC_CH1		(1<<1)	// ADC_CH1	
#define	VAI_ADC_CH2		(1<<2)	// ADC_CH2	
#define	VAI_ADC_CH3		(1<<3)	// ADC_CH3	
#define	VAI_ADC_CH4		(1<<4)	// ADC_CH4	
#define	VAI_ADC_CH5		(1<<5)	// ADC_CH5	
#define	VAI_ADC_CH6		(1<<6)	// ADC_CH6	
#define	VAI_ADC_CH7		(1<<7)	// ADC_CH7	
#define	VAI_ADC_ALL		(0xFF)	// ADC_CH_ALL	

// data type of Ginkgo series interface card  
typedef  struct  _VAI_BOARD_INFO{
	uint16_t	dr_Version;			// driver version, hex
	uint16_t	in_Version;			// API version, hex
	uint8_t		adc_num;			// number of ADC channel
	char		str_hw_Name[100];	// name of HW, e.g. ,"Ginkgo-SPI-Adapter"(include '\0’)
	char		str_fw_Version[100];// version of FW, e.g. "2.0.0" (include '\0’)
	char		str_hw_Version[100];// version of HW, e.g. ,"2.1.0"(include '\0’)
	char		str_Serial_Num[100];// adapter serial number
	uint16_t	Reserved[4];		// reserved
} VAI_BOARD_INFO,*PVAI_BOARD_INFO; 



#ifdef __cplusplus
extern "C"
{
#endif
int32_t WINAPI VAI_ScanDevice(uint8_t NeedInit=1);
int32_t WINAPI VAI_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI VAI_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI VAI_ReadBoardInfo(int32_t DevType,int32_t DevIndex,PVAI_BOARD_INFO pInfo);
int32_t WINAPI VAI_InitADC(int32_t DevType, int32_t DevIndex, uint8_t Channel, uint16_t Period);
int32_t WINAPI VAI_ReadDatas(int32_t DevType,int32_t DevIndex,uint16_t DataNum,uint16_t *pData);
#ifdef __cplusplus
}
#endif


#endif
