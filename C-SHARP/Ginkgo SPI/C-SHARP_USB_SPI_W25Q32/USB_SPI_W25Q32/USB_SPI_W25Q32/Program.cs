  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:26
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


namespace ControlSPI_W25Q32
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
            ret = ControlSPI.VSI_OpenDevice(ControlSPI.VSI_USBSPI,0,0);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            // Initialize device
            pSPI_Config.ControlMode = 1;
            pSPI_Config.MasterMode = 1;
            pSPI_Config.ClockSpeed = 36000000;
            pSPI_Config.CPHA = 0;
            pSPI_Config.CPOL = 0;
            pSPI_Config.LSBFirst = 0;
            pSPI_Config.TranBits = 8;
            ret = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI, 0, ref pSPI_Config);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            // Get JEDEC ID
            Byte[] write_buffer = new Byte[10240];
            Byte[] Read_buffer = new Byte[10240];
            write_buffer[0] = 0x9F;//SPI Flash get JEDEC ID command
                        ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, Read_buffer, 3);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Get JEDEC ID error!");
                return;
            }
            else
            {
                Int32 JEDEC_ID = (Read_buffer[0] << 16) | (Read_buffer[1] << 8) | Read_buffer[2];
                Console.WriteLine("JEDEC ID(Hex):{0}", JEDEC_ID.ToString("X6"));
            }
            // Send write enable
            write_buffer[0] = 0x06;//SPI Flash write enable command
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            // Erase first sector start address h
            write_buffer[0] = 0x20;//SPI Flash sector erase command
            write_buffer[1] = 0x00;
            write_buffer[2] = 0x00;
            write_buffer[3] = 0x00;
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            // Delay to ensure the operation to complete
            System.Threading.Thread.Sleep(100);
            // Write Enable
            write_buffer[0] = 0x06;//SPI Flash write enable command
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            // Page Program
            write_buffer[0] = 0x02;//SPI Flash write data command
            write_buffer[1] = 0x00;//address
            write_buffer[2] = 0x00;//
            write_buffer[3] = 0x00;//
            for (int i = 0; i < 256; i++)
            {
                write_buffer[4 + i] = (Byte)i;
            }
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4+256);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            else
            {
                Console.WriteLine("Write data succeed!");
            }
            // Delay to ensure the operation to complete
            System.Threading.Thread.Sleep(10);
            // Read Data
            write_buffer[0] = 0x03;//SPI Flash read data command
            write_buffer[1] = 0x00;//
            write_buffer[2] = 0x00;//
            write_buffer[3] = 0x00;//
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4, Read_buffer, 256);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Read data error!");
                return;
            }
            else
            {
                Int32 read_data;
                String read_data_str = "";
                Console.WriteLine("Get Data:");
                for (int i = 0; i < 256; i++)
                {
                    read_data = Read_buffer[i];
                    read_data_str += read_data.ToString("X2") + " ";
                    if (((i+1) % 24) == 0)
                    {
                        read_data_str += "\n";
                    }
                }
                Console.WriteLine(read_data_str);
            }
            Console.ReadLine();
        }
    }
}
