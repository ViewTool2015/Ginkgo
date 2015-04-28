  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/09 17:31
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
  Hardware Connection  (This is for your reference only)
  BH1750FVI      Ginkgo USB-I2C Adapter
  1.VCC	    <-->  VCC(Pin2)
  2.ADDR    <-->  GND(Pin19/Pin20)
  3.GND     <-->  GND(Pin19/Pin20)
  4.SDA	    <-->  HI2C_SDA0(Pin8)
  5.DVI     <-->  VCC(Pin2)
  6.SCL	    <-->  HI2C_SCL0 (Pin6)
  */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using Ginkgo;



namespace ControlI2C_BH1750FVI
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
            // Send wait for measuring instructions
            Byte[] write_buffer = new Byte[8];
            write_buffer[0] = 0x01;
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0x46, 0x00, write_buffer, 1);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            // Start measurement in 11x resolution ratio
            write_buffer[0] = 0x10;
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0x46, 0x00, write_buffer, 1);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            // Loop measurement
            while (true)
            {
                // Waiting for measurement complete
                System.Threading.Thread.Sleep(1000);
                // Get measurement data
                Byte[] read_buffer = new Byte[8];
                ret = ControlI2C.VII_ReadBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0x46, 0x00, read_buffer, 2);
                if (ret != ControlI2C.ERROR.SUCCESS)
                {
                    Console.WriteLine("Read data error!");
                    return;
                }
                else
                {
                    double illuminance = ((read_buffer[0] << 8) | read_buffer[1]) / 1.2;
                    Console.Clear();
                    Console.WriteLine("illuminance:" + illuminance.ToString("0.00") + " lx");
                }
            }
        }
    }
}
