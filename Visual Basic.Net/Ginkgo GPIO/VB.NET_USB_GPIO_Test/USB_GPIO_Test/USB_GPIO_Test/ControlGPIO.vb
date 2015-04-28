Option Strict Off
Option Explicit On

Imports System.Runtime.InteropServices
Module ControlGPIO
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
    Public Const VGI_USBGPIO As Int32 = 1
    ' GPIO pin
    Public Const VGI_GPIO_PIN0 As UInt16 = 1 << 0       'GPIO_0
    Public Const VGI_GPIO_PIN1 As UInt16 = 1 << 1       'GPIO_1
    Public Const VGI_GPIO_PIN2 As UInt16 = 1 << 2       'GPIO_2
    Public Const VGI_GPIO_PIN3 As UInt16 = 1 << 3       'GPIO_3
    Public Const VGI_GPIO_PIN4 As UInt16 = 1 << 4       'GPIO_4
    Public Const VGI_GPIO_PIN5 As UInt16 = 1 << 5       'GPIO_5
    Public Const VGI_GPIO_PIN6 As UInt16 = 1 << 6       'GPIO_6
    Public Const VGI_GPIO_PIN7 As UInt16 = 1 << 7       'GPIO_7
    Public Const VGI_GPIO_PIN8 As UInt16 = 1 << 8       'GPIO_8
    Public Const VGI_GPIO_PIN9 As UInt16 = 1 << 9       'GPIO_9
    Public Const VGI_GPIO_PIN10 As UInt16 = 1 << 10     'GPIO_10
    Public Const VGI_GPIO_PIN11 As UInt16 = 1 << 11     'GPIO_11
    Public Const VGI_GPIO_PIN12 As UInt16 = 1 << 12     'GPIO_12
    Public Const VGI_GPIO_PIN13 As UInt16 = 1 << 13     'GPIO_13
    Public Const VGI_GPIO_PIN14 As UInt16 = 1 << 14     'GPIO_14
    Public Const VGI_GPIO_PIN15 As UInt16 = 1 << 15     'GPIO_15
    Public Const VGI_GPIO_PIN_ALL As UInt16 = &HFFFF    'GPIO_ALL
    ' Function
    Declare Function VGI_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Int32
    Declare Function VGI_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Reserved As Int32) As Int32
    Declare Function VGI_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32) As Int32
    ' Get pin value
    Declare Function VGI_ReadDatas Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal PinMask As UInt16, ByRef pData As UInt16) As Int32
    ' Control pin output
    Declare Function VGI_WriteDatas Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal PinMask As UInt16, ByVal Data As UInt16) As Int32
    Declare Function VGI_SetPins Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal PinMask As UInt16) As Int32
    Declare Function VGI_ResetPins Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal PinMask As UInt16) As Int32
    ' Set input/output and work mode
    Declare Function VGI_InitGPIO Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal PinMask As UInt16, ByVal PinMode As UInt32) As Int32
    Declare Function VGI_SetInput Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal PinMask As UInt16) As Int32
    Declare Function VGI_SetOutput Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal PinMask As UInt16) As Int32
    Declare Function VGI_SetOpenDrain Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal PinMask As UInt16) As Int32

End Module
