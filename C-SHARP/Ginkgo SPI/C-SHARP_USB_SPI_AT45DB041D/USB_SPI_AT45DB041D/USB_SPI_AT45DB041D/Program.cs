  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 10:56
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

/*
Hardware Connection
AT45DB041D      Ginkgo USB-SPI Adapter
1.SI        <-->  SPI_MOSI(Pin17)
2.SCK	    <-->  SPI_SCK(Pin13)
3.RESET     <-->  VCC(Pin2)
4.CS        <-->  SPI_SEL0(Pin11)
5.WP	    <-->  VCC(Pin2)
6.VCC	    <-->  VCC(Pin2)
7.GND	    <-->  GND(Pin19/Pin20)
8.SO	    <-->  SPI_MISO(Pin15)
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Ginkgo;


namespace USB_SPI_AT45DB041D
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            Byte[] write_buffer = new Byte[10240];
            Byte[] Read_buffer = new Byte[10240];
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
            //Wait Busy
            do
            {
                write_buffer[0] = 0xD7;//SPI Flash read status register
                ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, Read_buffer, 1);
                if (ret != ControlSPI.ERROR.SUCCESS)
                {
                    Console.WriteLine("Get status error!");
                    return;
                }
            } while ((Read_buffer[0]&0x80)==0);
            //Page Erase
            write_buffer[0] = 0x81;
            write_buffer[1] = 0x00;
            write_buffer[2] = 0x00;
            write_buffer[3] = 0x00;
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Sector Erase start error!");
                return ;
            }
            else
            {
                Console.WriteLine("Sector erase start success!");
            }
            //Wait Busy
            do
            {
                write_buffer[0] = 0xD7;//SPI Flash read status register
                ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, Read_buffer, 1);
                if (ret != ControlSPI.ERROR.SUCCESS)
                {
                    Console.WriteLine("Get status error!");
                    return;
                }
            } while ((Read_buffer[0] & 0x80) == 0);
            //Write data to buffer
            write_buffer[0] = 0x87;
            write_buffer[1] = 0x00;
            write_buffer[2] = 0x00;
            write_buffer[3] = 0x00;
            for (int i = 0; i < 528; i++)
            {
                write_buffer[4 + i] = (Byte)i;
            }
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4 + 528);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            else
            {
                Console.WriteLine("Write data succeed!");
            }
            //Buffer to Main Memory Page Program without Built-in Erase
            write_buffer[0] = 0x89;
            write_buffer[1] = 0x00;
            write_buffer[2] = 0x00;
            write_buffer[3] = 0x00;
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data to main memory error!");
                return;
            }
            else
            {
                Console.WriteLine("Write data to main memory success!");
            }
            //Wait Busy
            do
            {
                write_buffer[0] = 0xD7;
                ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, Read_buffer, 1);
                if (ret != ControlSPI.ERROR.SUCCESS)
                {
                    Console.WriteLine("Get status error!");
                    return;
                }
            } while ((Read_buffer[0] & 0x80) == 0);
            //Main Memory Page Read
            write_buffer[0] = 0xD2;
            write_buffer[1] = 0x00;
            write_buffer[2] = 0x00;
            write_buffer[3] = 0x00;
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 8,Read_buffer,528);
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
                for (int i = 0; i < 528; i++)
                {
                    read_data = Read_buffer[i];
                    read_data_str += read_data.ToString("X2") + " ";
                    if (((i + 1) % 16) == 0)
                    {
                        read_data_str += "\n";
                    }
                }
                Console.WriteLine(read_data_str);
            }
            //Close device
            ret = ControlSPI.VSI_CloseDevice(ControlSPI.VSI_USBSPI, 0);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("close device error!");
            }
            else
            {
                Console.WriteLine("close device success!");
            }
            Console.ReadLine();
        }
    }
}
