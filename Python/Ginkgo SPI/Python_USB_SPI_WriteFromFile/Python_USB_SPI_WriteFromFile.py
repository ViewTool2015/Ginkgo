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

    # Open file
fp = open("data.txt")

#FileList = fp.read()
#strLine = FileList.split(' ')

#print(strLine)
#for xx in strLine:
#    print(xx)

done = 0
x = 0
WriteDataTemp = (c_uint8 * 10240)()

while not done:
    data_num = 0
    arr = []
    ALine = fp.readline()
    if(ALine != ''):    # not EOF
        x += 1
        print(" %d Line"%x)
        print(ALine)
    else:
        done = 1        #EOF
    strLine = ALine.split(' ')
    for xx in strLine:
        if (xx != ''):
            yy = int(xx)
            arr.append(yy)
            WriteDataTemp[data_num] = c_uint8(yy)
            data_num += 1
            print(data_num)
    if(data_num > 0):
        print(arr)
        nRet = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI,0,0,WriteDataTemp,data_num)
        if(nRet != ControlSPI.ERR_SUCCESS):
            print(" Write enable error")
            exit()
fp.close() 

























