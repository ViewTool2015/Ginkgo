  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 10:44
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

namespace ControlGPIO_Test
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            // Scan connected device
            ret = ControlGPIO.VGI_ScanDevice(1);
            if (ret <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
            // Open device
            ret = ControlGPIO.VGI_OpenDevice(ControlGPIO.VGI_USBGPIO, 0, 0);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            // Set GPIO_7 and GPIO_8 to output
            ret = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.GPIO_MASK.VGI_GPIO_PIN7 | ControlGPIO.GPIO_MASK.VGI_GPIO_PIN8);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Set pin output error!");
                return;
            }
            // Set GPIO_7 and GPIO_8 to high level
            ret = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.GPIO_MASK.VGI_GPIO_PIN7 | ControlGPIO.GPIO_MASK.VGI_GPIO_PIN8);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Set pin high error!");
                return;
            }
            // Set GPIO_7 and GPIO_8 to low level
            ret = ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.GPIO_MASK.VGI_GPIO_PIN7 | ControlGPIO.GPIO_MASK.VGI_GPIO_PIN8);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Set pin low error!");
                return;
            }
            // Set GPIO_4 and GPIO_5 to input
            ret = ControlGPIO.VGI_SetInput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.GPIO_MASK.VGI_GPIO_PIN4 | ControlGPIO.GPIO_MASK.VGI_GPIO_PIN5);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Set pin input error!");
                return;
            }
            // Get GPIO_4 and GPIO_5 value
            UInt16 pin_value = 0;
            ret = ControlGPIO.VGI_ReadDatas(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.GPIO_MASK.VGI_GPIO_PIN4 | ControlGPIO.GPIO_MASK.VGI_GPIO_PIN5, ref pin_value);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Get pin data error!");
                return;
            }
            else
            {
                if ((pin_value & ControlGPIO.GPIO_MASK.VGI_GPIO_PIN4) != 0)
                {
                    Console.WriteLine("GPIO_4 is high-level!");
                }
                else
                {
                    Console.WriteLine("GPIO_4 is low-level!");
                }
                if ((pin_value & ControlGPIO.GPIO_MASK.VGI_GPIO_PIN5) != 0)
                {
                    Console.WriteLine("GPIO_5 is high-level!");
                }
                else
                {
                    Console.WriteLine("GPIO_5 is low-level!");
                }
            }
            // Set GPIO_4 and GPIO_5 to OD(Bi-directional, need pull-up resistor)
            ret = ControlGPIO.VGI_SetOpenDrain(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.GPIO_MASK.VGI_GPIO_PIN4 | ControlGPIO.GPIO_MASK.VGI_GPIO_PIN5);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Set pin input error!");
                return;
            }
            // Set GPIO_4 and GPIO_5 to high level 
            ret = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.GPIO_MASK.VGI_GPIO_PIN4 | ControlGPIO.GPIO_MASK.VGI_GPIO_PIN5);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Set pin high error!");
                return;
            }
            // Set GPIO_4 and GPIO_5 to low level
            ret = ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.GPIO_MASK.VGI_GPIO_PIN4 | ControlGPIO.GPIO_MASK.VGI_GPIO_PIN5);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Set pin high error!");
                return;
            }
            // Get GPIO_4 and GPIO_5 value
            ret = ControlGPIO.VGI_ReadDatas(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.GPIO_MASK.VGI_GPIO_PIN4 | ControlGPIO.GPIO_MASK.VGI_GPIO_PIN5, ref pin_value);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Get pin data error!");
                return;
            }
            else
            {
                if ((pin_value & ControlGPIO.GPIO_MASK.VGI_GPIO_PIN4) != 0)
                {
                    Console.WriteLine("GPIO_4 is high-level!");
                }
                else
                {
                    Console.WriteLine("GPIO_4 is low-level!");
                }
                if ((pin_value & ControlGPIO.GPIO_MASK.VGI_GPIO_PIN5) != 0)
                {
                    Console.WriteLine("GPIO_5 is high-level!");
                }
                else
                {
                    Console.WriteLine("GPIO_5 is low-level!");
                }
            }
            // Close device
            ret = ControlGPIO.VGI_CloseDevice(ControlGPIO.VGI_USBGPIO, 0);
            if (ret != ControlGPIO.ERROR.SUCCESS)
            {
                Console.WriteLine("Close device error!");
                return;
            }
        }
    }
}
