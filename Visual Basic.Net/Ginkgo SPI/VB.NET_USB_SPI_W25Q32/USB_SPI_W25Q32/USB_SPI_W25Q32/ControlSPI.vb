Option Strict Off
Option Explicit On

Imports System.Runtime.InteropServices
Module ControlSPI
    '错误类型定义
    Public Const SUCCESS As Int32 = 0                   '没有错误
    Public Const PARAMETER_NULL As Int32 = -1           '传入的指针为空指针
    Public Const INPUT_DATA_TOO_MUCH As Int32 = -2      '参数输入个数多余规定个数
    Public Const INPUT_DATA_TOO_LESS As Int32 = -3      '参数输入个数少余规定个数
    Public Const INPUT_DATA_ILLEGALITY As Int32 = -4    '参数传入格式和规定的不符合
    Public Const USB_WRITE_DATA_ERROR As Int32 = -5     'USB写数据错误
    Public Const USB_READ_DATA_ERROR As Int32 = -6      'USB读数据错误
    Public Const READ_NO_DATA As Int32 = -7             '请求读数据时返回没有数据
    Public Const OPEN_DEVICE_FAILD As Int32 = -8        '打开设备失败
    Public Const CLOSE_DEVICE_FAILD As Int32 = -9       '关闭设备失败
    Public Const EXECUTE_CMD_FAILD As Int32 = -10       '设备执行命令失败
    Public Const SELECT_DEVICE_FAILD As Int32 = -11     '选择设备失败
    Public Const DEVICE_OPENED As Int32 = -12           '设备已经打开
    Public Const DEVICE_NOTOPEN As Int32 = -13          '设备没有打开
    Public Const BUFFER_OVERFLOW As Int32 = -14         '缓冲区溢出
    Public Const DEVICE_NOTEXIST As Int32 = -15         '此设备不存在
    Public Const LOAD_KERNELDLL As Int32 = -16          '装载动态库失败
    Public Const CMD_FAILED As Int32 = -17              '执行命令失败错误码
    Public Const BUFFER_CREATE As Int32 = -18           '内存不足
    '定义适配器类型
    Public Const VSI_USBSPI As Int32 = 2
    '定义初始化I2C的数据类型
    Public Structure VSI_INIT_CONFIG
        Dim ControlMode As Byte     'SPI控制方式:0-硬件控制（全双工模式）,1-硬件控制（半双工模式），2-软件控制（半双工模式）,3-单总线模式，数据线输入输出都为MOSI
        Dim TranBits As Byte        '数据传输字节宽度，在8和16之间取值
        Dim MasterMode As Byte      '主从选择控制:0-从机，1-主机
        Dim CPOL As Byte            '时钟极性控制:0-SCK高有效，1-SCK低有效
        Dim CPHA As Byte            '时钟相位控制:0-第一个SCK时钟采样，1-第二个SCK时钟采样
        Dim LSBFirst As Byte        '数据移位方式:0-MSB在前，1-LSB在前
        Dim SelPolarity As Byte     '片选信号极性:0-低电平选中，1-高电平选中
        Dim ClockSpeed As UInt32    'SPI时钟频率:单位为Hz，硬件模式下只能设置为36000000,18000000,9000000,4500000,2250000,1125000,562500,281250，软件模式下可以设置为低于1000000，高于1000之间的任意值
    End Structure
    'Ginkgo系列适配器信息的数据类型
    Public Structure VSI_BOARD_INFO
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim ProductName As Byte()        '硬件名称
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim FirmwareVersion As Byte()    '固件版本
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim HardwareVersion As Byte()    '硬件版本
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=12)> _
        Dim SerialNumber As Byte()       '适配器序列号
    End Structure
    '函数声明
    Declare Function VSI_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Int32
    Declare Function VSI_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Reserved As Int32) As Int32
    Declare Function VSI_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32) As Int32
    Declare Function VSI_ReadBoardInfo Lib "Ginkgo_Driver.dll" (ByVal DevIndex As Int32, ByRef pInfo As VSI_BOARD_INFO) As Int32
    Declare Function VSI_InitSPI Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByRef pInitConfig As VSI_INIT_CONFIG) As Int32
    Declare Function VSI_WriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteData() As Byte, ByVal WriteLen As UInt16) As Int32
    Declare Function VSI_ReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pReadData() As Byte, ByVal ReadLen As UInt16) As Int32
    Declare Function VSI_WriteReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteData() As Byte, ByVal WriteLen As UInt16, ByVal pReadData() As Byte, ByVal ReadLen As UInt16) As Int32
    '只能在软件模式下使用的函数
    Declare Function VSI_WriteBits Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteBitStr() As Byte) As Int32
    Declare Function VSI_ReadBits Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pReadBitStr() As Byte, ByVal ReadBitsNum As UInt32) As Int32
    Declare Function VSI_WriteReadBits Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteBitStr() As Byte, ByVal pReadBitStr() As Byte, ByVal ReadBitsNum As UInt32) As Int32
    '从机模式函数
    Declare Function VSI_SlaveReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pReadData() As Byte, ByRef pBytesNum As Int32, ByVal WaitTime As Int32) As Int32
    Declare Function VSI_SlaveWriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pWriteData() As Byte, ByVal WriteBytesNum As Int32) As Int32
    '用户加密设置相关函数
    Declare Function VSI_SetUserKey Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pUserKey() As Byte) As Int32
    Declare Function VSI_CheckUserKey Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pUserKey() As Byte) As Int32
    '块模式操作相关函数
    Declare Function VSI_BlockWriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteData() As Byte, ByVal BlockSize As UInt16, ByVal BlockNum As UInt16, ByVal IntervalTime As UInt32) As Int32
    Declare Function VSI_BlockReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pReadData() As Byte, ByVal BlockSize As UInt16, ByVal BlockNum As UInt16, ByVal IntervalTime As UInt32) As Int32
    Declare Function VSI_BlockWriteReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteData() As Byte, ByVal WriteBlockSize As UInt16, ByVal pReadData() As Byte, ByVal ReadBlockSize As UInt16, ByVal BlockNum As UInt16, ByVal IntervalTime As UInt32) As Int32

End Module
