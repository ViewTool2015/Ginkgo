/**
  ******************************************************************************
  * @file    ControlSMBus.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   Ginkgo USB-I2C适配器底层控制相关API函数定义.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  *
  ******************************************************************************
  */
#ifndef _CONTROLSMBUS_H_
#define _CONTROLSMBUS_H_

#include <stdint.h>
#include "ErrorType.h"
#ifndef OS_UNIX
#include <Windows.h>

#else
#ifndef WINAPI

#define WINAPI

#include <unistd.h>
#endif
#endif

//适配器类型定义
#define VII_USBI2C			(1)		//设备类型

//SMBUS总线错误码
#define SMBUS_OK                        0x00    ///< Packet was sent correctly
#define SMBUS_ERROR_SLAVE_NOT_SUPPORTED 0x01    ///< Slave mode is not supported
#define SMBUS_ERROR_BUSOFF              0x02
#define SMBUS_ERROR_TXFULL              0x03
#define SMBUS_ERROR_BUSY                0x04
#define SMBUS_ERROR_RXEMPTY             0x05
#define SMBUS_ERROR_OVERRUN             0x06
#define SMBUS_ERROR_TIMEOUT             0x07    ///< Timeout occured during sending the packet
#define SMBUS_ERROR_INVALID_SIZE        0x08    ///< Invalid size of received packet
#define SMBUS_ERROR_PACKET_TOO_LONG     0x09    ///< Packet to sent does not fit into internal buffer
#define SMBUS_ERROR_PARAMETER           0x0A    ///< Invalid parameter
#define SMBUS_ERROR_PEC                 0x0B    ///< PEC error
#define SMBUS_ERROR_NACK				0x0C	///< NACK error
#define SMBUS_ERROR_ARLO				0x0D	///< Arbitration lost (master mode)


#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI SMBus_ScanDevice(uint8_t NeedInit=1);
int32_t WINAPI SMBus_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI SMBus_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI SMBus_HardInit(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint32_t ClockSpeed, uint8_t OwnAddr);
int32_t WINAPI SMBus_QuickCommand(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr);
int32_t WINAPI SMBus_WriteByte(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t Data,uint8_t PEC);
int32_t WINAPI SMBus_ReadByte(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t *pData,uint8_t PEC);
int32_t WINAPI SMBus_WriteByteProtocol(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t CommandCode,uint8_t Data,uint8_t PEC);
int32_t WINAPI SMBus_WriteWordProtocol(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t CommandCode,uint16_t Data,uint8_t PEC);
int32_t WINAPI SMBus_ReadByteProtocol(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t CommandCode, uint8_t *pData,uint8_t PEC);
int32_t WINAPI SMBus_ReadWordProtocol(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t CommandCode, uint16_t *pData,uint8_t PEC);
int32_t WINAPI SMBus_ProcessCall(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t CommandCode,uint16_t WriteData, uint16_t *pReadData,uint8_t PEC);
int32_t WINAPI SMBus_BlockWrite(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t CommandCode,uint8_t *pData,uint8_t ByteCount,uint8_t PEC);
int32_t WINAPI SMBus_BlockRead(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t CommandCode,uint8_t *pData,uint8_t *pByteCount,uint8_t PEC);
int32_t WINAPI SMBus_BlockProcessCall(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t SlaveAddr,uint8_t CommandCode,uint8_t *pWriteData,uint8_t WriteByteCount,uint8_t *pReadData,uint8_t *pReadByteCount,uint8_t PEC);
int32_t WINAPI SMBus_GetAlert(int32_t DevType,int32_t DevIndex,int32_t SMBUSIndex,uint8_t *pAlertFlag);

#ifdef __cplusplus
}
#endif


#endif

