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
# Initialize device(Master Mode, Hardware SPI, Half-Duplex)
SPI_Init = ControlSPI.VSI_INIT_CONFIG()
SPI_Init.ClockSpeed = 36000000;
SPI_Init.ControlMode = 1;
SPI_Init.CPHA = 0;
SPI_Init.CPOL = 0;
SPI_Init.LSBFirst = 0;
SPI_Init.MasterMode = 1;
SPI_Init.SelPolarity = 0;
SPI_Init.TranBits = 8;
nRet = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI,0,byref(SPI_Init))
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Initialization device error!")
	exit()
else:
	print("Initialization device success!")

write_buffer = (c_uint8 * 1024)()
read_buffer = (c_uint8 * 1024)()
block_num = 5
block_size = 2
IntervalTime = 100

# Block mode write data, data will be send out block_num time(s), send out block_size bytes of data every time
# Set CS to low before send data and set CS to high after send data complete when every time send data
# Each time send data, interval is IntervalTime us, that is CS set to high and hold IntervalTime us

for i in range(0, block_num):
    for j in range(0, block_size):
        write_buffer[i*block_size + j] = i * j
nRet = ControlSPI.VSI_BlockWriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 2,5,100)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Block write data error!")
	exit()
else:
    print("Block write")
    


# Block mode read data: same as write data
nRet = ControlSPI.VSI_BlockReadBytes(ControlSPI.VSI_USBSPI, 0, 0, read_buffer, block_size,block_num,IntervalTime)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Block read data error!")
	exit()
else:
    print("Read data (hex):")
    for i in range(0, block_num):
        for j in range(0, block_size):
            print("%02X "%read_buffer[i*block_size + j])

            
# Block mode: write and read data
write_block_size = 1
read_block_size = 2
for i in range(0, block_num):
    for j in range(0, write_block_size):
        write_buffer[i*write_block_size+j] = c_uint8(i*j)
nRet = ControlSPI.VSI_BlockWriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, write_block_size,read_buffer,read_block_size,block_num,IntervalTime)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Block write read data error!")
	exit()
else:
    print("Read data(Hex):\n")
    for i in range(0, block_num):
        for j in range(0, write_block_size):
            print("%02X "%read_buffer[i*read_block_size + j])
  








