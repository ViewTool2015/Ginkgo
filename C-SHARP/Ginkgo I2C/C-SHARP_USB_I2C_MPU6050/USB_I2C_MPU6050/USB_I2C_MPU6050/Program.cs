  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/09 17:33
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


namespace ControlI2C_MPU6050
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
            // Initialize MPU6050 sensor
            Byte[] write_buffer = new Byte[8];
            write_buffer[0] = 0;
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xD0, 0x6B, write_buffer, 1);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            System.Threading.Thread.Sleep(10);
            write_buffer[0] = 0;
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xD0, 0x6C, write_buffer, 1);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            System.Threading.Thread.Sleep(10);
            write_buffer[0] = 0x08;
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xD0, 0x1B, write_buffer, 1);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            System.Threading.Thread.Sleep(10);
            write_buffer[0] = 0x01;
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xD0, 0x1A, write_buffer, 1);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            System.Threading.Thread.Sleep(10);
            write_buffer[0] = 0x08;
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xD0, 0x1C, write_buffer, 1);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            System.Threading.Thread.Sleep(10);
            while (true)
            {
                // Get data
                Byte[] read_buffer = new Byte[16];
                ret = ControlI2C.VII_ReadBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xD0, 0x3B, read_buffer, 14);
                if (ret != ControlI2C.ERROR.SUCCESS)
                {
                    Console.WriteLine("Read data error!");
                    return;
                }
                // Process data
                Int16 AccelX, AccelY, AccelZ;
                AccelX = (Int16)((read_buffer[0] << 8) | read_buffer[1]);
                AccelY = (Int16)((read_buffer[2] << 8) | read_buffer[3]);
                AccelZ = (Int16)((read_buffer[4] << 8) | read_buffer[5]);
                Int16 GyroX, GyroY, GyroZ;
                GyroX = (Int16)((read_buffer[8] << 8) | read_buffer[9]);
                GyroY = (Int16)((read_buffer[10] << 8) | read_buffer[11]);
                GyroZ = (Int16)((read_buffer[12] << 8) | read_buffer[13]);
                // Show data
                Console.Clear();
                Console.WriteLine("AccelX = " + AccelX);
                Console.WriteLine("AccelY = " + AccelY);
                Console.WriteLine("AccelZ = " + AccelZ);
                Console.WriteLine("GyroX = " + GyroX);
                Console.WriteLine("GyroY = " + GyroY);
                Console.WriteLine("GyroZ = " + GyroZ);
                System.Threading.Thread.Sleep(100);
            }
        }
    }
}
