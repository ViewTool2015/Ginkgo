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
import ControlGPIO

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


# Write 8 byte data to 0x08
WriteData = (c_byte * 8)()
for i in range(0, len(WriteData)):
    WriteData[i] = i
nRet = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C,0,0,0xA0,0x08,byref(WriteData),8)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Write data error! %d"%nRet)
    exit()
else:
    print("Write data success!")
# Delay to ensure write complete
sleep(0.1)
# Control GPIO
# Set GPIO_7 and GPIO_8 to output mode
nRet = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN7 | ControlGPIO.VGI_GPIO_PIN8)
if(nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin output error!")
    exit()
else:
    print("Set pin output success!")
# GPIO_7 and GPIO_8 -> high level
nRet = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN7 | ControlGPIO.VGI_GPIO_PIN8);
if (nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin high error!")
    exit()
else:
    print("Set pin high success!")
# GPIO_7 and GPIO_8 -> low level
nRet = ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN7 | ControlGPIO.VGI_GPIO_PIN8);
if (nRet != ControlGPIO.ERR_SUCCESS):
    print("Set pin low error!")
    exit()
else:
    print("Set pin low success!")

# Read data from 0x08
ReadData = (c_byte * 8)()
for i in range(0, len(ReadData)):
    ReadData[i] = 0
nRet = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C,0,0,0xA0,0x08,byref(ReadData),8)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Read data error! %d"%nRet)
    exit()
else:
    print("Read data success!")
    print("Read Data(Hex):",end='')
    for i in range(0, len(ReadData)):
        print("%02X "%ReadData[i],end='')
    print("")
# Close device
nRet = ControlI2C.VII_CloseDevice(ControlI2C.VII_USBI2C,0)
if(nRet != ControlI2C.ERR_SUCCESS):
    print("Close device error! %d"%nRet)
    exit()
else:
    print("Close device success!")




