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
	USHORT	hw_Version;			//硬件版本号，用16 进制表示。比如0x0100 表示V1.00。
	USHORT	fw_Version;			//固件版本号，用16 进制表示。
	USHORT	dr_Version;			//驱动程序版本号，用16 进制表示。
	USHORT	in_Version;			//接口库版本号，用16 进制表示。
	USHORT	irq_Num;			//板卡所使用的中断号。
	BYTE	can_Num;			//表示有几路CAN 通道。
	CHAR	str_Serial_Num[20];	//此板卡的序列号。
	CHAR	str_hw_Type[40];	//硬件类型，比如“USBCAN V1.00”（注意：包括字符串结束符‘\0’）。
	USHORT	Reserved[4];		//系统保留。
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//1.Ginkgo系列接口卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO_EX{
	USHORT	dr_Version;			//驱动程序版本号，用16 进制表示。
	USHORT	in_Version;			//接口库版本号，用16 进制表示。
	BYTE	can_Num;			//表示有几路CAN 通道。
	CHAR	str_hw_Name[100];	//硬件名称，比如“Ginkgo-CAN-Adaptor”（注意：包括字符串结束符‘\0’）
	CHAR	str_fw_Version[100];//固件版本字符串，比如“2.0.0”（注意：包括字符串结束符‘\0’）
	CHAR	str_hw_Version[100];//硬件版本字符串，比如“2.1.0”（注意：包括字符串结束符‘\0’）
	CHAR	str_Serial_Num[100];//适配器序列号字符串
	USHORT	Reserved[4];		//系统保留。
} VCI_BOARD_INFO_EX,*PVCI_BOARD_INFO_EX; 

//2.定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
	UINT	ID;			//报文ID。
	UINT	TimeStamp;	//接收到信息帧时的时间标识，从CAN 控制器初始化开始计时。
	BYTE	TimeFlag;	//是否使用时间标识，为1 时TimeStamp 有效，TimeFlag 和TimeStamp 只在此帧为接收帧时有意义。
	BYTE	SendType;	//发送帧类型，=0 时为正常发送，=1 时为单次发送，=2 时为自发自收，=3 时为单次自发自收，只在此
						//帧为发送帧时有意义。（当设备类型为EG20T-CAN 时，发送方式在VCI_InitCan 中通过设置，此处的
						//设置无效，设置为自发自收模式后EG20T-CAN 不能从总线上接收数据，只能收到自己发出的数据）
	BYTE	RemoteFlag;	//是否是远程帧
	BYTE	ExternFlag;	//是否是扩展帧
	BYTE	DataLen;	//数据长度(<=8)，即Data 的长度。
	BYTE	Data[8];	//报文的数据。
	BYTE	Reserved[3];//系统保留。
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
	UCHAR	ErrInterrupt;	//中断记录，读操作会清除。
	UCHAR	regMode;		//CAN 控制器模式寄存器。
	UCHAR	regStatus;		//CAN 控制器状态寄存器。
	UCHAR	regALCapture;	//CAN 控制器仲裁丢失寄存器。
	UCHAR	regECCapture;	//CAN 控制器错误寄存器。 
	UCHAR	regEWLimit;		//CAN 控制器错误警告限制寄存器。
	UCHAR	regRECounter;	//CAN 控制器接收错误寄存器。 
	UCHAR	regTECounter;	//CAN 控制器发送错误寄存器。
	UINT	regESR;			//CAN 控制器错误状态寄存器。
	UINT	regTSR;			//CAN 控制器发送状态寄存器
	UINT	BufferSize;		//CAN 控制器接收缓冲区大小
	DWORD	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.定义错误信息的数据类型。
