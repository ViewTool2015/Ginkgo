  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 10:53
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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UART_Test
{
    class USART_Test
    {
        static void Main(string[] args)
        {
            int ret;
            // Scan connected device
            ret = ControlUART.UART_ScanDevice(1);
            if (ret <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
            // Open device
            ret = ControlUART.UART_OpenDevice(0, 0);
            if (ret != ControlUART.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            // Get device information
            ControlUART.UART_BOARD_INFO UART_BoardInfo = new ControlUART.UART_BOARD_INFO();
            ret = ControlUART.UART_ReadBoardInfo(0, ref UART_BoardInfo);//It will open device
            if (ret != ControlUART.ERROR.SUCCESS)
            {
                Console.WriteLine("Get board info failed!");
                return;
            }
            else
            {
                Console.WriteLine("--BoardInfo.ProductName = " + Encoding.Default.GetString(UART_BoardInfo.ProductName));
                Console.WriteLine("--BoardInfo.FirmwareVersion = v" + UART_BoardInfo.FirmwareVersion[1].ToString() + "." + UART_BoardInfo.FirmwareVersion[2].ToString() + "." + UART_BoardInfo.FirmwareVersion[3].ToString());
                Console.WriteLine("--BoardInfo.HardwareVersion = v" + UART_BoardInfo.HardwareVersion[1].ToString() + "." + UART_BoardInfo.HardwareVersion[2].ToString() + "." + UART_BoardInfo.HardwareVersion[3].ToString());
                Console.Write("--BoardInfo.SerialNumber = ");
                for (int i = 0; i < 12; i++)
                {
                    Console.Write(UART_BoardInfo.SerialNumber[i].ToString("X2"));
                }
                Console.WriteLine("");
            }
            // Initialize device
            ControlUART.UART_INIT_CONFIG InitConfig = new ControlUART.UART_INIT_CONFIG();
            InitConfig.BaudRate = 115200;   // band rate
            InitConfig.Parity = 0;          // Parity
            InitConfig.RS485Mode = 232;     // RS485 mode
            InitConfig.StopBits = 0;        // Stop Bits, 1bit
            InitConfig.WordLength = 8;      // Word Length, 8bit
            ret = ControlUART.UART_InitDevice(2, 0, 0, ref InitConfig);
            if (ret != ControlUART.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            // Send data
            Byte[] WriteBuffer = new Byte[64];
            for (int i = 0; i < WriteBuffer.Length; i++)
            {
                WriteBuffer[i] = (Byte)i;
            }
            ret = ControlUART.UART_WriteBytes(2, 0, 0, WriteBuffer, (UInt16)WriteBuffer.Length);
            if (ret != ControlUART.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            // Loop read data
            UInt16 Len = 0;
            Byte[] ReadBuffer = new Byte[10240];
            int AllDataNum = 0;
            while (true)
            {
                ret = ControlUART.UART_ReadBytes(2, 0, 0, ReadBuffer, ref Len);
                if (ret == ControlUART.ERROR.READ_NO_DATA)
                {
                    System.Threading.Thread.Sleep(10);
                    continue;
                }
                else if (ret == ControlUART.ERROR.SUCCESS)
                {
                    // Print data if receive data
                    if (Len > 0)
                    {
                        AllDataNum += Len;
                        Byte read_data;
                        String read_data_str = "";
                        for (int i = 0; i < Len; i++)
                        {
                            read_data = ReadBuffer[i];
                            read_data_str += read_data.ToString("X2") + " ";
                            if (((i + 1) % 16) == 0)
                            {
                                read_data_str += "\n";
                            }
                        }
                        Console.WriteLine(read_data_str);
                        Console.WriteLine("AllDataNum = " + AllDataNum.ToString());
                    }
                    System.Threading.Thread.Sleep(10);
                    continue;
                }
                else
                {
                    Console.WriteLine("Read data error!");
                    return;
                }
            }
        }
    }
}
