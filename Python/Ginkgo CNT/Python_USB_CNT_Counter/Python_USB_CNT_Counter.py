"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.7.21
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlCNT.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlCNT.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlCNT.py
More Infomation:www.viewtool.com
"""
from ctypes import *
from time import sleep
# import USB-CNT module
import ControlCNT

# Scan device(must call one more time)
nRet = c_int(0)
nRet = ControlCNT.CNT_ScanDevice(1)
if(nRet <= 0):
	print("No device connect!")
	exit()
else:
	print("Connected device number is:"+repr(nRet))
# Open device(must call)
nRet = ControlCNT.CNT_OpenDevice(ControlCNT.CNT_USBCNT,0,0)
if(nRet != ControlCNT.ERR_SUCCESS):
	print("Open device error!")
	exit()
else:
	print("Open device success!")
# Config device
CNTConfig = ControlCNT.CNT_INIT_CONFIG()
CNTConfig.CounterMode = 0;
CNTConfig.CounterPolarity = 0;
CNTConfig.CounterBitWide = 32;
nRet = ControlCNT.CNT_InitCounter(ControlCNT.CNT_USBCNT,0,ControlCNT.CNT_CH0,byref(CNTConfig))
if(nRet != ControlCNT.ERR_SUCCESS):
    print("Config device error!!")
    exit()
# Clear counter
CounterValue = (c_uint32 * 4)()
CounterValue[0] = 0
nRet = ControlCNT.CNT_SetCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNT_CH0, CounterValue)
if(nRet != ControlCNT.ERR_SUCCESS):
    print("Set counter value error!")
    exit()
# Start counter
nRet = ControlCNT.CNT_StartCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNT_CH0)
if(nRet != ControlCNT.ERR_SUCCESS):
    print("Start counter error!")
    exit()
while 1:
    nRet = ControlCNT.CNT_GetCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNT_CH0, CounterValue)
    if(nRet != ControlCNT.ERR_SUCCESS):
        print("Get counter value error!")
    else:
        print("Counter Value :%d"%CounterValue[0])
    sleep(0.1)

