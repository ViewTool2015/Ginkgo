  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/09 17:44
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

namespace USB_I2C_TCS3414
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            //Scan device
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
            //Initialize device
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
            //Write CONTROL register
            UInt32 cmd;
            Byte[] ControlRegister = new Byte[1];
            ControlRegister[0] = 0x03;//POWER = 1,ADC_EN = 1
            cmd = 0x80 | 0x00 | 0x00;//Select command register,Byte protocol,Register Address.
            ret = ControlI2C.VII_WriteBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xA0, cmd, ControlRegister, 8);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            //Delay
            System.Threading.Thread.Sleep(10);
            //Get ADC data
            Byte[] adc_buffer = new Byte[2];
            cmd = 0x80 | 0x20 | 0x10;//Select command register,Word protocol,Register Address.
            ret = ControlI2C.VII_ReadBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xA0, cmd, adc_buffer, 2);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Read data error!");
                return;
            }
            else
            {
                Console.WriteLine("");
                Console.WriteLine("Get Color Data:");
                Console.WriteLine("  Green ADC Data : " + String.Format("{0:D}", adc_buffer[0] | (adc_buffer[1]<<8)));
            }
            cmd = 0x80 | 0x20 | 0x12;//Select command register,Word protocol,Register Address.
            ret = ControlI2C.VII_ReadBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xA0, cmd, adc_buffer, 2);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Read data error!");
                return;
            }
            else
            {
                Console.WriteLine("    Red ADC Data : " + String.Format("{0:D}", adc_buffer[0] | (adc_buffer[1] << 8)));
            }
            cmd = 0x80 | 0x20 | 0x14;//Select command register,Word protocol,Register Address.
            ret = ControlI2C.VII_ReadBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xA0, cmd, adc_buffer, 2);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Read data error!");
                return;
            }
            else
            {
                Console.WriteLine("   Blue ADC Data : " + String.Format("{0:D}", adc_buffer[0] | (adc_buffer[1] << 8)));
            }
            cmd = 0x80 | 0x20 | 0x16;//Select command register,Word protocol,Register Address.
            ret = ControlI2C.VII_ReadBytes(ControlI2C.INIT_CONFIG.VII_USBI2C, 0, 0, 0xA0, cmd, adc_buffer, 2);
            if (ret != ControlI2C.ERROR.SUCCESS)
            {
                Console.WriteLine("Read data error!");
                return;
            }
            else
            {
                Console.WriteLine("  Clear ADC Data : " + String.Format("{0:D}", adc_buffer[0] | (adc_buffer[1] << 8)));
            }

        }
    }
}
