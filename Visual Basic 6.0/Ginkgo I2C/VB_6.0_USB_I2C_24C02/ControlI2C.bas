Attribute VB_Name = "ControlI2C"
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

Public Const VII_USBI2C As Long = 1

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
    
Public Type VII_BOARD_INFO
    ProductName(31) As Byte
    FirmwareVersion(3) As Byte
    HardwareVersion(3) As Byte
    SerialNumber(11) As Byte
End Type

Public Type VII_INIT_CONFIG
    MasterMode As Byte
    ControlMode As Byte
    AddrType As Byte
    SubAddrWidth As Byte
    Addr As Integer
    ClockSpeed As Long
End Type

Public Type VII_TIME_CONFIG
    tHD_STA As Integer
    tSU_STA As Integer
    tLOW As Integer
    tHIGH As Integer
    tSU_DAT As Integer
    tSU_STO As Integer
    tDH As Integer
    tDH_DAT As Integer
    tAA As Integer
    tR As Integer
    tF As Integer
    tBuf As Integer
    tACK(3) As Byte
    tStart As Integer
    tStop As Integer
End Type

Declare Function VII_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Long
Declare Function VII_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal Reserved As Long) As Long
Declare Function VII_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long) As Long
Declare Function VII_ReadBoardInfo Lib "Ginkgo_Driver.dll" (ByVal DevIndex As Long, ByRef pInfo As VII_BOARD_INFO) As Long
Declare Function VII_TimeConfig Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal I2CIndex As Long, ByRef pTimeConfig As VII_TIME_CONFIG) As Long
Declare Function VII_InitI2C Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal I2CIndex As Long, ByRef pInitConfig As VII_INIT_CONFIG) As Long
Declare Function VII_WriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal I2CIndex As Long, ByVal Addr As Integer, ByVal SubAddr As Long, ByVal pWriteData As Any, ByVal WriteLen As Integer) As Long
Declare Function VII_ReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal I2CIndex As Long, ByVal Addr As Integer, ByVal SubAddr As Long, ByVal pReadData As Any, ByVal ReadLen As Integer) As Long
Declare Function VII_SlaveReadBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal I2CIndex As Long, ByVal pReadData As Any, ByRef pLen As Integer) As Long
Declare Function VII_SlaveWriteBytes Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal I2CIndex As Long, ByVal pWriteData As Any, ByVal WriteLen As Integer) As Long
Declare Function VII_SetUserKey Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal pUserKey As Any) As Long
Declare Function VII_CheckUserKey Lib "Ginkgo_Driver.dll" (ByVal DevType As Long, ByVal DevIndex As Long, ByVal pUserKey As Any) As Long


