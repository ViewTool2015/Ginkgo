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
from time import sleep
# import USB-SPI module
import ControlSPI

# Scan device(must call one more time)
nRet = c_int(0)
nRet = ControlSPI.VSI_ScanDevice(1)
if(nRet <= 0):
	print("No device connect!")
	exit()
else:
	print("Connected device number is:"+repr(nRet))

# Open device(must call)
nRet = ControlSPI.VSI_OpenDevice(ControlSPI.VSI_USBSPI,0,0)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Open device error!")
	exit()
else:
	print("Open device success!")
# Initialize device(Host&hardware control, half duplex mode)
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
write_buffer = (c_ubyte * 1024)()
read_buffer = (c_ubyte * 1024)()
# Write 8 bytes data
for i in range(0,8):
	write_buffer[i] = i
nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,8)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Write data error :%d"%nRet)
	exit()
else:
	print("Write data success!")
# Read 8 bytes data
nRet = ControlSPI.VSI_ReadBytes(ControlSPI.VSI_USBSPI,0,0,read_buffer,8)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Read data error :%d"%nRet)
	exit()
else:
	print("Read data(Hex):")
	for i in range(0,8):
		print("%02X "%(read_buffer[i]),end='')
	print()
# Write&Read data, during the whole operation selected signal has been effective
for i in range(0,5):
	write_buffer[i] = i
nRet = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 5, read_buffer, 8)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Write&Read data error :%d"%nRet)
	exit()
else:
	print("Read data(Hex):")
	for i in range(0,8):
		print("%02X "%(read_buffer[i]),end='')
	print()
# Initialize device(Host&software control, half duplex mode)
SPI_Init.ControlMode = 2
SPI_Init.MasterMode = 1
SPI_Init.ClockSpeed = 100000
SPI_Init.CPHA = 0
SPI_Init.CPOL = 0
SPI_Init.LSBFirst = 0
SPI_Init.TranBits = 8
SPI_Init.SelPolarity = 0
nRet = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI,0,byref(SPI_Init))
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Initialization device error!")
	exit()
else:
	print("Initialization device success!")
# Binary mode to send data, send the bit number up to 10240 - bit
nRet = ControlSPI.VSI_WriteBits(ControlSPI.VSI_USBSPI,0,0,b"10110100100101")
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Write bits error :%d"%nRet)
	exit()
else:
	print("Write bits success!")
# Binary mode to read data.The maximum number of read up to 10240 - bit
read_buffer_bin = create_string_buffer(b"\0"*10240)
nRet = ControlSPI.VSI_ReadBits(ControlSPI.VSI_USBSPI,0,0,read_buffer_bin,19)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Read bits error :%d"%nRet)
	exit()
else:
	print("Read bits:",read_buffer_bin.value)
# Initialize device(Slave mode)
SPI_Init.ControlMode = 0
SPI_Init.MasterMode = 0
SPI_Init.CPHA = 0
SPI_Init.CPOL = 0
SPI_Init.LSBFirst = 0
SPI_Init.TranBits = 8
SPI_Init.SelPolarity = 0
nRet = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI,0,byref(SPI_Init))
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Initialization device error!")
	exit()
else:
	print("Initialization device success!")
# Slave write data
for i in range(0,256):
	write_buffer[i] = i
nRet = ControlSPI.VSI_SlaveWriteBytes(ControlSPI.VSI_USBSPI, 0, write_buffer, 256)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Slave write data error :%d"%nRet)
	exit()
else:
	print("Slave write data success!")
# Slave read data
WriteBytesNum = c_int()
nRet = ControlSPI.VSI_SlaveReadBytes(ControlSPI.VSI_USBSPI, 0, read_buffer, byref(WriteBytesNum),100)# wait 100 millisecond
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Slave Read data error :%d"%nRet)
	exit()
else:
	if(WriteBytesNum.value > 0):
		print("Slave read data(Hex):")
		for i in range(0,WriteBytesNum):
			print("%02X "%read_buffer[i])
			if(((i+1)%8)==0):
				print()
	else:
		print("No data!")
# Close device
nRet = ControlSPI.VSI_CloseDevice(ControlSPI.VSI_USBSPI, 0)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Close device error!")
	exit()
else:
	print("Close device success!")