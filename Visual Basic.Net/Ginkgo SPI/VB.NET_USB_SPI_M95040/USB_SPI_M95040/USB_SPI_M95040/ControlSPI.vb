Option Strict Off
Option Explicit On

Imports System.Runtime.InteropServices
Module ControlSPI
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
    Public Const VSI_USBSPI As Int32 = 2
    ' The adapter type definition and data initialization definition
    Public Structure VSI_INIT_CONFIG
        Dim ControlMode As Byte     ' SPI control mode: 0->hardware control(full duplex) 1->hardware control(half duplex) 2->software control(half duplex) 3-> one wire mode
        Dim TranBits As Byte        ' Width  of data (between 8 and 16)
        Dim MasterMode As Byte      ' Master mode : 0 -> slave mode 1 -> master mode
        Dim CPOL As Byte            ' Clock Polarity: 0 -> SCK active-high  1->SCK active-low 
        Dim CPHA As Byte            ' Clock Phase: 0 -> sample on the leading (first) clock edge 1-> sample on the trailing (second) clock edge
        Dim LSBFirst As Byte        ' whether or not LSB first: 0->MSB first 1-> LSB first
        Dim SelPolarity As Byte     ' Chip select Polarity: 0-> low to select 1-> high to select
        Dim ClockSpeed As UInt32    ' SPI clock frequency(in Hz), 36000000,18000000,9000000,4500000,2250000,1125000,562500,281250 available in hardware mode，1KHz to 1MHz available in software mode
    End Structure
    ' Adapter information data type
    Public Structure VSI_BOARD_INFO
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim ProductName As Byte()        'Product Name
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim FirmwareVersion As Byte()    'Firmware Version
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim HardwareVersion As Byte()    'Hardware Version
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=12)> _
        Dim SerialNumber As Byte()       'Serial Number
    End Structure
    ' Function
    Declare Function VSI_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Int32
    Declare Function VSI_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Reserved As Int32) As Int32
    Declare Function VSI_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32) As Int32
    Declare Function VSI_ReadBoardInfo Lib "Ginkgo_Driver.dll" (ByVal DevIndex As Int32, ByRef pInfo As VSI_BOARD_INFO) As Int32
    Declare Function VSI_InitSPI Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByRef pInitConfig As VSI_INIT_CONFIG) As Int32
    Declare Function VSI_WriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteData() As Byte, ByVal WriteLen As UInt16) As Int32
    Declare Function VSI_ReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pReadData() As Byte, ByVal ReadLen As UInt16) As Int32
    Declare Function VSI_WriteReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteData() As Byte, ByVal WriteLen As UInt16, ByVal pReadData() As Byte, ByVal ReadLen As UInt16) As Int32
    ' Only available in software mode 
    Declare Function VSI_WriteBits Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteBitStr() As Byte) As Int32
    Declare Function VSI_ReadBits Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pReadBitStr() As Byte, ByVal ReadBitsNum As UInt32) As Int32
    Declare Function VSI_WriteReadBits Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteBitStr() As Byte, ByVal pReadBitStr() As Byte, ByVal ReadBitsNum As UInt32) As Int32
    ' Slave mode function
    Declare Function VSI_SlaveReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pReadData() As Byte, ByRef pBytesNum As Int32, ByVal WaitTime As Int32) As Int32
    Declare Function VSI_SlaveWriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pWriteData() As Byte, ByVal WriteBytesNum As Int32) As Int32
    ' Cryptographic function
    Declare Function VSI_SetUserKey Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pUserKey() As Byte) As Int32
    Declare Function VSI_CheckUserKey Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal pUserKey() As Byte) As Int32
    ' Block option function
    Declare Function VSI_BlockWriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteData() As Byte, ByVal BlockSize As UInt16, ByVal BlockNum As UInt16, ByVal IntervalTime As UInt32) As Int32
    Declare Function VSI_BlockReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pReadData() As Byte, ByVal BlockSize As UInt16, ByVal BlockNum As UInt16, ByVal IntervalTime As UInt32) As Int32
    Declare Function VSI_BlockWriteReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SPIIndex As Int32, ByVal pWriteData() As Byte, ByVal WriteBlockSize As UInt16, ByVal pReadData() As Byte, ByVal ReadBlockSize As UInt16, ByVal BlockNum As UInt16, ByVal IntervalTime As UInt32) As Int32

End Module
