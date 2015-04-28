Option Strict Off
Option Explicit On

Imports System.Runtime.InteropServices
Module ControlI2C
    'General Error Code
    Public Const SUCCESS As Int32 = 0                   ' no Error
    Public Const PARAMETER_NULL As Int32 = -1           ' void pointer 
    Public Const INPUT_DATA_TOO_MUCH As Int32 = -2      ' too many parameters
    Public Const INPUT_DATA_TOO_LESS As Int32 = -3      ' too few parameters
    Public Const INPUT_DATA_ILLEGALITY As Int32 = -4    ' illegal parameter
    Public Const USB_WRITE_DATA_ERROR As Int32 = -5     ' USB write data error
    Public Const USB_READ_DATA_ERROR As Int32 = -6      ' USB read data error
    Public Const READ_NO_DATA As Int32 = -7             ' no data return when request data
    Public Const OPEN_DEVICE_FAILD As Int32 = -8        ' failed to open device
    Public Const CLOSE_DEVICE_FAILD As Int32 = -9       ' failed to close device
    Public Const EXECUTE_CMD_FAILD As Int32 = -10       ' the command failed to execute  
    Public Const SELECT_DEVICE_FAILD As Int32 = -11     ' failed to select device
    Public Const DEVICE_OPENED As Int32 = -12           ' device has open
    Public Const DEVICE_NOTOPEN As Int32 = -13          ' device not open
    Public Const BUFFER_OVERFLOW As Int32 = -14         ' buffer overflow
    Public Const DEVICE_NOTEXIST As Int32 = -15         ' device not exist
    Public Const LOAD_KERNELDLL As Int32 = -16          ' failed to load KernelDLL
    Public Const CMD_FAILED As Int32 = -17              ' failed to execute command
    Public Const BUFFER_CREATE As Int32 = -18           ' out of memory
    ' Define Adapter type
    Public Const VII_USBI2C As Int32 = 1
    ' The adapter type definition and data initialization definition
    Public Const VII_ADDR_7BIT As Byte = 7
    Public Const VII_ADDR_10BIT As Byte = 10
    Public Const VII_HCTL_MODE As Byte = 1
    Public Const VII_SCTL_MODE As Byte = 2
    Public Const VII_MASTER As Byte = 1
    Public Const VII_SLAVE As Byte = 0
    Public Const VII_SUB_ADDR_NONE As Byte = 0
    Public Const VII_SUB_ADDR_1BYTE As Byte = 1
    Public Const VII_SUB_ADDR_2BYTE As Byte = 2
    Public Const VII_SUB_ADDR_3BYTE As Byte = 3
    Public Const VII_SUB_ADDR_4BYTE As Byte = 4
    ' I2C data types initialization Define
    Public Structure VII_INIT_CONFIG
        Dim MasterMode As Byte      'Master-slave choice: 0-slave, 1-master
        Dim ControlMode As Byte     'Control mode: 0-Standard slave mode, 1-Standard mode, 2-GPIO mode
        Dim AddrType As Byte        '7-7bit mode, 10-10bit mode
        Dim SubAddrWidth As Byte    'Sub-Address width: value of 0~4, 0 means no Sub-Address mode
        Dim Addr As UInt16          'Device address in salve mode
        Dim ClockSpeed As UInt32    'Clock frequency(HZ)
    End Structure
    ' Adapter information data type
    Public Structure VII_BOARD_INFO
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim ProductName As Byte()        'Product Name
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim FirmwareVersion As Byte()    'Firmware Version
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim HardwareVersion As Byte()    'Hardware Version
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=12)> _
        Dim SerialNumber As Byte()       'Serial Number
    End Structure
    ' I2C time parameter definition in GPIO mode(ms)
    Public Structure VII_TIME_CONFIG
        Dim tHD_STA As UInt16   ' Timing for start signal Keeping
        Dim tSU_STA As UInt16   ' Timing for start signal be established
        Dim tLOW As UInt16      ' Timing for clock low level
        Dim tHIGH As UInt16     ' Timing for clock high level
        Dim tSU_DAT As UInt16   ' Timing for data input be established
        Dim tSU_STO As UInt16   ' Timing for stop signal be established
        Dim tDH As UInt16       ' Timing for data output Keeping
        Dim tDH_DAT As UInt16   ' Timing for data input Keeping
        Dim tAA As UInt16       ' SCL lower to SDA output and response signal
        Dim tR As UInt16        ' Timing for SDA and SCL rising
        Dim tF As UInt16        ' Timing for SDA and SCL going down
        Dim tBuf As UInt16      ' Free timing of the bus until the new mission
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim tACK As Byte()
        Dim tStart As UInt16
        Dim tStop As UInt16
    End Structure
    ' Function
    Declare Function VII_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Int32
    Declare Function VII_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Reserved As Int32) As Int32
    Declare Function VII_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32) As Int32
    Declare Function VII_ReadBoardInfo Lib "Ginkgo_Driver.dll" (ByVal DevIndex As Int32, ByRef pInfo As VII_BOARD_INFO) As Int32
    Declare Function VII_TimeConfig Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal I2CIndex As Int32, ByRef pTimeConfig As VII_TIME_CONFIG) As Int32
    Declare Function VII_InitI2C Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal I2CIndex As Int32, ByRef pInitConfig As VII_INIT_CONFIG) As Int32
    Declare Function VII_WriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal I2CIndex As Int32, ByVal Addr As UInt16, ByVal SubAddr As UInt32, ByVal pWriteData() As Byte, ByVal Len As UInt16) As Int32
    Declare Function VII_ReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal I2CIndex As Int32, ByVal Addr As UInt16, ByVal SubAddr As UInt32, ByVal pReadData() As Byte, ByVal Len As UInt16) As Int32
    Declare Function VII_SlaveReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal I2CIndex As Int32, ByVal pReadData() As Byte, ByRef pLen As UInt16) As Int32
    Declare Function VII_SlaveWriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal I2CIndex As Int32, ByVal pWriteData() As Byte, ByVal Len As UInt16) As Int32
    Declare Function VII_SetUserKey Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pUserKey() As Byte) As Int32
    Declare Function VII_CheckUserKey Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pUserKey() As Byte) As Int32
    Declare Function VII_SlaveWriteRemain Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal I2CIndex As Int32, ByRef pRemainBytes As UInt16) As Int32
End Module
