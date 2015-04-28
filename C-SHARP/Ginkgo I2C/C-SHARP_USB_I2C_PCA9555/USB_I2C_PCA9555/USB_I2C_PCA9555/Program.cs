  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:30
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

namespace ControlI2C_PCA9555
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            //Scan connected device
            ret = ControlI2C.VII_ScanDevice(1);
            if (ret <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
            //Open device
            ret = ControlI2C.VII_OpenDevice(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            //Initialize device(Hardware control mode)
            ControlI2C.I2C_Config.AddrType = ControlI2C.INIT_CONFIG.VII_ADDR_7BIT;
            ControlI2C.I2C_Config.ClockSpeed = 400000;
            ControlI2C.I2C_Config.ControlMode = ControlI2C.INIT_CONFIG.VII_HCTL_MODE;
            ControlI2C.I2C_Config.MasterMode = ControlI2C.INIT_CONFIG.VII_MASTER;
            ControlI2C.I2C_Config.SubAddrWidth = ControlI2C.INIT_CONFIG.VII_SUB_ADDR_1BYTE;
            ret = ControlI2C.VII_InitI2C(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, ref ControlI2C.I2C_Config);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            // Set all ports to output mode
            Byte[] write_buffer = new Byte[8];
            write_buffer[0] = 0;
            write_buffer[1] = 0;
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0x40, 0x06, write_buffer, 2);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            while (true)
            {
                //Control all the port output low level
                write_buffer[0] = 0;
                write_buffer[1] = 0;
                ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0x40, 0x02, write_buffer, 2);
                if (ret != ControlI2C.ERROR.SUCCESS)
                {
                    Console.WriteLine("Write data error!");
                    return;
                }
                System.Threading.Thread.Sleep(500);
                //Control of all port output high level
                write_buffer[0] = 0xFF;
                write_buffer[1] = 0xFF;
                ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0x40, 0x02, write_buffer, 2);
                if (ret != ControlI2C.ERROR.SUCCESS)
                {
                    Console.WriteLine("Write data error!");
                    return;
                }
                System.Threading.Thread.Sleep(500);
            }
        }
    }
}
