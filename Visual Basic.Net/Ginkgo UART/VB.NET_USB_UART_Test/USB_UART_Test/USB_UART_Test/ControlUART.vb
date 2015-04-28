Option Strict Off
Option Explicit On

Imports System.Runtime.InteropServices
Module ControlUART
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

    ' Initialize configure
    Public Structure UART_INIT_CONFIG
        Dim StopBits As Byte       ' Stop Bit，0-1bit,1-1.5bit,2-2bit,3-1.5bit
        Dim WordLength As Byte     ' 8
        Dim Parity As Byte         ' 0-No,4-Even,6-Odd
        Dim RS485Mode As UInt16    ' 485-RS485 mode，232-RS232 mode
        Dim BaudRate As UInt32     ' BaudRate
    End Structure

    ' Adapter information data type
    Public Structure UART_BOARD_INFO
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim ProductName As Byte()        'Product Name
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim FirmwareVersion As Byte()    'Firmware Version
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim HardwareVersion As Byte()    'Hardware Version
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=12)> _
        Dim SerialNumber As Byte()       'Serial Number
    End Structure
    ' Define Adapter type
    Public Const UART_USBUART As Int32 = 2

    ' Function
    Declare Function UART_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Int32
    Declare Function UART_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Reserved As Int32) As Int32
    Declare Function UART_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevIndex As Int32) As Int32
    Declare Function UART_ReadBoardInfo Lib "Ginkgo_Driver.dll" (ByVal DevIndex As Int32, ByRef pinfo As UART_BOARD_INFO) As Int32
    Declare Function UART_InitDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal UARTIndex As Int32, ByRef pInitConfig As UART_INIT_CONFIG) As Int32
    Declare Function UART_WriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal UARTIndex As Int32, ByVal pWriteData() As Byte, ByVal Len As UInt16) As Int32
    Declare Function UART_ReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal UARTIndex As Int32, ByVal pReadData() As Byte, ByRef pLen As UInt16) As Int32
End Module
