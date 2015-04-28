unit ControlCAN;

interface

const
//Device type definition
VCI_USBCAN1	=	3;
VCI_USBCAN2	=	4;
//CAN error code
ERR_CAN_OVERFLOW		= $0001;	//CAN controller internal FIFO overflow
ERR_CAN_ERRALARM		= $0002;	//CAN controller error alarm
ERR_CAN_PASSIVE			= $0004;	//CAN controller passive error
ERR_CAN_LOSE				= $0008;	//CAN controller arbitration lost
ERR_CAN_BUSERR			= $0010;	//CAN arbitration bus error
ERR_CAN_BUSOFF			= $0020;	//CAN arbitration bus off

//Generic error code
ERR_DEVICEOPENED			= $0100;	//Device is opened
ERR_DEVICEOPEN				= $0200;	//Device open failed
ERR_DEVICENOTOPEN			= $0400;	//Device not open
ERR_BUFFEROVERFLOW		= $0800;	//Buffer overflow
ERR_DEVICENOTEXIST		= $1000;	//Device is not exist
ERR_LOADKERNELDLL			= $2000;	//Load dll failed
ERR_CMDFAILED				  = $4000;	//Command execution failed
ERR_BUFFERCREATE			= $8000;	//Memory is not enough


//Function return status
STATUS_OK					= 1;
STATUS_ERR				= 0;

//Compatible with other CAN adapter datatype
type
PVCI_BOARD_INFO = ^VCI_BOARD_INFO;
VCI_BOARD_INFO = record
	hw_Version:Word;			//hardware version, in hex format, for example: 0x0100 present version is 1.00
	fw_Version:Word;			//firmware version£¬in hex format
	dr_Version:Word;			//driver version, in hex format
	in_Version:Word;			//interface library version, in hex format
	irq_Num:Word;			    //interrupt number used by board
	can_Num:Byte;			    //CAN channel number
	str_Serial_Num:Array[0..19] Of Byte;	//CAN board serial number
	str_hw_Type:Array[0..39] Of Byte;	    //string for hardware type£¬for example: ¡°USBCAN V1.00\0¡±£¨note: include string null end¡®\0¡¯£©¡£
	Reserved:Array[0..3] Of Word;		      //reserved
end;
//Ginkgo serial board data type
type
PVCI_BOARD_INFO_EX = ^VCI_BOARD_INFO_EX;
VCI_BOARD_INFO_EX = record
	ProductName:Array[0..31] Of Byte;	    //hardware name,for example:"Ginkgo-CAN-Adaptor\0"(note: include string null end '\0')
	FirmwareVersion:Array[0..3] Of Byte;	//firmware version
	HardwareVersion:Array[0..3] Of Byte;	//hardware version
	SerialNumber:Array[0..11] Of Byte;	  //adatper serial number
end;
//Definition of CAN frame
type
PVCI_CAN_OBJ = ^VCI_CAN_OBJ;
VCI_CAN_OBJ = record
	ID:Longword;			    //text ID¡£
	TimeStamp:Longword;	  //timestamp of the frame arriving£¬started from initialization of CAN controller
	TimeFlag:Byte;	      // if using timestamp£¬1: use TimeStamp, 0£ºnot use. TimeFlag and TimeStamp is available when the frame is recived frame
	SendType:Byte;	      //send frame type. 0: normal send£¬1: single send£¬2: self send/receive£¬3: single self send/receive£¬only available when
						            //the frame is send frame.£¨when device type is EG20T-CAN£¬send type will be set at VCI_InitCan and it's invalid set herein
                        //When set to self send/receive mode, EG20T-CAN can not receive from bus£¬only can receive from itself£©
	RemoteFlag:Byte;	    //remote frame flag
	ExternFlag:Byte;	    //extended frame flag
	DataLen:Byte;	        //Data length(<=8)£¬how many uint8_ts of data
	Data:Array[0..7] Of Byte;	    //text data
	Reserved:Array[0..2] Of Byte; //reserved
