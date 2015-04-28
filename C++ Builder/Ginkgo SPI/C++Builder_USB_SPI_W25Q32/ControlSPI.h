/**
  ******************************************************************************
  * @file    ControlSPI.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   Ginkgo USB-SPI适配器底层控制相关API函数定义.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef _CONTROLSPI_H_
#define _CONTROLSPI_H_

#include <stdint.h>
#include "ErrorType.h"
#ifndef OS_LINUX
#include <Windows.h>
#endif

//适配器类型定义
#define VSI_USBSPI		(2)

//1.Ginkgo系列接口卡信息的数据类型。
typedef  struct  _VSI_BOARD_INFO{
	uint16_t	dr_Version;			//驱动程序版本号，用16 进制表示。
	uint16_t	in_Version;			//接口库版本号，用16 进制表示。
    uint8_t     spi_Num;			//表示有几路SPI 通道。
    char        str_hw_Name[100];	//硬件名称，比如“Ginkgo-SPI-Adaptor”（注意：包括字符串结束符‘\0’）
    char        str_fw_Version[100];//固件版本字符串，比如“2.0.0”（注意：包括字符串结束符‘\0’）
    char        str_hw_Version[100];//硬件版本字符串，比如“2.1.0”（注意：包括字符串结束符‘\0’）
    char        str_Serial_Num[100];//适配器序列号字符串
	uint16_t	Reserved[4];		//系统保留。
} VSI_BOARD_INFO,*PVSI_BOARD_INFO; 


//2.定义初始化SPI的数据类型
typedef struct _VSI_INIT_CONFIG{
    uint8_t     ControlMode;	//SPI控制方式:0-硬件控制（全双工模式）,1-硬件控制（半双工模式），2-软件控制（半双工模式）,3-单总线模式，数据线输入输出都为MOSI
    uint8_t     TranBits;		//数据传输字节宽度，在8和16之间取值
    uint8_t     MasterMode;		//主从选择控制:0-从机，1-主机
    uint8_t     CPOL;			//时钟极性控制:0-SCK高有效，1-SCK低有效
    uint8_t     CPHA;			//时钟相位控制:0-第一个SCK时钟采样，1-第二个SCK时钟采样
    uint8_t     LSBFirst;		//数据移位方式:0-MSB在前，1-LSB在前
    uint8_t     SelPolarity;	//片选信号极性:0-低电平选中，1-高电平选中
	uint32_t	ClockSpeed;		//SPI时钟频率:单位为HZ
}VSI_INIT_CONFIG,*PVSI_INIT_CONFIG;

//3.SPI Flash操作初始化数据结构
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
int32_t WINAPI VSI_ReadBoardInfo(int32_t DevType,int32_t DevIndex,PVSI_BOARD_INFO pInfo);
int32_t WINAPI VSI_WriteBytes(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pData,uint16_t Len);
int32_t WINAPI VSI_ReadBytes(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pData,uint16_t Len);
int32_t WINAPI VSI_WriteReadBytes(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t* pWriteData,uint16_t WriteLen,uint8_t * pReadData,uint16_t ReadLen);
int32_t WINAPI VSI_WriteBits(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pWriteBitStr);
int32_t WINAPI VSI_ReadBits(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pReadBitStr,int32_t ReadBitsNum);
int32_t WINAPI VSI_WriteReadBits(int32_t DevType,int32_t DevIndex,int32_t SPIIndex,uint8_t *pWriteBitStr,uint8_t *pReadBitStr,int32_t ReadBitsNum);

int32_t WINAPI VSI_SlaveModeSet(int32_t DevType,int32_t DevIndex,uint8_t SlaveMode);
int32_t WINAPI VSI_SlaveReadBytes(int32_t DevType,int32_t DevIndex,uint8_t *pReadData,int32_t *pBytesNum,int32_t WaitTime);
int32_t WINAPI VSI_SlaveWriteBytes(int32_t DevType,int32_t DevIndex,uint8_t *pWriteData,int32_t WriteBytesNum);

int32_t WINAPI VSI_FlashInit(int32_t DevType,int32_t DevIndex, PVSI_FLASH_INIT_CONFIG pFlashInitConfig);
int32_t WINAPI VSI_FlashWriteBytes(int32_t DevType,int32_t DevIndex,int32_t PageAddr,uint8_t *pWriteData,uint16_t WriteLen);
int32_t WINAPI VSI_FlashReadBytes(int32_t DevType,int32_t DevIndex,int32_t PageAddr,uint8_t *pReadData,uint16_t ReadLen);
#ifdef __cplusplus
}
#endif

#endif

