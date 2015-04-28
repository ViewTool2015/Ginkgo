/*
******************************************************************************
* @file     : Program.cs
* @Copyright: ViewTool 
* @Revision : ver 1.0
* @Date     : 2015/02/10 12:58
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


//hardware connection
// Pin 3, channel 0
// Pin 5, channel 1

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using Ginkgo;

namespace ControlDAC_Test
{
    class Program
    {
        static void Main(string[] args)
        {

	int ret;
	UInt16[] WaveDataBuff = new UInt16 [1024];
	//Scan device
	ret = ControlDAC.VDA_ScanDevice(1);
	if(ret <= 0){
		System.Console.WriteLine("No device connected!");
		return;
	}
	//Open device
	ret = ControlDAC.VDA_OpenDevice(ControlDAC.VDA_USBDAC,0,0);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Open device error!");
		return;
	}


	//Configuration DAC to output dc voltage
	WaveDataBuff[0] = 1532;//Vout = (3.3V/4095)*1532 = 1.23V
	System.Console.WriteLine("Output DC Voltage: {0}",(1532*3.3/4095));
    ret = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC, 0, ControlDAC.VDA_DAC_CH0, WaveDataBuff, 1, 0);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Config DAC error!");
		return;
	}
	//Start DAC to output
	ret = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Start DAC error!");
		return;
	}
	System.Console.WriteLine("Press Enter to continue...");
            System.Console.ReadLine();
	ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0);
    
    

	//Configuration DAC to output square wave
	for(int i=0;i<64;i++){
		WaveDataBuff[i] = 0xFFF;
	}
	for(int i=0;i<64;i++){
		WaveDataBuff[64+i] = 0x00;
	}
	//Square wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
	System.Console.WriteLine("Output Square Wave!");
	ret = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0,WaveDataBuff,128,100);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Config DAC error!");
		return;
	}
	//Start DAC to output
	ret = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Start DAC error!");
		return;
	}
	System.Console.WriteLine("Press Enter to continue...");
    System.Console.ReadLine();
    ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC, 0, ControlDAC.VDA_DAC_CH0);
    
    
	//Configuration DAC to output sawtooth wave 
	for(int i=0;i<128;i++){
        WaveDataBuff[i] = (UInt16)(i * 0xFFF / 128);
	}
	//Sawtooth wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
	System.Console.WriteLine("Output Sawtooth Wave!");
	ret = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0,WaveDataBuff,128,100);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Config DAC error!");
		return;
	}
	//Start DAC to output
	ret = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Start DAC error!");
		return;
	}
	System.Console.WriteLine("Press Enter to continue...");
    System.Console.ReadLine();
	ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0);
    
    
	//Configuration DAC to output triangle wave
    for (int i = 0; i < 64; i++)
    {
		WaveDataBuff[i] = (UInt16)(i*0xFFF/64);
	}
    for (int i = 0; i < 64; i++)
    {
        WaveDataBuff[64 + i] = (UInt16)((64 - i) * 0xFFF / 64);
	}
	//Triangle wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
	System.Console.WriteLine("Output Triangle Wave!");
	ret = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0,WaveDataBuff,128,100);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Config DAC error!");
		return;
	}
	//Start DAC to output
	ret = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Start DAC error!");
		return;
	}
	System.Console.WriteLine("Press Enter to continue...");System.Console.ReadLine();
	ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0);
    
    
	//Configuration DAC to output Sine  wave
    for (int i = 0; i < 128; i++)
    {
		WaveDataBuff[i] = (UInt16)( 0xFFF*(1+  Math.Sin(2*Math.PI*i/128))/2.0);

	}
	//Sine wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
	System.Console.WriteLine("Output Sine Wave!");
	ret = ControlDAC.VDA_InitDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0,WaveDataBuff,128,100);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Config DAC error!");
		return;
	}
	//Start DAC to output
	ret = ControlDAC.VDA_StartDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0);
	if(ret != ControlDAC.ERROR.SUCCESS){
		System.Console.WriteLine("Start DAC error!");
		return;
	}
	System.Console.WriteLine("Press Enter to continue...");
    System.Console.ReadLine();
	ControlDAC.VDA_StopDAC(ControlDAC.VDA_USBDAC,0,ControlDAC.VDA_DAC_CH0);
	//Close device
	ControlDAC.VDA_CloseDevice(ControlDAC.VDA_USBDAC,0);
	return;



        }
    }
}
