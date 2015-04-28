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

# ADC Channel
VAI_ADC_CH0    =    (1<<0)    # ADC_CH0
VAI_ADC_CH1    =    (1<<1)    # ADC_CH1
VAI_ADC_CH2    =    (1<<2)    # ADC_CH2
VAI_ADC_CH3    =    (1<<3)    # ADC_CH3
VAI_ADC_CH4    =    (1<<4)    # ADC_CH4
VAI_ADC_CH5    =    (1<<5)    # ADC_CH5
VAI_ADC_CH6    =    (1<<6)    # ADC_CH6
VAI_ADC_CH7    =    (1<<7)    # ADC_CH7
VAI_ADC_ALL    =    (0xFF)    # ADC_ALL
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
def VAI_ScanDevice(NeedInit=1):
    'Scan device'
    return GinkgoLib.VAI_ScanDevice(c_ubyte(NeedInit))
# Open device
def VAI_OpenDevice(DevType, DevIndex, Reserved):
    'Open device'
    try:
        assert DevType == VAI_USBADC, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VAI_OpenDevice(c_int(DevType), c_int(DevIndex), c_int(Reserved))
# Close device
def VAI_CloseDevice( DevType, DevIndex):
    'Close device'
    try:
        assert DevType == VAI_USBADC, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VAI_CloseDevice( c_int(DevType), c_int(DevIndex))
# Initialize device
def VAI_InitADC(DevType, DevIndex, Channel, Period):
    'Initialize device'
    try:
        assert DevType == VAI_USBADC, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VAI_InitADC(c_int(DevType), c_int(DevIndex), c_ubyte(Channel), c_ushort(Period))
# Get ADC data
def VAI_ReadDatas(DevType,DevIndex, DataNum, pData):
    'Read ADC devdataice'
    try:
        assert DevType == VAI_USBADC, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.VAI_ReadDatas(c_int(DevType),c_int(DevIndex), c_ushort(DataNum), pData)
    
