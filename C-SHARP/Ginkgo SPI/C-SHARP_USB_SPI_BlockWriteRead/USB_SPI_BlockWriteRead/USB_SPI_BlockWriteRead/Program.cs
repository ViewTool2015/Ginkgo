  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 10:57
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
using System.Runtime.InteropServices;
using Ginkgo;

namespace ControlSPI_Test
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            ControlSPI.VSI_INIT_CONFIG pSPI_Config = new ControlSPI.VSI_INIT_CONFIG();
            //Scan connected device
            ret = ControlSPI.VSI_ScanDevice(1);
            if (ret <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
            // Open device
            ret = ControlSPI.VSI_OpenDevice(ControlSPI.VSI_USBSPI, 0, 0);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }

            // Initialize device(Master Mode, Hardware SPI, Half-Duplex)
            // function VSI_WriteBytes，VSI_ReadBytes，VSI_WriteReadBytes can be support in software SPI mode            
            pSPI_Config.ControlMode = 1;
            pSPI_Config.MasterMode = 1;
            pSPI_Config.ClockSpeed = 36000000;
            pSPI_Config.CPHA = 0;
            pSPI_Config.CPOL = 0;
            pSPI_Config.LSBFirst = 0;
            pSPI_Config.TranBits = 8;
            pSPI_Config.SelPolarity = 0;
            ret = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI, 0, ref pSPI_Config);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            Byte[] write_buffer = new Byte[10240];
            Byte[] read_buffer = new Byte[10240];
            Byte BlockNum = 5;
            Byte BlockSize = 2;
            UInt32 IntervalTime = 100;
            // Block mode write data, data will be send out BlockNum time(s), send out BlockSize bytes of data every time
            // Set CS to low before send data and set CS to high after send data complete when every time send data
            // Each time send data, interval is IntervalTime us, that is CS set to high and hold IntervalTime us
            for (Byte i = 0; i < BlockNum; i++ )
            {
                for (Byte j = 0; j < BlockSize; j++ )
                {
                    write_buffer[i * BlockSize + j] = (Byte)(i * j);
                }
            }
            ret = ControlSPI.VSI_BlockWriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, BlockSize, BlockNum, IntervalTime);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Block write data error!\n");
                return;
            }

            // Block mode read data: same as write data
            ret = ControlSPI.VSI_BlockReadBytes(ControlSPI.VSI_USBSPI, 0, 0, read_buffer, BlockSize, BlockNum, IntervalTime);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Block read data error!\n");
                return;
            }
            else
            {
                Console.WriteLine("Read data(Hex):\n");
                for (int i = 0; i < BlockNum; i++)
                {
                    for (int j = 0; j < BlockSize; j++)
                    {
                        Console.Write("{0,-4:x}", read_buffer[i * BlockSize + j]);
                    }
                    //Console.Write("  \n");
                }
            }
            Console.WriteLine();
            // Block mode: write and read data
            UInt16 write_BlockSize = 1;
            UInt16 read_BlockSize = 2;
            for (int i = 0; i < BlockNum; i++)
            {
                for (int j = 0; j < write_BlockSize; j++)
                {
                    write_buffer[i * write_BlockSize + j] = (Byte)(i * j);
                }
            }
            ret = ControlSPI.VSI_BlockWriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, write_BlockSize, read_buffer, read_BlockSize, BlockNum, IntervalTime);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Block write read data error!\n");
                return;
            }
            else
            {
                Console.WriteLine("Read data(Hex):\n");
                for (int i = 0; i < BlockNum; i++)
                {
                    for (int j = 0; j < read_BlockSize; j++)
                    {
                        Console.Write("{0,-4:x}", read_buffer[i * read_BlockSize + j]);
                    }
                    //Console.WriteLine("\n");
                }
            }
            Console.ReadLine();

            return;


        }
    }
}