end;
//definition of CAN controller status
type
PVCI_CAN_STATUS = ^VCI_CAN_STATUS;
VCI_CAN_STATUS = record
	ErrInterrupt:Byte;	  //interrupt record£¬will be cleared while reading
	regMode:Byte;		      //CAN controller mode register
	regStatus:Byte;		    //CAN controller status register
	regALCapture:Byte;	  //CAN controller arbitrator lost register
	regECCapture:Byte;	  //CAN controller error register
	regEWLimit:Byte;		  //CAN controller error alarm limitation register
	regRECounter:Byte;	  //CAN controller receive error register
	regTECounter:Byte;	  //CAN controller send error register
	regESR:Longword;	    //CAN controller status register
	regTSR:Longword;		  //CAN controller status register
	BufferSize:Longword;  //CAN controller receive buffer size
	Reserved:Longword;
end;

//definition of error data type
type
PVCI_ERR_INFO = ^VCI_ERR_INFO;
VCI_ERR_INFO = record
	ErrCode:Longword;			//error code
	Passive_ErrData:Array[0..2] Of Byte;	//error identification data when error has passive error
	ArLost_ErrData:Byte;		//error identification data when error has arbitration lost error
end;
//definition of CAN initialization data type
type
PVCI_INIT_CONFIG = ^VCI_INIT_CONFIG;
VCI_INIT_CONFIG = record
	AccCode:Longword;	    //ACC code (for verification)
	AccMask:Longword;	    //Mask code
	Reserved:Longword;	  //reserved
	Filter:Byte;		      //filter type.0: double filter£¬1: single filter
	Timing0:Byte;	        //Timer 0£¨BTR0£©
	Timing1:Byte;	        //Timer 1£¨BTR1£©
	Mode:Byte;		        //Mode
end;
//Definition of CAN initialization data type
type
PVCI_INIT_CONFIG_EX = ^VCI_INIT_CONFIG_EX;
VCI_INIT_CONFIG_EX = record
	//CAN baudrate = 36MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
	CAN_BRP:Longword;	  //range: 1~1024
	CAN_SJW:Byte;	      //range: 1~4
	CAN_BS1:Byte;	      //range: 1~16
	CAN_BS2:Byte;	      //range: 1~8
	CAN_Mode:Byte;	    //CAN working mode. 0: normal£¬1: loopback£¬2: silent£¬3: silent loopback
	CAN_ABOM:Byte;	    //auto off line management£¬0: prohibit£¬1: enable
	CAN_NART:Byte;	    //text repeat send management£¬0: enable text repeat sending£¬1: disable text repeat sending
	CAN_RFLM:Byte;	    //FIFO lock management£¬0: new text overwrite old£¬1: ignore new text
	CAN_TXFP:Byte;	    //send priority management, 0: by ID£¬1: by order
	CAN_RELAY:Byte;	    //relay feature enable£¬0x00: close relay function£¬0x10: relay from CAN1 to CAN2£¬0x01: relay from CAN2 to CAN1£¬0x11: bidirectionaly relay
	Reserved:Longword;	//reserved
end;
//definition of CAN filter setting
type
PVCI_FILTER_CONFIG = ^VCI_FILTER_CONFIG;
VCI_FILTER_CONFIG = record
	Enable:Byte;			    //filter enable£¬1: enable£¬0: disable
	FilterIndex:Byte;	    //filter index£¬range: 0~13
	FilterMode:Byte;		  //filter mode£¬0: mask bit£¬1: id list
	ExtFrame:Byte;		    //filter frame flag£¬1: the frame to be filtered is extended frame£¬0£ºthe frame to be filtered is standard frame
	ID_Std_Ext:Longword;	//verification code ID
	ID_IDE:Longword;			//verification code IDE
	ID_RTR:Longword;			//verification code RTR
	MASK_Std_Ext:Longword;//Mask code ID£¬only available when filter mode set to mask bit mode
	MASK_IDE:Longword;		//Mask code IDE£¬only available when filter mode set to mask bit mode
	MASK_RTR:Longword;		//Mask code RTR£¬only available when filter mode set to mask bit mode
	Reserved:Longword;		//reserved
end;

type
PVCI_RECEIVE_CALLBACK = procedure(DevIndex,CANIndex,Len:integer);stdcall;


