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
# USB-PWM initialize structure
class VPI_INIT_CONFIG(Structure):  
    _fields_ = [("PWM_ChannelMask", c_ubyte), # PWM channel, each bit corresponds to a channel
                ("PWM_Mode", c_ubyte),        # PWM output mode
                ("PWM_Pulse", c_ubyte),       # PWM Output duty cycle
                ("PWM_Polarity",c_ubyte),     # PWM output polarity
                ("PWM_Frequency",c_int),      # PWM output clock frequency, the unit for Hz, value range is 1 ~ 20000000
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
# PWM Channel
VPI_PWM_CH0    =    (1<<0)    # PWM_CH0
VPI_PWM_CH1    =    (1<<1)    # PWM_CH1
VPI_PWM_CH2    =    (1<<2)    # PWM_CH2
VPI_PWM_CH3    =    (1<<3)    # PWM_CH3
VPI_PWM_CH4    =    (1<<4)    # PWM_CH4
VPI_PWM_CH5    =    (1<<5)    # PWM_CH5
VPI_PWM_CH6    =    (1<<6)    # PWM_CH6
VPI_PWM_CH7    =    (1<<7)    # PWM_CH7
VPI_PWM_ALL    =    (0xFF)    # PWM_ALL

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
def VPI_ScanDevice(NeedInit=1):
    'Scan device'
    return GinkgoLib.VSI_ScanDevice(c_ubyte(NeedInit))
# Open device
def VPI_OpenDevice(DevType, DevIndex, Reserved):
    'Open device'
    return GinkgoLib.VSI_OpenDevice(c_int(DevType), c_int(DevIndex), c_int(Reserved))
# Close device
def VPI_CloseDevice( DevType, DevIndex):
    'Close device'
    return GinkgoLib.VSI_CloseDevice( c_int(DevType), c_int(DevIndex))
# Initialize device
def VPI_InitPWM(DevType, DevIndex, pInitConfig):
    'Initialize device'
    return GinkgoLib.VPI_InitPWM(c_int(DevType), c_int(DevIndex), pInitConfig)
# Start PWM
def VPI_StartPWM(DevType, DevIndex, ChannelMask):
    'Start PWM'
    return GinkgoLib.VPI_StartPWM(c_int(DevType), c_int(DevIndex), c_ubyte(ChannelMask))
# Stop PWM
def VPI_StopPWM(DevType, DevIndex, ChannelMask):
    'Stop PWM'
    return GinkgoLib.VPI_StopPWM(c_int(DevType), c_int(DevIndex), c_ubyte(ChannelMask))
# Set PWM duty cycle
def VPI_SetPWMPulse(DevType, DevIndex, ChannelMask, pPulse):
    'Set PWM duty cycle'
    return GinkgoLib.VPI_SetPWMPulse(c_int(DevType), c_int(DevIndex), c_ubyte(ChannelMask), pPulse)
# Set PWM frequency
def VPI_SetPWMPeriod(DevType, DevIndex, ChannelMask, pFrequency):
    'Set PWM frequency'
    return GinkgoLib.VPI_SetPWMPeriod(c_int(DevType), c_int(DevIndex), c_ubyte(ChannelMask), pFrequency)