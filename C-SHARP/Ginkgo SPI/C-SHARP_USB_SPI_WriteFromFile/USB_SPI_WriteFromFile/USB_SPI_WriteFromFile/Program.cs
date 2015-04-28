  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:27
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
using System.IO;
using System.Runtime.InteropServices;
using Ginkgo;


namespace ControlSPI_W25Q32
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
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
            ControlSPI.VSI_INIT_CONFIG SPI_Config;

            SPI_Config.ControlMode = 1;
            SPI_Config.MasterMode = 1;
            SPI_Config.ClockSpeed = 36000000;
            SPI_Config.CPHA = 0;
            SPI_Config.CPOL = 0;
            SPI_Config.LSBFirst = 0;
            SPI_Config.TranBits = 8;
            SPI_Config.SelPolarity = 0;
            ret = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI, 0, ref SPI_Config);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }

            
            Byte[] WriteDataTemp = new Byte[512];
            char[] data_buff = new char[4000];
            String strBuff = new string(' ', 512);
            String[] strTmp = new string[1000];
            UInt16 j;
            
            // Open file
            FileStream fs = new FileStream("data.txt", FileMode.Open, FileAccess.Read);
            StreamReader ftmp = new StreamReader(fs);

            // Read file and write to SPI and console
            do
            {              
                // Read data one row from file                
                strBuff = ftmp.ReadLine();


                // The string every row use space to separate
                strTmp = strBuff.Split(new char[]{' '});
                // The string convert to numerical 
                if(strBuff.Length > 1)//if (i > 1)
                {
                    Console.Write("Write Data(Hex): ");
                }
                Console.WriteLine();
                for (j = 0; j < (strTmp.Length); j++)
                {
                    WriteDataTemp[j] = (Byte)(int.Parse(strTmp[j]));// attention data overflow
                    Console.Write("{0,3:d}  ", int.Parse(strTmp[j]));
                }
                Console.WriteLine();
                // Send out the data via USB_SPI
                if (j > 0)
                {
                    ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, WriteDataTemp, j);
                    if (ret != ControlSPI.ERROR.SUCCESS)
                    {
                        Console.WriteLine("Write enable error");
                        return ;
                    }
                }
            } while (!ftmp.EndOfStream);
            fs.Close();
            return ;

        }
    }
}
