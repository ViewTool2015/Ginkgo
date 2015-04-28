  /*
  ******************************************************************************
  * @file     : ControlCAN.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 9:28
  * @brief    : ControlCAN demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
#ifndef _CONTROLCAN_H_
#define _CONTROLCAN_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#define WINAPI
#endif

//CAN type definition
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4


//CAN error code
#define	ERR_CAN_OVERFLOW			0x0001	//CAN controller internal FIFO overflow
#define	ERR_CAN_ERRALARM			0x0002	//CAN controller error alarm
#define	ERR_CAN_PASSIVE				0x0004	//CAN controller passive error
#define	ERR_CAN_LOSE				0x0008	//CAN controller arbitration lost
#define	ERR_CAN_BUSERR				0x0010	//CAN arbitration bus error
#define	ERR_CAN_BUSOFF				0x0020	//CAN arbitration bus off

//Generic error code
#define	ERR_DEVICEOPENED			0x0100	//Device is opened
#define	ERR_DEVICEOPEN				0x0200	//Device open failed
#define	ERR_DEVICENOTOPEN			0x0400	//Device not open
#define	ERR_BUFFEROVERFLOW			0x0800	//Buffer overflow
#define	ERR_DEVICENOTEXIST			0x1000	//Device is not exist
#define	ERR_LOADKERNELDLL			0x2000	//Load *.dll failed
#define ERR_CMDFAILED				0x4000	//Command execution failed
#define	ERR_BUFFERCREATE			0x8000	//Memory is not enough


//Function return status
#define	STATUS_OK					1
#define STATUS_ERR					0
	

//1.Compatible with other CAN adapter data type
typedef  struct  _VCI_BOARD_INFO{
	uint16_t	hw_Version;			//hardware version, in hex format, for example: 0x0100 present version is 1.00
	uint16_t	fw_Version;			//firmware version, in hex format
	uint16_t	dr_Version;			//driver version, in hex format
	uint16_t	in_Version;			//interface library version, in hex format
	uint16_t	irq_Num;			//interrupt number used by board
	uint8_t	can_Num;			//CAN channel number
	int8_t	str_Serial_Num[20];	//CAN board serial number
	int8_t	str_hw_Type[40];	//string for hardware type, for example: ¡°USBCAN V1.00\0¡±(note: include string null end'\0').
	uint16_t	Reserved[4];		//reserved
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//1.Ginkgo serial board data type
typedef  struct  _VCI_BOARD_INFO_EX{
	uint8_t		ProductName[32];	//hardware name,for example: ¡°Ginkgo-CAN-Adapter\0¡±(note: include string null end'\0¡¯)
	uint8_t		FirmwareVersion[4];	//firmware version
	uint8_t		HardwareVersion[4];	//hardware version
	uint8_t		SerialNumber[12];	//adapter serial number
} VCI_BOARD_INFO_EX,*PVCI_BOARD_INFO_EX; 

//2.Definition of CAN frame
typedef  struct  _VCI_CAN_OBJ{
	uint32_t	ID;			//text ID.
	uint32_t	TimeStamp;	//timestamp of the frame arriving, started from initialization of CAN controller
	uint8_t	TimeFlag;	// if using timestamp, 1: use TimeStamp, 0£ºnot use. TimeFlag and TimeStamp is available when the frame is received frame
	uint8_t	SendType;	//send frame type. 0: normal send, 1: single send, 2: self send/receive, 3: single self send/receive, only available when 
						//the frame is send frame.(when device type is EG20T-CAN, send type will be set at VCI_InitCan and it's invalid set herein
						//When set to self send/receive mode, EG20T-CAN can not receive from bus, only can receive from itself)
	uint8_t	RemoteFlag;	//remote frame flag
	uint8_t	ExternFlag;	//extended frame flag
	uint8_t	DataLen;	//Data length(<=8), how many uint8_ts of data
	uint8_t	Data[8];	//text data
	uint8_t	Reserved[3];//reserved
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.definition of CAN controller status
typedef struct _VCI_CAN_STATUS{
	uint8_t	ErrInterrupt;	//interrupt record, will be cleared while reading
	uint8_t	regMode;		//CAN controller mode register
	uint8_t	regStatus;		//CAN controller status register
	uint8_t	regALCapture;	//CAN controller arbitrator lost register
	uint8_t	regECCapture;	//CAN controller error register
	uint8_t	regEWLimit;		//CAN controller error alarm limitation register
	uint8_t	regRECounter;	//CAN controller receive error register
	uint8_t	regTECounter;	//CAN controller send error register
	uint32_t	regESR;			//CAN controller status register
	uint32_t	regTSR;			//CAN controller status register
	uint32_t	BufferSize;		//CAN controller receive buffer size
	uint32_t	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.definition of error data type
typedef struct _ERR_INFO{
	uint32_t	ErrCode;			//error code
	uint8_t	Passive_ErrData[3];	//error identification data when error has passive error
	uint8_t	ArLost_ErrData;		//error identification data when error has arbitration lost error
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.definition of CAN initialization data type
typedef struct _INIT_CONFIG{
	uint32_t	AccCode;	//ACC code (for verification)
	uint32_t	AccMask;	//Mask code
	uint32_t	Reserved;	//reserved
	uint8_t	Filter;		//filter type.0: double filter, 1: single filter
	uint8_t	Timing0;	//Timer 0(BTR0).	
	uint8_t	Timing1;	//Timer 1(BTR1).	
	uint8_t	Mode;		//Mode
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

//Definition of CAN initialization data type
typedef struct _INIT_CONFIG_EX
{
	//CAN baud rate = 36MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
	uint32_t	CAN_BRP;	//range: 1~1024
	uint8_t	CAN_SJW;	//range: 1~4          
	uint8_t	CAN_BS1;	//range: 1~16          
	uint8_t	CAN_BS2;	//range: 1~8          
	uint8_t	CAN_Mode;	//CAN working mode. 0: normal, 1: loop back, 2: silent, 3: silent loop back
	uint8_t	CAN_ABOM;	//auto off line management, 0: prohibit, 1: enable
	uint8_t	CAN_NART;	//text repeat send management, 0: enable text repeat sending, 1: disable text repeat sending
	uint8_t	CAN_RFLM;	//FIFO lock management, 0: new text overwrite old, 1: ignore new text
	uint8_t	CAN_TXFP;	//send priority management, 0: by ID, 1: by order
	uint8_t	CAN_RELAY;	//relay feature enable, 0x00: close relay function, 0x10: relay from CAN1 to CAN2, 0x01: relay from CAN2 to CAN1, 0x11: bidirectionally relay
	uint32_t	Reserved;	//reserved
}VCI_INIT_CONFIG_EX,*PVCI_INIT_CONFIG_EX;


//6.definition of CAN filter setting
typedef struct _VCI_FILTER_CONFIG{
	uint8_t	Enable;			//filter enable, 1: enable, 0: disable
	uint8_t	FilterIndex;	//filter index, range: 0~13
	uint8_t	FilterMode;		//filter mode, 0: mask bit, 1: id list
	uint8_t	ExtFrame;		//filter frame flag, 1: the frame to be filtered is extended frame, 0£ºthe frame to be filtered is standard frame
	uint32_t	ID_Std_Ext;		//verification code ID
	uint32_t	ID_IDE;			//verification code IDE
	uint32_t	ID_RTR;			//verification code RTR
	uint32_t	MASK_Std_Ext;	//Mask code ID, only available when filter mode set to mask bit mode
	uint32_t	MASK_IDE;		//Mask code IDE, only available when filter mode set to mask bit mode
	uint32_t	MASK_RTR;		//Mask code RTR, only available when filter mode set to mask bit mode
	uint32_t	Reserved;		//reserved
} VCI_FILTER_CONFIG,*PVCI_FILTER_CONFIG;

typedef void(WINAPI *PVCI_RECEIVE_CALLBACK)(uint32_t DevIndex,uint32_t CANIndex,uint32_t Len);

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

uint32_t WINAPI VCI_RegisterReceiveCallback(uint32_t DevIndex,PVCI_RECEIVE_CALLBACK pReceiveCallBack);
uint32_t WINAPI VCI_LogoutReceiveCallback(uint32_t DevIndex);

uint32_t WINAPI VCI_Transmit(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex,PVCI_CAN_OBJ pSend,uint32_t Len);
uint32_t WINAPI VCI_Receive(uint32_t DevType,uint32_t DevIndex,uint32_t CANIndex,PVCI_CAN_OBJ pReceive,uint32_t Len,uint32_t WaitTime=0);

#ifdef __cplusplus
}
#endif

#endif

