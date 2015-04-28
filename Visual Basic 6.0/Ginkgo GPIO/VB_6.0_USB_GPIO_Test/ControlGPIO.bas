Attribute VB_Name = "ControlGPIO"
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

Public Const VPI_USBGPIO As Long = 1

Public Const VGI_GPIO_PIN0 As Long = &H1        'GPIO_0
Public Const VGI_GPIO_PIN1 As Long = &H2        'GPIO_1
Public Const VGI_GPIO_PIN2 As Long = &H4        'GPIO_2
Public Const VGI_GPIO_PIN3 As Long = &H8        'GPIO_3
Public Const VGI_GPIO_PIN4 As Long = &H10       'GPIO_4
Public Const VGI_GPIO_PIN5 As Long = &H20       'GPIO_5
Public Const VGI_GPIO_PIN6 As Long = &H40       'GPIO_6
Public Const VGI_GPIO_PIN7 As Long = &H80       'GPIO_7
Public Const VGI_GPIO_PIN8 As Long = &H100      'GPIO_8
Public Const VGI_GPIO_PIN9 As Long = &H200      'GPIO_9
Public Const VGI_GPIO_PIN10 As Long = &H400     'GPIO_10
Public Const VGI_GPIO_PIN11 As Long = &H800     'GPIO_11
Public Const VGI_GPIO_PIN12 As Long = &H1000    'GPIO_12
Public Const VGI_GPIO_PIN13 As Long = &H2000    'GPIO_13
Public Const VGI_GPIO_PIN14 As Long = &H4000    'GPIO_14
Public Const VGI_GPIO_PIN15 As Long = &H8000    'GPIO_15
Public Const VGI_GPIO_ALL As Long = &HFFFF      'GPIO_ALL

Public Type VGI_BOARD_INFO
    ProductName(31) As Byte
    FirmwareVersion(3) As Byte
    HardwareVersion(3) As Byte
    SerialNumber(11) As Byte
End Type

Declare Function VGI_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Long
Declare Function VGI_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal Reserved As Long) As Long
Declare Function VGI_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long)
Declare Function VGI_ReadBoardInfo Lib "Ginkgo_Driver.dll" (ByVal DevIndex As Long, ByRef pBoardInfo As VGI_BOARD_INFO) As Long
Declare Function VGI_InitGPIO Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal PinMask As Integer, ByVal PinMode As Long) As Long
Declare Function VGI_WriteDatas Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal PinMask As Integer, ByVal Data As Integer) As Long
Declare Function VGI_ReadDatas Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal PinMask As Integer, ByRef pData As Integer) As Long
Declare Function VGI_SetPins Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal PinMask As Integer) As Long
Declare Function VGI_ResetPins Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal PinMask As Integer) As Long
Declare Function VGI_SetInput Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal PinMask As Integer) As Long
Declare Function VGI_SetOutput Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal PinMask As Integer) As Long
Declare Function VGI_SetOpenDrain Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal PinMask As Integer) As Long
