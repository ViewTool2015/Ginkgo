unit ControlI2C;

interface

const
//Device type definition
VAI_USBADC	=	1;
VCI_USBCAN1	=	3;
VCI_USBCAN2	=	4;
VGI_USBGPIO	=	1;
VII_USBI2C	=	1;
VSI_USBSPI	=	2;
//USB-I2C适配器初始化数据定义
VII_ADDR_7BIT		=	7;	    // 7bit地址模式
VII_ADDR_10BIT	=	10;	    // 10bit地址模式
VII_HCTL_MODE		=	1;	    // 硬件控制
VII_SCTL_MODE		=	2;	    // 软件控制
VII_MASTER			=	1;	    // 主机
VII_SLAVE			  =	0;	    // 从机
VII_SUB_ADDR_NONE	  =	0;	// 无子地址
VII_SUB_ADDR_1BYTE	=	1;	// 1Byte子地址
VII_SUB_ADDR_2BYTE	=	2;	// 2Byte子地址
VII_SUB_ADDR_3BYTE	=	3;	// 3Byte子地址
VII_SUB_ADDR_4BYTE	=	4;	// 4Byte子地址
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
//USB-I2C初始化结构体
type
PVII_INIT_CONFIG = ^VII_INIT_CONFIG;
VII_INIT_CONFIG = record
  MasterMode:Byte;    //主从选择控制:0-从机，1-主机
  ControlMode:Byte;   //控制方式:1-硬件控制，2-软件控制
  AddrType:Byte;      //7-7bit模式，10-10bit模式
  SubAddrWidth:Byte;  //子地址宽度，0到4取值，0时表示无子地址模式
  Addr:Word;          //从机模式时候的设备地址
  ClockSpeed:Longword;//时钟频率:单位为HZ
end;
//软件I2C时间参数定义，时间单位为微秒
type
PVII_TIME_CONFIG = ^VII_TIME_CONFIG;
VII_TIME_CONFIG = record
  tHD_STA:Word;             //起始信号保持时间
  tSU_STA:Word;             //起始信号建立时间
  tLOW:Word;                //时钟低电平时间
  tHIGH:Word;               //时钟高电平时间
  tSU_DAT:Word;             //数据输入建立时间
  tSU_STO:Word;             //停止信号建立时间
  tDH:Word;                 //数据输出保持时间
  tDH_DAT:Word;             //数据输入保持时间
  tAA:Word;                 //SCL变低至SDA数据输出及应答信号
  tR:Word;                  //SDA及SCL上升时间
  tF:Word;                  //SDA及SCL下降时间
  tBuf:Word;                //新的发送开始前总线空闲时间
  tACK:Array[0..3] Of Byte;
  tStart:Word;
  tStop:Word;
end;
//函数声明
function VII_ScanDevice(NeedInit:Byte):Integer; stdcall;
function VII_OpenDevice(DevType,DevIndex,Reserved:Integer):Integer; stdcall;
function VII_CloseDevice(DevType,DevIndex:Integer):Integer; stdcall;
function VII_InitI2C(DevType, DevIndex, I2CIndex:Integer; pInitConfig:PVII_INIT_CONFIG):Integer; stdcall;
function VII_WriteBytes(DevType, DevIndex, I2CIndex:Integer;Addr:Word;SubAddr:Integer;pWriteData:PByte;Len:Word):Integer; stdcall;
function VII_ReadBytes(DevType, DevIndex, I2CIndex:Integer;Addr:Word;SubAddr:Integer;pReadData:PByte;Len:Word):Integer; stdcall;
function VII_TimeConfig(DevType, DevIndex, I2CIndex:Integer;pTimeConfig:PVII_TIME_CONFIG):Integer; stdcall;

implementation

function VII_ScanDevice;external 'Ginkgo_Driver.dll' name 'VII_ScanDevice';
function VII_OpenDevice;external 'Ginkgo_Driver.dll' name 'VII_OpenDevice';
function VII_CloseDevice;external 'Ginkgo_Driver.dll' name 'VII_CloseDevice';
function VII_InitI2C;external 'Ginkgo_Driver.dll' name 'VII_InitI2C';
function VII_WriteBytes;external 'Ginkgo_Driver.dll' name 'VII_WriteBytes';
function VII_ReadBytes;external 'Ginkgo_Driver.dll' name 'VII_ReadBytes';
function VII_TimeConfig;external 'Ginkgo_Driver.dll' name 'VII_TimeConfig';
end.
