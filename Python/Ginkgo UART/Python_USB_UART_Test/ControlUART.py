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

# UART data types initialization Define
class UART_INIT_CONFIG(Structure):  
    _fields_ = [
        ("StopBits", c_ubyte),    # Stop bits:0-1bit,1-1.5bit,2-2bit,3-1.5bit
        ("WordLength", c_ubyte),# 8
        ("Parity", c_ubyte),    # 0-No,4-Even,6-Odd
        ("RS485Mode",c_ushort),    # 485-RS485 mode,232-RS232 mode
        ("BaudRate",c_uint)        # Baudrate
    ];
# Data types of Ginkgo series adapter information.
class VII_BOARD_INFO(Structure):  
    _fields_ = [
        ("ProductName", c_ubyte*32),    # Product name
        ("FirmwareVersion", c_ubyte*4),    # Firmware version
        ("HardwareVersion", c_ubyte*4),    # Hardware version
        ("SerialNumber",c_ubyte*12),    # The adapter serial number
    ];

# The adapter type definition
UART_USBUART    =    2

# Error Type
ERR_SUCCESS                 =    (0)     #
ERR_PARAMETER_NULL          =    (-1)    #
ERR_INPUT_DATA_TOO_MUCH     =    (-2)    # 
ERR_INPUT_DATA_TOO_LESS     =    (-3)    # 
ERR_INPUT_DATA_ILLEGALITY   =    (-4)    #
ERR_USB_WRITE_DATA          =    (-5)    # 
ERR_USB_READ_DATA           =    (-6)    # 
ERR_READ_NO_DATA            =    (-7)    #
ERR_OPEN_DEVICE             =    (-8)    #
ERR_CLOSE_DEVICE            =    (-9)    #
ERR_EXECUTE_CMD             =    (-10)   #
ERR_SELECT_DEVICE           =    (-11)   #
ERR_DEVICE_OPENED           =    (-12)   #
ERR_DEVICE_NOTOPEN          =    (-13)   #
ERR_BUFFER_OVERFLOW         =    (-14)   #
ERR_DEVICE_NOTEXIST         =    (-15)   #
ERR_LOAD_KERNELDLL          =    (-16)   #
ERR_CMD_FAILED              =    (-17)   #
ERR_BUFFER_CREATE           =    (-18)   #

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
def UART_ScanDevice(NeedInit=1):
    'Scan device'
    return GinkgoLib.UART_ScanDevice(c_ubyte(NeedInit))
# Open device 
def UART_OpenDevice(DevType, DevIndex, Reserved=0):
    'Open device'
    try:
        assert DevType == UART_USBUART, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.UART_OpenDevice(c_int(DevType), c_int(DevIndex), c_int(Reserved))

# Close device
def UART_CloseDevice( DevType, DevIndex):
    'Close device'
    try:
        assert DevType == UART_USBUART, 'Device type error!'
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
    return GinkgoLib.UART_ReadBoardInfo(c_int(DevIndex), pBoardInfo)

# Config UART
def UART_InitDevice(DevType,DevIndex,UARTIndex,pInitConfig):
    'Config USB-UART adapter'
    try:
        assert DevType == UART_USBUART, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0 <= UARTIndex < 2, 'Channel index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.UART_InitDevice(c_int(DevType),c_int(DevIndex),c_int(UARTIndex), pInitConfig)
# Write bytes to UART bus
def UART_WriteBytes(DevType, DevIndex, UARTIndex, pWriteData, WriteLen):
    'Write bytes to UART bus'
    try:
        assert DevType == UART_USBUART, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0 <= UARTIndex < 2, 'Channel index error!'
        assert 0 < WriteLen < 10230, 'Too much data'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.UART_WriteBytes(DevType,c_int(DevIndex),c_int(UARTIndex), pWriteData, c_ushort(WriteLen))
# Read bytes from buffer
def UART_ReadBytes( DevType, DevIndex, UARTIndex, pReadData, pReadLen):
    'Read bytes from buffer'
    try:
        assert DevType == UART_USBUART, 'Device type error!'
        assert DevIndex >= 0, 'Device index error!'
        assert 0 <= UARTIndex < 2, 'Channel index error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.UART_ReadBytes(c_int(DevType), c_int(DevIndex), c_int(UARTIndex), pReadData, pReadLen)
