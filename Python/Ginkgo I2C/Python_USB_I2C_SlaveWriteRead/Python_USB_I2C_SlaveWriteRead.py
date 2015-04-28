#coding=gbk
#####################################################################################################
# Program test environment
# Pyhone version:3.4.1
# Firmware version:2.8.28
# Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlI2C.py
# Dependent files(Windows):Ginkgo_Driver.dll,ControlI2C.py
# Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlI2C.py
#####################################################################################################
from ctypes import *
from time import sleep
# Import USB-I2C
import ControlI2C    

# Scan device
nRet = ControlI2C.VII_ScanDevice(1)
if(nRet <= 0):
    print("No device connect!")
    exit()
else:
    print("Connected device number is:"+repr(nRet))

# Open device
nRet = ControlI2C.VII_OpenDevice(ControlI2C.VII_USBI2C,0,0)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Open device error!")
    exit()
else:
    print("Open device success!")
# Initialize device
I2C_InitConfig = ControlI2C.VII_INIT_CONFIG()
# Master mode
I2C_InitConfig.MasterMode = ControlI2C.VII_MASTER
# Hardware control
I2C_InitConfig.ControlMode = ControlI2C.VII_HCTL_MODE
# 7bit address mode
I2C_InitConfig.AddrType = ControlI2C.VII_ADDR_7BIT
# Sub Address Width -> 1byte
I2C_InitConfig.SubAddrWidth = ControlI2C.VII_SUB_ADDR_NONE
# Clock Speed (in Hz)
I2C_InitConfig.ClockSpeed = 400000
nRet = ControlI2C.VII_InitI2C(ControlI2C.VII_USBI2C,0,0,byref(I2C_InitConfig))
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Initialize device error!")
    exit()
else:
    print("Initialize device success!")

write_buffer = (c_uint8 * 512)()
read_buffer = (c_uint8 * 512)()
    
# Slave write data
for i in range(0, 16):
    write_buffer[i] = i
nRet = ControlI2C.VII_SlaveWriteBytes(ControlI2C.VII_USBI2C, 0, 0, write_buffer, 16)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Slave write data error!")
    exit()
else:
    print("Write data:")
    for i in range(0, 16):
        print("%02X "%write_buffer[i])
        if(((i + 1) % 16) == 0):
            print('\n')
# Slave read data
while(1):
    sleep(0.05)
    nRet = ControlI2C.VII_SlaveReadBytes(ControlI2C.VII_USBI2C, 0, 0, read_buffer, byref(SlaveReadLen))
    if((nRet == ControlI2C.ERR_SUCCESS) and (SlaveReadLen > 0)):
        print("Write data:")
        for i in range(0, SlaveReadLen):
            print("%02X "%read_buffer[i])
            if(((i + 1) % 16) == 0):
                print('\n')
        print('\n')
    if(nRet == ControlI2C.ERROR.READ_NO_DATA):
        continue
    else:
        print("Slave read data error")
        exit()
















