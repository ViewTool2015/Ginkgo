"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.2.12
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib
Dependent files(Windows):Ginkgo_Driver.dll
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so
More Infomation:www.viewtool.com
"""
from ctypes import *
import platform
# USB - SPI initialization structure
class VSI_INIT_CONFIG(Structure):  
    _fields_ = [("ControlMode", c_ubyte), # 0-Hardware control (full-duplex mode),1-Hardware control (half duplex mode),2-Software control (half duplex mode),3-Single bus model, data input and output for MOSI
                ("TranBits", c_ubyte),    # 8-8bit,16-16bit
                ("MasterMode", c_ubyte),  # 0-Slave,1-Host
                ("CPOL",c_ubyte),         # 0-Hight,1-Low
                ("CPHA",c_ubyte),         # 0-First,1-Second
                ("LSBFirst",c_ubyte),     # 0-MSB First,1-LSB First
                ("SelPolarity",c_ubyte),  # 0-Sel Low,1-Sel Hight
                ("ClockSpeed",c_int),     # Hardware(36000000,18000000,9000000,4500000,2250000,1125000),Software(1000 To 1000000)
                ];

# Device Type
VAI_USBADC    =    1
VCI_USBCAN1   =    3
VCI_USBCAN2   =    4
VGI_USBGPIO   =    1
VII_USBI2C    =    1
VSI_USBSPI    =    2
VPI_USBPWM    =    2
# Error Code
ERR_SUCCESS               = (0)  #
ERR_PARAMETER_NULL        = (-1) #
ERR_INPUT_DATA_TOO_MUCH   = (-2) #
ERR_INPUT_DATA_TOO_LESS   = (-3) #
ERR_INPUT_DATA_ILLEGALITY = (-4) #
ERR_USB_WRITE_DATA        = (-5) #
ERR_USB_READ_DATA         = (-6) #
ERR_READ_NO_DATA          = (-7) #
ERR_OPEN_DEVICE           = (-8) #
ERR_CLOSE_DEVICE          = (-9) #
ERR_EXECUTE_CMD           = (-10)#
ERR_SELECT_DEVICE         = (-11)#
ERR_DEVICE_OPENED         = (-12)#
ERR_DEVICE_NOTOPEN        = (-13)#
ERR_BUFFER_OVERFLOW       = (-14)#
ERR_DEVICE_NOTEXIST       = (-15)#
ERR_LOAD_KERNELDLL        = (-16)#
ERR_CMD_FAILED            = (-17)#
ERR_BUFFER_CREATE         = (-18)#


# Import library
if(platform.system()=="Windows"):
    if "64bit" in platform.architecture():
        GinkgoLib = windll.LoadLibrary( ".\\lib\\windows\\64bit\\Ginkgo_Driver.dll" )
    else:
        GinkgoLib = windll.LoadLibrary( ".\\lib\\windows\\32bit\\Ginkgo_Driver.dll" )
elif(platform.system()=="Darwin"):
    GinkgoLib = cdll.LoadLibrary( "./lib/macos/libGinkgo_Driver.dylib" )
elif(platform.system()=="Linux"):
    if "64bit" in platform.architecture():
        GinkgoLib = cdll.LoadLibrary( "./lib/linux/64bit/libGinkgo_Driver.so" )
    else:
        GinkgoLib = cdll.LoadLibrary( "./lib/linux/32bit/libGinkgo_Driver.so" )
else:
    print("Unknown system")

# Scan device
def VSI_ScanDevice(NeedInit=1):
    return GinkgoLib.VSI_ScanDevice(c_ubyte(NeedInit))
# Open device
def VSI_OpenDevice(DevType, DevIndex, Reserved):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_OpenDevice(c_int(DevType), c_int(DevIndex), c_int(Reserved))
# Close device
def VSI_CloseDevice( DevType, DevIndex):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_CloseDevice( c_int(DevType), c_int(DevIndex))
# Initialize device
def VSI_InitSPI( DevType,  DevIndex, pInitConfig):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_InitSPI( c_int(DevType),  c_int(DevIndex), pInitConfig)
# Write data to SPI
def VSI_WriteBytes(DevType, DevIndex, SPIIndex, pData, Len):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 <= SPIIndex <= 4, 'SPI index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_WriteBytes(c_int(DevType), c_int(DevIndex), c_int(SPIIndex), pData, c_ushort(Len))
# Read data from SPI
def VSI_ReadBytes(DevType, DevIndex, SPIIndex, pData, ReadLen):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 <= SPIIndex <= 4, 'SPI index error!'
        assert 0 < ReadLen <= 10240, 'Read data too more!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_ReadBytes(c_int(DevType), c_int(DevIndex), c_int(SPIIndex), pData, c_ushort(ReadLen))
# Write&Read data from SPI
def VSI_WriteReadBytes(DevType, DevIndex, SPIIndex, pWriteData, WriteLen, pReadData, ReadLen):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 <= SPIIndex <= 4, 'SPI index error!'
        assert 0 < ReadLen <= 10240, 'Read data too more!'
        assert 0 < WriteLen <= 10230, 'Write data too more!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_WriteReadBytes(c_int(DevType), c_int(DevIndex), c_int(SPIIndex), pWriteData, c_short(WriteLen), pReadData, c_short(ReadLen))
# Binary mode write data to SPI
def VSI_WriteBits(DevType, DevIndex, SPIIndex, pWriteBitStr):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 <= SPIIndex <= 4, 'SPI index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_WriteBits(c_int(DevType), c_int(DevIndex), c_int(SPIIndex), pWriteBitStr)
# Binary mode read data from SPI
def VSI_ReadBits(DevType, DevIndex, SPIIndex, pReadBitStr, ReadBitsNum):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 <= SPIIndex <= 4, 'SPI index error!'
        assert 0 < ReadBitsNum <= 10240*8, 'Write bits too more!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_ReadBits(c_int(DevType), c_int(DevIndex), c_int(SPIIndex), pReadBitStr, c_int(ReadBitsNum))
# Binary mode write&read data to SPI
def VSI_WriteReadBits(DevType, DevIndex, SPIIndex, pWriteBitStr, pReadBitStr, ReadBitsNum):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 <= SPIIndex <= 4, 'SPI index error!'
        assert 0 < ReadBitsNum <= 10240*8, 'Write bits too more!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_WriteReadBits(c_int(DevType), c_int(DevIndex), c_int(SPIIndex), pWriteBitStr, pReadBitStr, c_int(ReadBitsNum))
# Slave mode read data from buffer
def VSI_SlaveReadBytes(DevType,DevIndex, pReadData, pBytesNum, WaitTime):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_SlaveReadBytes(c_int(DevType),c_int(DevIndex), pReadData, pBytesNum, c_int(WaitTime))
# Slave mode write to buffer
def VSI_SlaveWriteBytes(DevType,DevIndex, pWriteData, WriteBytesNum):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 < WriteBytesNum <= 10230, 'Write data too more!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_SlaveWriteBytes(c_int(DevType),c_int(DevIndex), pWriteData, c_int(WriteBytesNum))
# Block mode write
def VSI_BlockWriteBytes(DevType,DevIndex,SPIIndex,pWriteData,BlockSize,BlockNum,IntervalTime):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 <= SPIIndex <= 4, 'SPI index error!'
        assert 0 < (BlockSize*BlockNum) <= 10230, 'Write data too more!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_BlockWriteBytes(c_int(DevType),c_int(DevIndex),c_int(SPIIndex),pWriteData,c_ushort(BlockSize),c_ushort(BlockNum),c_uint(IntervalTime))
# Block mode read
def VSI_BlockReadBytes(DevType,DevIndex,SPIIndex,pReadData,BlockSize,BlockNum,IntervalTime):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 <= SPIIndex <= 4, 'SPI index error!'
        assert 0 < (BlockSize*BlockNum) <= 10240, 'Write data too more!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VSI_BlockReadBytes(c_int(DevType),c_int(DevIndex),c_int(SPIIndex),pReadData,c_ushort(BlockSize),c_ushort(BlockNum),c_uint(IntervalTime))
# Block mode write&read
def VSI_BlockWriteReadBytes(DevType,DevIndex,SPIIndex,pWriteData,WriteBlockSize,pReadData,ReadBlockSize,BlockNum,IntervalTime):
    try:
        assert DevType == VSI_USBSPI, 'Device type error!'
        assert 0 <= SPIIndex <= 4, 'SPI index error!'
        assert 0 < (WriteBlockSize*BlockNum) <= 10230, 'Write data too more!'
        assert 0 < (ReadBlockSize*BlockNum) <= 10240, 'Read data too more!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()    
    return GinkgoLib.VSI_BlockWriteReadBytes(c_int(DevType),c_int(DevIndex),c_int(SPIIndex),pWriteData,c_ushort(WriteBlockSize),pReadData,c_ushort(ReadBlockSize),c_ushort(BlockNum),c_uint(IntervalTime))