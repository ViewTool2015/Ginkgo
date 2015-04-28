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
from ctypes import windll
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

#Start Write Data Speed Test
print('Start Write Data Speed Test...')
litmp = c_long(0)
windll.Kernel32.QueryPerformanceFrequency(byref(litmp)) # Get the performance counter frequency, in n/s
dfFreq = litmp.value
windll.Kernel32.QueryPerformanceCounter(byref(litmp))   # Get the current value of the performance counter
StartTime = litmp.value                                       # Start time
for i in range(0, 400):
    nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,write_buffer,10000)
    if(nRet != ControlSPI.ERR_SUCCESS):
        print("Write data error!!")
        exit()
windll.Kernel32.QueryPerformanceCounter(byref(litmp))   # Get the current value of the performance counter
EndTime = litmp.value                # Stop time
# Print the write data speed information
print("Write Data Numbers: %d Bytes"%(i*10000))
print("Write Data Elapsed Time: %f"%((EndTime-StartTime)/dfFreq))
print("Write Data Speed: %.2f KByte/s"%(((i*10000)/1024)/((EndTime-StartTime)/dfFreq)))

# Start Read Data Speed Test
windll.Kernel32.QueryPerformanceCounter(byref(litmp))   # Get the current value of the performance counter
StartTime = litmp.value                                       # Start time
for i in range(0, 400):
    nRet = ControlSPI.VSI_ReadBytes(ControlSPI.VSI_USBSPI,0,0,read_buffer,10000)
    if(nRet != ControlSPI.ERR_SUCCESS):
        print("Read data error!")
        exit()
windll.Kernel32.QueryPerformanceCounter(byref(litmp))   # Get the current value of the performance counter
EndTime = litmp.value                # Stop time
# Print the write data speed information
print("Write Data Numbers: %d Bytes"%(i*10000))
print("Write Data Elapsed Time: %f"%((EndTime-StartTime)/dfFreq))
print("Write Data Speed: %.2f KByte/s"%(((i*10000)/1024)/((EndTime-StartTime)/dfFreq)))
exit()