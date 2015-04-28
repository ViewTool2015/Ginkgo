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
            ControlI2C.I2C_Config.MasterMode = ControlI2C.INIT_CONFIG.VII_MASTER;
            ControlI2C.I2C_Config.SubAddrWidth = ControlI2C.INIT_CONFIG.VII_SUB_ADDR_1BYTE;
            ret = ControlI2C.VII_InitI2C(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, ref ControlI2C.I2C_Config);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }

            //Put the HMC5883 IC into the correct operating mode
            UInt16 Address = 0x1E;				//0011110b, I2C 7bit address of HMC5883
            Address = (UInt16)(Address << 1);
            Byte[] write_buffer = new Byte[8];
            Byte[] read_buffer = new Byte[8];
            write_buffer[0] = 0x00;
            ret = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, Address, 0x02, write_buffer, 1);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            while (true)
            {
                //Read status
                ret = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, Address, 0x09, read_buffer, 1);
                if (ret != ControlI2C.ERROR.SUCCESS)
                {
                    Console.WriteLine("Read data error!");
                    return;
                }
                //Data ready
                if ((read_buffer[0] & 0x01) > 0)
                {
                    //Tell the HMC5883 where to begin reading data
                    ret = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, Address, 0x03, read_buffer, 6);
                    if (ret != ControlI2C.ERROR.SUCCESS)
                    {
                        Console.WriteLine("Read data error!\n");
                        return;
                    }
                    int x, y, z;
                    x = (read_buffer[0] << 8) | read_buffer[1];
                    y = (read_buffer[2] << 8) | read_buffer[3];
                    z = (read_buffer[4] << 8) | read_buffer[5];
                    Console.WriteLine("------------------------------------------------------\n");
                    Console.WriteLine("x = {0:d}", x);
                    Console.WriteLine("y = {0:d}", y);
                    Console.WriteLine("z = {0:d}", z);
                    Console.WriteLine("------------------------------------------------------\n");
                }
                //Console.ReadLine();
            }
        }
    }
}