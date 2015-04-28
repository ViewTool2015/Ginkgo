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
  AT45DB041D      Ginkgo USB-SPI Adapter
  1.SI      <-->  SPI_MOSI(Pin17)
  2.SCK	    <-->  SPI_SCK(Pin13)
  3.RESET   <-->  VCC(Pin2)
  4.CS      <-->  SPI_SEL0(Pin11)
  5.WP	    <-->  VCC(Pin2)
  6.VCC	    <-->  VCC(Pin2)
  7.GND	    <-->  GND(Pin19/Pin20)
  8.SO	    <-->  SPI_MISO(Pin15)
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
SPI_Init.ClockSpeed = 1125000;
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
# Page Erase
write_buffer[0] = 0x81
write_buffer[1] = 0x00
write_buffer[2] = 0x00
write_buffer[3] = 0x00
nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,4)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Write data error :%d"%nRet)
	exit()
else:
	print("Page erase start success!")
# Check the operation to complete
while True:
	write_buffer[0] = 0xD7
	nRet = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,1,read_buffer,1)
	if(((read_buffer[0]&0x80)==0x80)or(nRet != ControlSPI.ERR_SUCCESS)):
		if(nRet != ControlSPI.ERR_SUCCESS):
			print("Page Erase error :%d"%nRet)
			exit()
		else:
			print("Page erase success!")		
		break
# Write data to buffer
write_buffer[0] = 0x87
write_buffer[1] = 0x00
write_buffer[2] = 0x00
write_buffer[3] = 0x00
for i in range(0,528):
	write_buffer[4+i] = i&0xFF
nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,4+528)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Write data error :%d"%nRet)
	exit()
else:
	print("Write data to buffer success!")
# Buffer to Main Memory Page Program without Built-in Erase
write_buffer[0] = 0x89
write_buffer[1] = 0x00
write_buffer[2] = 0x00
write_buffer[3] = 0x00
nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,4)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Write data to main memory error :%d"%nRet)
	exit()
else:
	print("Write data to main memory success!")
	
# Check the operation to complete
while True:
	write_buffer[0] = 0xD7
	nRet = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,1,read_buffer,1)
	if(((read_buffer[0]&0x80)==0x80)or(nRet != ControlSPI.ERR_SUCCESS)):
		if(nRet != ControlSPI.ERR_SUCCESS):
			print("Page Erase error :%d"%nRet)
			exit()
		else:
			print("Page erase success!")		
		break
# Main Memory Page Read
write_buffer[0] = 0xD2
write_buffer[1] = 0x00
write_buffer[2] = 0x00
write_buffer[3] = 0x00
nRet = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,8,read_buffer,528)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Read data to buffer error :%d"%nRet)
	exit()
else:
	print("Read data(Hex):")
	for i in range(0,528):
		print("%02X "%read_buffer[i],end='')
		if(((i+1)%8)==0):
			print()
# Close device
nRet = ControlSPI.VSI_CloseDevice(ControlSPI.VSI_USBSPI, 0)
if(nRet != ControlSPI.ERR_SUCCESS):
	print("Close device error!")
	exit()
else:
	print("Close device success!")