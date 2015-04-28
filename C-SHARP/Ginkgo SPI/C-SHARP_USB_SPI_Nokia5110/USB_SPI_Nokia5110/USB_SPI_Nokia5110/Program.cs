  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:00
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

using LCD5110;

namespace USB_SPI_Nokia5110
{
    class Program
    {
        static void Main(string[] args)
        {
            ControlLCD5110.LCD_Init();
            ControlLCD5110.LCD_Clear();
            
            ControlLCD5110.LCD_WriteString(0, 1, "Hello viewtool");
            ControlLCD5110.LCD_WriteString(0, 3, "Ginkgo USB-SPI");
        }
    }
}
