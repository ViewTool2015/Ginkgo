"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.7.21
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib
Dependent files(Windows):Ginkgo_Driver.dll
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so
More Infomation:www.viewtool.com
"""
from ctypes import *
import platform

# Device Type
VAI_USBADC	=	1
CNT_USBCNT  =   1
VCI_USBCAN1	=	3
VCI_USBCAN2	=	4
VGI_USBGPIO	=	1
VII_USBI2C	=	1
VSI_USBSPI	=	2
VPI_USBPWM	=	2
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

# Define Counter channel
CNT_CH0  =      (1<<0)	#CNT_CH0	
CNT_CH1	 =	(1<<1)	#CNT_CH1	
CNT_CH2	 =	(1<<2)	#CNT_CH2	
CNT_CH3	 =	(1<<3)	#CNT_CH3	
CNT_ALL	 =	(0x0F)	#CNT_CH_ALL	


class CNT_INIT_CONFIG(Structure):  
	_fields_ = [("CounterMode", c_uint8),     # Counter Mode: 0-Up,1-Down
	            ("CounterPolarity", c_uint8), # Counter Polarity:0-Rising,1-Falling,2-BothEdge
	            ("CounterBitWide", c_uint8),  # Counter Bit Wide:16-16bit,32-32bit
	            ];



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
def CNT_ScanDevice(NeedInit=1):
	return GinkgoLib.CNT_ScanDevice(c_ubyte(NeedInit))
# Open device
def CNT_OpenDevice(DevType, DevIndex, Reserved):
    try:
        assert DevType == CNT_USBCNT, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.CNT_OpenDevice(c_int(DevType), c_int(DevIndex), c_int(Reserved))
# Close device
def CNT_CloseDevice(DevType, DevIndex):
    try:
        assert DevType == CNT_USBCNT, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.CNT_CloseDevice( c_int(DevType), c_int(DevIndex))
# Initialize Counter
def CNT_InitCounter(DevType, DevIndex, Channel, pInitConfig):
    try:
        assert DevType == CNT_USBCNT, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.CNT_InitCounter(c_int(DevType), c_int(DevIndex), c_uint8(Channel), pInitConfig)
# Set Counter value
def CNT_SetCounter( DevType, DevIndex, Channel, pCounterValue):
    try:
        assert DevType == CNT_USBCNT, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.CNT_SetCounter(c_int32(DevType), c_int32(DevIndex), c_uint8(Channel), pCounterValue)
# Get Counter value
def CNT_GetCounter( DevType, DevIndex, Channel, pCounterValue):
    try:
        assert DevType == CNT_USBCNT, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.CNT_GetCounter(c_int32(DevType), c_int32(DevIndex), c_uint8(Channel), pCounterValue)
# Start Counter
def CNT_StartCounter( DevType, DevIndex, Channel):
    try:
        assert DevType == CNT_USBCNT, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.CNT_StartCounter(c_int32(DevType), c_int32(DevIndex), c_uint8(Channel))
# Stop Counter
def CNT_StopCounter( DevType, DevIndex, Channel):
    try:
        assert DevType == CNT_USBCNT, 'Device type error!'
    except AssertionError as args:
        print('%s: %s'%(args.__class__.__name__, args))
        exit()
    return GinkgoLib.CNT_StopCounter(c_int32(DevType), c_int32(DevIndex), c_uint8(Channel))
