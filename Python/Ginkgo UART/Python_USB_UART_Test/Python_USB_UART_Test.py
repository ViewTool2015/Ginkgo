"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.8.28
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlUART.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlUART.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlUART.py
More Infomation:www.viewtool.com
"""
from ctypes import *
from time import sleep
from ctypes import *
import signal
import sys
# import USB-UART module
import ControlUART
    
# Scan device(must call one more time)
nRet = c_int(0)
nRet = ControlUART.UART_ScanDevice(1)
if(nRet <= 0):
	print("No device connect!")
	exit()
else:
	print("Connected device number is:"+repr(nRet))

# Open device(must call)
nRet = ControlUART.UART_OpenDevice(ControlUART.UART_USBUART,0,0)
if(nRet != ControlUART.ERR_SUCCESS):
	print("Open device error!")
	exit()
else:
	print("Open device success!")
# Initialize device
UART_Init = ControlUART.UART_INIT_CONFIG()
UART_Init.StopBits = 0;
UART_Init.WordLength = 8;
UART_Init.Parity = 0;
UART_Init.RS485Mode = 232;
UART_Init.BaudRate = 115200;
nRet = ControlUART.UART_InitDevice(ControlUART.UART_USBUART,0,0,byref(UART_Init))
if(nRet != ControlUART.ERR_SUCCESS):
	print("Initialization device error!")
	exit()
else:
	print("Initialization device success!")
write_buffer = (c_ubyte * 1024)()

# Write 8 bytes data
for i in range(0,8):
	write_buffer[i] = i
nRet = ControlUART.UART_WriteBytes(ControlUART.UART_USBUART,0,0,write_buffer,8)
if(nRet != ControlUART.ERR_SUCCESS):
	print("Write data error :%d"%nRet)
	exit()
else:
	print("Write data success!")

def signal_handler(signal, frame):
	sys.exit()
signal.signal(signal.SIGINT, signal_handler)

# Read data
read_buffer = (c_ubyte * 10240)()
readBytesNum = c_int(0)
print('Start read data,Pressed Ctrl+C abort it')
while True:
    nRet = ControlUART.UART_ReadBytes(ControlUART.UART_USBUART, 0, 0, read_buffer, byref(readBytesNum))
    if readBytesNum.value > 0:
        print('Read data count is %d'%readBytesNum.value)
        for i in range(0,readBytesNum.value):
            print("%02X "%read_buffer[i])
            if(((i+1)%16)==0):
                print()
    else:
        pass
