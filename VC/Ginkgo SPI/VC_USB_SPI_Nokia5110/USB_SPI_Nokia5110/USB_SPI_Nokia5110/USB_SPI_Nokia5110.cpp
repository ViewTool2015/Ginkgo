  /*
  ******************************************************************************
  * @file     : USB_SPI_Nokia5110.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/17 18:15
  * @brief    : USB_SPI_Nokia5110 demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
/*
Hardware connection
 Nokia5110	Ginkgo USB-SPI
 1.RST		GPIO_10(P7)
 2.CE		SPI_SEL0(P11)
 3.DC		GPIO_11(P9)
 4.DIN		SPI_MOSI(P17)
 5.CLK		SPI_SCK(P13)
 6.VCC		5V
 7.BL		GPIO_9(P5)
 8.GND		GND
*/

#include "stdafx.h"
#include <stdint.h>
#include "LCD5110.h"


int _tmain(int argc, _TCHAR* argv[])
{
	LCD_Init();
	LCD_Clear();
	LCD_WriteString(20,1,"viewtool");
	LCD_WriteString(0,3,"Ginkgo USB-SPI");
	return 0;
}

