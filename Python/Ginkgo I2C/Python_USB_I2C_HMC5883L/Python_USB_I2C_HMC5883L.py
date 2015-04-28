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
# import i2c module
import ControlI2C

import os

# Scan connected device
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
# Initialize device(hardware mode)
I2C_InitConfig = ControlI2C.VII_INIT_CONFIG()
# Master Mode
I2C_InitConfig.MasterMode = ControlI2C.VII_MASTER
# Control Mode
I2C_InitConfig.ControlMode = ControlI2C.VII_HCTL_MODE
# 7bit address
I2C_InitConfig.AddrType = ControlI2C.VII_ADDR_7BIT
# Sub Address Width -> 1byte
I2C_InitConfig.SubAddrWidth = ControlI2C.VII_SUB_ADDR_1BYTE
# Clock Speed(in Hz)
I2C_InitConfig.ClockSpeed = 400000
nRet = ControlI2C.VII_InitI2C(ControlI2C.VII_USBI2C,0,0,byref(I2C_InitConfig))
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Initialize device error!")
    exit()
else:
    print("Initialize device success!")

# Put the HMC5883 IC into the correct operating mode
Address = 0x1e
Address = Address << 1
write_buffer = (c_ubyte * 8)()
read_buffer = (c_ubyte * 8)()
write_buffer[0] = 0x00
nRet = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C,0,0,Address,0x02,byref(write_buffer),1)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Write data error! %d"%nRet)
    exit()

while(1):
    # Read status
    nRet = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, Address, 0x09, read_buffer, 1)
    if(nRet != ControlI2C.ERR_SUCCESS):
        print("Read data error! %d"%nRet)
        exit()
    # Data ready
    if((read_buffer[0] & 0x01) > 0):
        #Tell the HMC5883 where to begin reading data
        nRet = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, Address, 0x03, read_buffer, 6)
        if(nRet != ControlI2C.ERR_SUCCESS):
            print("Read data error! %d"%nRet)
            exit()
    else:
        x = (read_buffer[0] << 8)|read_buffer[1]
        y = (read_buffer[2] << 8)|read_buffer[3]
        z = (read_buffer[4] << 8)|read_buffer[5]
        print("||",x)
        print("||",y)
        print("||",z)
        print("--------------------------------------")