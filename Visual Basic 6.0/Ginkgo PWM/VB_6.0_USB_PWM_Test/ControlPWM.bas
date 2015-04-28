Attribute VB_Name = "ControlPWM"
'Error Type
Public Const SUCCESS As Long = 0
Public Const PARAMETER_NULL As Long = -1
Public Const INPUT_DATA_TOO_MUCH As Long = -2
Public Const INPUT_DATA_TOO_LESS As Long = -3
Public Const INPUT_DATA_ILLEGALITY As Long = -4
Public Const USB_WRITE_DATA_ERROR As Long = -5
Public Const USB_READ_DATA_ERROR As Long = -6
Public Const READ_NO_DATA As Long = -7
Public Const OPEN_DEVICE_FAILD As Long = -8
Public Const CLOSE_DEVICE_FAILD As Long = -9
Public Const EXECUTE_CMD_FAILD As Long = -10
Public Const SELECT_DEVICE_FAILD As Long = -11
Public Const DEVICE_OPENED As Long = -12
Public Const DEVICE_NOTOPEN As Long = -13
Public Const BUFFER_OVERFLOW As Long = -14
Public Const DEVICE_NOTEXIST As Long = -15
Public Const LOAD_KERNELDLL As Long = -16
Public Const CMD_FAILED As Long = -17
Public Const BUFFER_CREATE As Long = -18

Public Const VPI_USBPWM As Long = 2

Public Const VPI_PWM_CH0 As Long = 1    'PWM_CH0
Public Const VPI_PWM_CH1 As Long = 2    'PWM_CH1
Public Const VPI_PWM_CH2 As Long = 4    'PWM_CH2
Public Const VPI_PWM_CH3 As Long = 8    'PWM_CH3
Public Const VPI_PWM_CH4 As Long = 16   'PWM_CH4
Public Const VPI_PWM_CH5 As Long = 32   'PWM_CH5
Public Const VPI_PWM_CH6 As Long = 64   'PWM_CH6
Public Const VPI_PWM_CH7 As Long = 128  'PWM_CH7

Public Type VPI_INIT_CONFIG
    PWM_ChannelMask As Byte
    PWM_Mode As Byte
    PWM_Pulse As Byte
    PWM_Polarity As Byte
    PWM_Frequency As Long
End Type

Declare Function VPI_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Long
Declare Function VPI_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal Reserved As Long) As Long
Declare Function VPI_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long) As Long
Declare Function VPI_InitPWM Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByRef pInitConfig As VPI_INIT_CONFIG) As Long
Declare Function VPI_StartPWM Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal ChannelMask As Byte) As Long
Declare Function VPI_StopPWM Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal ChannelMask As Byte) As Long
Declare Function VPI_SetPWMPulse Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal ChannelMask As Byte, ByRef pPulse As Byte) As Long
Declare Function VPI_SetPWMPeriod Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal ChannelMask As Byte, ByRef pFrequency As Long) As Long
