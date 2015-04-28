"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.2.12
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlSPI.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlSPI.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlSPI.py
More Infomation:www.viewtool.com
"""
"""
  Hardware Connection
  W25Q32         Ginkgo USB-SPI Adapter
  1.CS      <-->  SPI_SEL0(Pin11)
  2.DO	    <-->  SPI_MISO(Pin15)
  3.WP      <-->  VCC(Pin2)
  4.GND     <-->  GND(Pin19/Pin20)
  5.DI	    <-->  SPI_MOSI(Pin17)
  6.CLK	    <-->  SPI_SCK(Pin13)
  7.HOLD    <-->  VCC(Pin2) 
  8.VCC	    <-->  VCC(Pin2)
"""

from ctypes import *
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
# Get JEDEC ID
write_buffer = (c_ubyte * 1024)()
read_buffer = (c_ubyte * 1024)()
write_buffer[0] = 0x9F;
nRet = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,1,read_buffer,3)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Read flash ID error :%d"%nRet);
	exit()
else:
	ID = ((read_buffer[0]<<16)|(read_buffer[1]<<8)|read_buffer[2])&0xFFFF
	print("Flash ID = 0x%06X"%ID)
# Write Enable
write_buffer[0] = 0x06
nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,1)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Flash write enable error :%d"%nRet)
	exit()
else:
	print("Write enable success!")
# Sector Erase (4KB)
write_buffer[0] = 0x20
write_buffer[1] = 0x00
write_buffer[2] = 0x00
write_buffer[3] = 0x00
nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,4)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Write data error :%d"%nRet)
	exit()
else:
	print("Sector erase start success!")
# Check the operation to complete
while True:
	write_buffer[0] = 0x05
	nRet = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,1,read_buffer,1)
	if(((read_buffer[0]&0x01)==0)or(nRet != ControlSPI.ERR_SUCCESS)):
		if(nRet != ControlSPI.ERR_SUCCESS):
			print("Sector Erase error :%d"%nRet)
			exit()
		else:
			print("Sector erase success!")		
		break
# Write Enable
write_buffer[0] = 0x06
nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,1)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Flash write enable error :%d"%nRet)
	exit()
else:
	print("Write enable success!")
# Page Program
write_buffer[0] = 0x02
write_buffer[1] = 0x00
write_buffer[2] = 0x00
write_buffer[3] = 0x00
for i in range(0,256):
	write_buffer[4+i] = i
nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,256+4)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Write data error :%d"%nRet)
	exit()
else:
	print("Write data success!")
# Check the operation to complete
while True:
	write_buffer[0] = 0x05
	nRet = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,1,read_buffer,1)
	if(((read_buffer[0]&0x01)==0)or(nRet != ControlSPI.ERR_SUCCESS)):
		if(nRet != ControlSPI.ERR_SUCCESS):
			print("Sector Erase error :%d"%nRet)
			exit()
		else:
			print("Sector erase success!")		
		break
# Read Data
write_buffer[0] = 0x03
write_buffer[1] = 0x00
write_buffer[2] = 0x00
write_buffer[3] = 0x00
nRet = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,4,read_buffer,256)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Read data error!")
	exit()
else:
	print("Read datas:")
	for i in range(0,256):
		print("%02X "%(read_buffer[i]),end='')
		if(((i+1)%8)==0):
			print()
# Close device
nRet = ControlSPI.VSI_CloseDevice(ControlSPI.VSI_USBSPI, 0)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Close device error!")
	exit()
else:
	print("Close device success!")