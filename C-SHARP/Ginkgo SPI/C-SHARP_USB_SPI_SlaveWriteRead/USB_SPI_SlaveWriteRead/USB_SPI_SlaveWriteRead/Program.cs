  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:01
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


            for (Byte j = 0; j < 64; j++ )
            {
                write_buffer[j] = (Byte)(j);
            }
            ret = ControlSPI.VSI_SlaveWriteBytes(ControlSPI.VSI_USBSPI, 0, write_buffer, 64);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Slave write data error!!\n");
                return;
            }

            while (true)
            {
                Int32 read_data_num = 0;
                ret = ControlSPI.VSI_SlaveReadBytes(ControlSPI.VSI_USBSPI, 0, read_buffer, ref read_data_num,100);
                if (ret != ControlSPI.ERROR.SUCCESS)
                {
                    Console.WriteLine("Slave write data error!!\n");
                    return;
                }
                else if (read_data_num > 0)
                {
                    Console.WriteLine("Read data ");
                    for ( int i = 0; i < read_data_num; i++)
                    {
                        Console.Write("{0,-3:x}",read_buffer[i]);
                    }
                    Console.WriteLine();
                }
            }


        }
    }
}
