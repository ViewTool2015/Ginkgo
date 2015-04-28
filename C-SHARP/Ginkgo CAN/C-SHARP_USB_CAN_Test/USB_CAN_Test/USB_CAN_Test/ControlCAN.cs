using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Ginkgo
{
    class ControlCAN
    {
        //CAN Error
        public struct VCI_CAN_ERROR
        {
            public const Int32  ERR_CAN_OVERFLOW = 0x0001;//CAN controller internal FIFO overflow
            public const Int32  ERR_CAN_ERRALARM = 0x0002;//CAN controller error alarm
            public const Int32  ERR_CAN_PASSIVE  = 0x0004;//CAN controller passive error
            public const Int32  ERR_CAN_LOSE     = 0x0008;//CAN controller arbitration lost
            public const Int32  ERR_CAN_BUSERR   = 0x0010;//CAN arbitration bus error
            public const Int32  ERR_CAN_BUSOFF   = 0x0020;//CAN arbitration bus off
            public const Int32	ERR_DEVICEOPENED    =	0x0100;	//Device is opened
            public const Int32	ERR_DEVICEOPEN      =	0x0200;	//Device open failed
            public const Int32	ERR_DEVICENOTOPEN   =	0x0400;	//Device not open
            public const Int32	ERR_BUFFEROVERFLOW	=	0x0800;	//Buffer overflow
            public const Int32	ERR_DEVICENOTEXIST	=	0x1000;	//Device is not exist
            public const Int32  ERR_LOADKERNELDLL	=	0x2000;	//Load dll failed
            public const Int32  ERR_CMDFAILED		=	0x4000;	//Command execution failed
            public const Int32  ERR_BUFFERCREATE     =   0x8000;	//Memory is not enough
        }
        //Return status
        public struct VCI_STATUS
        {
            public const Int32 STATUS_OK = 0x0001;
            public const Int32 STATUS_ERR = 0x0000;
        }
        //Device type
        public struct VCI_DEVICE_TYPE
        {
            public const Int32 VCI_USBCAN1 = 3;
            public const Int32 VCI_USBCAN2 = 4;
        }
        //Compatible with other CAN adapter data type
        public struct VCI_BOARD_INFO
        {
	        public UInt16	hw_Version;			//hardware version, in hex format, for example: 0x0100 present version is 1.00
	        public UInt16	fw_Version;			//firmware version, in hex format
	        public UInt16	dr_Version;			//driver version, in hex format
	        public UInt16	in_Version;			//interface library version, in hex format
	        public UInt16	irq_Num;			//interrupt number used by board
	        public Byte		can_Num;			//CAN channel number
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
	        public Byte[]	str_Serial_Num; 	//CAN board serial number
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
	        public Byte[]	str_hw_Type;	    //string for hardware type, for example: “USBCAN V1.00\0”（note: include string null end‘\0’）。
	        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public UInt16[]	Reserved;		    //reserved
        }
        //Ginkgo serial board data type
        public struct VCI_BOARD_INFO_EX
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
	        public Byte[]		ProductName;	    //hardware name, for example: “Ginkgo-CAN-Adapter\0”（note: include string null end‘\0’）
	        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public Byte[]		FirmwareVersion;	//firmware version
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	        public Byte[]		HardwareVersion;	//hardware version
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 12)]
	        public Byte[]		SerialNumber;	    //adapter serial number
        }
        //Definition of CAN frame
        public struct VCI_CAN_OBJ
        {
            public UInt32   ID;         //text ID
            public UInt32   TimeStamp;  //timestamp of the frame arriving, started from initialization of CAN controller
            public Byte     TimeFlag;   //if using timestamp，1: use TimeStamp, 0：not use. TimeFlag and TimeStamp is available when the frame is recived frame
            public Byte     SendType;   //send frame type. 0: normal send，1: single send，2: self send/receive，3: single self send/receive，only available when
                                        //the frame is send frame.（when device type is EG20T-CAN，send type will be set at VCI_InitCan and it's invalid set herein
                                        //When set to self send/receive mode, EG20T-CAN can not receive from bus，only can receive from itself）
            public Byte     RemoteFlag; //remote frame flag
            public Byte     ExternFlag; //extended frame flag
            public Byte     DataLen;    //Data length(<=8)，how many uint8_ts of data
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public Byte[]   Data;       //text data
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public Byte[]   Reserved;
        }
        //definition of CAN controller status
        public struct VCI_CAN_STATUS
        {
            public Byte ErrInterrupt;	//interrupt record, will be cleared while reading
            public Byte regMode;		//CAN controller mode register
            public Byte regStatus;		//CAN controller status register
            public Byte regALCapture;	//CAN controller arbitrator lost register
            public Byte regECCapture;	//CAN controller error register
            public Byte regEWLimit;		//CAN controller error alarm limitation register
            public Byte regRECounter;	//CAN controller receive error register
            public Byte regTECounter;	//CAN controller send error register
            public UInt32 regESR;		//CAN controller status register
            public UInt32 regTSR;		//CAN controller status register
            public UInt32 BufferSize;	//CAN controller receive buffer size
            public UInt32 Reserved;
        }
        //definition of error data type
        public struct VCI_ERR_INFO
        {
	        public UInt32	ErrCode;			//error code
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
	        public Byte[]	Passive_ErrData;	//error identification data when error has passive error
            public Byte     ArLost_ErrData;		//error identification data when error has arbitration lost error
        }
        //definition of CAN initialization data type
        public struct VCI_INIT_CONFIG
        {
            public UInt32   AccCode;	//ACC code (for verification)
            public UInt32   AccMask;	//Mask code
            public UInt32   Reserved;	//reserved
            public Byte     Filter;		//filter type.0: double filter，1: single filter
            public Byte     Timing0;	//Timer 0 (BTR0)
            public Byte     Timing1;	//Timer 1 (BTR1)
            public Byte     Mode;		//Mode
        }
        //Definition of CAN initialization data type
        public struct VCI_INIT_CONFIG_EX
        {
            //CAN baud rate = 36MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
            public UInt32   CAN_BRP;	//range: 1~1024
            public Byte     CAN_SJW;	//range: 1~4          
            public Byte     CAN_BS1;	//range: 1~16          
            public Byte     CAN_BS2;	//range: 1~8          
            public Byte     CAN_Mode;	//CAN working mode. 0: normal，1: loopback，2: silent，3: silent loopback
            public Byte     CAN_ABOM;	//auto off line management，0: prohibit，1: enable
            public Byte     CAN_NART;	//text repeat send management，0: enable text repeat sending，1: disable text repeat sending
            public Byte     CAN_RFLM;	//FIFO lock management，0: new text overwrite old，1: ignore new text
            public Byte     CAN_TXFP;	//send priority management, 0: by ID，1: by order
            public Byte     CAN_RELAY;	//relay feature enable，0x00: close relay function，0x10: relay from CAN1 to CAN2，0x01: relay from CAN2 to CAN1，0x11: bidirectionaly relay
            public UInt32   Reserved;	//reserved
        }
        //definition of CAN filter setting
        public struct VCI_FILTER_CONFIG
        {
            public Byte     Enable;			//filter enable，1: enable，0: disable
            public Byte     FilterIndex;	//filter index, range: 0~13
            public Byte     FilterMode;		//filter mode，0: mask bit，1: id list
            public Byte     ExtFrame;		//filter frame flag，1: the frame to be filtered is extended frame，0：the frame to be filtered is standard frame
            public UInt32   ID_Std_Ext;		//verification code ID
            public UInt32   ID_IDE;			//verification code IDE
            public UInt32   ID_RTR;			//verification code RTR
            public UInt32   MASK_Std_Ext;	//Mask code ID，only available when filter mode set to mask bit mode
            public UInt32   MASK_IDE;		//Mask code IDE，only available when filter mode set to mask bit mode
            public UInt32   MASK_RTR;		//Mask code RTR，only available when filter mode set to mask bit mode
            public UInt32   Reserved;		//reserved
        }
        //Get data callback function
        public delegate void PVCI_RECEIVE_CALLBACK(UInt32 DevIndex, UInt32 CANIndex, UInt32 Len);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_ScanDevice(Byte NeedInit = 1);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_OpenDevice(UInt32 DevType,UInt32 DevIndex,UInt32 Reserved);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_CloseDevice(UInt32 DevType,UInt32 DevIndex);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_InitCAN(UInt32 DevType, UInt32 DevIndex, UInt32 CANIndex, ref VCI_INIT_CONFIG pInitConfig);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_InitCANEx(UInt32 DevType, UInt32 DevIndex, UInt32 CANIndex, ref VCI_INIT_CONFIG_EX pInitConfig);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_ReadBoardInfo(UInt32 DevType,UInt32 DevIndex,ref VCI_BOARD_INFO pInfo);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_ReadBoardInfoEx(UInt32 DevIndex, ref VCI_BOARD_INFO_EX pInfo);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_ReadErrInfo(UInt32 DevType,UInt32 DevIndex,UInt32 CANIndex,ref VCI_ERR_INFO pErrInfo);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_ReadCANStatus(UInt32 DevType,UInt32 DevIndex,UInt32 CANIndex,ref VCI_CAN_STATUS pCANStatus);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_SetFilter(UInt32 DevType,UInt32 DevIndex,UInt32 CANIndex,ref VCI_FILTER_CONFIG pFilter);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_GetReceiveNum(UInt32 DevType,UInt32 DevIndex,UInt32 CANIndex);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_ClearBuffer(UInt32 DevType,UInt32 DevIndex,UInt32 CANIndex);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_StartCAN(UInt32 DevType,UInt32 DevIndex,UInt32 CANIndex);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_ResetCAN(UInt32 DevType,UInt32 DevIndex,UInt32 CANIndex);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_RegisterReceiveCallback(UInt32 DevIndex,PVCI_RECEIVE_CALLBACK pReceiveCallBack);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_LogoutReceiveCallback(UInt32 DevIndex);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_Transmit(UInt32 DevType, UInt32 DevIndex, UInt32 CANIndex, [In]VCI_CAN_OBJ[] pSend, UInt32 Len);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern UInt32 VCI_Receive(UInt32 DevType, UInt32 DevIndex, UInt32 CANIndex, [Out]VCI_CAN_OBJ[] pReceive, UInt32 Len, UInt32 WaitTime = 0);

    }
}
