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
# import USB-I2C module
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
# 7-bit address mode
I2C_InitConfig.AddrType = ControlI2C.VII_ADDR_7BIT
# Sub address width->1byte
I2C_InitConfig.SubAddrWidth = ControlI2C.VII_SUB_ADDR_1BYTE
# Clock speed (in Hz)
I2C_InitConfig.ClockSpeed = 400000
nRet = ControlI2C.VII_InitI2C(ControlI2C.VII_USBI2C,0,0,byref(I2C_InitConfig))
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Initialize device error!")
    exit()
else:
    print("Initialize device success!")
# Initialize MPU6050
write_buffer = (c_ubyte * 8)()
write_buffer[0] = 0x00
nRet = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, 0xD0, 0x6B, write_buffer, 1)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Write data error!")
    exit()
sleep(0.01)
write_buffer[0] = 0x00
nRet = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, 0xD0, 0x6C, write_buffer, 1)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Write data error!")
    exit()
sleep(0.01)
write_buffer[0] = 0x08
nRet = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, 0xD0, 0x1B, write_buffer, 1)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Write data error!")
    exit()
sleep(0.01)
write_buffer[0] = 0x01
nRet = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, 0xD0, 0x1A, write_buffer, 1)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Write data error!")
    exit()
sleep(0.01)
write_buffer[0] = 0x08
nRet = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, 0xD0, 0x1C, write_buffer, 1)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Write data error!")
    exit()
sleep(0.01)


while(1):
    read_buffer = (c_ubyte * 14)()
    nRet = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C,0,0,0xD0,0x3B,byref(read_buffer),14)
    if(nRet != ControlI2C.ERR_SUCCESS):
        print("Read data error! %d"%nRet)
        exit()
    else:
        # Processing data
        AccelX = ((read_buffer[0] << 8) | read_buffer[1])
        AccelY = ((read_buffer[2] << 8) | read_buffer[3])
        AccelZ = ((read_buffer[4] << 8) | read_buffer[5])

        GyroX = ((read_buffer[8] << 8) | read_buffer[9])
        GyroY = ((read_buffer[10] << 8) | read_buffer[11])
        GyroZ = ((read_buffer[12] << 8) | read_buffer[13])
        # Clear display
        import os
        os.system('cls')
        # Display
        print("AccelX = %d" % AccelX)
        print("AccelY = %d" % AccelY)
        print("AccelZ = %d" % AccelZ)
        print("GyroX = %d" % GyroX)
        print("GyroY = %d" % GyroY)
        print("GyroZ = %d" % GyroZ)
        sleep(0.1)




