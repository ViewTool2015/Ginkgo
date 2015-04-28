  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:31
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
using System.Threading;
using System.Runtime.InteropServices;
using Ginkgo;


namespace ControlI2C_AT24C02
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            // Scan connected device
            ret = ControlI2C.VII_ScanDevice(1);
            if (ret <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
            // Open device
            ret = ControlI2C.VII_OpenDevice(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            // Initialize device(hardware mode)
            ControlI2C.I2C_Config.AddrType = ControlI2C.INIT_CONFIG.VII_ADDR_7BIT;
            ControlI2C.I2C_Config.ClockSpeed = 400000;
            ControlI2C.I2C_Config.ControlMode = ControlI2C.INIT_CONFIG.VII_HCTL_MODE;
            ControlI2C.I2C_Config.MasterMode = ControlI2C.INIT_CONFIG.VII_SLAVE;
            ControlI2C.I2C_Config.SubAddrWidth = ControlI2C.INIT_CONFIG.VII_SUB_ADDR_1BYTE;
            ret = ControlI2C.VII_InitI2C(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, ref ControlI2C.I2C_Config);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }

            Byte i;
            UInt16 SlaveReadLen = 0;
            Byte[] write_buffer = new Byte[512];
	        Byte[] read_buffer = new Byte[512];

            //Slave write data
            for (i = 0; i < 16; i++)
            {
                write_buffer[i] = i;
            }
            ret = ControlI2C.VII_SlaveWriteBytes(ControlI2C.VII_USBI2C, 0, 0, write_buffer, 16);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Slave write data error!\n");
                return ;
            }
            else
            {
                Console.WriteLine("Write Data:\n");
                for (i = 0; i < 16; i++)
                {
                    Console.WriteLine("%02X ", write_buffer[i]);
                    if (((i + 1) % 16) == 0)
                    {
                        Console.WriteLine("\n");
                    }
                }
            }
            //Slave read data
            while (true)
            {
                Thread.Sleep(50);
                ret = ControlI2C.VII_SlaveReadBytes(ControlI2C.VII_USBI2C, 0, 0, read_buffer, ref SlaveReadLen);
                if ((ret == ControlI2C.ERROR.SUCCESS) && (SlaveReadLen > 0))
                {
                    Console.WriteLine("Read Data:\n");
                    for (i = 0; i < SlaveReadLen; i++)
                    {
                        Console.WriteLine("%02X ", read_buffer[i]);
                        if (((i + 1) % 16) == 0)
                        {
                            Console.WriteLine("\n");
                        }
                    }
                    Console.WriteLine("\n");
                    continue;
                }
                if (ret == ControlI2C.ERROR.READ_NO_DATA)
                {
                    continue;
                }
                else
                {
                    Console.WriteLine("Slave read data error!\n");
                    return ;
                }
            }
            return ;

        }
    }
}