  /*
  ******************************************************************************
  * @file     : ControlADC.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/10 12:58
  * @brief    : ControlADC demo
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

namespace Ginkgo
{
    class ControlADC
    {
        // General Error Code
        public struct ERROR
        {
            public const Int32 SUCCESS = 0;	                // no Error
            public const Int32 PARAMETER_NULL = -1;	        // void pointer 
            public const Int32 INPUT_DATA_TOO_MUCH = -2;	// too many parameters
            public const Int32 INPUT_DATA_TOO_LESS = -3;	// too few parameters
            public const Int32 INPUT_DATA_ILLEGALITY = -4;	// illegal parameter
            public const Int32 USB_WRITE_DATA_ERROR = -5;	// USB write data error
            public const Int32 USB_READ_DATA_ERROR = -6;	// USB read data error
            public const Int32 READ_NO_DATA = -7;	        // no data return when request data
            public const Int32 OPEN_DEVICE_FAILD = -8;	    // failed to open device
            public const Int32 CLOSE_DEVICE_FAILD = -9;	    // failed to close device
            public const Int32 EXECUTE_CMD_FAILD = -10;	    // the command failed to execute
            public const Int32 SELECT_DEVICE_FAILD = -11;	// failed to select device
            public const Int32 DEVICE_OPENED = -12;       	// device has open
            public const Int32 DEVICE_NOTOPEN = -13;	    // device not open
            public const Int32 BUFFER_OVERFLOW = -14;	    // buffer overflow
            public const Int32 DEVICE_NOTEXIST = -15;	    // device not exist
            public const Int32 LOAD_KERNELDLL = -16;	    // failed to load KernelDLL
            public const Int32 CMD_FAILED = -17;	        // failed to execute command
            public const Int32 BUFFER_CREATE = -18;	        // out of memory

        }

        // Define ADC channel
        public struct ADC_MASK
        {
            public const Byte VAI_ADC_CH0 = 1 << 0;     //ADC_CH0
            public const Byte VAI_ADC_CH1 = 1 << 1;     //ADC_CH1
            public const Byte VAI_ADC_CH2 = 1 << 2;     //ADC_CH2
            public const Byte VAI_ADC_CH3 = 1 << 3;     //ADC_CH3
            public const Byte VAI_ADC_CH4 = 1 << 4;     //ADC_CH4
            public const Byte VAI_ADC_CH5 = 1 << 5;     //ADC_CH5
            public const Byte VAI_ADC_CH6 = 1 << 6;     //ADC_CH6
            public const Byte VAI_ADC_CH7 = 1 << 7;     //ADC_CH7
            public const Byte VAI_ADC_ALL = 0XFF;       //ADC_ALL
        }
        // Device type 
        public const Int32 VAI_USBADC = 1;
        // Device dll
        [DllImport("Ginkgo_Driver.dll")]
        // Scan device 
        public static extern int VAI_ScanDevice(Byte NeedInit = 1);
        [DllImport("Ginkgo_Driver.dll")]
        // Open device 
        public static extern int VAI_OpenDevice(Int32 DevType, Int32 DevIndex, Int32 Reserved);
        [DllImport("Ginkgo_Driver.dll")]
        // Close device
        public static extern int VAI_CloseDevice(Int32 DevType, Int32 DevIndex);
        [DllImport("Ginkgo_Driver.dll")]
        // Initialize ADC module
        public static extern int VAI_InitADC(Int32 DevType, Int32 DevIndex, Byte Channel, UInt16 Period);
        [DllImport("Ginkgo_Driver.dll")]
        // Get ADC value
        public static extern int VAI_ReadDatas(Int32 DevType, Int32 DevIndex, UInt16 DataNum, UInt16[] pData);
    }  
}
