  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:03
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
//using System.Text.Encoding;
using System.Runtime.InteropServices;
using Ginkgo;
using System.IO;



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
            // function VSI_WriteBytes，VSI_ReadBytes，VSI_WriteReadBytes can be support in software SPI mode            //该模式下可以使用VSI_WriteBytes，VSI_ReadBytes，VSI_WriteReadBytes，VSI_BlockWriteBytes，VSI_BlockReadBytes，VSI_BlockWriteReadBytes这些函数
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
            
            Byte[] WriteDataTemp = new Byte[512];
            Byte[] ReadDataTemp = new Byte[10240];
            Int32 StartAddr = 0x000000;// Read data start address
            Int32 EndAddr = 0x400000;// Read data end address
            Int32 Addr = 0;
            char[] charTmp = new char[1024];
            Byte[] byteTmp = new Byte[1024];
            String strTmp = new String(' ',1024);
            Int64 temp = 0;

            // JEDEC ID
            WriteDataTemp[0] = 0x9F;
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, WriteDataTemp, 1, ReadDataTemp, 3);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Read flash ID error :%d\n", ret);
                return;
            }
            else
            {
                Console.WriteLine("Flash ID = {0, -6:x}", (ReadDataTemp[0] << 16) | (ReadDataTemp[1] << 8) | (ReadDataTemp[2]));
            }
            // Open file
            

       
            FileStream fs = new FileStream("Data.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite);
            //FileStream aFile = File.OpenRead("Data.txt");
            Console.WriteLine("Start read data,please wait...\n");
            // Read data
            for (Addr = StartAddr; Addr < EndAddr; Addr += 4096)
            {
                WriteDataTemp[0] = 0x0B;
                WriteDataTemp[1] = (Byte)(Addr >> 16);
                WriteDataTemp[2] = (Byte)(Addr >> 8);
                WriteDataTemp[3] = (Byte)(Addr >> 0);
                WriteDataTemp[4] = 0x00;
                ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, WriteDataTemp, 4, ReadDataTemp, 4096);
                if (ret != ControlSPI.ERROR.SUCCESS)
                {
                    Console.WriteLine("Read data error :%d\n", ret);
                    Console.ReadLine();
                    return;
                }
                else
                {
                    for (int j = 0; j < 4096; j += 64)
                    {
                        strTmp = null;
                        strTmp = new string(' ', 1024);
                        for (i = 0; i < 32; i++)
                        {
                            temp = (ReadDataTemp[j + i * 2] << 8) | ReadDataTemp[j + i * 2 + 1];
                            strTmp = strTmp + temp.ToString();
                            strTmp = strTmp.Replace(" ","");
                        }
                       
                        charTmp = strTmp.ToCharArray(); 
                        byteTmp = Encoding.Default.GetBytes(charTmp);
                        fs.Write(byteTmp, 0, strTmp.Length);
                    }
                }

            }
            fs.Close();
            Console.WriteLine("Read data end!\n");
            Console.ReadLine();
            return ;

        }
    }
}
