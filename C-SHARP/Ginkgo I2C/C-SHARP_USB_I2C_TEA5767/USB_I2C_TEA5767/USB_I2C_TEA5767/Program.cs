  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:49
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


namespace ControlI2C_TEA5767
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


    Byte[] write_buffer = new Byte[8];
	Byte[] read_buffe = new Byte[8];
	String cmdString ;
	int pll;
	int frequency;
	int dotNum;
    int i;

 	while(true)
    {
		Console.WriteLine("Please input the frequency of radio(The unit is MHz):");
		cmdString = Console.ReadLine();
		if(cmdString == "quit"){
			Console.WriteLine("Exit");
			break;
		}else{
			dotNum = 0;
			for(i=0;i<cmdString.Length;i++){
				if((cmdString[i]!='.')&&(cmdString[i]<'0')&&(cmdString[i]>'9')){
					Console.WriteLine("Input error!");
					return ;
				}
				if(cmdString.ToCharArray()[i]=='.'){
					dotNum++;
				}
			}
			if(dotNum>1){
				Console.WriteLine("Input error!");
				return ;
			}else if(dotNum>0){
				//frequency = atof(cmdString)*1000;
                frequency = (int)float.Parse(cmdString) * 1000;
			}else{
				//frequency = atoi(cmdString)*1000;
                frequency = int.Parse(cmdString) * 1000;
 
			}
			pll = (Int32)((float)((frequency+225))/(float)8.192);
			write_buffer[0] = (Byte)(pll>>8);
			write_buffer[1] = (Byte)pll;
			write_buffer[2] = 0x70;
			write_buffer[3] = 0x16;
			write_buffer[4] = 0x80;
			ret = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, 0xC0, 0x00, write_buffer, 5);
			if (ret != ControlI2C.ERROR.SUCCESS)
			{
				Console.WriteLine("Set frequency error:{0}", ret);
				return;
			}else{
				Console.WriteLine("Set frequency success!");
			}
		}
	}





        }
    }
}