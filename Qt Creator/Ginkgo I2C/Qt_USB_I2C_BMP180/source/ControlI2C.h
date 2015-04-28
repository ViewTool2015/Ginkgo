/**
  ******************************************************************************
  * @file    ControlI2CSPI.h
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
#ifndef _CONTROLI2C_H_
#define _CONTROLI2C_H_

#include <stdint.h>
#include "ErrorType.h"
#ifndef OS_LINUX
#include <Windows.h>
#endif

//适配器类型定义
#define VII_USBI2C			(1)		//设备类型
//适配器初始化数据定义
#define VII_ADDR_7BIT		(7)		//7bit地址模式
#define VII_ADDR_10BIT		(10)	//10bit地址模式
#define VII_HCTL_MODE		(1)		//硬件控制
#define VII_SCTL_MODE		(2)		//软件控制
#define VII_MASTER			(1)		//主机
#define VII_SLAVE			(0)		//从机
#define VII_SUB_ADDR_NONE	(0)		//无子地址
#define VII_SUB_ADDR_1BYTE	(1)		//1Byte子地址
#define VII_SUB_ADDR_2BYTE	(2)		//2Byte子地址
#define VII_SUB_ADDR_3BYTE	(3)		//3Byte子地址
#define VII_SUB_ADDR_4BYTE	(4)		//4Byte子地址



//1.Ginkgo系列适配器信息的数据类型。
typedef  struct  _VII_BOARD_INFO{
	uint16_t	dr_Version;			//驱动程序版本号，用16 进制表示。
	uint16_t	in_Version;			//接口库版本号，用16 进制表示。
	uint8_t		i2c_num;			//表示有几路I2C 通道。
	char		str_hw_Name[100];	//硬件名称，比如“Ginkgo-SPI-Adaptor”（注意：包括字符串结束符‘\0’）
	char		str_fw_Version[100];//固件版本字符串，比如“2.0.0”（注意：包括字符串结束符‘\0’）
	char		str_hw_Version[100];//硬件版本字符串，比如“2.1.0”（注意：包括字符串结束符‘\0’）
	char		str_Serial_Num[100];//适配器序列号字符串
	uint16_t	Reserved[4];		//系统保留。
} VII_BOARD_INFO,*PVII_BOARD_INFO; 


//2.定义初始化I2C的数据类型
typedef struct _VII_INIT_CONFIG{
	uint8_t		MasterMode;		//主从选择控制:0-从机，1-主机
	uint8_t		ControlMode;	//控制方式:1-硬件控制，2-软件控制
	uint8_t		AddrType;		//7-7bit模式，10-10bit模式
	uint8_t		SubAddrWidth;	//子地址宽度，0到4取值，0时表示无子地址模式
	uint16_t	Addr;			//从机模式时候的设备地址
	uint32_t	ClockSpeed;		//时钟频率:单位为HZ
}VII_INIT_CONFIG,*PVII_INIT_CONFIG;

//软件I2C时间参数定义，时间单位为微秒
typedef struct _VII_TIME_CONFIG
{
    uint16_t tHD_STA;   //起始信号保持时间
    uint16_t tSU_STA;   //起始信号建立时间
    uint16_t tLOW;      //时钟低电平时间
    uint16_t tHIGH;     //时钟高电平时间
    uint16_t tSU_DAT;   //数据输入建立时间
    uint16_t tSU_STO;   //停止信号建立时间
    uint16_t tDH;       //数据输出保持时间
    uint16_t tDH_DAT;   //数据输入保持时间
    uint16_t tAA;       //SCL变低至SDA数据输出及应答信号
    uint16_t tR;        //SDA及SCL上升时间
    uint16_t tF;        //SDA及SCL下降时间
    uint16_t tBuf;      //新的发送开始前总线空闲时间
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
int32_t WINAPI VII_ReadBoardInfo(int32_t DevType,int32_t DevIndex,PVII_BOARD_INFO pInfo);
int32_t WINAPI VII_TimeConfig(int32_t DevType, int32_t DevIndex, int32_t I2CIndex, PVII_TIME_CONFIG pTimeConfig);
int32_t WINAPI VII_InitI2C(int32_t DevType, int32_t DevIndex, int32_t I2CIndex, PVII_INIT_CONFIG pInitConfig);
int32_t WINAPI VII_WriteBytes(int32_t DevType,int32_t DevIndex,int32_t I2CIndex,uint16_t Addr,uint32_t SubAddr,uint8_t *pWriteData,uint16_t Len);
int32_t WINAPI VII_ReadBytes(int32_t DevType,int32_t DevIndex,int32_t I2CIndex,uint16_t Addr,uint32_t SubAddr,uint8_t *pReadData,uint16_t Len);
#ifdef __cplusplus
}
#endif


#endif