function VCI_ScanDevice(NeedInit:Byte):Integer; stdcall;
function VCI_OpenDevice(DevType,DevIndex,Reserved:Integer):Integer; stdcall;
function VCI_CloseDevice(DevType,DevIndex:Integer):Integer; stdcall;
function VCI_InitCAN(DevType, DevIndex, CANIndex:Integer;pInitConfig:PVCI_INIT_CONFIG):Integer; stdcall;
function VCI_InitCANEx(DevType, DevIndex, CANIndex:Integer;pInitConfig:PVCI_INIT_CONFIG_EX):Integer; stdcall;

function VCI_ReadBoardInfo(DevType,DevIndex:Integer;pInfo:PVCI_BOARD_INFO):Integer; stdcall;
function VCI_ReadBoardInfoEx(DevIndex:Integer;pInfo:PVCI_BOARD_INFO_EX):Integer; stdcall;
function VCI_ReadErrInfo(DevType,DevIndex,CANIndex:Integer;pErrInfo:PVCI_ERR_INFO):Integer; stdcall;
function VCI_ReadCANStatus(DevType,DevIndex,CANIndex:Integer;pCANStatus:PVCI_CAN_STATUS):Integer; stdcall;

function VCI_SetFilter(DevType,DevIndex,CANIndex:Integer;pFilter:PVCI_FILTER_CONFIG):Integer; stdcall;

function VCI_GetReceiveNum(DevType,DevIndex,CANIndex:Integer):Integer; stdcall;
function VCI_ClearBuffer(DevType,DevIndex,CANIndex:Integer):Integer; stdcall;

function VCI_StartCAN(DevType,DevIndex,CANIndex:Integer):Integer; stdcall;
function VCI_ResetCAN(DevType,DevIndex,CANIndex:Integer):Integer; stdcall;

function VCI_RegisterReceiveCallback(DevIndex:Integer;pReceiveCallBack:PVCI_RECEIVE_CALLBACK):Integer; stdcall;
function VCI_LogoutReceiveCallback(DevIndex:Integer):Integer; stdcall;

function VCI_Transmit(DevType,DevIndex,CANIndex:Integer;pSend:PVCI_CAN_OBJ;Len:Integer):Integer; stdcall;
function VCI_Receive(DevType,DevIndex,CANIndex:Integer;pReceive:PVCI_CAN_OBJ;Len,WaitTime:Integer):Integer; stdcall;


implementation

function VCI_ScanDevice;external 'Ginkgo_Driver.dll' name 'VCI_ScanDevice';
function VCI_OpenDevice;external 'Ginkgo_Driver.dll' name 'VCI_OpenDevice';
function VCI_CloseDevice;external 'Ginkgo_Driver.dll' name 'VCI_CloseDevice';
function VCI_InitCAN;external 'Ginkgo_Driver.dll' name 'VCI_InitCAN';
function VCI_InitCANEx;external 'Ginkgo_Driver.dll' name 'VCI_InitCANEx';
function VCI_ReadBoardInfo;external 'Ginkgo_Driver.dll' name 'VCI_ReadBoardInfo';
function VCI_ReadBoardInfoEx;external 'Ginkgo_Driver.dll' name 'VCI_ReadBoardInfoEx';
function VCI_ReadErrInfo;external 'Ginkgo_Driver.dll' name 'VCI_ReadErrInfo';
function VCI_ReadCANStatus;external 'Ginkgo_Driver.dll' name 'VCI_ReadCANStatus';
function VCI_SetFilter;external 'Ginkgo_Driver.dll' name 'VCI_SetFilter';
function VCI_GetReceiveNum;external 'Ginkgo_Driver.dll' name 'VCI_GetReceiveNum';
function VCI_ClearBuffer;external 'Ginkgo_Driver.dll' name 'VCI_ClearBuffer';
function VCI_StartCAN;external 'Ginkgo_Driver.dll' name 'VCI_StartCAN';
function VCI_ResetCAN;external 'Ginkgo_Driver.dll' name 'VCI_ResetCAN';
function VCI_RegisterReceiveCallback;external 'Ginkgo_Driver.dll' name 'VCI_RegisterReceiveCallback';
function VCI_LogoutReceiveCallback;external 'Ginkgo_Driver.dll' name 'VCI_LogoutReceiveCallback';
function VCI_Transmit;external 'Ginkgo_Driver.dll' name 'VCI_Transmit';
function VCI_Receive;external 'Ginkgo_Driver.dll' name 'VCI_Receive';

end.
