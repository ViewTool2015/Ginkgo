using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
namespace UART_Test
{
    class ControlUART
    {
        //Initialize configure
        public struct UART_INIT_CONFIG
        {
            public Byte StopBits;		// Stop Bit，0-1bit,1-1.5bit,2-2bit,3-1.5bit
            public Byte WordLength;		// 8
            public Byte Parity;			// 0-No,4-Even,6-Odd
            public UInt16 RS485Mode;	// 485-RS485 mode，232-RS232 mode
            public UInt32 BaudRate;		// BaudRate
        }
        // Adapter data type
        public struct UART_BOARD_INFO
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public Byte[] ProductName;      // Product name, e.g."Ginkgo-I2C-Adaptor"
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public Byte[] FirmwareVersion;  // Firmware version
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public Byte[] HardwareVersion;  // Hardware version
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 12)]
            public Byte[] SerialNumber;     // The adapter serial number
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
        // Device dll
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int UART_ScanDevice(Byte NeedInit = 1);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int UART_OpenDevice(Int32 DevIndex, Int32 Reserved);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int UART_CloseDevice(Int32 DevIndex);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int UART_ReadBoardInfo(Int32 DevIndex, ref UART_BOARD_INFO pInfo);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int UART_InitDevice(Int32 DevType, Int32 DevIndex, Int32 UARTIndex, ref UART_INIT_CONFIG pInitConfig);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int UART_WriteBytes(Int32 DevType, Int32 DevIndex, Int32 UARTIndex, Byte[] pWriteData, UInt16 Len);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int UART_ReadBytes(Int32 DevType, Int32 DevIndex, Int32 UARTIndex, Byte[] pReadData, ref UInt16 pLen);

    }
}
