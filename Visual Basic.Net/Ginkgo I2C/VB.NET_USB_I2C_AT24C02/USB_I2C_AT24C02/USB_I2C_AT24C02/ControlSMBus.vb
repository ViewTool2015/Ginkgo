Option Strict Off
Option Explicit On

Imports System.Runtime.InteropServices
Module ControlSMBus
    ' General Error Code
    Public Const SMBUS_OK As Int32 = 0                          'Packet was sent correctly
    Public Const SMBUS_ERROR_SLAVE_NOT_SUPPORTED As Int32 = 1   'Slave mode is not supported 
    Public Const SMBUS_ERROR_BUSOFF As Int32 = 2
    Public Const SMBUS_ERROR_TXFULL As Int32 = 3
    Public Const SMBUS_ERROR_BUSY As Int32 = 4
    Public Const SMBUS_ERROR_RXEMPTY As Int32 = 5
    Public Const SMBUS_ERROR_OVERRUN As Int32 = 6
    Public Const SMBUS_ERROR_TIMEOUT As Int32 = 7               'Timeout occurred during sending the packet 
    Public Const SMBUS_ERROR_INVALID_SIZE As Int32 = 8          'Invalid size of received packet
    Public Const SMBUS_ERROR_PACKET_TOO_LONG As Int32 = 9       'Packet to sent does not fit into internal buffer
    Public Const SMBUS_ERROR_PARAMETER As Int32 = 10            'Invalid parameter
    Public Const SMBUS_ERROR_PEC As Int32 = 11                  'PEC error
    Public Const SMBUS_ERROR_NACK As Int32 = 12                 'NACK error
    Public Const SMBUS_ERROR_ARLO As Int32 = 13                 'Arbitration lost (master mode)
    ' Function
    Declare Function SMBus_ScanDevice Lib "Ginkgo_Driver.dll" (ByVal NeedInit As Byte) As Int32
    Declare Function SMBus_OpenDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal Reserved As Int32) As Int32
    Declare Function SMBus_CloseDevice Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32) As Int32
    Declare Function SMBus_HardInit Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal ClockSpeed As Int32, ByVal OwnAddr As Byte) As Int32
    Declare Function SMBus_QuickCommand Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte) As Int32
    Declare Function SMBus_WriteByte Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte, ByVal Data As Byte, ByVal PEC As Byte) As Int32
    Declare Function SMBus_ReadByte Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte, ByVal pData() As Byte, ByVal PEC As Byte) As Int32
    Declare Function SMBus_WriteByteProtocol Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte, ByVal CommandCode As Byte, ByVal Data As Byte, ByVal PEC As Byte) As Int32
    Declare Function SMBus_WriteWordProtocol Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte, ByVal CommandCode As Byte, ByVal Data As UInt16, ByVal PEC As Byte) As Int32
    Declare Function SMBus_ReadByteProtocol Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte, ByVal CommandCode As Byte, ByVal pData() As Byte, ByVal PEC As Byte) As Int32
    Declare Function SMBus_ReadWordProtocol Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte, ByVal CommandCode As Byte, ByVal pData() As UInt16, ByVal PEC As Byte) As Int32
    Declare Function SMBus_ProcessCall Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte, ByVal CommandCode As Byte, ByVal WriteData As UInt16, ByVal pReadData() As UInt16, ByVal PEC As Byte) As Int32
    Declare Function SMBus_BlockWrite Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte, ByVal CommandCode As Byte, ByVal pData() As Byte, ByVal ByteCount As Byte, ByVal PEC As Byte) As Int32
    Declare Function SMBus_BlockProcessCall Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByVal SlaveAddr As Byte, ByVal CommandCode As Byte, ByVal pWriteData() As Byte, ByVal WriteByteCount As Byte, ByVal pReadData() As Byte, ByRef pReadByteCount As Byte, ByVal PEC As Byte) As Int32
    Declare Function SMBus_GetAlert Lib "Ginkgo_Driver.dll" (ByVal DevType As Int32, ByVal DevIndex As Int32, ByVal SMBUSIndex As Int32, ByRef pAlertFlag As Byte) As Int32

End Module
