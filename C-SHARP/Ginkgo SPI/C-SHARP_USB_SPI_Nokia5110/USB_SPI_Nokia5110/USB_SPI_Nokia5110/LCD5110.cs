﻿  /*
  ******************************************************************************
  * @file     : LCD5110.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:00
  * @brief    : LCD5110 demo
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

using Ginkgo;



namespace LCD5110
{
    class ControlLCD5110
    {
        public struct ControlPin
        {
            public const UInt16 LCD_RST_PIN = ControlGPIO.GPIO_MASK.VGI_GPIO_PIN10;
            public const UInt16 LCD_DC_PIN = ControlGPIO.GPIO_MASK.VGI_GPIO_PIN11;
            public const UInt16 LCD_BL_PIN = ControlGPIO.GPIO_MASK.VGI_GPIO_PIN9;
        }
        public static Byte su = 3;
            public static Byte[ , ] font6x8 = new Byte[ , ]
               {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },// space
                { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },// !
                { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },// "
                { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },// #
                { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },// $
                { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },// %
                { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },// &
                { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },// '
                { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },// (
                { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },// )
                { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },// *
                { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },// +
                { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },// ,
                { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },// -
                { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },// .
                { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },// /
                { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },// 0
                { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },// 1
                { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },// 2
                { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },// 3
                { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },// 4
                { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },// 5
                { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },// 6
                { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },// 7
                { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },// 8
                { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },// 9
                { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },// :
                { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },// ;
                { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },// <
                { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },// =
                { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },// >
                { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },// ?
                { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },// @
                { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },// A
                { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },// B
                { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },// C
                { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },// D
                { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },// E
                { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },// F
                { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },// G
                { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },// H
                { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },// I
                { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },// J
                { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },// K
                { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },// L
                { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },// M
                { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },// N
                { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },// O
                { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },// P
                { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },// Q
                { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },// R
                { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },// S
                { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },// T
                { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },// U
                { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },// V
                { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },// W
                { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },// X
                { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },// Y
                { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },// Z
                { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },// [
                { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },// 55
                { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },// ]
                { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },// ^
                { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },// _
                { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },// '
                { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },// a
                { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },// b
                { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },// c
                { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },// d
                { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },// e
                { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },// f
                { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },// g
                { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },// h
                { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },// i
                { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },// j
                { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },// k
                { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },// l
                { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },// m
                { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },// n
                { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },// o
                { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },// p
                { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },// q
                { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },// r
                { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },// s
                { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },// t
                { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },// u
                { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },// v
                { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },// w
                { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },// x
                { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },// y
                { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },// z
                { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }// horizon lines
                };


        //public static extern void LCD_SetBacklightState(Byte state);
        public static Int32 ConfigAdapter()
        {
        Int32 ret;
        ControlSPI.VSI_INIT_CONFIG pSPI_Config = new ControlSPI.VSI_INIT_CONFIG(); 
        // Scan device connected
        ret = ControlGPIO.VGI_ScanDevice();
        if (ret <= 0)
        {
            Console.WriteLine("No device connect!\n");
            return ret;
        }
        // Open device
        ret = ControlSPI.VSI_OpenDevice(ControlSPI.VSI_USBSPI, 0, 0);
        if (ret != ControlSPI.ERROR.SUCCESS)
        {
            Console.WriteLine("Open device error!\n");
            return ret;
        }
        // Initialize device
        pSPI_Config.ControlMode = 1;
        pSPI_Config.MasterMode = 1;
        pSPI_Config.ClockSpeed = 4500000;
        pSPI_Config.CPHA = 0;
        pSPI_Config.CPOL = 0;
        pSPI_Config.LSBFirst = 0;
        pSPI_Config.TranBits = 8;
        pSPI_Config.SelPolarity = 0;
        ret = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI, 0,ref pSPI_Config);
        if (ret != ControlSPI.ERROR.SUCCESS)
        {
            Console.WriteLine("Initialize device error!\n");
            return ret;
        }
        //Config RST,DC,BL pin
        ret = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_BL_PIN | ControlPin.LCD_DC_PIN | ControlPin.LCD_RST_PIN);
        if (ret != ControlSPI.ERROR.SUCCESS)
        {
            Console.WriteLine("Config GPIO error!\n");
            return ret;
        }
        return 0;
        }

        /*
        *Set back light state
        */
        public static void LCD_SetBacklightState(Byte state)
        {
	        if(state != 0){
		        ControlGPIO.VGI_SetPins(ControlGPIO.VGI_USBGPIO,0,ControlPin.LCD_BL_PIN);
	        }else{
		        ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO,0,ControlPin.LCD_BL_PIN);
	        }
        }

        /*
        *Reset LCD
        */
        public static void LCD_Reset()
        {
	        ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_RST_PIN);
            Thread.Sleep(10);
	        ControlGPIO.VGI_SetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_RST_PIN);
        }
        /*
        *Write 1 byte data to 5110
        */
        public static void LCD_WriteByte(Byte Data,Byte Command)
        {
	        if(Command != 0){
		        ControlGPIO.VGI_SetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_DC_PIN);
	        }else{
		        ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_DC_PIN);
	        }
            Byte[] temp = new Byte[1];
            temp[0] = Data;
	        ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, temp,1);
        }

        /*
        *Initialize 5110
        */
        public static void LCD_Init()
        {
	        ControlLCD5110.ConfigAdapter();
	        LCD_Reset();
	        LCD_SetBacklightState(1);
	        LCD_WriteByte(0x21,0);
	        LCD_WriteByte(0xC0,0);
	        LCD_WriteByte(0x06,0);
	        LCD_WriteByte(0x13,0);
	        LCD_WriteByte(0x20,0);
	        LCD_WriteByte(0x0C,0);
        }
        /*
        *Set display position
        */
        public static void LCD_SetPosition(Byte x,Byte y)
        {
            y |= 0x40; 
            x |= 0x80;
            LCD_WriteByte( y, 0);
            LCD_WriteByte( x, 0);
        }
        /*
        *Clear screen
        */
        public static void LCD_Clear()
        {
	        Byte t;
	        Byte k;
	        LCD_SetPosition(0,0);
	        for(t=0;t<6;t++){ 
		        for(k=0;k<84;k++){ 
			        LCD_WriteByte(0x00,1);	 						
		        } 
	        }
        }
        /*
        *Display a character
        */
        public static void LCD_DisplayChar(char c)
        {
	        Byte line;
            
	        c -= ' ';
	        for (line=0; line<6; line++){
		        LCD_WriteByte(ControlLCD5110.font6x8[c,line], 1);
	        }
        }
        /*
        *Display a character string
        */
        public static void LCD_WriteString(Byte x, Byte y, String str)
        {
            Console.WriteLine("Display {0} at(x={1},y={2})\n",str, x, y);
	        LCD_SetPosition(x,y);
            foreach (char i in str.ToCharArray())
            {
                LCD_DisplayChar(i);
            }
        } 

    }

}


