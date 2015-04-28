unit ControlGPIO;

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
//定义GPIO引脚号
VGI_GPIO_PIN0		= (1 shl 0);	//GPIO_0
VGI_GPIO_PIN1		= (1 shl 1);	//GPIO_1
VGI_GPIO_PIN2		= (1 shl 2);	//GPIO_2
VGI_GPIO_PIN3		= (1 shl 3);	//GPIO_3
VGI_GPIO_PIN4		= (1 shl 4);	//GPIO_4
VGI_GPIO_PIN5		= (1 shl 5);	//GPIO_5
VGI_GPIO_PIN6		= (1 shl 6);	//GPIO_6
VGI_GPIO_PIN7		= (1 shl 7);	//GPIO_7
VGI_GPIO_PIN8		= (1 shl 8);	//GPIO_1
VGI_GPIO_PIN9		= (1 shl 9);	//GPIO_2
VGI_GPIO_PIN10	= (1 shl 10);	//GPIO_3
VGI_GPIO_PIN11	= (1 shl 11);	//GPIO_4
VGI_GPIO_PIN12	= (1 shl 12);	//GPIO_5
VGI_GPIO_PIN13	= (1 shl 13);	//GPIO_6
VGI_GPIO_PIN14	= (1 shl 14);	//GPIO_7
VGI_GPIO_PIN15	= (1 shl 15);	//GPIO_7
VGI_GPIO_PIN_ALL= ($FFFF);    //ALL PINS

//函数声明
function VGI_ScanDevice(NeedInit:Byte):Integer; stdcall;
function VGI_OpenDevice(DevType,DevIndex,Reserved:Integer):Integer; stdcall;
function VGI_CloseDevice(DevType,DevIndex:Integer):Integer; stdcall;

function VGI_SetInput(DevType,DevIndex:Integer;Pins:Word):Integer; stdcall;
function VGI_SetOutput(DevType,DevIndex:Integer; Pins:Word):Integer; stdcall;
function VGI_SetOpenDrain(DevType,DevIndex:Integer; Pins:Word):Integer; stdcall;

function VGI_SetPins(DevType,DevIndex:Integer; Pins:Word):Integer; stdcall;
function VGI_ResetPins(DevType,DevIndex:Integer; Pins:Word):Integer; stdcall;

function VGI_ReadDatas(DevType,DevIndex:Integer;PinMask:Word;pData:PWord):Integer; stdcall;

implementation
function VGI_ScanDevice;external 'Ginkgo_Driver.dll' name 'VGI_ScanDevice';
function VGI_OpenDevice;external 'Ginkgo_Driver.dll' name 'VGI_OpenDevice';
function VGI_CloseDevice;external 'Ginkgo_Driver.dll' name 'VGI_CloseDevice';
function VGI_SetInput;external 'Ginkgo_Driver.dll' name 'VGI_SetInput';
function VGI_SetOutput;external 'Ginkgo_Driver.dll' name 'VGI_SetOutput';
function VGI_SetOpenDrain;external 'Ginkgo_Driver.dll' name 'VGI_SetOpenDrain';
function VGI_ResetPins;external 'Ginkgo_Driver.dll' name 'VGI_ResetPins';
function VGI_SetPins;external 'Ginkgo_Driver.dll' name 'VGI_SetPins';
function VGI_ReadDatas;external 'Ginkgo_Driver.dll' name 'VGI_ReadDatas';
end.
