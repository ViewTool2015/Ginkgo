"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.7.21
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlGPIO.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlGPIO.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlGPIO.py
More Infomation:www.viewtool.com
"""
from ctypes import *
from time import sleep
# import USB-GPIO module
import ControlGPIO

# Scan device(must call one more time)
nRet = c_int(0)
nRet = ControlGPIO.VGI_ScanDevice(1)
if(nRet <= 0):
    print("No device connect!")
    exit()
else:
    print("Connected device number is:"+repr(nRet))

# Open device(must call)
nRet = ControlGPIO.VGI_OpenDevice(ControlGPIO.VGI_USBGPIO,0,0)
if(nRet != ControlGPIO.ERR_SUCCESS):
    print("Open device error!")
    exit()
else:
    print("Open device success!")
# Set GPIO_7 and GPIO_8 to output
nRet = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN7 | ControlGPIO.VGI_GPIO_PIN8)
if(nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin output error!")
    exit()
else:
    print("Set pin output success!")
# Set GPIO_7 and GPIO_8 to hight
nRet = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN7 | ControlGPIO.VGI_GPIO_PIN8);
if (nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin high error!")
    exit()
else:
    print("Set pin high success!")
# Set GPIO_7 and GPIO_8 to low
nRet = ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN7 | ControlGPIO.VGI_GPIO_PIN8);
if (nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin low error!")
    exit()
else:
    print("Set pin low success!")
# Set GPIO_4 adn GPIO_5 to input
nRet = ControlGPIO.VGI_SetInput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 | ControlGPIO.VGI_GPIO_PIN5);
if (nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin input error!")
    exit()
else:
    print("Set pin input success!")
# Get GPIO_4 and GPIO_5 pin status
pin_value = c_ushort(0)
nRet = ControlGPIO.VGI_ReadDatas(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 | ControlGPIO.VGI_GPIO_PIN5, byref(pin_value))
if (nRet != ControlGPIO.ERR_SUCCESS):
    print("Get pin data error!")
    exit()
else:
    if ((pin_value.value & ControlGPIO.VGI_GPIO_PIN4) != 0):
                print("GPIO_4 is high-level!")
    else:
                print("GPIO_4 is low-level!")
    if ((pin_value.value & ControlGPIO.VGI_GPIO_PIN5) != 0):
                print("GPIO_5 is high-level!")
    else:
                print("GPIO_5 is low-level!")
# Set GPIO_4 adn GPIO_5 pins to opendrain
nRet = ControlGPIO.VGI_SetOpenDrain(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 | ControlGPIO.VGI_GPIO_PIN5)
if(nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin OpenDrain error!")
    exit()
else:
    print("Set pin OpenDrain success!")
# Set GPIO_4 and GPIO_5 to hight
nRet = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 | ControlGPIO.VGI_GPIO_PIN5);
if (nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin high error!")
    exit()
else:
    print("Set pin high success!")
# Set GPIO_4 and GPIO_5 to low
nRet = ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 | ControlGPIO.VGI_GPIO_PIN5);
if (nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin low error!")
    exit()
else:
    print("Set pin low success!")
    # Get GPIO_4 and GPIO_5 pins status
    pin_value = c_ushort(0)
    nRet = ControlGPIO.VGI_ReadDatas(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 | ControlGPIO.VGI_GPIO_PIN5, byref(pin_value))
    if (nRet != ControlGPIO.ERR_SUCCESS):
        print("Get pin data error!")
        exit()
    else:
        if ((pin_value.value & ControlGPIO.VGI_GPIO_PIN4) != 0):
            print("GPIO_4 is high-level!")
        else:
            print("GPIO_4 is low-level!")
        if ((pin_value.value & ControlGPIO.VGI_GPIO_PIN5) != 0):
            print("GPIO_5 is high-level!")
        else:
            print("GPIO_5 is low-level!")
# Close device
nRet = ControlGPIO.VGI_CloseDevice(ControlGPIO.VGI_USBGPIO, 0)
if(nRet != ControlGPIO.ERR_SUCCESS):
    print("Close device error!")
    exit()
else:
    print("Close device success!")