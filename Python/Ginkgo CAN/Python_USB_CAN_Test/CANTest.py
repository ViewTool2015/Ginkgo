################################################################################################
# Program test environment
# Pyhone version:3.4.1
# Firmware version:2.8.28
# Dependent files(MacOSX):libGinkgo_Driver.dylib,libusb-0.1.4.dylib,ControlCAN.py
# Dependent files(Windows):Ginkgo_Driver.dll,ControlCAN.py
# Dependent files(Linux):Ginkgo_Driver.so,ControlCAN.py
################################################################################################
from ctypes import *
from time import sleep
# import USB-CAN module
import ControlCAN
# function define
CAN_GET_BOARD_INFO = 1
CAN_MODE_LOOP_BACK = 1
CAN_SEND_DATA = 1
CAN_READ_DATA = 0
CAN_CALLBACK_READ_DATA = 1
CAN_INIT_EX = 1
CAN_GET_STATUS = 1
# parameters define
DeviceIndex = 0
CANIndex = 0
DevType = ControlCAN.VCI_USBCAN2
# define callback function
def GetDataCallback(cb_DeviceIndex,cb_CANIndex,cb_Len):
    DataNum = ControlCAN.VCI_GetReceiveNum(DevType,cb_DeviceIndex,cb_CANIndex)
    CAN_ReceiveData = (ControlCAN.VCI_CAN_OBJ*DataNum)()
    if(DataNum > 0):
        ReadDataNum = ControlCAN.VCI_Receive(DevType,cb_DeviceIndex,cb_CANIndex, byref(CAN_ReceiveData), DataNum,0)
        for i in range(0,DataNum):
            print("")
            print("--CAN_ReceiveData.RemoteFlag = %d"%CAN_ReceiveData[i].RemoteFlag)
            print("--CAN_ReceiveData.ExternFlag = %d"%CAN_ReceiveData[i].ExternFlag)
            print("--CAN_ReceiveData.ID = 0x%X"%CAN_ReceiveData[i].ID)
            print("--CAN_ReceiveData.DataLen = %d"%CAN_ReceiveData[i].DataLen)
            print("--CAN_ReceiveData.Data:",end='')
            for j in range(0,CAN_ReceiveData[i].DataLen):
                print("%02X "%CAN_ReceiveData[i].Data[j],end='')
            print("")
            print("--CAN_ReceiveData.TimeStamp = %d"%CAN_ReceiveData[i].TimeStamp)

# Scan device
nRet = ControlCAN.VCI_ScanDevice(1)
if(nRet == 0):
    print("No device connected!")
    exit()
else:
    print("Have %d device connected!"%nRet)
# Get board info
if(CAN_GET_BOARD_INFO == 1):
    CAN_BoardInfo = ControlCAN.VCI_BOARD_INFO_EX()
    nRet = ControlCAN.VCI_ReadBoardInfoEx(DeviceIndex,byref(CAN_BoardInfo))
    if(nRet == ControlCAN.STATUS_ERR):
        print("Get board info failed!")
        exit()
    else:
        print("--CAN_BoardInfo.ProductName = %s"%bytes(CAN_BoardInfo.ProductName).decode('ascii'))
        print("--CAN_BoardInfo.FirmwareVersion = V%d.%d.%d"%(CAN_BoardInfo.FirmwareVersion[1],CAN_BoardInfo.FirmwareVersion[2],CAN_BoardInfo.FirmwareVersion[3]))
        print("--CAN_BoardInfo.HardwareVersion = V%d.%d.%d"%(CAN_BoardInfo.HardwareVersion[1],CAN_BoardInfo.HardwareVersion[2],CAN_BoardInfo.HardwareVersion[3]))
        print("--CAN_BoardInfo.SerialNumber = ",end='')
        for i in range(0, len(CAN_BoardInfo.SerialNumber)):
            print("%02X"%CAN_BoardInfo.SerialNumber[i],end='')
        print("")
else:
    # Open device
    nRet = ControlCAN.VCI_OpenDevice(DevType,DeviceIndex,0)
    if(nRet == ControlCAN.STATUS_ERR):
        print("Open device failed!")
        exit()
    else:
        print("Open device success!")
if(CAN_INIT_EX == 1):
    CAN_InitEx = ControlCAN.VCI_INIT_CONFIG_EX()
    CAN_InitEx.CAN_ABOM = 0
    if(CAN_MODE_LOOP_BACK == 1):
        CAN_InitEx.CAN_Mode = 1
    else:
        CAN_InitEx.CAN_Mode = 0
    # Baud Rate
    CAN_InitEx.CAN_BRP = 6
    CAN_InitEx.CAN_BS1 = 3
    CAN_InitEx.CAN_BS2 = 2
    CAN_InitEx.CAN_SJW = 1

    CAN_InitEx.CAN_NART = 0
    CAN_InitEx.CAN_RFLM = 0
    CAN_InitEx.CAN_TXFP = 1
    CAN_InitEx.CAN_RELAY = 0

    nRet = ControlCAN.VCI_InitCANEx(DevType,DeviceIndex,CANIndex,byref(CAN_InitEx))
    if(nRet == ControlCAN.STATUS_ERR):
        print("Init device failed!")
        exit()
    else:
        print("Init device success!")
    # Set filter
    CAN_FilterConfig = ControlCAN.VCI_FILTER_CONFIG()
    CAN_FilterConfig.FilterIndex = 0
    CAN_FilterConfig.Enable = 1        
    CAN_FilterConfig.ExtFrame = 0
    CAN_FilterConfig.FilterMode = 0
    CAN_FilterConfig.ID_IDE = 0
    CAN_FilterConfig.ID_RTR = 0
    CAN_FilterConfig.ID_Std_Ext = 0
    CAN_FilterConfig.MASK_IDE = 0
    CAN_FilterConfig.MASK_RTR = 0
    CAN_FilterConfig.MASK_Std_Ext = 0
    nRet = ControlCAN.VCI_SetFilter(DevType,DeviceIndex,CANIndex,byref(CAN_FilterConfig))
    if(nRet == ControlCAN.STATUS_ERR):
        print("Set filter failed!")
        exit()
    else:
        print("Set filter success!")
