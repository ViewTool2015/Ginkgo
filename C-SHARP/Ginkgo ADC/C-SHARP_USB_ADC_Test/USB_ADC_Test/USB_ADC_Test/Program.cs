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

namespace ControlADC_Test
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            // Scan connected device
            ret = ControlADC.VAI_ScanDevice(1);
            if (ret <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
            // Open device
            ret = ControlADC.VAI_OpenDevice(ControlADC.VAI_USBADC, 0, 0);
            if (ret != ControlADC.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            // Initialize ADC_CH0 channel
            ret = ControlADC.VAI_InitADC(ControlADC.VAI_USBADC, 0, ControlADC.ADC_MASK.VAI_ADC_CH0, 0);// Cycle can be set to 0 if each channel transfer a byte of data at a time.为0
            if (ret != ControlADC.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize ADC error!");
                return;
            }
            // Get ADC_CH0 channel voltage values
            UInt16[] adc_datas = new UInt16[4096];
            ret = ControlADC.VAI_ReadDatas(ControlADC.VAI_USBADC, 0, 1, adc_datas);
            if (ret != ControlADC.ERROR.SUCCESS)
            {
                Console.WriteLine("Read ADC data error!");
                return;
            }
            else
            {
                Console.WriteLine("ADC_CH0 = " + ((adc_datas[0]*3.3)/4095).ToString("0.000"));
                Console.WriteLine("\n");
            }
            // Initialize ADC_CH0 and ADC_CH1 channel
            ret = ControlADC.VAI_InitADC(ControlADC.VAI_USBADC, 0, ControlADC.ADC_MASK.VAI_ADC_CH0 | ControlADC.ADC_MASK.VAI_ADC_CH1, 0);// Cycle can be set to 0 if each channel transfer a byte of data at a time.
            if (ret != ControlADC.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize ADC error!");
                return;
            }
            // Get ADC_CH0 and ADC_CH1 voltage values
            ret = ControlADC.VAI_ReadDatas(ControlADC.VAI_USBADC, 0, 1, adc_datas);
            if (ret != ControlADC.ERROR.SUCCESS)
            {
                Console.WriteLine("Read ADC data error!");
                return;
            }
            else
            {
                Console.WriteLine("ADC_CH0 = " + ((adc_datas[0] * 3.3) / 4095).ToString("0.000"));
                Console.WriteLine("ADC_CH1 = " + ((adc_datas[1] * 3.3) / 4095).ToString("0.000"));
                Console.WriteLine("\n");
            }
            // Initialize ADC_CH0 & ADC_CH1, each channel sampling interval is set to 1000us
            ret = ControlADC.VAI_InitADC(ControlADC.VAI_USBADC, 0, ControlADC.ADC_MASK.VAI_ADC_CH0 | ControlADC.ADC_MASK.VAI_ADC_CH1, 1000);// Cycle can be set to 0 if each channel transfer a byte of data at a time.
            if (ret != ControlADC.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize ADC error!");
                return;
            }
            // Get voltage values of ADC_CH0 & ADC_CH1
            ret = ControlADC.VAI_ReadDatas(ControlADC.VAI_USBADC, 0, 10, adc_datas);//Get 10 values each channel, return a total of 20 values
            if (ret != ControlADC.ERROR.SUCCESS)
            {
                Console.WriteLine("Read ADC data error!");
                return;
            }
            else
            {
                for (int i = 0; i < 10; i++)
                {
                    Console.WriteLine("ADC_CH0["+i+"] = " + ((adc_datas[i*2] * 3.3) / 4095).ToString("0.000"));
                }
                Console.WriteLine("\n");
                for (int i = 0; i < 10; i++)
                {
                    Console.WriteLine("ADC_CH1[" + i + "] = " + ((adc_datas[i * 2+1] * 3.3) / 4095).ToString("0.000"));
                }
            }
            // Close device
            ret = ControlADC.VAI_CloseDevice(ControlADC.VAI_USBADC, 0);
            if (ret != ControlADC.ERROR.SUCCESS)
            {
                Console.WriteLine("Close device error!");
                return;
            }
            Console.ReadLine();

        }
    }
}

/*
 * 
 * Channel 0 <-> 3.3v 
 * Channel 1 <-> 0v
 * 

ADC_CH0 = 3.297

ADC_CH0 = 3.293
ADC_CH1 = 0.000

ADC_CH0[0] = 3.292
ADC_CH0[1] = 3.295
ADC_CH0[2] = 3.293
ADC_CH0[3] = 3.297
ADC_CH0[4] = 3.296
ADC_CH0[5] = 3.293
ADC_CH0[6] = 3.294
ADC_CH0[7] = 3.292
ADC_CH0[8] = 3.295
ADC_CH0[9] = 3.294

ADC_CH1[0] = 0.000
ADC_CH1[1] = 0.000
ADC_CH1[2] = 0.000
ADC_CH1[3] = 0.000
ADC_CH1[4] = 0.000
ADC_CH1[5] = 0.000
ADC_CH1[6] = 0.000
ADC_CH1[7] = 0.000
ADC_CH1[8] = 0.000
ADC_CH1[9] = 0.000

*/