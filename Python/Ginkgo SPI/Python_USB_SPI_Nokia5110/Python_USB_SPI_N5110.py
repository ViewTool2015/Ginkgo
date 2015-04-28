"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.2.12
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlSPI.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlSPI.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlSPI.py
More Infomation:www.viewtool.com
"""


#Hardware connection
# Nokia5110 Ginkgo USB-SPI
# 1.RST     GPIO_10(P7)
# 2.CE      SPI_SEL0(P11)
# 3.DC      GPIO_11(P9)
# 4.DIN     SPI_MOSI(P17)
# 5.CLK     SPI_SCK(P13)
# 6.VCC     5V
# 7.BL      GPIO_9(P5)
# 8.GND     GND

from ctypes import *
from time import sleep
# Import module
import LCD5110



LCD5110.LCD_Init()
LCD5110.LCD_Clear()

LCD5110.LCD_WriteString(0, 1, "Hello viewtool")
LCD5110.LCD_WriteString(0, 3, "Ginkgo USB-SPI")