Option Strict Off
Option Explicit On

Imports System.Runtime.InteropServices
Module ControlCAN
    'CAN Error
    Public Const ERR_CAN_OVERFLOW As Int32 = &H1        'CAN controller internal FIFO overflow
    Public Const ERR_CAN_ERRALARM As Int32 = &H2        'CAN controller error alarm
    Public Const ERR_CAN_PASSIVE As Int32 = &H4         'CAN controller passive error
    Public Const ERR_CAN_LOSE As Int32 = &H8            'CAN controller arbitration lost
    Public Const ERR_CAN_BUSERR As Int32 = &H10         'CAN arbitration bus error
    Public Const ERR_CAN_BUSOFF As Int32 = &H20         'CAN arbitration bus off
    Public Const ERR_DEVICEOPENED As Int32 = &H100      'Device is opened
    Public Const ERR_DEVICEOPEN As Int32 = &H200        'Device open failed
    Public Const ERR_DEVICENOTOPEN As Int32 = &H400     'Device not open
    Public Const ERR_BUFFEROVERFLOW As Int32 = &H800    'Buffer overflow
    Public Const ERR_DEVICENOTEXIST As Int32 = &H1000   'Device is not exist
    Public Const ERR_LOADKERNELDLL As Int32 = &H2000    'Load dll failed
    Public Const ERR_CMDFAILED As Int32 = &H4000        'Command execution failed
    Public Const ERR_BUFFERCREATE As Int32 = &H8000     'Memory is not enough
    'Return status
    Public Const STATUS_OK As Int32 = &H1
    Public Const STATUS_ERR As Int32 = &H0
    'Device type
    Public Const VCI_USBCAN1 As Int32 = &H3
    Public Const VCI_USBCAN2 As Int32 = &H4
    'Compatible with other CAN adapter datatype
    Public Structure VCI_BOARD_INFO
        Dim hw_Version As UInt16
        Dim fw_Version As UInt16
        Dim dr_Version As UInt16
        Dim in_Version As UInt16
        Dim irq_Num As UInt16
        Dim can_Num As Byte
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=20)> _
        Dim str_Serial_Num As Byte()
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=40)> _
        Dim str_hw_Type As Byte()
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim Reserved As UInt16()
    End Structure
    'Ginkgo serial board data type
    Public Structure VCI_BOARD_INFO_EX
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim ProductName As Byte()
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim FirmwareVersion As Byte()
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=4)> _
        Dim HardwareVersion As Byte()
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=12)> _
        Dim SerialNumber As Byte()
    End Structure
    'Definition of CAN frame
    Public Structure VCI_CAN_OBJ
        'Frame ID
        Dim ID As UInt32
        'timestamp of the frame arriving，started from initialization of CAN controller
        Dim TimeStamp As UInt32
        'if using timestamp，1: use TimeStamp, 0：not use. TimeFlag and TimeStamp is available when the frame is recived frame
        Dim TimeFlag As Byte
        'send frame type. 0: normal send，1: single send，2: self send/receive，3: single self send/receive，only available when
        'the frame is send frame.（when device type is EG20T-CAN，send type will be set at VCI_InitCan and it's invalid set herein
        'When set to self send/receive mode, EG20T-CAN can not receive from bus，only can receive from itself）
        Dim SendType As Byte
        'remote frame flag
        Dim RemoteFlag As Byte
        'extended frame flag
        Dim ExternFlag As Byte
        'Data length(<=8)，how many uint8_ts of data
        Dim DataLen As Byte
        'Frame data
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=8)> _
        Dim Data As Byte()
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=3)> _
        Dim Reserved As Byte()
    End Structure
    'definition of CAN controller status
    Public Structure VCI_CAN_STATUS
        Dim ErrInterrupt As Byte    'interrupt record，will be cleared while reading
        Dim regMode As Byte         'CAN controller mode register
        Dim regStatus As Byte       'CAN controller status register
        Dim regALCapture As Byte    'CAN controller arbitrator lost register
        Dim regECCapture As Byte    'CAN controller error register
        Dim regEWLimit As Byte      'CAN controller error alarm limitation register
        Dim regRECounter As Byte    'CAN controller receive error register
        Dim regTECounter As Byte    'CAN controller send error register
        Dim regESR As UInt32        'CAN controller status register
        Dim regTSR As UInt32        'CAN controller status register
        Dim BufferSize As UInt32    'CAN controller receive buffer size
        Dim Reserved As UInt32
    End Structure
    'definition of error data type
    Public Structure VCI_ERR_INFO
        Dim ErrCode As UInt32
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=3)> _
        Dim Passive_ErrData As Byte()
        Dim ArLost_ErrData As Byte
    End Structure
    'definition of CAN initialization data type
    Public Structure VCI_INIT_CONFIG
        Dim AccCode As UInt32
        Dim AccMask As UInt32
        Dim Reserved As UInt32
        Dim Filter As Byte
        Dim Timing0 As Byte
        Dim Timing1 As Byte
        Dim Mode As Byte
    End Structure
    'Definition of CAN initialization data type
    Public Structure VCI_INIT_CONFIG_EX
        'CAN baudrate = 36MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
        Dim CAN_BRP As UInt32   'range: 1~1024
        Dim CAN_SJW As Byte     'range: 1~4
        Dim CAN_BS1 As Byte     'range: 1~16 
        Dim CAN_BS2 As Byte     'range: 1~8 
        Dim CAN_Mode As Byte    'CAN working mode. 0: normal，1: loopback，2: silent，3: silent loopback
        Dim CAN_ABOM As Byte    'auto off line management，0: prohibit，1: enable
        Dim CAN_NART As Byte    'text repeat send management，0: enable text repeat sending，1: disable text repeat sending
        Dim CAN_RFLM As Byte    'FIFO lock management，0: new text overwrite old，1: ignore new text
        Dim CAN_TXFP As Byte    'send priority management, 0: by ID，1: by order
        Dim CAN_RELAY As Byte   'relay feature enable，0x00: close relay function，0x10: relay from CAN1 to CAN2，0x01: relay from CAN2 to CAN1，0x11: bidirectionaly relay
        Dim Reserved As UInt32
    End Structure
    'definition of CAN filter setting
    Public Structure VCI_FILTER_CONFIG
        Dim Enable As Byte      'filter enable，1: enable，0: disable
        Dim FilterIndex As Byte 'filter index，range: 0~13
        Dim FilterMode As Byte  'filter mode，0: mask bit，1: id list
        Dim ExtFrame As Byte    'filter frame flag，1: the frame to be filtered is extended frame，0：the frame to be filtered is standard frame
        Dim ID_Std_Ext As UInt32    'verification code ID
        Dim ID_IDE As UInt32        'verification code IDE
        Dim ID_RTR As UInt32        'verification code RTR
        Dim MASK_Std_Ext As UInt32  'Mask code ID，only available when filter mode set to mask bit mode
        Dim MASK_IDE As UInt32      'Mask code IDE，only available when filter mode set to mask bit mode
        Dim MASK_RTR As UInt32      'Mask code RTR，only available when filter mode set to mask bit mode
        Dim Reserved As UInt32
    End Structure

    Public Delegate Sub PVCI_RECEIVE_CALLBACK(ByVal DevIndex As UInt32, ByVal CANIndex As UInt32, ByVal Len As UInt32)

    Declare Function VCI_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Int32
    Declare Function VCI_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal Reserved As UInt32) As Int32
    Declare Function VCI_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32) As Int32
    Declare Function VCI_InitCAN Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32, ByRef pInitConfig As VCI_INIT_CONFIG) As Int32
    Declare Function VCI_InitCANEx Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32, ByRef pInitConfig As VCI_INIT_CONFIG_EX) As Int32

    Declare Function VCI_ReadBoardInfo Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByRef pInfo As VCI_BOARD_INFO) As Int32
    Declare Function VCI_ReadBoardInfoEx Lib "Ginkgo_Driver.dll" (ByVal DevIndex As UInt32, ByRef pInfo As VCI_BOARD_INFO_EX) As Int32
    Declare Function VCI_ReadErrInfo Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32, ByRef pErrInfo As VCI_ERR_INFO) As Int32
    Declare Function VCI_ReadCANStatus Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32, ByRef pCANStatus As VCI_CAN_STATUS) As Int32

    Declare Function VCI_SetFilter Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32, ByRef pFilter As VCI_FILTER_CONFIG) As Int32

    Declare Function VCI_GetReceiveNum Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32) As Int32
    Declare Function VCI_ClearBuffer Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32) As Int32

    Declare Function VCI_StartCAN Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32) As Int32
    Declare Function VCI_ResetCAN Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32) As Int32

    Declare Function VCI_RegisterReceiveCallback Lib "Ginkgo_Driver.dll" (ByVal DevIndex As UInt32, ByVal pReceiveCallBack As PVCI_RECEIVE_CALLBACK) As Int32
    Declare Function VCI_LogoutReceiveCallback Lib "Ginkgo_Driver.dll" (ByVal DevIndex As UInt32) As Int32

    Declare Function VCI_Transmit Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32, ByVal pSend As VCI_CAN_OBJ(), ByVal Len As UInt32) As Int32
    Declare Function VCI_Receive Lib "Ginkgo_Driver.dll" (ByVal DevType As UInt32, ByVal DevIndex As UInt32, ByVal CANIndex As UInt32, <Out()> ByVal pReceive As VCI_CAN_OBJ(), ByVal Len As UInt32, ByRef WaitTime As UInt32) As Int32


End Module
