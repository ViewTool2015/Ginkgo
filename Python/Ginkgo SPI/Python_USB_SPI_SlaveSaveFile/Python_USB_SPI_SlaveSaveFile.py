"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.2.12
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlSPI.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlSPI.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlSPI.py
More Infomation:www.viewtool.com
"""
from ctypes import *
import signal
import sys
from time import sleep
# Import module
import ControlSPI

# Scan device
nRet = ControlSPI.VSI_ScanDevice(1)
if(nRet <= 0):
	print("No device connect!")
	exit()
else:
	print("Connected device number is:"+repr(nRet))

# Open device
nRet = ControlSPI.VSI_OpenDevice(ControlSPI.VSI_USBSPI,0,0)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Open device error!")
	exit()
else:
	print("Open device success!")
# Initialize device
SPI_Init = ControlSPI.VSI_INIT_CONFIG()
SPI_Init.ClockSpeed = 18000000;
SPI_Init.ControlMode = 0;
SPI_Init.CPHA = 0;
SPI_Init.CPOL = 0;
SPI_Init.LSBFirst = 0;
SPI_Init.MasterMode = 0;
SPI_Init.SelPolarity = 0;
SPI_Init.TranBits = 8;
nRet = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI,0,byref(SPI_Init))
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Initialization device error!")
	exit()
else:
	print("Initialization device success!")

def signal_handler(signal, frame):
	sys.exit()
signal.signal(signal.SIGINT, signal_handler)

fp = open('data.bin','ab')
readData = (c_byte * 10240)()
readBytesNum = c_int(0)
print('Start read data,Pressed Ctrl+C abort it')
while True:
    nRet = ControlSPI.VSI_SlaveReadBytes(ControlSPI.VSI_USBSPI, 0, readData, byref(readBytesNum), 5)
    if readBytesNum.value > 0:
        print('Read data count is %d'%readBytesNum.value)
        realReadData = (c_byte * readBytesNum.value)()
        for i in range(0,readBytesNum.value):
            realReadData[i] = readData[i]
        fp.write(realReadData)
        fp.flush()
    else:
        pass
    sleep(0.5)

