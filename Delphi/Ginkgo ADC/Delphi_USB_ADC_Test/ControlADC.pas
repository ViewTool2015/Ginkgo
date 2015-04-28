unit ControlADC;

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
//定义ADC通道
VAI_ADC_CH0		= (1 shl 0);	//ADC_CH0
VAI_ADC_CH1		= (1 shl 1);	//ADC_CH1
VAI_ADC_CH2		= (1 shl 2);	//ADC_CH2
VAI_ADC_CH3		= (1 shl 3);	//ADC_CH3
VAI_ADC_CH4		= (1 shl 4);	//ADC_CH4
VAI_ADC_CH5		= (1 shl 5);	//ADC_CH5
VAI_ADC_CH6		= (1 shl 6);	//ADC_CH6
VAI_ADC_CH7		= (1 shl 7);	//ADC_CH7
VAI_ADC_ALL		= ($FF);    	//ADC_CH_ALL

//函数声明
function VAI_ScanDevice(NeedInit:Byte):Integer; stdcall;
function VAI_OpenDevice(DevType,DevIndex,Reserved:Integer):Integer; stdcall;
function VAI_CloseDevice(DevType,DevIndex:Integer):Integer; stdcall;
function VAI_InitADC(DevType,DevIndex:Integer;Channel:Byte;Period:Word):Integer; stdcall;
function VAI_ReadDatas(DevType,DevIndex:Integer;DataNum:Word;pData:PWord):Integer; stdcall;
implementation

function VAI_ScanDevice;external 'Ginkgo_Driver.dll' name 'VAI_ScanDevice';
function VAI_OpenDevice;external 'Ginkgo_Driver.dll' name 'VAI_OpenDevice';
function VAI_CloseDevice;external 'Ginkgo_Driver.dll' name 'VAI_CloseDevice';
function VAI_InitADC;external 'Ginkgo_Driver.dll' name 'VAI_InitADC';
function VAI_ReadDatas;external 'Ginkgo_Driver.dll' name 'VAI_ReadDatas';
end.
