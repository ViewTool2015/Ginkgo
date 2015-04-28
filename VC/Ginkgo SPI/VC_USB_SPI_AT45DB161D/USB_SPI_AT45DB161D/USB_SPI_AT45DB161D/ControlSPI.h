  /*
  ******************************************************************************
  * @file       ControlSPI.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/17 17:11
  * @brief    : SPI HAL API
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
#ifndef _CONTROLSPI_H_
#define _CONTROLSPI_H_

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
#define VSI_USBSPI		(2)

// Define adapter type
#define VSI_USBSPI		(2)

//1. Data type of Ginkgo serial adapter 
typedef  struct  _VSI_BOARD_INFO{
    uint8_t		ProductName[32];	// Product Name e.g.,"Ginkgo-SPI-Adapter"(include '\0)
    uint8_t		FirmwareVersion[4];	// version of FW, e.g. "2.0.0" (include '\0)
    uint8_t		HardwareVersion[4];	// version of HW, e.g. ,"2.1.0"(include '\0)
    uint8_t		SerialNumber[12];	// adapter serial number
} VSI_BOARD_INFO,*PVSI_BOARD_INFO; 

//2. Define data type of initialize SPI
typedef struct _VSI_INIT_CONFIG{
    uint8_t     ControlMode;
    // SPI control mode: 0->hardware control(full duplex) 1->hardware control(half duplex) 2->software control(half duplex) 3-> one wire mode
    uint8_t     TranBits;
    // Width  of data (between 8 and 16)
    uint8_t     MasterMode;	
    // Master mode : 0 -> slave mode 1 -> master mode
    uint8_t     CPOL;		
    // Clock Polarity: 0 -> SCK active-high  1->SCK active-low 
    uint8_t     CPHA;		
    // Clock Phase: 0 -> sample on the leading (first) clock edge 1-> sample on the trailing (second) clock edge
    uint8_t     LSBFirst;	
    // whether or not LSB first: 0->MSB first 1-> LSB first
    uint8_t     SelPolarity;
    // Chip select Polarity: 0-> low to select 1-> high to select
    uint32_t	ClockSpeed;		
    // SPI clock frequency
}VSI_INIT_CONFIG,*PVSI_INIT_CONFIG;

//3. Define data structure of initialize SPI
typedef struct _VSI_FLASH_INIT_CONFIG
{
	uint32_t	page_size;
	uint32_t	page_num;
	uint8_t		write_enable[8];
	uint8_t		read_status[8];
	uint8_t		chip_erase[8];
	uint8_t		write_data[8];
	uint8_t		read_data[8];
	uint8_t		first_cmd[8];
	uint8_t		busy_bit;
	uint8_t		busy_mask;
	uint8_t		addr_bytes;
	uint8_t		addr_shift;
	uint8_t		init_flag;
}VSI_FLASH_INIT_CONFIG,*PVSI_FLASH_INIT_CONFIG;

#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI VSI_ScanDevice(uint8_t NeedInit=1);
int32_t WINAPI VSI_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI VSI_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI VSI_InitSPI(int32_t DevType, int32_t DevIndex, PVSI_INIT_CONFIG pInitConfig);
int32_t WINAPI VSI_ReadBoardInfo(int32_t DevIndex,PVSI_BOARD_INFO pInfo);
int32_t WINAPI VSI_WriteBytes(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pWriteData,uint16_t Len);
int32_t WINAPI VSI_ReadBytes(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pReadData,uint16_t Len);
int32_t WINAPI VSI_WriteReadBytes(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t* pWriteData,uint16_t WriteLen,uint8_t * pReadData,uint16_t ReadLen);
int32_t WINAPI VSI_WriteBits(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pWriteBitStr);
int32_t WINAPI VSI_ReadBits(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pReadBitStr,int32_t ReadBitsNum);
int32_t WINAPI VSI_WriteReadBits(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pWriteBitStr,uint8_t *pReadBitStr,int32_t ReadBitsNum);

int32_t WINAPI VSI_SlaveModeSet(int32_t DevType,int32_t DevIndex,uint8_t SlaveMode,uint16_t CmdBytes,uint16_t DataBytes);
int32_t WINAPI VSI_SlaveReadBytes(int32_t DevType,int32_t DevIndex,uint8_t *pReadData,int32_t *pBytesNum,int32_t WaitTime);
int32_t WINAPI VSI_SlaveWriteBytes(int32_t DevType,int32_t DevIndex,uint8_t *pWriteData,int32_t WriteBytesNum);

int32_t WINAPI VSI_FlashInit(int32_t DevType,int32_t DevIndex, PVSI_FLASH_INIT_CONFIG pFlashInitConfig);
int32_t WINAPI VSI_FlashWriteBytes(int32_t DevType,int32_t DevIndex,int32_t PageAddr,uint8_t *pWriteData,uint16_t WriteLen);
int32_t WINAPI VSI_FlashReadBytes(int32_t DevType,int32_t DevIndex,int32_t PageAddr,uint8_t *pReadData,uint16_t ReadLen);

int32_t WINAPI VSI_SetUserKey(int32_t DevType,int32_t DevIndex,uint8_t* pUserKey);
int32_t WINAPI VSI_CheckUserKey(int32_t DevType,int32_t DevIndex,uint8_t* pUserKey);

int32_t WINAPI VSI_BlockWriteBytes(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pWriteData,uint16_t BlockSize,uint16_t BlockNum,uint32_t IntervalTime);
int32_t WINAPI VSI_BlockReadBytes(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pReadData,uint16_t BlockSize,uint16_t BlockNum,uint32_t IntervalTime);
int32_t WINAPI VSI_BlockWriteReadBytes(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pWriteData,uint16_t WriteBlockSize,uint8_t *pReadData,uint16_t ReadBlockSize,uint16_t BlockNum,uint32_t IntervalTime);

#ifdef __cplusplus
}
#endif

#endif

