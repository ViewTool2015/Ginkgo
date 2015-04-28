"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.8.28
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib
Dependent files(Windows):Ginkgo_Driver.dll
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so
More Infomation:www.viewtool.com
"""
from ctypes import *
import platform

# I2C data types initialization Define
class VII_INIT_CONFIG(Structure):  
	_fields_ = [
		("MasterMode", c_ubyte),	# Master-slave choice: 0-slave, 1-master
		("ControlMode", c_ubyte),	# Control mode: 0-Standard slave mode, 1-Standard mode, 2-GPIO mode
		("AddrType", c_ubyte),		# 7-7bit mode, 10-10bit mode
		("SubAddrWidth",c_ubyte),	# Sub-Address width: value of 0~4, 0 means no Sub-Address mode
		("Addr",c_ushort),			# Device address in salve mode
		("ClockSpeed",c_uint)		# Clock frequency(HZ)
	];
# Data types of Ginkgo series adapter information.
class VII_BOARD_INFO(Structure):  
	_fields_ = [
		("ProductName", c_ubyte*32),	# Product name
		("FirmwareVersion", c_ubyte*4),	# Firmware version
		("HardwareVersion", c_ubyte*4),	# Hardware version
		("SerialNumber",c_ubyte*12),	# The adapter serial number
	];
# I2C time parameter definition in GPIO mode(ms)
class VII_TIME_CONFIG(Structure):
	_fields_ = [
		("tHD_STA",c_ushort),		# Timing for start signal Keeping
		("tSU_STA",c_ushort),		# Timing for start signal be established
		("tLOW",c_ushort),			# Timing for clock low level
		("tHIGH",c_ushort),			# Timing for clock high level
		("tSU_DAT",c_ushort),		# Timing for data input be established
		("tSU_STO",c_ushort),		# Timing for stop signal be established
		("tDH",c_ushort),			# Timing for data output Keeping
		("tDH_DAT",c_ushort),		# Timing for data input Keeping
		("tAA",c_ushort),			# SCL lower to SDA output and response signal
		("tR",c_ushort),			# Timing for SDA and SCL rising
		("tF",c_ushort),			# Timing for SDA and SCL going down
		("tBuf",c_ushort),			# Free timing of the bus until the new mission
		("tACK",c_ubyte*4),
		("tStart",c_ushort),
		("tStop",c_ushort)
	];
# The adapter type definition
VAI_USBADC	=	1
VCI_USBCAN1	=	3
VCI_USBCAN2	=	4
VGI_USBGPIO	=	1
VII_USBI2C	=	1
VSI_USBSPI	=	2
# The adapter data initialization definition
VII_ADDR_7BIT		=	7	# 7-bit address mode
VII_ADDR_10BIT		=	10	# 10-bit address mode
VII_HCTL_SLAVE_MODE =   0   # Standard slave mode
VII_HCTL_MODE		=	1	# Standard mode
VII_SCTL_MODE		=	2	# GPIO mode
VII_MASTER			=	1	# Master
VII_SLAVE			=	0	# Slave
VII_SUB_ADDR_NONE	=	0	# No sub-address
VII_SUB_ADDR_1BYTE	=	1	# 1Byte sub-address
VII_SUB_ADDR_2BYTE	=	2	# 2Byte sub-address
VII_SUB_ADDR_3BYTE	=	3	# 3Byte sub-address
VII_SUB_ADDR_4BYTE	=	4	# 4Byte sub-address
# Error Type
ERR_SUCCESS					=	(0)		#
ERR_PARAMETER_NULL			=	(-1)	#
ERR_INPUT_DATA_TOO_MUCH		=	(-2)	# 
ERR_INPUT_DATA_TOO_LESS		=	(-3)	# 
ERR_INPUT_DATA_ILLEGALITY	=	(-4)	#
ERR_USB_WRITE_DATA			=	(-5)	# 
ERR_USB_READ_DATA			=	(-6)	# 
ERR_READ_NO_DATA			=	(-7)	#
ERR_OPEN_DEVICE				=	(-8)	#
ERR_CLOSE_DEVICE			=	(-9)	#
ERR_EXECUTE_CMD				=	(-10)	#
ERR_SELECT_DEVICE			=	(-11)	#
ERR_DEVICE_OPENED			=	(-12)	#
ERR_DEVICE_NOTOPEN			=	(-13)	#
ERR_BUFFER_OVERFLOW			=	(-14)	#
ERR_DEVICE_NOTEXIST			=	(-15)	#
ERR_LOAD_KERNELDLL			=	(-16)	#
ERR_CMD_FAILED				=	(-17)	#
ERR_BUFFER_CREATE			=	(-18)	#

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
def VII_ScanDevice(NeedInit=1):
    'Scan device'
    return GinkgoLib.VII_ScanDevice(c_ubyte(NeedInit))
# Open device 
def VII_OpenDevice(DevType, DevIndex, Reserved=0):
    'Open device'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_OpenDevice(c_int(DevType), c_int(DevIndex), c_int(Reserved))

# Close device
def VII_CloseDevice( DevType, DevIndex):
    'Close device'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_CloseDevice(c_int(DevType), c_int(DevIndex))
# Read board infomation
def VII_ReadBoardInfo(DevIndex, pBoardInfo):
    'Read board infomation'
    try:
        assert DevIndex >= 0, 'Device index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_ReadBoardInfo(c_int(DevIndex), pBoardInfo)
# Config I2C Timing(GPIO Mode)
def VII_TimeConfig( DevType,  DevIndex,  I2CIndex, pTimeConfig):
    'Read board infomation'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0<=I2CIndex<8, 'Channel index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_TimeConfig(c_int(DevType),c_int(DevIndex),c_int(I2CIndex), pTimeConfig)
# Config I2C
def VII_InitI2C(DevType,DevIndex,I2CIndex,pInitConfig):
    'Config USB-I2C adapter'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0<=I2CIndex<8, 'Channel index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_InitI2C(c_int(DevType),c_int(DevIndex),c_int(I2CIndex), pInitConfig)
# Write bytes to I2C bus
def VII_WriteBytes(DevType, DevIndex, I2CIndex, SlaveAddr, SubAddr, pWriteData, WriteLen):
    'Write bytes to I2C bus'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0<=I2CIndex<8, 'Channel index error!'
        assert 0 < WriteLen < 10230, 'Too much data'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_WriteBytes(DevType,c_int(DevIndex),c_int(I2CIndex),c_ushort(SlaveAddr), c_int(SubAddr), pWriteData, c_ushort(WriteLen))
# Read bytes from I2C bus
def VII_ReadBytes( DevType, DevIndex, I2CIndex, SlaveAddr, SubAddr, pReadData, ReadLen):
    'Read bytes from I2C bus'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0 <= I2CIndex < 8, 'Channel index error!'
        assert 0 < ReadLen < 10230, 'Too much data'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_ReadBytes(c_int(DevType), c_int(DevIndex), c_int(I2CIndex), c_int(SlaveAddr), c_int(SubAddr), pReadData, c_ushort(ReadLen))
# Read bytes in slave mode
def VII_SlaveReadBytes( DevType, DevIndex, I2CIndex, pReadData, pReadLen):
    'Read bytes in slave mode'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0 <= I2CIndex < 8, 'Channel index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_SlaveReadBytes(c_int(DevType), c_int(DevIndex), c_int(I2CIndex), pReadData, pReadLen)
# Write bytes in slave mode
def VII_SlaveWriteBytes(DevType,DevIndex,I2CIndex,pWriteData,WriteLen):
    'Write bytes in slave mode'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0 <= I2CIndex < 8, 'Channel index error!'
        assert 0 < WriteLen < 10230, 'Too much data'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_SlaveWriteBytes(c_int(DevType),c_int(DevIndex),c_int(I2CIndex),pWriteData,c_ushort(WriteLen))
# Query the remaining bytes in slave mode
def VII_SlaveWriteRemain(DevType, DevIndex, I2CIndex,pRemainBytes):
    'Query the remaining bytes in slave mode'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0 <= I2CIndex < 2, 'Channel index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_SlaveWriteRemain(c_int(DevType),c_int(DevIndex),c_int(I2CIndex),pRemainBytes)
# Write user key data to adapter
def VII_SetUserKey(DevType,DevIndex,pUserKey):
    'Write user key data to adapter'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_SetUserKey(c_int(DevType),c_int(DevIndex),pUserKey)
# Check user key from adapter
def VII_CheckUserKey(DevType,DevIndex,pUserKey):
    'Check user key from adapter'
    try:
        assert DevType == VII_USBI2C, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VII_CheckUserKey(c_int(DevType),c_int(DevIndex),pUserKey)


