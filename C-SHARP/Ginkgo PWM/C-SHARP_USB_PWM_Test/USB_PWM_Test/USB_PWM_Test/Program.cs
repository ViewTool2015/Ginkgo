  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 10:45
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



namespace ControlPWM_Test
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            // Scan connected device
            ret = ControlPWM.VPI_ScanDevice(1);
            if (ret <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
            // Open device
            ret = ControlPWM.VPI_OpenDevice(ControlPWM.VPI_USBPWM, 0, 0);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            // Initialize configure
            ControlPWM.PWM_Config.PWM_ChannelMask = ControlPWM.PWM_MASK.VPI_PWM_CH0;
            ControlPWM.PWM_Config.PWM_Frequency = 10000;
            ControlPWM.PWM_Config.PWM_Mode = 0;
            ControlPWM.PWM_Config.PWM_Polarity = 0;
            ControlPWM.PWM_Config.PWM_Pulse = 50;
            ret = ControlPWM.VPI_InitPWM(ControlPWM.VPI_USBPWM,0,ref ControlPWM.PWM_Config);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            // Start PWM_CH0 channel
            ret = ControlPWM.VPI_StartPWM(ControlPWM.VPI_USBPWM,0,ControlPWM.PWM_MASK.VPI_PWM_CH0);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Start pwm error!");
                return;
            }
            // Run 1s
            System.Threading.Thread.Sleep(1000);
            // Stop PWM_CH0 channel 
            ret = ControlPWM.VPI_StopPWM(ControlPWM.VPI_USBPWM, 0, ControlPWM.PWM_MASK.VPI_PWM_CH0);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Stop PWM error!");
                return;
            }
            // Initialize all PWM channel 
            ControlPWM.PWM_Config.PWM_ChannelMask = ControlPWM.PWM_MASK.VPI_PWM_ALL;
            ControlPWM.PWM_Config.PWM_Frequency = 100000;
            ControlPWM.PWM_Config.PWM_Mode = 0;
            ControlPWM.PWM_Config.PWM_Polarity = 0;
            ControlPWM.PWM_Config.PWM_Pulse = 50;
            ret = ControlPWM.VPI_InitPWM(ControlPWM.VPI_USBPWM, 0, ref ControlPWM.PWM_Config);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            // Start PWM_CH0 and PWM_CH2 channel
            ret = ControlPWM.VPI_StartPWM(ControlPWM.VPI_USBPWM, 0, ControlPWM.PWM_MASK.VPI_PWM_CH0 | ControlPWM.PWM_MASK.VPI_PWM_CH2);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Start pwm error!");
                return;
            }
            // Run 1S
            System.Threading.Thread.Sleep(1000);
            // Adjust PWM_CH0 frequency
            Int32[] Frequency = new Int32[8];
            Frequency[0] = 50000;
            ret = ControlPWM.VPI_SetPWMPeriod(ControlPWM.VPI_USBPWM, 0, ControlPWM.PWM_MASK.VPI_PWM_CH0, Frequency);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Set frequency error!");
                return;
            }
            // Adjust PWM_CH2 Pulse
            Byte[] Pulse = new Byte[8];
            Pulse[2] = 80;
            ret = ControlPWM.VPI_SetPWMPulse(ControlPWM.VPI_USBPWM, 0, ControlPWM.PWM_MASK.VPI_PWM_CH2, Pulse);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Set pulse error!");
                return;
            }
            // Run 1s
            System.Threading.Thread.Sleep(1000);

            // Adjust PWM_CH0 and PWM_CH2 pulse
            Pulse[0] = 20;
            Pulse[2] = 80;
            ret = ControlPWM.VPI_SetPWMPulse(ControlPWM.VPI_USBPWM, 0, ControlPWM.PWM_MASK.VPI_PWM_CH0|ControlPWM.PWM_MASK.VPI_PWM_CH2, Pulse);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Set pulse error!");
                return;
            }
            // Run 1s
            System.Threading.Thread.Sleep(1000);
            // Attention: PWM_CH(2n) and PWM_CH(2n+1) cannot separate set
            // Each channel duty ratio can separate set
            // Close  device
            ret = ControlPWM.VPI_CloseDevice(ControlPWM.VPI_USBPWM, 0);
            if (ret != ControlPWM.ERROR.SUCCESS)
            {
                Console.WriteLine("Close device error!");
                return;
            }
        }
    }
}
