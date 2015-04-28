  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/10 17:43
  * @brief    : Program demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2015, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
 

#define	CAN_MODE_LOOP_BACK
#define	CAN_SEND_DATA
#define CAN_GET_BOARD_INFO
//#define CAN_READ_DATA			
#define CAN_CALLBACK_READ_DATA
#define	CAN_INIT_EX
#define CAN_GET_STATUS

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using Ginkgo;



namespace CANTest
{
    class CANTest
    {
        public static void GetDataCallback(UInt32 DevIndex, UInt32 CANIndex, UInt32 Len)
        {
            ControlCAN.VCI_CAN_OBJ[] CAN_ReceiveData = new ControlCAN.VCI_CAN_OBJ[Len];
            for (int i = 0; i < CAN_ReceiveData.Length; i++)
            {
                CAN_ReceiveData[i].Data = new Byte[8];
            }

            UInt32 ReadDataNum;
            UInt32 DataNum = ControlCAN.VCI_GetReceiveNum(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0);

            if (DataNum > 0)
            {
                ReadDataNum = ControlCAN.VCI_Receive(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0, CAN_ReceiveData, (UInt32)CAN_ReceiveData.Length);
                for (int i = 0; i < ReadDataNum; i++)
                {
                    Console.WriteLine("");
                    Console.WriteLine("--CAN_ReceiveData.RemoteFlag = " + CAN_ReceiveData[i].RemoteFlag.ToString());
                    Console.WriteLine("--CAN_ReceiveData.ExternFlag = " + CAN_ReceiveData[i].ExternFlag.ToString());
                    Console.WriteLine("--CAN_ReceiveData.ID = 0x" + CAN_ReceiveData[i].ID.ToString("X4"));
                    Console.WriteLine("--CAN_ReceiveData.DataLen = " + CAN_ReceiveData[i].DataLen.ToString());
                    Console.Write("--CAN_ReceiveData.Data:");
                    for (int j = 0; j < CAN_ReceiveData[i].DataLen; j++)
                    {
                        Console.Write(CAN_ReceiveData[i].Data[j].ToString("X2") + " ");
                    }
                    Console.WriteLine("");
                    Console.WriteLine("--CAN_ReceiveData.TimeStamp = " + CAN_ReceiveData[i].TimeStamp.ToString());
                }
            }
        }
        static void Main(string[] args)
        {
            UInt32 Status;
            //Scan device
            Status = ControlCAN.VCI_ScanDevice(1);
            if (Status <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
#if CAN_GET_BOARD_INFO
            ControlCAN.VCI_BOARD_INFO_EX CAN_BoardInfo = new ControlCAN.VCI_BOARD_INFO_EX();
            Status = ControlCAN.VCI_ReadBoardInfoEx(0, ref CAN_BoardInfo);//It will open device
            if (Status == ControlCAN.VCI_STATUS.STATUS_ERR)
            {
                Console.WriteLine("Get board info failed!");
                return;
            }
            else
            {
                Console.WriteLine("--CAN_BoardInfo.ProductName = " + Encoding.Default.GetString(CAN_BoardInfo.ProductName));
                Console.WriteLine("--CAN_BoardInfo.FirmwareVersion = v" + CAN_BoardInfo.FirmwareVersion[1].ToString() + "." + CAN_BoardInfo.FirmwareVersion[2].ToString() + "." + CAN_BoardInfo.FirmwareVersion[3].ToString());
                Console.WriteLine("--CAN_BoardInfo.HardwareVersion = v" + CAN_BoardInfo.HardwareVersion[1].ToString() + "." + CAN_BoardInfo.HardwareVersion[2].ToString() + "." + CAN_BoardInfo.HardwareVersion[3].ToString());
                Console.Write("--CAN_BoardInfo.SerialNumber = ");
                for (int i = 0; i < 12; i++)
                {
                    Console.Write(CAN_BoardInfo.SerialNumber[i].ToString("X2"));
                }
                Console.WriteLine("");
            }
#else
            //Open device
            Status = ControlCAN.VCI_OpenDevice(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0);
            if (Status == ControlCAN.VCI_STATUS.STATUS_ERR)
            {
                Console.WriteLine("Open device failed!");
                return ;
            }else{
                Console.WriteLine("Open device success!");
            }
#endif
#if CAN_INIT_EX
            ControlCAN.VCI_INIT_CONFIG_EX CAN_InitEx = new ControlCAN.VCI_INIT_CONFIG_EX();
            //Config device
            CAN_InitEx.CAN_ABOM = 0;//Automatic bus-off management
#if CAN_MODE_LOOP_BACK
            CAN_InitEx.CAN_Mode = 1;
#else
            CAN_InitEx.CAN_Mode = 0;
#endif
            //1Mbps
            CAN_InitEx.CAN_BRP = 6;
            CAN_InitEx.CAN_BS1 = 3;
            CAN_InitEx.CAN_BS2 = 2;
            CAN_InitEx.CAN_SJW = 1;

            CAN_InitEx.CAN_NART = 1;//No automatic retransmission
            CAN_InitEx.CAN_RFLM = 0;//Receive FIFO locked mode
            CAN_InitEx.CAN_TXFP = 0;//Transmit FIFO priority
            CAN_InitEx.CAN_RELAY = 0;
            Status = ControlCAN.VCI_InitCANEx(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0, ref CAN_InitEx);
            if (Status == ControlCAN.VCI_STATUS.STATUS_ERR)
            {
                Console.WriteLine("Init device failed!");
                return;
            }
            else
            {
                Console.WriteLine("Init device success!");
            }
            //Set filter
            ControlCAN.VCI_FILTER_CONFIG CAN_FilterConfig = new ControlCAN.VCI_FILTER_CONFIG();
            CAN_FilterConfig.FilterIndex = 0;
            CAN_FilterConfig.Enable = 1;//Enable
            CAN_FilterConfig.ExtFrame = 0;
            CAN_FilterConfig.FilterMode = 0;
            CAN_FilterConfig.ID_IDE = 0;
            CAN_FilterConfig.ID_RTR = 0;
            CAN_FilterConfig.ID_Std_Ext = 0;
            CAN_FilterConfig.MASK_IDE = 0;
            CAN_FilterConfig.MASK_RTR = 0;
            CAN_FilterConfig.MASK_Std_Ext = 0;
            Status = ControlCAN.VCI_SetFilter(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0, ref CAN_FilterConfig);
            if (Status == ControlCAN.VCI_STATUS.STATUS_ERR)
            {
                Console.WriteLine("Set filter failed!");
                return;
            }
            else
            {
                Console.WriteLine("Set filter success!");
            }
#else
            ControlCAN.VCI_INIT_CONFIG CAN_Init = new ControlCAN.VCI_INIT_CONFIG();
            //Config device
	        CAN_Init.AccCode = 0x00000000;
	        CAN_Init.AccMask = 0xFFFFFFFF;
	        CAN_Init.Filter = 1;
	        CAN_Init.Mode = 0;
	        CAN_Init.Timing0 = 0x00;
	        CAN_Init.Timing1 = 0x14;
            Status = ControlCAN.VCI_InitCAN(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0, ref CAN_Init);
            if (Status == ControlCAN.VCI_STATUS.STATUS_ERR)
            {
                Console.WriteLine("Init device failed!");
                return ;
            }else{
                Console.WriteLine("Init device success!");
            }
#endif
            //Start CAN
            Status = ControlCAN.VCI_StartCAN(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0);
            if (Status == ControlCAN.VCI_STATUS.STATUS_ERR)
            {
                Console.WriteLine("Start CAN failed!");
                return;
            }
            else
            {
                Console.WriteLine("Start CAN success!");
            }
            //Register receive callback
#if CAN_CALLBACK_READ_DATA
            ControlCAN.VCI_RegisterReceiveCallback(0, GetDataCallback);
#endif
            //Send data
#if CAN_SEND_DATA
            ControlCAN.VCI_CAN_OBJ[] CAN_SendData = new ControlCAN.VCI_CAN_OBJ[2];
            for (int i = 0; i < CAN_SendData.Length; i++)
            {
                CAN_SendData[i].DataLen = 8;
                CAN_SendData[i].Data = new Byte[8];
                for (int j = 0; j < CAN_SendData[i].DataLen; j++)
                {
                    CAN_SendData[i].Data[j] = (Byte)(i + j);
                }
                CAN_SendData[i].ExternFlag = 0;
                CAN_SendData[i].RemoteFlag = 0;
                CAN_SendData[i].ID = 0x123 + (UInt32)i;
#if CAN_MODE_LOOP_BACK
                CAN_SendData[i].SendType = 2;
#else
                CAN_SendData[i].SendType = 0;
#endif
            }
            Status = ControlCAN.VCI_Transmit(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0, CAN_SendData, (UInt32)CAN_SendData.Length);
            if (Status == ControlCAN.VCI_STATUS.STATUS_ERR)
            {
                Console.WriteLine("Send CAN data failed!");
            }
            else
            {
                Console.WriteLine("Send CAN data success!");
            }
#endif
            //Delay
            System.Threading.Thread.Sleep(50);
#if CAN_GET_STATUS
            ControlCAN.VCI_CAN_STATUS CAN_Status = new ControlCAN.VCI_CAN_STATUS();
            Status = ControlCAN.VCI_ReadCANStatus(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0, ref CAN_Status);
            if (Status == ControlCAN.VCI_STATUS.STATUS_ERR)
            {
                Console.WriteLine("Get CAN status failed!");
                return;
            }
            else
            {
                Console.WriteLine("Buffer Size : " + CAN_Status.BufferSize.ToString());
                Console.WriteLine("ESR : 0x" + CAN_Status.regESR.ToString("X8"));
                Console.WriteLine("------Error warning flag : " + ((CAN_Status.regESR >> 0) & 0x01).ToString());
                Console.WriteLine("------Error passive flag : " + ((CAN_Status.regESR >> 1) & 0x01).ToString());
                Console.WriteLine("------Bus-off flag : " + ((CAN_Status.regESR >> 2) & 0x01).ToString());
                Console.Write("------Last error code(" + ((CAN_Status.regESR >> 4) & 0x07).ToString() + ") : ");
                switch ((CAN_Status.regESR >> 4) & 0x07)
                {
                    case 0:
                        Console.WriteLine("No Error");
                        break;
                    case 1:
                        Console.WriteLine("Stuff Error");
                        break;
                    case 2:
                        Console.WriteLine("Form Error");
                        break;
                    case 3:
                        Console.WriteLine("Acknowledgment Error");
                        break;
                    case 4:
                        Console.WriteLine("Bit recessive Error");
                        break;
                    case 5:
                        Console.WriteLine("Bit dominant Error");
                        break;
                    case 6:
                        Console.WriteLine("CRC Error");
                        break;
                    case 7:
                        Console.WriteLine("Set by software");
                        break;
                    default:
                        break;
                }
                Console.WriteLine("------Transmit error counter : " + ((CAN_Status.regESR >> 16) & 0xFF).ToString());
                Console.WriteLine("------Receive error counter : " + ((CAN_Status.regESR >> 24) & 0xFF).ToString());
                Console.WriteLine("TSR : 0x" + CAN_Status.regTSR.ToString("X8"));

            }
#endif
            //Read data
#if CAN_READ_DATA
            ControlCAN.VCI_CAN_OBJ[] CAN_ReceiveData = new ControlCAN.VCI_CAN_OBJ[2];
            for (int i = 0; i < CAN_ReceiveData.Length; i++)
            {
                CAN_ReceiveData[i].Data = new Byte[8];
            }

            UInt32 ReadDataNum;
            UInt32 DataNum = ControlCAN.VCI_GetReceiveNum(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0);

            if (DataNum > 0)
            {
                ReadDataNum = ControlCAN.VCI_Receive(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0,CAN_ReceiveData, (UInt32)CAN_ReceiveData.Length);
                for (int i = 0; i < ReadDataNum;i++ ){
                    Console.WriteLine("");
                    Console.WriteLine("--CAN_ReceiveData.RemoteFlag = " + CAN_ReceiveData[i].RemoteFlag.ToString());
                    Console.WriteLine("--CAN_ReceiveData.ExternFlag = "+ CAN_ReceiveData[i].ExternFlag.ToString());
                    Console.WriteLine("--CAN_ReceiveData.ID = 0x"+ CAN_ReceiveData[i].ID.ToString("X4"));
                    Console.WriteLine("--CAN_ReceiveData.DataLen = "+ CAN_ReceiveData[i].DataLen.ToString());
                    Console.Write("--CAN_ReceiveData.Data:");
                    for (int j = 0; j < CAN_ReceiveData[i].DataLen; j++)
                    {
                        Console.Write(CAN_ReceiveData[i].Data[j].ToString("X2")+" ");
                    }
                    Console.WriteLine("");
                    Console.WriteLine("--CAN_ReceiveData.TimeStamp = "+CAN_ReceiveData[i].TimeStamp.ToString());
                }
            }
#endif
            Console.WriteLine("");
#if CAN_CALLBACK_READ_DATA
            ControlCAN.VCI_LogoutReceiveCallback(0);
#endif
            //Stop receive can data
            ControlCAN.VCI_ResetCAN(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0, 0);
            Console.WriteLine("VCI_ResetCAN");
            //Close Device
            ControlCAN.VCI_CloseDevice(ControlCAN.VCI_DEVICE_TYPE.VCI_USBCAN2, 0);
            Console.WriteLine("VCI_CloseDevice");
        }
    }
}
