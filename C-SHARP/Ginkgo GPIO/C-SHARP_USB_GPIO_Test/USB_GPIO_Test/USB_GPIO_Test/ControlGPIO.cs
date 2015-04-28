using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Ginkgo
{
    class ControlGPIO
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
        //Define GPIO
        public struct GPIO_MASK
        {
            public const UInt16 VGI_GPIO_PIN0 = 1 << 0;     //GPIO_0
            public const UInt16 VGI_GPIO_PIN1 = 1 << 1;     //GPIO_1
            public const UInt16 VGI_GPIO_PIN2 = 1 << 2;     //GPIO_2
            public const UInt16 VGI_GPIO_PIN3 = 1 << 3;     //GPIO_3
            public const UInt16 VGI_GPIO_PIN4 = 1 << 4;     //GPIO_4
            public const UInt16 VGI_GPIO_PIN5 = 1 << 5;     //GPIO_5
            public const UInt16 VGI_GPIO_PIN6 = 1 << 6;     //GPIO_6
            public const UInt16 VGI_GPIO_PIN7 = 1 << 7;     //GPIO_7
            public const UInt16 VGI_GPIO_PIN8 = 1 << 8;     //GPIO_8
            public const UInt16 VGI_GPIO_PIN9 = 1 << 9;     //GPIO_9
            public const UInt16 VGI_GPIO_PIN10 = 1 << 10;   //GPIO_10
            public const UInt16 VGI_GPIO_PIN11 = 1 << 11;   //GPIO_11
            public const UInt16 VGI_GPIO_PIN12 = 1 << 12;   //GPIO_12
            public const UInt16 VGI_GPIO_PIN13 = 1 << 13;   //GPIO_13
            public const UInt16 VGI_GPIO_PIN14 = 1 << 14;   //GPIO_14
            public const UInt16 VGI_GPIO_PIN15 = 1 << 15;   //GPIO_15
            public const UInt16 VGI_GPIO_PIN_ALL = 0xFFFF;  //GPIO_ALL
        }
        // Device type
        public const Int32 VGI_USBGPIO = 1;
        // Device dll
        [DllImport("Ginkgo_Driver.dll")]
        // Scan device
        public static extern int VGI_ScanDevice(Byte NeedInit = 1);
        [DllImport("Ginkgo_Driver.dll")]
        // Open device
        public static extern int VGI_OpenDevice(Int32 DevType, Int32 DevIndex, Int32 Reserved);
        [DllImport("Ginkgo_Driver.dll")]
        // Close device
        public static extern int VGI_CloseDevice(Int32 DevType, Int32 DevIndex);
        [DllImport("Ginkgo_Driver.dll")]
        // Set specified pin to input
        public static extern int VGI_SetInput(Int32 DevType, Int32 DevIndex, UInt16 Pins);
        [DllImport("Ginkgo_Driver.dll")]
        // Set specified pin to output
        public static extern int VGI_SetOutput(Int32 DevType, Int32 DevIndex, UInt16 Pins);
        [DllImport("Ginkgo_Driver.dll")]
        // Set specified pin to output(Bi-directional, need pull-up resistor)
        public static extern int VGI_SetOpenDrain(Int32 DevType, Int32 DevIndex, UInt16 Pins);
        [DllImport("Ginkgo_Driver.dll")]
        // Set specified pin to high level
        public static extern int VGI_SetPins(Int32 DevType, Int32 DevIndex, UInt16 Pins);
        [DllImport("Ginkgo_Driver.dll")]
        // Set specified pin to low level
        public static extern int VGI_ResetPins(Int32 DevType, Int32 DevIndex, UInt16 Pins);
        [DllImport("Ginkgo_Driver.dll")]
        // Get specified pin value
        public static extern int VGI_ReadDatas(Int32 DevType, Int32 DevIndex, UInt16 PinMask, ref UInt16 pData);
    }
}
