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
# GPIO Pin define
VGI_GPIO_PIN0    =    (1<<0)    # GPIO_0
VGI_GPIO_PIN1    =    (1<<1)    # GPIO_1
VGI_GPIO_PIN2    =    (1<<2)    # GPIO_2
VGI_GPIO_PIN3    =    (1<<3)    # GPIO_3
VGI_GPIO_PIN4    =    (1<<4)    # GPIO_4
VGI_GPIO_PIN5    =    (1<<5)    # GPIO_5
VGI_GPIO_PIN6    =    (1<<6)    # GPIO_6
VGI_GPIO_PIN7    =    (1<<7)    # GPIO_7
VGI_GPIO_PIN8    =    (1<<8)    # GPIO_8
VGI_GPIO_PIN9    =    (1<<9)    # GPIO_9
VGI_GPIO_PIN10   =    (1<<10)    # GPIO_10
VGI_GPIO_PIN11   =    (1<<11)    # GPIO_11
VGI_GPIO_PIN12   =    (1<<12)    # GPIO_12
VGI_GPIO_PIN13   =    (1<<13)    # GPIO_13
VGI_GPIO_PIN14   =    (1<<14)    # GPIO_14
VGI_GPIO_PIN15   =    (1<<15)    # GPIO_15
VGI_GPIO_PIN_ALL =    (0xFFFF)# ALL PIN

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
def VGI_ScanDevice(NeedInit=1):
    return GinkgoLib.VGI_ScanDevice(c_ubyte(NeedInit))
# Open device
def VGI_OpenDevice(DevType, DevIndex, Reserved):
    return GinkgoLib.VGI_OpenDevice(c_int(DevType), c_int(DevIndex), c_int(Reserved))
# Close device
def VGI_CloseDevice( DevType, DevIndex):
    return GinkgoLib.VGI_CloseDevice( c_int(DevType), c_int(DevIndex))
# Set GPIO input
def VGI_SetInput(DevType, DevIndex, PinsMask):
    return GinkgoLib.VGI_SetInput(c_int(DevType), c_int(DevIndex), c_ushort(PinsMask))
# Set GPIO output
def VGI_SetOutput(DevType, DevIndex, PinsMask):
    return GinkgoLib.VGI_SetOutput(c_int(DevType), c_int(DevIndex), c_ushort(PinsMask))
# Set GPIO input&output
def VGI_SetOpenDrain(DevType, DevIndex, PinsMask):
    return GinkgoLib.VGI_SetOpenDrain(c_int(DevType), c_int(DevIndex), c_ushort(PinsMask))
# Set GPIO mode
def VGI_InitGPIO(DevType, DevIndex, PinMask, PinMode):
    return GinkgoLib.VGI_InitGPIO(c_int(DevType), c_int(DevIndex), c_ushort(PinMask), c_uint(PinMode))
# Get GPIO value
def VGI_ReadDatas(DevType, DevIndex, PinsMask, pData):
    return GinkgoLib.VGI_ReadDatas(c_int(DevType), c_int(DevIndex), c_ushort(PinsMask), pData)    
# Set GPIO pins to hight
def VGI_SetPins(DevType, DevIndex, PinsMask):
    return GinkgoLib.VGI_SetPins(c_int(DevType), c_int(DevIndex), c_ushort(PinsMask))
# Set GPIO pins to low
def VGI_ResetPins(DevType, DevIndex, PinsMask):
    return GinkgoLib.VGI_ResetPins(c_int(DevType), c_int(DevIndex), c_ushort(PinsMask))
# Write data to GPIO
def VGI_WriteDatas(DevType,DevIndex, PinMask,Data):
    return GinkgoLib.VGI_WriteDatas(c_int(DevType),c_int(DevIndex), c_ushort(PinMask),c_ushort(Data));
