  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/10 12:56
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

// Hardware connection
// No connect

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Timers;
using Ginkgo;

//hardware connection
//Pin3 -> 

namespace USB_CNT_Frequency
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            ControlCNT.CNT_INIT_CONFIG InitConfig;
            // Scan connected device
            ret = ControlCNT.CNT_ScanDevice(1);
            if (ret <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
            // Open device
            ret = ControlCNT.CNT_OpenDevice(ControlCNT.CNT_USBCNT, 0, 0);
            if (ret != ControlCNT.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            // Config counter
            InitConfig.CounterBitWide = 32;
            InitConfig.CounterMode = 0;
            InitConfig.CounterPolarity = 0;
            ret = ControlCNT.CNT_InitCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNTPin.CNT_CH0, ref InitConfig);
            if (ret != ControlCNT.ERROR.SUCCESS)
            {
                Console.WriteLine("Config device error!");
                return;
            }
            // Clear counter
            UInt32[] CounterValue = new UInt32[4];
            CounterValue[0] = 0;
            ret = ControlCNT.CNT_SetCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNTPin.CNT_CH0, CounterValue);
            if (ret != ControlCNT.ERROR.SUCCESS)
            {
                Console.WriteLine("Set counter value error!");
                return;
            }
            // Start counter
            ret = ControlCNT.CNT_StartCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNTPin.CNT_CH0);
            if (ret != ControlCNT.ERROR.SUCCESS)
            {
                Console.WriteLine("Start counter error!");
                return;
            }
            // Start timing with 1s
            System.Timers.Timer aTimer = new System.Timers.Timer();
            aTimer.Elapsed += new ElapsedEventHandler(TimeEvent);
            // Set interval to 1000ms
            aTimer.Interval = 1000;
            aTimer.Enabled = true;
            Console.WriteLine("Press enter key to stop program");
            Console.WriteLine("Waiting for measure frequency ......");
            Console.ReadLine();
        }
        private static void TimeEvent(object source, ElapsedEventArgs e)
        {
            int ret;
            UInt32[] CounterValue = new UInt32[4];
            // Get counter value
            ret = ControlCNT.CNT_GetCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNTPin.CNT_CH0, CounterValue);
            if (ret != ControlCNT.ERROR.SUCCESS)
            {
                Console.WriteLine("Get counter value error!");
                return;
            }
            else
            {
                Console.WriteLine("Current Frequency : " + String.Format("{0}",CounterValue[0]));
            }
            // Clear counter
            CounterValue[0] = 0;
            ret = ControlCNT.CNT_SetCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNTPin.CNT_CH0, CounterValue);
            if (ret != ControlCNT.ERROR.SUCCESS)
            {
                Console.WriteLine("Set counter value error!");
                return;
            }
        }
    }
}
