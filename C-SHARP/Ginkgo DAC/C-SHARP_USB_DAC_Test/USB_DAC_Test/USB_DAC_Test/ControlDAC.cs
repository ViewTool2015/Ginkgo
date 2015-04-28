  /*
  ******************************************************************************
  * @file     : ControlDAC.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/10 12:58
  * @brief    : ControlDAC demo
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
    class ControlDAC
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

        // Device type 
        public const Int32 VDA_USBDAC = 1;
        public const Byte VDA_DAC_CH0 = 0;
        public const Byte VDA_DAC_CH1 = 0;
        // Device dll
        // Scan device 
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VDA_ScanDevice(Byte NeedInit = 1);
        // Open device 
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VDA_OpenDevice(Int32 DevType, Int32 DevIndex, Int32 Reserved);
        // Close device
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VDA_CloseDevice(Int32 DevType, Int32 DevIndex);
        // Initialize DAC module
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VDA_InitDAC(Int32 DevType, Int32 DevIndex, Byte Channel,  UInt16[] pWaveData, UInt16 WaveDataNum, UInt16 Period);
        // Start DAC        
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VDA_StartDAC(Int32 DevType, Int32 DevIndex, Byte Channel);

        // Stop DAC
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VDA_StopDAC(Int32 DevType, Int32 DevIndex, Byte Channel);
    }  
}
