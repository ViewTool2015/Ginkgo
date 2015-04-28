using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Ginkgo
{
    class ControlPWM
    {
        //Initialize data type
        public struct VPI_INIT_CONFIG
        {
            public Byte PWM_ChannelMask;    //PWM Channel Mask,bit0 to channel 0，bit7 to channel 7，1-> enable
            public Byte PWM_Mode;		    //PWM Mode，0-mode 0,1-mode 1
            public Byte PWM_Pulse;		    //PWM Pulse,0 to 100
            public Byte PWM_Polarity;	    //PWM Polarity, 0-low，1-high
            public UInt32 PWM_Frequency;	//PWM Frequency，1Hz to 20000000Hz
        }
        // Define PWM channel
        public struct PWM_MASK
        {
            public const Byte VPI_PWM_CH0 = 1 << 0;     //PWM_CH0
            public const Byte VPI_PWM_CH1 = 1 << 1;     //PWM_CH1
            public const Byte VPI_PWM_CH2 = 1 << 2;     //PWM_CH2
            public const Byte VPI_PWM_CH3 = 1 << 3;     //PWM_CH3
            public const Byte VPI_PWM_CH4 = 1 << 4;     //PWM_CH4
            public const Byte VPI_PWM_CH5 = 1 << 5;     //PWM_CH5
            public const Byte VPI_PWM_CH6 = 1 << 6;     //PWM_CH6
            public const Byte VPI_PWM_CH7 = 1 << 7;     //PWM_CH7
            public const Byte VPI_PWM_ALL = 0xFF;       //PWM_ALL
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
        // Device type
        public const Int32 VPI_USBPWM = 2;          
        public static VPI_INIT_CONFIG PWM_Config;
        // Device dll
        [DllImport("Ginkgo_Driver.dll")]
        // Scan device
        public static extern int VPI_ScanDevice(Byte NeedInit = 1);
        [DllImport("Ginkgo_Driver.dll")]
        //Open device
        public static extern int VPI_OpenDevice(Int32 DevType, Int32 DevIndex, Int32 Reserved);
        [DllImport("Ginkgo_Driver.dll")]
        //Close device
        public static extern int VPI_CloseDevice(Int32 DevType, Int32 DevIndex);
        [DllImport("Ginkgo_Driver.dll")]
        //Initialize PWM module
        public static extern int VPI_InitPWM(Int32 DevType, Int32 DevIndex, ref VPI_INIT_CONFIG pInitConfig);
        [DllImport("Ginkgo_Driver.dll")]
        // Start PWM
        public static extern int VPI_StartPWM(Int32 DevType, Int32 DevIndex, Byte ChannelMask);
        [DllImport("Ginkgo_Driver.dll")]
        // Stop PWM
        public static extern int VPI_StopPWM(Int32 DevType, Int32 DevIndex, Byte ChannelMask);
        [DllImport("Ginkgo_Driver.dll")]
        //Set PWM Pulse(after start PWM)
        public static extern int VPI_SetPWMPulse(Int32 DevType, Int32 DevIndex, Byte ChannelMask, Byte[] pPulse);
        [DllImport("Ginkgo_Driver.dll")]
        // Set PWM period(after start PWM)
        public static extern int VPI_SetPWMPeriod(Int32 DevType, Int32 DevIndex, Byte ChannelMask, Int32[] Frequency);
    }
}