typedef struct _ERR_INFO{
	UINT	ErrCode;			//错误码
	BYTE	Passive_ErrData[3];	//当产生的错误中有消极错误时表示为消极错误的错误标识数据。
	BYTE	ArLost_ErrData;		//当产生的错误中有仲裁丢失错误时表示为仲裁丢失错误的错误标识数据。
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.定义初始化CAN的数据类型
typedef struct _INIT_CONFIG{
	DWORD	AccCode;	//验收码
	DWORD	AccMask;	//屏蔽码
	DWORD	Reserved;	//保留
	UCHAR	Filter;		//滤波方式,0-双滤波，1-单滤波
	UCHAR	Timing0;	//定时器0（BTR0）。	
	UCHAR	Timing1;	//定时器1（BTR1）。	
	UCHAR	Mode;		//模式。
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

//定义初始化CAN的数据类型
typedef struct _INIT_CONFIG_EX
{
	//CAN波特率 = 36MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
	DWORD	CAN_BRP;	//取值范围1~1024
	UCHAR	CAN_SJW;	//取值范围1~4          
	UCHAR	CAN_BS1;	//取值范围1~16          
	UCHAR	CAN_BS2;	//取值范围1~8          
	UCHAR	CAN_Mode;	//CAN工作模式，0-正常模式，1-环回模式，2-静默模式，3-静默环回模式
	UCHAR	CAN_ABOM;	//自动离线管理，0-禁止，1-使能
	UCHAR	CAN_NART;	//报文重发管理，0-使能报文重传，1-禁止报文重传
	UCHAR	CAN_RFLM;	//FIFO锁定管理，0-新报文覆盖旧报文，1-丢弃新报文
	UCHAR	CAN_TXFP;	//发送优先级管理，0-标识符决定，1-发送请求顺序决定
	UCHAR	CAN_RELAY;	//是否开启中继功能，0x00-关闭中继功能，0x10-CAN1到CAN2中继，0x01-CAN2到CAN1中继，0x11-双向中继
	DWORD	Reserved;	//系统保留
}VCI_INIT_CONFIG_EX,*PVCI_INIT_CONFIG_EX;


//6.定义了CAN 滤波器的设置
typedef struct _VCI_FILTER_CONFIG{
	UCHAR	Enable;			//使能该过滤器，1-使能，0-禁止
	UCHAR	FilterIndex;	//过滤器索引号，取值范围为0到13
	UCHAR	FilterMode;		//过滤器模式，0-屏蔽位模式，1-标识符列表模式
	UCHAR	ExtFrame;		//过滤的帧类型标志，为1 代表要过滤的为扩展帧，为0 代表要过滤的为标准帧。
	DWORD	ID_Std_Ext;		//验收码ID
	DWORD	ID_IDE;			//验收码IDE
	DWORD	ID_RTR;			//验收码RTR
	DWORD	MASK_Std_Ext;	//屏蔽码ID，该项只有在过滤器模式为屏蔽位模式时有用
	DWORD	MASK_IDE;		//屏蔽码IDE，该项只有在过滤器模式为屏蔽位模式时有用
	DWORD	MASK_RTR;		//屏蔽码RTR，该项只有在过滤器模式为屏蔽位模式时有用
	DWORD	Reserved;		//系统保留
} VCI_FILTER_CONFIG,*PVCI_FILTER_CONFIG;

#ifdef __cplusplus
extern "C"
{
#endif

DWORD WINAPI VCI_ScanDevice(UCHAR NeedInit);
DWORD WINAPI VCI_OpenDevice(DWORD DevType,DWORD DevIndex,DWORD Reserved);
DWORD WINAPI VCI_CloseDevice(DWORD DevType,DWORD DevIndex);
DWORD WINAPI VCI_InitCAN(DWORD DevType, DWORD DevIndex, DWORD CANIndex, PVCI_INIT_CONFIG pInitConfig);
DWORD WINAPI VCI_InitCANEx(DWORD DevType, DWORD DevIndex, DWORD CANIndex, PVCI_INIT_CONFIG_EX pInitConfig);

DWORD WINAPI VCI_ReadBoardInfo(DWORD DevType,DWORD DevIndex,PVCI_BOARD_INFO pInfo);
DWORD WINAPI VCI_ReadBoardInfoEx(DWORD DevType, DWORD DevIndex, PVCI_BOARD_INFO_EX pInfo);
DWORD WINAPI VCI_ReadErrInfo(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_ERR_INFO pErrInfo);
DWORD WINAPI VCI_ReadCANStatus(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_CAN_STATUS pCANStatus);

DWORD WINAPI VCI_GetReference(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD RefType,PVOID pData);
DWORD WINAPI VCI_SetReference(DWORD DevType,DWORD DevIndex,DWORD CANIndex,DWORD RefType,PVOID pData);
DWORD WINAPI VCI_SetFilter(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_FILTER_CONFIG pFilter);

ULONG WINAPI VCI_GetReceiveNum(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
DWORD WINAPI VCI_ClearBuffer(DWORD DevType,DWORD DevIndex,DWORD CANIndex);

DWORD WINAPI VCI_StartCAN(DWORD DevType,DWORD DevIndex,DWORD CANIndex);
DWORD WINAPI VCI_ResetCAN(DWORD DevType,DWORD DevIndex,DWORD CANIndex);

ULONG WINAPI VCI_Transmit(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_CAN_OBJ pSend,ULONG Len);
ULONG WINAPI VCI_Receive(DWORD DevType,DWORD DevIndex,DWORD CANIndex,PVCI_CAN_OBJ pReceive,ULONG Len,INT WaitTime=-1);

#ifdef __cplusplus
}
#endif

#endif

