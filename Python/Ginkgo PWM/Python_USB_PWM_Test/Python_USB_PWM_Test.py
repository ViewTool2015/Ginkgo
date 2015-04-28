"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.8.28
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlPWM.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlPWM.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlPWM.py
More Infomation:www.viewtool.com
"""
from ctypes import *
from time import sleep
# import USB-SPI module
import ControlPWM

# Scan device(must call one more time)
nRet = c_int(0)
nRet = ControlPWM.VPI_ScanDevice(1)
if(nRet <= 0):
    print("No device connect!")
    exit()
else:
    print("Connected device number is:"+repr(nRet))

# Open device(must call)
nRet = ControlPWM.VPI_OpenDevice(ControlPWM.VPI_USBPWM,0,0)
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Open device error!")
    exit()
else:
    print("Open device success!")
# Initialize PWM_CH0
PWM_Config = ControlPWM.VPI_INIT_CONFIG()
PWM_Config.PWM_ChannelMask = ControlPWM.VPI_PWM_CH0
PWM_Config.PWM_Frequency = 10000
PWM_Config.PWM_Mode = 0
PWM_Config.PWM_Polarity = 0
PWM_Config.PWM_Pulse = 50
nRet = ControlPWM.VPI_InitPWM(ControlPWM.VPI_USBPWM,0,byref(PWM_Config))
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Initialize device error!")
    exit()
else:
    print("Initialize device success!")
# Start PWM_CH0
nRet = ControlPWM.VPI_StartPWM(ControlPWM.VPI_USBPWM,0,ControlPWM.VPI_PWM_CH0)
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Start PWM error!")
    exit()
else:
    print("Start PWM success!")
# Run 1 second
sleep(1)
# Stop PWM_CH0
nRet = ControlPWM.VPI_StopPWM(ControlPWM.VPI_USBPWM,0,ControlPWM.VPI_PWM_CH0)
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Stop PWM error!")
    exit()
else:
    print("Stop PWM success!")    
# Initialize PWM_ALL
PWM_Config = ControlPWM.VPI_INIT_CONFIG()
PWM_Config.PWM_ChannelMask = ControlPWM.VPI_PWM_ALL
PWM_Config.PWM_Frequency = 10000
PWM_Config.PWM_Mode = 0
PWM_Config.PWM_Polarity = 0
PWM_Config.PWM_Pulse = 50
nRet = ControlPWM.VPI_InitPWM(ControlPWM.VPI_USBPWM,0,byref(PWM_Config))
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Initialize device error!")
    exit()
else:
    print("Initialize device success!")
# Start PWM_CH0 and PWM_CH2
nRet = ControlPWM.VPI_StartPWM(ControlPWM.VPI_USBPWM,0,ControlPWM.VPI_PWM_CH0|ControlPWM.VPI_PWM_CH2)
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Start PWM error!")
    exit()
else:
    print("Start PWM success!")
# Run 1 second
sleep(1)
# Adjust the PWM_CH0 clock frequency
Frequency = (c_int * 8)()
Frequency[0] = 50000
nRet = ControlPWM.VPI_SetPWMPeriod(ControlPWM.VPI_USBPWM, 0, ControlPWM.VPI_PWM_CH0, Frequency)
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Set frequency error!")
    exit()
else:
    print("Set frequency success!")
# Adjust PWM_CH2 duty ratio
Pulse = (c_ubyte * 8)()
Pulse[2] = 80
nRet = ControlPWM.VPI_SetPWMPulse(ControlPWM.VPI_USBPWM, 0, ControlPWM.VPI_PWM_CH2, Frequency)
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Set duty ratio error!")
    exit()
else:
    print("Set duty ratio success!")
# Run 1 second
sleep(1)
# Adjust PWM_CH0 and PWM_CH2 duty ratio
Pulse = (c_ubyte * 8)()
Pulse[0] = 80
Pulse[2] = 20
nRet = ControlPWM.VPI_SetPWMPulse(ControlPWM.VPI_USBPWM, 0, ControlPWM.VPI_PWM_CH0|ControlPWM.VPI_PWM_CH2, Frequency)
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Set duty ratio error!")
    exit()
else:
    print("Set duty ratio success!")
# Run 1 second
sleep(1)
# Close device
nRet = ControlPWM.VPI_CloseDevice(ControlPWM.VPI_USBPWM,0)
if(nRet != ControlPWM.ERR_SUCCESS):
    print("Close device error! %d"%nRet)
    exit()
else:
    print("Close device success!")