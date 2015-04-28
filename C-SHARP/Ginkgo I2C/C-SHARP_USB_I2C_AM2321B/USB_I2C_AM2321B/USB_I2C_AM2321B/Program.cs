  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:28
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

namespace ControlI2C_AM2321B
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
            //Initialize Device(Hardware control mode)
            ControlI2C.I2C_Config.AddrType = ControlI2C.INIT_CONFIG.VII_ADDR_7BIT;
            ControlI2C.I2C_Config.ClockSpeed = 100000;
            ControlI2C.I2C_Config.ControlMode = ControlI2C.INIT_CONFIG.VII_HCTL_MODE;
            ControlI2C.I2C_Config.MasterMode = ControlI2C.INIT_CONFIG.VII_MASTER;
            ControlI2C.I2C_Config.SubAddrWidth = ControlI2C.INIT_CONFIG.VII_SUB_ADDR_NONE;
            ret = ControlI2C.VII_InitI2C(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, ref ControlI2C.I2C_Config);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            while (true)
            {
                Byte[] write_buffer = new Byte[8];
                // Wake up AM2311 sensor
                ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xB8, 0x00, write_buffer, 1);
                //Send out read temperature and humidity command
                write_buffer[0] = 0x03;
                write_buffer[1] = 0x00;
                write_buffer[2] = 0x04;
                ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xB8, 0x00, write_buffer, 3);
                if (ret != ControlI2C.ERROR.SUCCESS)
                {
                    Console.WriteLine("Write data error!");
                    return;
                }
                // Read out temperature and humidity
                Byte[] read_buffer = new Byte[8];
                ret = ControlI2C.VII_ReadBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xB8, 0x00, read_buffer, 8);
                if (ret != ControlI2C.ERROR.SUCCESS)
                {
                    Console.WriteLine("Read data error!");
                    return;
                }
                else
                {
                    double t = ((read_buffer[4] << 8) | read_buffer[5]) / 10.0;
                    Console.Clear();
                    Console.WriteLine("temperature:" + t.ToString("0.0") + " ℃");
                    double h = ((read_buffer[2] << 8) | read_buffer[3]) / 10.0;
                    Console.WriteLine("humidity:" + h.ToString("0.0") + " ％");
                }
                System.Threading.Thread.Sleep(1000);
            }
        }
    }
}
