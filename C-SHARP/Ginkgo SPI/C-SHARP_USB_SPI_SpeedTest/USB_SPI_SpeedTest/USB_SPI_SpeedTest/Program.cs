  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:02
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
using Timer;

namespace ControlSPI_Test
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret, i;
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

            // Start write data speed test
            Console.WriteLine("Start write data speed test.....");
            // Write Data Speed Test
            Byte[] write_buffer = new Byte[10240];
            Byte[] read_buffer = new Byte[10240];
            long litmp = 0;
            double StartTime, EndTime;
            double dfFreq;

            timer.QueryPerformanceFrequency(ref litmp);// Get the performance counter frequency, in n/s
            dfFreq = litmp;
            timer.QueryPerformanceCounter(ref litmp);  // Get the current value of the performance counter
            StartTime = litmp;                         // Start time
            for (i = 0; i < 400; i++ )
            {
                ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 10000);
                if (ret != ControlSPI.ERROR.SUCCESS)
                {
                    Console.WriteLine("write data error!");
                    break;
                }
            }
            timer.QueryPerformanceCounter(ref litmp);   // Get the current value of the performance counter
            EndTime = litmp;

            // Print the write data speed information
            Console.WriteLine("Write Data Numbers: {0} Bytes\n", i * 10000);
            Console.WriteLine("Write Data Elapsed Time: {0:###.000}\n", (EndTime - StartTime) / dfFreq);
            Console.WriteLine("Write Data Speed: {0:###.000} KByte/s\n", ((i * 10000) / 1024) / ((EndTime - StartTime) / dfFreq));
            // Start Write Data Speed Test
            Console.WriteLine("\nStart Read Data Speed Test...\n");
            timer.QueryPerformanceCounter(ref litmp);// Get the current value of the performance counter
            StartTime = litmp; // Start time
            for (i = 0; i < 400; i++)
            {
                ret = ControlSPI.VSI_ReadBytes(ControlSPI.VSI_USBSPI, 0, 0, read_buffer, 10000);
                if (ret != ControlSPI.ERROR.SUCCESS)
                {
                    Console.WriteLine("Read data error!\n");
                    break;
                }
            }
            timer.QueryPerformanceCounter(ref litmp);// Get the current value of the performance counter
            EndTime = litmp; // Stop time
            // Print the write data speed information
            Console.WriteLine("Read Data Numbers: {0:d} Bytes\n", i * 10000);
            Console.WriteLine("Read Data Elapsed Time: {0:f}\n", (EndTime - StartTime) / dfFreq);
            Console.WriteLine("Read Data Speed: {0:f} KByte/s\n", ((i * 10000) / 1024) / ((EndTime - StartTime) / dfFreq));

            return;

        }
    }
}
