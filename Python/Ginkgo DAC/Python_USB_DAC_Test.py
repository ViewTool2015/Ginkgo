"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.8.21
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlADC.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlADC.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlADC.py
"""
from ctypes import *
from math import *
from time import sleep
# import USB-DAC module
import ControlDAC
import math

# Scan device(must call one more time)
nRet = c_int(0)
nRet = ControlDAC.VDA_ScanDevice(1)
if(nRet <= 0):
	print("No device connect!")
	exit()
else:
	print("Connected device number is:"+repr(nRet))

# Open device(must call)
nRet = ControlDAC.VDA_OpenDevice(ControlDAC.VDA_USBDAC,0,0)
if(nRet != ControlDAC.ERR_SUCCESS):
	print("Open device error!")
	exit()
else:
	print("Open device success!")

WaveDataBuff=(c_ushort * 1024)()

#Configuration DAC to output dc voltage
WaveDataBuff[0] = 1532
print("Output DC Voltage",WaveDataBuff[0]*3.3/4095)
nRet = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC,0,0,WaveDataBuff,1,0)
if(nRet != ControlDAC.ERR_SUCCESS):
        print("Config error")
        exit()
else:
        print("Configuration DAC to output dc voltage")
#Start DAC
nRet = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)
if(nRet != ControlDAC.ERR_SUCCESS):
    print("Start DAC error!")
    exit()
print("Press Enter to continue...")
input()
#Stop DAC  
ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)
  
#Configuration DAC to output square wave
for i in range(0, 64):
    WaveDataBuff[i] = 0xfff
    WaveDataBuff[i + 64] = 0x00
# frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
nRet = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC,0,0,WaveDataBuff,128,100)
if(nRet != ControlDAC.ERR_SUCCESS):
        print("Config error")
        exit()
else:
        print("Configuration DAC to output square wave")
#Start DAC
nRet = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)
if(nRet != ControlDAC.ERR_SUCCESS):
    print("Start DAC error!")
    exit()
print("Press Enter to continue...")
input()
#Stop DAC  
ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)

#Configuration DAC to output square wave
for i in range(0, 128):
    WaveDataBuff[i] = c_uint16(i * 32)#    i * 0xfff/128
# frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
nRet = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC,0,0,WaveDataBuff,128,100)
if(nRet != ControlDAC.ERR_SUCCESS):
        print("Config error")
        exit()
else:
        print("Configuration DAC to output square wave")
#Start DAC
nRet = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)
if(nRet != ControlDAC.ERR_SUCCESS):
    print("Start DAC error!")
    exit()
print("Press Enter to continue...")
input()
#Stop DAC  
ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)

# Configuration DAC to output triangle wave
for i in range(0, 64):
    WaveDataBuff[i] = c_uint16(int((0xfff * i)/64))
    WaveDataBuff[i + 64] = c_uint16(int((64 - i) * 0xFFF / 64))
# frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
nRet = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC,0,0,WaveDataBuff,128,100)
if(nRet != ControlDAC.ERR_SUCCESS):
        print("Config error")
        exit()
else:
        print("Configuration DAC to output triangle wave")
#Start DAC
nRet = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)
if(nRet != ControlDAC.ERR_SUCCESS):
    print("Start DAC error!")
    exit()
print("Press Enter to continue...")
input()
#Stop DAC  
ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)

# Configuration DAC to output triangle wave
for i in range(0, 128):
    WaveDataBuff[i] = c_uint16(int( 0xFFF*(1+  sin(2*pi*i/128))/2.0))
# frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
nRet = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC,0,0,WaveDataBuff,128,100)
if(nRet != ControlDAC.ERR_SUCCESS):
        print("Config error")
        exit()
else:
        print("Configuration DAC to output triangle wave")
#Start DAC
nRet = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)
if(nRet != ControlDAC.ERR_SUCCESS):
    print("Start DAC error!")
    exit()
print("Press Enter to continue...")
input()
#Stop DAC  
ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0)

