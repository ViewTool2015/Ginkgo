"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.8.21
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib
Dependent files(Windows):Ginkgo_Driver.dll
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so
"""
from ctypes import *
import platform
# Device type
VDA_USBDAC    =    1
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

# DA Channel
VDA_DAC_CH0    =    (0)     # DAC_CH0
VDA_DAC_CH1    =    (1)     # DAC_CH1

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
def VDA_ScanDevice(NeedInit=1):
    'Scan device'
    return GinkgoLib.VDA_ScanDevice(c_ubyte(NeedInit))
# Open device
def VDA_OpenDevice(DevType, DevIndex, Reserved):
    'Open device'
    try:
        assert DevType == VDA_USBDAC, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VDA_OpenDevice(c_int(DevType), c_int(DevIndex), c_int(Reserved))
# Close device
def VDA_CloseDevice( DevType, DevIndex):
    'Close device'
    try:
        assert DevType == VDA_USBDAC, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VDA_CloseDevice( c_int(DevType), c_int(DevIndex))
# Initialize device
def VDA_InitDAC(DevType, DevIndex, Channel, pWaveData, WaveDataNum, Period):
    'Initialize device'
    try:
        assert DevType == VDA_USBDAC, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VDA_InitDAC(c_uint32(DevType), c_uint32(DevIndex), c_uint8(Channel),pWaveData, c_uint16(WaveDataNum), c_uint16(Period))
# Start DAC
def VDA_StartDAC(DevType,DevIndex, Channel):
    'Start DAC'
    try:
        assert DevType == VDA_USBDAC, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VDA_StartDAC(c_uint32(DevType),c_uint32(DevIndex), c_uint8(Channel))
# Stop DAC
def VDA_StopDAC(DevType,DevIndex, Channel):
    'Stop DAC'
    try:
        assert DevType == VDA_USBDAC, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VDA_StopDAC(c_uint32(DevType),c_uint32(DevIndex), c_uint8(Channel))

