"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.8.21
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlADC.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlADC.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlADC.py
"""
from ctypes import *
from time import sleep
# import USB-SPI module
import ControlADC

# Scan device(must call one more time)
nRet = c_int(0)
nRet = ControlADC.VAI_ScanDevice(1)
if(nRet <= 0):
    print("No device connect!")
    exit()
else:
    print("Connected device number is:"+repr(nRet))

# Open device(must call)
nRet = ControlADC.VAI_OpenDevice(ControlADC.VAI_USBADC,0,0)
if(nRet != ControlADC.ERR_SUCCESS):
    print("Open device error!")
    exit()
else:
    print("Open device success!")
# Initialize ADC_CH0
nRet = ControlADC.VAI_InitADC(ControlADC.VAI_USBADC, 0, ControlADC.VAI_ADC_CH0, 0)
if(nRet != ControlADC.ERR_SUCCESS):
    print("Initialize ADC error!")
    exit()
else:
    print("Initialize ADC success!")
# Get ADC_CH0 voltage
adc_datas = (c_ushort * 4096)()
nRet = ControlADC.VAI_ReadDatas(ControlADC.VAI_USBADC, 0, 1, adc_datas)
if(nRet != ControlADC.ERR_SUCCESS):
    print("Read ADC error!")
    exit()
else:
    print("ADC_CH0 = %.3f" %((adc_datas[0]*3.3)/4095))
# Initialize ADC_CH0 and ADC_CH1
nRet = ControlADC.VAI_InitADC(ControlADC.VAI_USBADC, 0, ControlADC.VAI_ADC_CH0|ControlADC.VAI_ADC_CH1, 0)
if(nRet != ControlADC.ERR_SUCCESS):
    print("Initialize ADC error!")
    exit()
else:
    print("Initialize ADC success!")
# Get ADC_CH0 and ADC_CH1 voltage
nRet = ControlADC.VAI_ReadDatas(ControlADC.VAI_USBADC, 0, 1, adc_datas)
if(nRet != ControlADC.ERR_SUCCESS):
    print("Read ADC error!")
    exit()
else:
    print("ADC_CH0 = %.3f" %((adc_datas[0]*3.3)/4095))
    print("ADC_CH1 = %.3f" %((adc_datas[1]*3.3)/4095))
# Initialize ADC_CH0 and ADC_CH1,each channel sampling time interval of 1000 microseconds
nRet = ControlADC.VAI_InitADC(ControlADC.VAI_USBADC, 0, ControlADC.VAI_ADC_CH0|ControlADC.VAI_ADC_CH1, 1000)
if(nRet != ControlADC.ERR_SUCCESS):
    print("Initialize ADC error!")
    exit()
else:
    print("Initialize ADC success!")
# Get ADC_CH0 and ADC_CH1 voltage,each channel gain 10 values
nRet = ControlADC.VAI_ReadDatas(ControlADC.VAI_USBADC, 0, 128, adc_datas)
if(nRet != ControlADC.ERR_SUCCESS):
    print("Read ADC error!")
    exit()
else:
    for i in range(0,128):
        print("ADC_CH0[%d] = %.3f" %(i,((adc_datas[i*2]*3.3)/4095)))
    for i in range(0,128):
        print("ADC_CH1[%d] = %.3f" %(i,((adc_datas[i*2+1]*3.3)/4095)))    
# Close device
nRet = ControlADC.VAI_CloseDevice(ControlADC.VAI_USBADC,0)
if(nRet != ControlADC.ERR_SUCCESS):
    print("Close device error! %d"%nRet)
    exit()
else:
    print("Close device success!")        