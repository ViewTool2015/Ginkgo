# -*- coding: utf-8 -*-
"""
Program test environment
Pyhone version:3.4.1
Firmware version:2.8.28
Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,libusb-1.0.0.dylib,ControlI2C.py
Dependent files(Windows):Ginkgo_Driver.dll,ControlI2C.py
Dependent files(Linux):libGinkgo_Driver.so,libusb-1.0.so,ControlI2C.py
"""

import sys
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from Ui_MainWindow import *
from ctypes import *
from ControlI2C import *
class MyApp(QMainWindow):
    def __init__(self,parent=None):
        QWidget.__init__(self,parent)
        self.loginGui()
        self.connect(self.ui.writePushButton,SIGNAL('clicked()'),self.onWritePushButtonClicked)
        self.connect(self.ui.readPushButton,SIGNAL('clicked()'),self.onReadPushButtonClicked)
    def loginGui(self):
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.show()
    def ConfigDevice(self):
        deviceIndex = self.ui.deviceIndexCmboBox.currentIndex()
        channelIndex = self.ui.channelIndexComboBox.currentIndex()
        # Scan device
        nRet = VII_ScanDevice(1)
        if(nRet <= 0):
            QMessageBox.warning(self,'Warning','No device connect!')
            return False
        # Open device
        nRet = VII_OpenDevice(VII_USBI2C,deviceIndex,channelIndex)
        if(nRet != ERR_SUCCESS):
            QMessageBox.warning(self,'Warning','Open device error!')
            return False
        # Initialize device
        I2C_InitConfig = VII_INIT_CONFIG()
        I2C_InitConfig.MasterMode = VII_MASTER
        I2C_InitConfig.ControlMode = VII_HCTL_MODE
        I2C_InitConfig.AddrType = VII_ADDR_7BIT
        I2C_InitConfig.SubAddrWidth = VII_SUB_ADDR_1BYTE
        I2C_InitConfig.ClockSpeed = 400000 >> self.ui.clockSpeedComboBox.currentIndex()
        nRet = VII_InitI2C(VII_USBI2C,deviceIndex,channelIndex,byref(I2C_InitConfig))
        if(nRet != ERR_SUCCESS):
            QMessageBox.warning(self,'Warning','Initialize device error!')
            return False
        else:
            return True
    def onWritePushButtonClicked(self):
        deviceIndex = self.ui.deviceIndexCmboBox.currentIndex()
        channelIndex = self.ui.channelIndexComboBox.currentIndex()
        slaveAddr = int(self.ui.slaveAddrLineEdit.text(),16)
        subAddr = int(self.ui.subAddrLineEdit.text(),16)
        # Config device
        if self.ConfigDevice() != True:
            return
        # Get&Write data
        WriteDataList = self.ui.writeDataLineEdit.text().split(' ')
        WriteData = (c_byte * len(WriteDataList))()
        for i in range(len(WriteDataList)):
            WriteData[i] = c_byte(int(WriteDataList[i],16))
        nRet = VII_WriteBytes(VII_USBI2C,deviceIndex,channelIndex,slaveAddr,subAddr ,byref(WriteData),len(WriteDataList))
        if(nRet != ERR_SUCCESS):
            QMessageBox.warning(self,'Warning','Write data error!')
            return
        # Close device
        VII_CloseDevice(VII_USBI2C,deviceIndex)
    def onReadPushButtonClicked(self):
        deviceIndex = self.ui.deviceIndexCmboBox.currentIndex()
        channelIndex = self.ui.channelIndexComboBox.currentIndex()
        slaveAddr = int(self.ui.slaveAddrLineEdit.text(),16)
        subAddr = int(self.ui.subAddrLineEdit.text(),16)
        # Config device
        if self.ConfigDevice() != True:
            return
        ReadDataNum = int(self.ui.readDataCountLineEdit.text(),16)
        ReadData = (c_byte * ReadDataNum)()
        nRet = VII_ReadBytes(VII_USBI2C,deviceIndex,channelIndex,slaveAddr,subAddr,byref(ReadData),ReadDataNum)
        if(nRet != ERR_SUCCESS):
            QMessageBox.warning(self,'Warning','Read data error!')
            return
        else:
            readDataStr = ''
            for i in range(ReadDataNum):
                readDataStr += format('%02X '%(ReadData[i]&0xFF))
            self.ui.readDataTextBrowser.append(readDataStr)

def main():
    app = QApplication(sys.argv)
    myqq = MyApp()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
