  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/09 17:09
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


  /*
  Hardware Connection  (This is for your reference only)
  AT24C02        Ginkgo USB-I2C Adapter
  1.A0      <-->  GND(Pin19/Pin20)
  2.A1	    <-->  GND(Pin19/Pin20)
  3.A2      <-->  GND(Pin19/Pin20)
  4.GND     <-->  GND(Pin19/Pin20)
  5.SDA	    <-->  SI2C_SDA0(Pin6)
  6.SCL	    <-->  SI2C_SCL0 (Pin4)
  7.WP	    <-->  GND(Pin19/Pin20)
  8.VCC	    <-->  VCC(Pin1/Pin2)
  */

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
            // Open Device
            ret = ControlI2C.VII_OpenDevice(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            // Initialize device 
            ControlI2C.I2C_Config.AddrType = ControlI2C.INIT_CONFIG.VII_ADDR_7BIT;
            ControlI2C.I2C_Config.ClockSpeed = 400000;
            ControlI2C.I2C_Config.ControlMode = ControlI2C.INIT_CONFIG.VII_SCTL_MODE;
            ControlI2C.I2C_Config.MasterMode = ControlI2C.INIT_CONFIG.VII_MASTER;
            ControlI2C.I2C_Config.SubAddrWidth = ControlI2C.INIT_CONFIG.VII_SUB_ADDR_1BYTE;
            ret = ControlI2C.VII_InitI2C(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, ref ControlI2C.I2C_Config);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            //Config timing,Unit of time for microseconds
            ControlI2C.I2C_TimeConfig.tSU_STA = 5;
            ControlI2C.I2C_TimeConfig.tHD_STA = 4;
            ControlI2C.I2C_TimeConfig.tLOW = 5;    
            ControlI2C.I2C_TimeConfig.tHIGH = 5;
            ControlI2C.I2C_TimeConfig.tSU_DAT = 1;
            ControlI2C.I2C_TimeConfig.tSU_STO = 4;
            ControlI2C.I2C_TimeConfig.tBuf = 5; 
            ret = ControlI2C.VII_TimeConfig(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, ref ControlI2C.I2C_TimeConfig);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Set time error!");
                return;
            }
            // Write data
            Byte[] write_buffer = new Byte[8];
            for (int i = 0; i < 8; i++)
            {
                write_buffer[i] = (Byte)i;
            }
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xA0, 0x00, write_buffer, 8);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            //Delay to ensure write complete
            System.Threading.Thread.Sleep(10);
            // Read data
            Byte[] read_buffer = new Byte[8];
            ret = ControlI2C.VII_ReadBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xA0, 0x00, read_buffer, 8);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Read data error!");
                return;
            }
            else
            {
                Int32 read_data;
                String read_data_str = "";
                Console.WriteLine("Get Data:");
                for (int i = 0; i < 8; i++)
                {
                    read_data = read_buffer[i];
                    read_data_str += read_data.ToString("X2") + " ";
                }
                Console.WriteLine(read_data_str);
            }
            Console.ReadLine();
        }
    }
}
