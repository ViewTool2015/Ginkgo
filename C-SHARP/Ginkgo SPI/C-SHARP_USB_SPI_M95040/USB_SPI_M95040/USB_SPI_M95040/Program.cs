  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 10:58
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
using Ginkgo;


namespace USB_SPI_M95040
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            Byte[] write_buffer = new Byte[10240];
            Byte[] read_buffer = new Byte[10240];
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

            // Write enable
            write_buffer[0] = 0x06; //write enable command
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, read_buffer, 3);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Flash write enable error {0}", ret);
                return;
            }
            else
            {            
                Console.WriteLine("Write enable success!\n");
            }
            // Write data to address 0
            write_buffer[0] = 0x02;
            write_buffer[1] = 0x00; // low 8bits address, please see m95040 datasheet for detail
            for (Int32 i = 0; i < 16; i++ )
            {
                write_buffer[i + 2] = (Byte)i;
            }
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 18);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write enable success!");
                return ;
            }
            else
            {
                Console.WriteLine("Write 16 byte success!");
            }
            // Delay to ensure data write completely
            System.Threading.Thread.Sleep(100);

            // Read the written data
            write_buffer[0] = 0x03;
            write_buffer[1] = 0x00; // low 8bits address, please see m95040 datasheet for detail
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 2, read_buffer, 16);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Read Data error {0}", ret);
                return;
            }
            else
            {
                Console.WriteLine("Read Data success");
            }

            for ( Int32 i = 0; i < 16; i++ )
            {
                Console.Write("{0,  2}  ", read_buffer[i]);
            }
  
        }
    }
}
