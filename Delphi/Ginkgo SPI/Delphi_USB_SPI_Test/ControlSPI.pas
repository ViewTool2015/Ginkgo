unit ControlSPI;

interface
const
//Device type definition
VAI_USBADC	=	1;
VCI_USBCAN1	=	3;
VCI_USBCAN2	=	4;
VGI_USBGPIO	=	1;
VII_USBI2C	=	1;
VSI_USBSPI	=	2;
//错误码定义
ERR_SUCCESS					      =	(0);		// 没有错误
ERR_PARAMETER_NULL			  =	(-1);	  // 传入的指针为空指针
ERR_INPUT_DATA_TOO_MUCH		=	(-2);	  // 参数输入个数多余规定个数
ERR_INPUT_DATA_TOO_LESS		=	(-3);	  // 参数输入个数少余规定个数
ERR_INPUT_DATA_ILLEGALITY	=	(-4);	  // 参数传入格式和规定的不符合
ERR_USB_WRITE_DATA			  =	(-5);	  // USB写数据错误
ERR_USB_READ_DATA			    =	(-6);	  // USB读数据错误
ERR_READ_NO_DATA			    =	(-7);	  // 请求读数据时返回没有数据
ERR_OPEN_DEVICE				    =	(-8);	  // 打开设备失败
ERR_CLOSE_DEVICE			    =	(-9);	  // 关闭设备失败
ERR_EXECUTE_CMD				    =	(-10);	// 设备执行命令失败
ERR_SELECT_DEVICE			    =	(-11);	// 选择设备失败
ERR_DEVICE_OPENED			    =	(-12);	// 设备已经打开
ERR_DEVICE_NOTOPEN			  =	(-13);	// 设备没有打开
ERR_BUFFER_OVERFLOW			  =	(-14);	// 缓冲区溢出
ERR_DEVICE_NOTEXIST			  =	(-15);	// 此设备不存在
ERR_LOAD_KERNELDLL			  =	(-16);	// 装载动态库失败
ERR_CMD_FAILED				    =	(-17);	// 执行命令失败错误码
ERR_BUFFER_CREATE			    =	(-18);	// 内存不足
//Ginkgo系列适配器信息的数据类型。
type
PVSI_BOARD_INFO = ^VSI_BOARD_INFO;
VSI_BOARD_INFO = record
	ProductName:Array[0..31] Of Byte;	    //硬件名称，比如“Ginkgo-SPI-Adaptor”（注意：包括字符串结束符‘\0’）
	FirmwareVersion:Array[0..3] Of Byte;	//固件版本
	HardwareVersion:Array[0..3] Of Byte;	//硬件版本
	SerialNumber:Array[0..11] Of Byte;	  //适配器序列号
end;
//USB-SPI初始化结构体
type
PVSI_INIT_CONFIG = ^VSI_INIT_CONFIG;
VSI_INIT_CONFIG = record
  ControlMode:Byte;     //SPI控制方式:0-硬件控制（全双工模式）,1-硬件控制（半双工模式），2-软件控制（半双工模式）,3-单总线模式，数据线输入输出都为MOSI
  TranBits:Byte;        //数据传输字节宽度，在8和16之间取值
  MasterMode:Byte;      //主从选择控制:0-从机，1-主机
  CPOL:Byte;            //时钟极性控制:0-SCK高有效，1-SCK低有效
  CPHA:Byte;            //时钟相位控制:0-第一个SCK时钟采样，1-第二个SCK时钟采样
  LSBFirst:Byte;        //数据移位方式:0-MSB在前，1-LSB在前
  SelPolarity:Byte;     //片选信号极性:0-低电平选中，1-高电平选中
  ClockSpeed:Integer;   //SPI时钟频率:单位为HZ
end;
//函数声明
function VSI_ScanDevice(NeedInit:Byte):Integer; stdcall;
function VSI_OpenDevice(DevType,DevIndex,Reserved:Integer):Integer; stdcall;
function VSI_CloseDevice(DevType,DevIndex:Integer):Integer; stdcall;
function VSI_ReadBoardInfo(DevIndex:Integer;pInfo:PVSI_BOARD_INFO):Integer; stdcall;
function VSI_InitSPI(DevType, DevIndex:Integer; pInitConfig:PVSI_INIT_CONFIG):Integer; stdcall;
function VSI_WriteBytes(DevType, DevIndex, SPIIndex:Integer;pWriteData:PByte;Len:Word):Integer; stdcall;
function VSI_ReadBytes(DevType, DevIndex, SPIIndex:Integer;pReadData:PByte;Len:Word):Integer; stdcall;
function VSI_WriteReadBytes(DevType, DevIndex, SPIIndex:Integer;pWriteData:PByte;WriteLen:Word;pReadData:PByte;ReadLen:Word):Integer; stdcall;
function VSI_WriteBits(DevType, DevIndex, SPIIndex:Integer;pWriteBitStr:PChar):Integer; stdcall;
function VSI_ReadBits(DevType, DevIndex, SPIIndex:Integer;pReadBitStr:PChar;ReadBitsNum:Integer):Integer; stdcall;
function VSI_WriteReadBits(DevType, DevIndex, SPIIndex:Integer;pWriteBitStr,pReadBitStr:PChar;ReadBitsNum:Integer):Integer; stdcall;
function VSI_SlaveReadBytes(DevType,DevIndex:Integer;pReadData:PByte;pBytesNum:PInteger;WaitTime:Integer):Integer; stdcall;
function VSI_SlaveWriteBytes(DevType,DevIndex:Integer;pWriteData:PByte;WriteBytesNum:Integer):Integer; stdcall;
function VSI_SetUserKey(DevType,DevIndex:Integer;pUserKey:PByte):Integer; stdcall;
function VSI_CheckUserKey(DevType,DevIndex:Integer;pUserKey:PByte):Integer; stdcall;

implementation

function VSI_ScanDevice;external 'Ginkgo_Driver.dll' name 'VSI_ScanDevice';
function VSI_OpenDevice;external 'Ginkgo_Driver.dll' name 'VSI_OpenDevice';
function VSI_CloseDevice;external 'Ginkgo_Driver.dll' name 'VSI_CloseDevice';
function VSI_ReadBoardInfo;external 'Ginkgo_Driver.dll' name 'VSI_ReadBoardInfo';
function VSI_InitSPI;external 'Ginkgo_Driver.dll' name 'VSI_InitSPI';
function VSI_WriteBytes;external 'Ginkgo_Driver.dll' name 'VSI_WriteBytes';
function VSI_ReadBytes;external 'Ginkgo_Driver.dll' name 'VSI_ReadBytes';
function VSI_WriteReadBytes;external 'Ginkgo_Driver.dll' name 'VSI_WriteReadBytes';
function VSI_WriteBits;external 'Ginkgo_Driver.dll' name 'VSI_WriteBits';
function VSI_ReadBits;external 'Ginkgo_Driver.dll' name 'VSI_ReadBits';
function VSI_WriteReadBits;external 'Ginkgo_Driver.dll' name 'VSI_WriteReadBits';
function VSI_SlaveReadBytes;external 'Ginkgo_Driver.dll' name 'VSI_SlaveReadBytes';
function VSI_SlaveWriteBytes;external 'Ginkgo_Driver.dll' name 'VSI_SlaveWriteBytes';
function VSI_SetUserKey;external 'Ginkgo_Driver.dll' name 'VSI_SlaveWriteBytes';
function VSI_CheckUserKey;external 'Ginkgo_Driver.dll' name 'VSI_CheckUserKey';
end.