else:
    CAN_Init = ControlCAN.VCI_INIT_CONFIG()
    # Config device
    CAN_Init.AccCode = 0x00000000
    CAN_Init.AccMask = 0xFFFFFFFF
    CAN_Init.Filter = 1
    CAN_Init.Mode = 0
    CAN_Init.Timing0 = 0x00
    CAN_Init.Timing1 = 0x14
    nRet = ControlCAN.VCI_InitCAN(DevType,DeviceIndex,CANIndex,byref(CAN_Init));
    if(nRet == ControlCAN.STATUS_ERR):
        print("Init device failed!")
        exit()
    else:
        print("Init device success!")
# Register callback function
if(CAN_CALLBACK_READ_DATA == 1):
    pGetDataCallback = ControlCAN.PVCI_RECEIVE_CALLBACK(GetDataCallback)
    ControlCAN.VCI_RegisterReceiveCallback(DeviceIndex,pGetDataCallback)
# Start CAN
nRet = ControlCAN.VCI_StartCAN(DevType,DeviceIndex,CANIndex);
if(nRet == ControlCAN.STATUS_ERR):
    print("Start CAN failed!")
    exit()
else:
    print("Start CAN success!")
# Send data
if(CAN_SEND_DATA == 1):
    CAN_SendData = (ControlCAN.VCI_CAN_OBJ*2)()
    for j in range(0,2):
        CAN_SendData[j].DataLen = 8
        for i in range(0,CAN_SendData[j].DataLen):
            CAN_SendData[j].Data[i] = i+j
        CAN_SendData[j].ExternFlag = 0
        CAN_SendData[j].RemoteFlag = 0
        CAN_SendData[j].ID = 0x155+j
        if(CAN_MODE_LOOP_BACK == 1):
            CAN_SendData[j].SendType = 2
        else:
            CAN_SendData[j].SendType = 0
    nRet = ControlCAN.VCI_Transmit(DevType,DeviceIndex,CANIndex,byref(CAN_SendData),2)
    if(nRet == ControlCAN.STATUS_ERR):
        print("Send CAN data failed!")
        VCI_ResetCAN(DevType,DeviceIndex,CANIndex)
    else:
        print("Send CAN data success!")
# Delay
sleep(0.5)
# Get CAN status
if(CAN_GET_STATUS == 1):
    CAN_Status = ControlCAN.VCI_CAN_STATUS()
    nRet = ControlCAN.VCI_ReadCANStatus(DevType,DeviceIndex,CANIndex, byref(CAN_Status))
    if(nRet == ControlCAN.STATUS_ERR):
        print("Get CAN status failed!")
    else:
        print("Buffer Size : %d"%CAN_Status.BufferSize)
        print("ESR : 0x%08X"%CAN_Status.regESR)
        print("------Error warning flag : %d"%((CAN_Status.regESR>>0)&0x01))
        print("------Error passive flag : %d"%((CAN_Status.regESR >> 1) & 0x01))
        print("------Bus-off flag : %d"%((CAN_Status.regESR >> 2) & 0x01))
        print("------Last error code(%d) : "%((CAN_Status.regESR>>4)&0x07),end='')
        Error = ["No Error","Stuff Error","Form Error","Acknowledgment Error","Bit recessive Error","Bit dominant Error","CRC Error","Set by software"]
        print(Error[(CAN_Status.regESR>>4)&0x07])

# Read data
if(CAN_READ_DATA == 1):
    DataNum = ControlCAN.VCI_GetReceiveNum(DevType,DeviceIndex,CANIndex)
    CAN_ReceiveData = (ControlCAN.VCI_CAN_OBJ*DataNum)()
    if(DataNum > 0):
        ReadDataNum = ControlCAN.VCI_Receive(DevType,DeviceIndex,CANIndex, byref(CAN_ReceiveData), DataNum,0)
        for i in range(0,DataNum):
            print("")
            print("--CAN_ReceiveData.RemoteFlag = %d"%CAN_ReceiveData[i].RemoteFlag)
            print("--CAN_ReceiveData.ExternFlag = %d"%CAN_ReceiveData[i].ExternFlag)
            print("--CAN_ReceiveData.ID = 0x%X"%CAN_ReceiveData[i].ID)
            print("--CAN_ReceiveData.DataLen = %d"%CAN_ReceiveData[i].DataLen)
            print("--CAN_ReceiveData.Data:",end='')
            for j in range(0,CAN_ReceiveData[i].DataLen):
                print("%02X "%CAN_ReceiveData[i].Data[j],end='')
            print("")
            print("--CAN_ReceiveData.TimeStamp = %d"%CAN_ReceiveData[i].TimeStamp)

# Enter the enter to continue
print("Enter the enter to continue")
input()

if(CAN_CALLBACK_READ_DATA == 1):
    ControlCAN.VCI_LogoutReceiveCallback(DeviceIndex)
# Stop receive can data
nRet = ControlCAN.VCI_ResetCAN(DevType,DeviceIndex,CANIndex)
print("VCI_ResetCAN nRet = %d"%nRet)
# Close device
nRet = ControlCAN.VCI_CloseDevice(DevType,DeviceIndex)
print("VCI_CloseDevice nRet = %d"%nRet)
