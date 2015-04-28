  /*
  ******************************************************************************
  * @file     : ControlCNT.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/10 12:57
  * @brief    : ControlCNT demo
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
    class ControlCNT
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
        // Define counter pin
        public struct CNTPin
        {
            public const Byte CNT_CH0 = 1 << 0;     //CNT_CH0
            public const Byte CNT_CH1 = 1 << 1;     //CNT_CH1
            public const Byte CNT_CH2 = 1 << 2;     //CNT_CH2
            public const Byte CNT_CH3 = 1 << 3;     //CNT_CH3
            public const Byte CNT_ALL = 1 << 4;     //CNT_ALL
        }
        // Initialize configure
        public struct CNT_INIT_CONFIG
        {
            public Byte CounterMode;		//Counter Mode: 0-Up,1-Down
            public Byte CounterPolarity;	//Counter Polarity:0-Rising,1-Falling,2-BothEdge
            public Byte CounterBitWide;		//Counter Bit Wide:16-16bit,32-32bit
        }
        // Device type
        public const Int32 CNT_USBCNT = 1;
        // Device dll
        [DllImport("Ginkgo_Driver.dll")]
        // Scan Device
        public static extern int CNT_ScanDevice(Byte NeedInit = 1);
        [DllImport("Ginkgo_Driver.dll")]
        // Open device
        public static extern int CNT_OpenDevice(Int32 DevType, Int32 DevIndex, Int32 Reserved);
        [DllImport("Ginkgo_Driver.dll")]
        // Close device
        public static extern int CNT_CloseDevice(Int32 DevType, Int32 DevIndex);
        [DllImport("Ginkgo_Driver.dll")]
        // Initialize counter
        public static extern int CNT_InitCounter(Int32 DevType, Int32 DevIndex, Byte Channel, ref CNT_INIT_CONFIG pInitConfig);
        [DllImport("Ginkgo_Driver.dll")]
        // Set counter value
        public static extern int CNT_SetCounter(Int32 DevType, Int32 DevIndex, Byte Channel, UInt32[] pCounterValue);
        [DllImport("Ginkgo_Driver.dll")]
        // Get counter current value
        public static extern int CNT_GetCounter(Int32 DevType, Int32 DevIndex, Byte Channel, UInt32[] pCounterValue);
        [DllImport("Ginkgo_Driver.dll")]
        // Start counter
        public static extern int CNT_StartCounter(Int32 DevType, Int32 DevIndex, Byte Channel);
        [DllImport("Ginkgo_Driver.dll")]
        // Stop counter
        public static extern int CNT_StopCounter(Int32 DevType, Int32 DevIndex, Byte Channel);
    }
}
