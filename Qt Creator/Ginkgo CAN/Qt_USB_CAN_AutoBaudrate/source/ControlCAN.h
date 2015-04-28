/**
  ******************************************************************************
  * @file    ControlCAN.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   Ginkgo USB-CAN适配器底层控制相关API函数定义.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef _CONTROLCAN_H_
#define _CONTROLCAN_H_

#include <stdint.h>
#include "ErrorType.h"
#ifndef OS_LINUX
#include <Windows.h>
#else
#define WINAPI
#endif

//适配器类型定义
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4


//CAN错误码
#define	ERR_CAN_OVERFLOW			0x0001	//CAN 控制器内部FIFO溢出
#define	ERR_CAN_ERRALARM			0x0002	//CAN 控制器错误报警
#define	ERR_CAN_PASSIVE				0x0004	//CAN 控制器消极错误
#define	ERR_CAN_LOSE				0x0008	//CAN 控制器仲裁丢失
#define	ERR_CAN_BUSERR				0x0010	//CAN 控制器总线错误
#define	ERR_CAN_BUSOFF				0x0020	//CAN 控制器总线关闭

//通用错误码
#define	ERR_DEVICEOPENED			0x0100	//设备已经打开
#define	ERR_DEVICEOPEN				0x0200	//打开设备错误
#define	ERR_DEVICENOTOPEN			0x0400	//设备没有打开
#define	ERR_BUFFEROVERFLOW			0x0800	//缓冲区溢出
#define	ERR_DEVICENOTEXIST			0x1000	//此设备不存在
#define	ERR_LOADKERNELDLL			0x2000	//装载动态库失败
#define ERR_CMDFAILED				0x4000	//执行命令失败错误码
#define	ERR_BUFFERCREATE			0x8000	//内存不足


//函数调用返回状态值
#define	STATUS_OK					1
#define STATUS_ERR					0
	

//1.兼容ZLGCAN系列接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
	uint16_t	hw_Version;			//硬件版本号，用16 进制表示。比如0x0100 表示V1.00。
	uint16_t	fw_Version;			//固件版本号，用16 进制表示。
	uint16_t	dr_Version;			//驱动程序版本号，用16 进制表示。
	uint16_t	in_Version;			//接口库版本号，用16 进制表示。
	uint16_t	irq_Num;			//板卡所使用的中断号。
	uint8_t		can_Num;			//表示有几路CAN 通道。
	int8_t		str_Serial_Num[20];	//此板卡的序列号。
	int8_t		str_hw_Type[40];	//硬件类型，比如“USBCAN V1.00”（注意：包括字符串结束符‘\0’）。
	uint16_t	Reserved[4];		//系统保留。
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//1.Ginkgo系列接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO_EX{
	uint8_t		ProductName[32];	//硬件名称，比如“Ginkgo-CAN-Adaptor”（注意：包括字符串结束符‘\0’）
	uint8_t		FirmwareVersion[4];	//固件版本
	uint8_t		HardwareVersion[4];	//硬件版本
	uint8_t		SerialNumber[12];	//适配器序列号
} VCI_BOARD_INFO_EX,*PVCI_BOARD_INFO_EX; 

//2.定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
	uint32_t	ID;			//报文ID。
	uint32_t	TimeStamp;	//接收到信息帧时的时间标识，从CAN 控制器初始化开始计时。
	uint8_t		TimeFlag;	//是否使用时间标识，为1 时TimeStamp 有效，TimeFlag 和TimeStamp 只在此帧为接收帧时有意义。
	uint8_t		SendType;	//发送帧类型，=0 时为正常发送，=1 时为单次发送，=2 时为自发自收，=3 时为单次自发自收，只在此
						//帧为发送帧时有意义。（当设备类型为EG20T-CAN 时，发送方式在VCI_InitCan 中通过设置，此处的
						//设置无效，设置为自发自收模式后EG20T-CAN 不能从总线上接收数据，只能收到自己发出的数据）
	uint8_t		RemoteFlag;	//是否是远程帧
	uint8_t		ExternFlag;	//是否是扩展帧
	uint8_t		DataLen;	//数据长度(<=8)，即Data 的长度。
	uint8_t		Data[8];	//报文的数据。
	uint8_t		Reserved[3];//系统保留。
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
	uint8_t		ErrInterrupt;	//中断记录，读操作会清除。
	uint8_t		regMode;		//CAN 控制器模式寄存器。
	uint8_t		regStatus;		//CAN 控制器状态寄存器。
	uint8_t		regALCapture;	//CAN 控制器仲裁丢失寄存器。
	uint8_t		regECCapture;	//CAN 控制器错误寄存器。 
	uint8_t		regEWLimit;		//CAN 控制器错误警告限制寄存器。
	uint8_t		regRECounter;	//CAN 控制器接收错误寄存器。 
	uint8_t		regTECounter;	//CAN 控制器发送错误寄存器。
	uint32_t	regESR;			//CAN 控制器错误状态寄存器。
	uint32_t	regTSR;			//CAN 控制器发送状态寄存器
	uint32_t	BufferSize;		//CAN 控制器接收缓冲区大小
	uint32_t	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.定义错误信息的数据类型。
typedef struct _ERR_INFO{
	uint32_t	ErrCode;			//错误码
	uint8_t		Passive_ErrData[3];	//当产生的错误中有消极错误时表示为消极错误的错误标识数据。
	uint8_t		ArLost_ErrData;		//当产生的错误中有仲裁丢失错误时表示为仲裁丢失错误的错误标识数据。
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.定义初始化CAN的数据类型
typedef struct _INIT_CONFIG{
	uint32_t	AccCode;	//验收码
	uint32_t	AccMask;	//屏蔽码
	uint32_t	Reserved;	//保留
	uint8_t		Filter;		//滤波方式,0-双滤波，1-单滤波
	uint8_t		Timing0;	//定时器0（BTR0）。	
	uint8_t		Timing1;	//定时器1（BTR1）。	
	uint8_t		Mode;		//模式。
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

//定义初始化CAN的数据类型
typedef struct _INIT_CONFIG_EX
{
	//CAN波特率 = 36MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
	uint32_t	CAN_BRP;	//取值范围1~1024
	uint8_t		CAN_SJW;	//取值范围1~4          
	uint8_t		CAN_BS1;	//取值范围1~16          
	uint8_t		CAN_BS2;	//取值范围1~8          
	uint8_t		CAN_Mode;	//CAN工作模式，0-正常模式，1-环回模式，2-静默模式，3-静默环回模式
	uint8_t		CAN_ABOM;	//自动离线管理，0-禁止，1-使能
	uint8_t		CAN_NART;	//报文重发管理，0-使能报文重传，1-禁止报文重传
	uint8_t		CAN_RFLM;	//FIFO锁定管理，0-新报文覆盖旧报文，1-丢弃新报文
	uint8_t		CAN_TXFP;	//发送优先级管理，0-标识符决定，1-发送请求顺序决定
	uint8_t		CAN_RELAY;	//是否开启中继功能，0x00-关闭中继功能，0x10-CAN1到CAN2中继，0x01-CAN2到CAN1中继，0x11-双向中继
	uint32_t	Reserved;	//系统保留
}VCI_INIT_CONFIG_EX,*PVCI_INIT_CONFIG_EX;


//6.定义了CAN 滤波器的设置
typedef struct _VCI_FILTER_CONFIG{
	uint8_t		Enable;			//使能该过滤器，1-使能，0-禁止
	uint8_t		FilterIndex;	//过滤器索引号，取值范围为0到13
	uint8_t		FilterMode;		//过滤器模式，0-屏蔽位模式，1-标识符列表模式
	uint8_t		ExtFrame;		//过滤的帧类型标志，为1 代表要过滤的为扩展帧，为0 代表要过滤的为标准帧。
	uint32_t	ID_Std_Ext;		//验收码ID
	uint32_t	ID_IDE;			//验收码IDE
	uint32_t	ID_RTR;			//验收码RTR
	uint32_t	MASK_Std_Ext;	//屏蔽码ID，该项只有在过滤器模式为屏蔽位模式时有用
	uint32_t	MASK_IDE;		//屏蔽码IDE，该项只有在过滤器模式为屏蔽位模式时有用
	uint32_t	MASK_RTR;		//屏蔽码RTR，该项只有在过滤器模式为屏蔽位模式时有用
	uint32_t	Reserved;		//系统保留
} VCI_FILTER_CONFIG,*PVCI_FILTER_CONFIG;

typedef void(*PVCI_RECEIVE_CALLBACK)(uint32_t DevIndex,uint32_t CANIndex,uint32_t Len);

#ifdef __cplusplus
extern "C"
{
#endif

uint32_t WINAPI VCI_ScanDevice(uint8_t NeedInit);
uint32_t WINAPI VCI_OpenDevice(uint32_t DevType,uint32_t DevIndex,uint32_t Reserved);
uint32_t WINAPI VCI_CloseDevice(uint32_t DevType,uint32_t DevIndex);
uint32_t WINAPI VCI_InitCAN(uint32_t DevType, uint32_t DevIndex, uint32_t CANIndex, PVCI_INIT_CONFIG pInitConfig);
uint32_t WINAPI VCI_InitCANEx(uint32_t DevType, uint32_t DevIndex, uint32_t CANIndex, PVCI_INIT_CONFIG_EX pInitConfig);

uint32_t WINAPI VCI_ReadBoardInfo(uint32_t DevType,uint32_t DevIndex,PVCI_BOARD_INFO pInfo);
uint32_t WINAPI VCI_ReadBoardInfoEx(uint32_t DevIndex, PVCI_BOARD_INFO_EX pInfo);
uint32_t WINAPI VCI_ReadErrInfo(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex,PVCI_ERR_INFO pErrInfo);
uint32_t WINAPI VCI_ReadCANStatus(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex,PVCI_CAN_STATUS pCANStatus);

uint32_t WINAPI VCI_GetReference(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex,uint32_t RefType,void *pData);
uint32_t WINAPI VCI_SetReference(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex,uint32_t RefType,void *pData);
uint32_t WINAPI VCI_SetFilter(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex,PVCI_FILTER_CONFIG pFilter);

uint32_t WINAPI VCI_GetReceiveNum(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex);
uint32_t WINAPI VCI_ClearBuffer(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex);

uint32_t WINAPI VCI_StartCAN(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex);
uint32_t WINAPI VCI_ResetCAN(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex);

uint32_t WINAPI VCI_Transmit(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex,PVCI_CAN_OBJ pSend,uint32_t Len);
uint32_t WINAPI VCI_Receive(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex,PVCI_CAN_OBJ pReceive,uint32_t Len,uint32_t WaitTime=0);

uint32_t WINAPI VCI_RegisterReceiveCallback(uint32_t DevIndex,PVCI_RECEIVE_CALLBACK pReceiveCallBack);
uint32_t WINAPI VCI_LogoutReceiveCallback(uint32_t DevIndex);

int32_t WINAPI VCI_SetUserKey(int32_t DevType,int32_t DevIndex,uint8_t* pUserKey);
int32_t WINAPI VCI_CheckUserKey(int32_t DevType,int32_t DevIndex,uint8_t* pUserKey);
#ifdef __cplusplus
}
#endif

#endif

