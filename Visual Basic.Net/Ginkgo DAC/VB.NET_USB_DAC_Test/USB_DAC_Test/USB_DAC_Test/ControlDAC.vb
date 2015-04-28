Option Strict Off
Option Explicit On

Imports System.Runtime.InteropServices
Module ControlDAC
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
    Public Const VDA_USBDAC As Int32 = 1

    ' Define the DAC channels
    Public Const VDA_DAC_CH0 As Byte = 0
    Public Const VDA_DAC_CH1 As Byte = 1

    ' Function
    Declare Function VDA_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Int32
    Declare Function VDA_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Reserved As Int32) As Int32
    Declare Function VDA_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32) As Int32
    ' Initialize DAC
    Declare Function VDA_InitDAC Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Channel As Byte, ByVal pWaveData() As UInt16, ByVal WaveDataNum As UInt16, ByVal Period As UInt16) As Int32
    ' DAC option
    Declare Function VDA_StartDAC Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Channel As Byte) As Int32
    Declare Function VDA_StopDAC Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Channel As Byte) As Int32

End Module
