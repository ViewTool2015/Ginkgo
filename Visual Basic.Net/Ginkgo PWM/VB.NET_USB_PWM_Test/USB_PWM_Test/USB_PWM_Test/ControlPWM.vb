Option Strict Off
Option Explicit On

Imports System.Runtime.InteropServices
Module ControlPWM
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
    'PWM channel
    Public Const VPI_PWM_CH0 As Byte = 1 << 0       'PWM_CH0
    Public Const VPI_PWM_CH1 As Byte = 1 << 1       'PWM_CH1
    Public Const VPI_PWM_CH2 As Byte = 1 << 2       'PWM_CH2
    Public Const VPI_PWM_CH3 As Byte = 1 << 3       'PWM_CH3
    Public Const VPI_PWM_CH4 As Byte = 1 << 4       'PWM_CH4
    Public Const VPI_PWM_CH5 As Byte = 1 << 5       'PWM_CH5
    Public Const VPI_PWM_CH6 As Byte = 1 << 6       'PWM_CH6
    Public Const VPI_PWM_CH7 As Byte = 1 << 7       'PWM_CH7
    Public Const VPI_PWM_ALL As Byte = &HFF         'ADC_ALL
    ' ' Define Adapter type
    Public Const VPI_USBPWM As Int32 = 2
    ' PWM initialize structure
    Public Structure VII_INIT_CONFIG
        Dim PWM_ChannelMask As Byte 'PWM index,bit0->channel0，bit7->channel7
        Dim PWM_Mode As Byte        'PWM mode，0-mode 0,1-mode1
        Dim PWM_Pulse As Byte       'PWM Duty Ratio, 0 to 100
        Dim PWM_Polarity As Byte    'PWM Polarity，0-low，1-high 
        Dim PWM_Frequency As UInt32 'PWMPWM_Frequency, 1Hz to 20000000Hz
    End Structure
    ' Function
    Declare Function VPI_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Int32
    Declare Function VPI_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Reserved As Int32) As Int32
    Declare Function VPI_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32) As Int32
    Declare Function VPI_InitPWM Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByRef pInitConfig As VII_INIT_CONFIG) As Int32
    Declare Function VPI_StartPWM Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal ChannelMask As Byte) As Int32
    Declare Function VPI_StopPWM Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal ChannelMask As Byte) As Int32
    Declare Function VPI_SetPWMPulse Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal ChannelMask As Byte, ByVal pPulse() As Byte) As Int32
    Declare Function VPI_SetPWMPeriod Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal ChannelMask As Byte, ByVal Frequency() As Int32) As Int32
End Module
