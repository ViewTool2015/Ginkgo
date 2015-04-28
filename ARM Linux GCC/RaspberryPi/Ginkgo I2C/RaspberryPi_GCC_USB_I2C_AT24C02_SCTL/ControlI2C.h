  /*
  ******************************************************************************
  * @file     : ControlI2C.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 9:32
  * @brief    : ControlI2C demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#ifndef _CONTROLI2C_H_
#define _CONTROLI2C_H_

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
#define VII_USBI2C			(1)		//Adapter Type
//The adapter data initialization definition
#define VII_ADDR_7BIT		(7)		//7-bit address mode
#define VII_ADDR_10BIT		(10)	//10-bit address mode
#define VII_HCTL_SLAVE_MODE	(0)		//Standard slave mode
#define VII_HCTL_MODE		(1)		//Standard mode
#define VII_SCTL_MODE		(2)		//GPIO mode
#define VII_MASTER			(1)		//Master
#define VII_SLAVE			(0)		//Slave
#define VII_SUB_ADDR_NONE	(0)		//No sub-address
#define VII_SUB_ADDR_1BYTE	(1)		//1Byte sub-address
#define VII_SUB_ADDR_2BYTE	(2)		//2Byte sub-address
#define VII_SUB_ADDR_3BYTE	(3)		//3Byte sub-address
#define VII_SUB_ADDR_4BYTE	(4)		//4Byte sub-address


//1.Data types of Ginkgo series adapter information.
typedef  struct  _VII_BOARD_INFO{
	uint8_t		ProductName[32];	//Product name
	uint8_t		FirmwareVersion[4];	//Firmware version
	uint8_t		HardwareVersion[4];	//Hardware version
	uint8_t		SerialNumber[12];	//The adapter serial number
} VII_BOARD_INFO,*PVII_BOARD_INFO; 


//2.I2C data types initialization Define
typedef struct _VII_INIT_CONFIG{
	uint8_t		MasterMode;		//Master-slave choice: 0-slave, 1-master
	uint8_t		ControlMode;	//Control mode: 0-Standard slave mode, 1-Standard mode, 2-GPIO mode
	uint8_t		AddrType;		//7-7bit mode, 10-10bit mode
	uint8_t		SubAddrWidth;	//Sub-Address width: value of 0~4, 0 means no Sub-Address mode
	uint16_t	Addr;			//Device address in salve mode
	uint32_t	ClockSpeed;		//Clock frequency(HZ)
}VII_INIT_CONFIG,*PVII_INIT_CONFIG;

//3.I2C time parameter definition in GPIO mode(ms)
typedef struct _VII_TIME_CONFIG
{
    uint16_t tHD_STA;   //Timing for start signal Keeping
    uint16_t tSU_STA;   //Timing for start signal be established
    uint16_t tLOW;      //Timing for clock low level
    uint16_t tHIGH;     //Timing for clock high level
    uint16_t tSU_DAT;   //Timing for data input be established
    uint16_t tSU_STO;   //Timing for stop signal be established
    uint16_t tDH;       //Timing for data output Keeping
    uint16_t tDH_DAT;   //Timing for data input Keeping
    uint16_t tAA;       //SCL lower to SDA output and response signal
    uint16_t tR;        //Timing for SDA and SCL rising
    uint16_t tF;        //Timing for SDA and SCL going down
    uint16_t tBuf;      //Free timing of the bus until the new mission
    uint8_t tACK[4];
    uint16_t tStart;
    uint16_t tStop;
}VII_TIME_CONFIG,*PVII_TIME_CONFIG;

#ifdef __cplusplus
extern "C"
{
#endif

int32_t WINAPI VII_ScanDevice(uint8_t NeedInit=1);
int32_t WINAPI VII_OpenDevice(int32_t DevType,int32_t DevIndex,int32_t Reserved);
int32_t WINAPI VII_CloseDevice(int32_t DevType,int32_t DevIndex);
int32_t WINAPI VII_ReadBoardInfo(int32_t DevIndex,PVII_BOARD_INFO pInfo);
int32_t WINAPI VII_TimeConfig(int32_t DevType, int32_t DevIndex, int32_t I2CIndex, PVII_TIME_CONFIG pTimeConfig);
int32_t WINAPI VII_InitI2C(int32_t DevType, int32_t DevIndex, int32_t I2CIndex, PVII_INIT_CONFIG pInitConfig);
int32_t WINAPI VII_WriteBytes(int32_t DevType,int32_t DevIndex,int32_t I2CIndex,uint16_t SlaveAddr,uint32_t SubAddr,uint8_t *pWriteData,uint16_t Len);
int32_t WINAPI VII_ReadBytes(int32_t DevType,int32_t DevIndex,int32_t I2CIndex,uint16_t SlaveAddr,uint32_t SubAddr,uint8_t *pReadData,uint16_t Len);
int32_t WINAPI VII_SetUserKey(int32_t DevType,int32_t DevIndex,uint8_t* pUserKey);
int32_t WINAPI VII_CheckUserKey(int32_t DevType,int32_t DevIndex,uint8_t* pUserKey);
int32_t WINAPI VII_SlaveReadBytes(int32_t DevType,int32_t DevIndex,int32_t I2CIndex,uint8_t* pReadData,uint16_t *pLen);
int32_t WINAPI VII_SlaveWriteBytes(int32_t DevType,int32_t DevIndex,int32_t I2CIndex,uint8_t* pWriteData,uint16_t Len);
#ifdef __cplusplus
}
#endif


#endif

