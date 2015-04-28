using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Ginkgo
{
    class ControlSPI
    {
        //Initialize configure
        public struct VSI_INIT_CONFIG
        {
            public Byte ControlMode;	    
            // SPI control mode: 0->hardware control(full duplex) 1->hardware control(half duplex) 2->software control(half duplex) 3-> one wire mode
            public Byte TranBits;		// Width  of data (between 8 and 16)
            public Byte MasterMode;		// Master mode : 0 -> slave mode 1 -> master mode
            public Byte CPOL;			// Clock Polarity: 0 -> SCK active-high  1->SCK active-low 
            public Byte CPHA;			// Clock Phase: 0 -> sample on the leading (first) clock edge 1-> sample on the trailing (second) clock edge
            public Byte LSBFirst;		// whether or not LSB first: 0->MSB first 1-> LSB first
            public Byte SelPolarity;	// Chip select Polarity: 0-> low to select 1-> high to select
            public UInt32 ClockSpeed;	// SPI clock frequency
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
        //Device type
        public const Int32 VSI_USBSPI = 2;
        // Device dll
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_ScanDevice(Byte NeedInit = 1);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_OpenDevice(Int32 DevType, Int32 DevIndex, Int32 Reserved);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_CloseDevice(Int32 DevType, Int32 DevIndex);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_InitSPI(Int32 DevType, Int32 DevIndex, ref VSI_INIT_CONFIG pInitConfig);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_WriteBytes(Int32 DevType, Int32 DevIndex, Int32 SPIIndex, Byte[] pData, UInt16 Len);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_ReadBytes(Int32 DevType, Int32 DevIndex, Int32 SPIIndex, Byte[] pData, UInt16 Len);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_WriteReadBytes(Int32 DevType, Int32 DevIndex, Int32 SPIIndex, Byte[] pWriteData, UInt16 WriteLen, Byte[] pReadData, UInt16 ReadLen);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_WriteBits(Int32 DevType, Int32 DevIndex, Int32 SPIIndex, StringBuilder pWriteBitStr);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_ReadBits(Int32 DevType, Int32 DevIndex, Int32 SPIIndex, StringBuilder pReadBitStr, Int32 ReadBitsNum);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_WriteReadBits(Int32 DevType, Int32 DevIndex, Int32 SPIIndex, StringBuilder pWriteBitStr, StringBuilder pReadBitStr, Int32 ReadBitsNum);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_SlaveReadBytes(Int32 DevType, Int32 DevIndex, Byte[] pReadData, ref Int32 pBytesNum, Int32 WaitTime);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_SlaveWriteBytes(Int32 DevType, Int32 DevIndex, Byte[] pWriteData, Int32 WriteBytesNum);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_BlockWriteBytes(Int32 DevType, Int32 DevIndex, Int32 SPIIndex, Byte[] pWriteData, UInt16 BlockSize, UInt16 BlockNum, UInt32 IntervalTime);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_BlockReadBytes(Int32 DevType, Int32 DevIndex, Int32 SPIIndex, Byte[] pReadData, UInt16 BlockSize, UInt16 BlockNum, UInt32 IntervalTime);
        [DllImport("Ginkgo_Driver.dll")]
        public static extern int VSI_BlockWriteReadBytes(Int32 DevType, Int32 DevIndex, Int32 SPIIndex, Byte[] pWriteData, UInt16 WriteBlockSize, Byte[] pReadData, UInt16 ReadBlockSize, UInt16 BlockNum, UInt32 IntervalTime);

    }
}
