/*
******************************************************************************
* @file     : ControlI2C.cs
* @Copyright: ViewTool 
* @Revision : ver 1.0
* @Date     : 2015/02/09 16:07
* @brief    : ControlI2C demo
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
    class ControlI2C
    {
        // I2C data types initialization Define
        public struct VII_INIT_CONFIG
        {
            public Byte MasterMode;		//Master-slave choice: 0-slave, 1-master
            public Byte ControlMode;	//Control mode: 0-Standard slave mode, 1-Standard mode, 2-GPIO mode
            public Byte AddrType;		//7-7bit mode, 10-10bit mode
            public Byte SubAddrWidth;	//Sub-Address width: value of 0~4, 0 means no Sub-Address mode
            public UInt16 Addr;			//Device address in salve mode
            public UInt32 ClockSpeed;	//Clock frequency(HZ)
        }
        // I2C time parameter definition in GPIO mode(ms)
        public struct VII_TIME_CONFIG
        {
            public UInt16 tHD_STA;   //Timing for start signal Keeping
            public UInt16 tSU_STA;   //Timing for start signal be established
            public UInt16 tLOW;      //Timing for clock low level
            public UInt16 tHIGH;     //Timing for clock high level
            public UInt16 tSU_DAT;   //Timing for data input be established
            public UInt16 tSU_STO;   //Timing for stop signal be established
            public UInt16 tDH;       //Timing for data output Keeping
            public UInt16 tDH_DAT;   //Timing for data input Keeping
            public UInt16 tAA;       //SCL lower to SDA output and response signal
            public UInt16 tR;        //Timing for SDA and SCL rising
            public UInt16 tF;        //Timing for SDA and SCL going down
            public UInt16 tBuf;      //Free timing of the bus until the new mission
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public Byte[] tACK;
            public UInt16 tStart;
            public UInt16 tStop;
        }
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
        // The adapter type definition and data initialization definition
        public struct INIT_CONFIG
        {
            public const Int32 VII_USBI2C = 1;          //Adapter Type
            public const Byte VII_ADDR_7BIT = 7;        //7-bit address mode
            public const Byte VII_ADDR_10BIT = 10;      //10-bit address mode
            public const Byte VII_HCTL_MODE = 1;        //Standard mode
            public const Byte VII_SCTL_MODE = 2;        //GPIO mode
            public const Byte VII_MASTER = 1;           //Master
            public const Byte VII_SLAVE = 0;            //Slave
            public const Byte VII_SUB_ADDR_NONE = 0;    //No sub-address
            public const Byte VII_SUB_ADDR_1BYTE = 1;   //1Byte sub-address
            public const Byte VII_SUB_ADDR_2BYTE = 2;   //2Byte sub-address
            public const Byte VII_SUB_ADDR_3BYTE = 3;   //3Byte sub-address
            public const Byte VII_SUB_ADDR_4BYTE = 4;   //4Byte sub-address
        }
        public static int VII_USBI2C = 1;

        public static VII_INIT_CONFIG I2C_Config;
        public static VII_TIME_CONFIG I2C_TimeConfig;
        // Function
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VII_ScanDevice(Byte NeedInit = 1);// Get function from .dll 
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VII_OpenDevice(Int32 DevType, Int32 DevIndex, Int32 Reserved);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VII_CloseDevice(Int32 DevType, Int32 DevIndex);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VII_InitI2C(Int32 DevType, Int32 DevIndex, Int32 I2CIndex, ref VII_INIT_CONFIG pInitConfig);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VII_WriteBytes(Int32 DevType, Int32 DevIndex, Int32 I2CIndex, UInt16 Addr, UInt32 SubAddr, Byte[] pWriteData, UInt16 Len);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VII_ReadBytes(Int32 DevType, Int32 DevIndex, Int32 I2CIndex, UInt16 Addr, UInt32 SubAddr, Byte[] pReadData, UInt16 Len);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VII_TimeConfig(Int32 DevType, Int32 DevIndex, Int32 I2CIndex, ref VII_TIME_CONFIG pTimeConfig);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VII_SlaveWriteBytes(Int32 DevType, Int32 DevIndex, Int32 I2CIndex, Byte[] pWriteData, UInt16 Len);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VII_SlaveReadBytes(Int32 DevType, Int32 DevIndex, Int32 I2CIndex, Byte[] pWriteData, ref UInt16 Len);

    }
}
