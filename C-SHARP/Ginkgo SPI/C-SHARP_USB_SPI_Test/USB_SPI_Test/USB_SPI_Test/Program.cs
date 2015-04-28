  /*
  ******************************************************************************
  * @file     : Program.cs
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/02/13 11:26
  * @brief    : Program demo
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
using Ginkgo;

namespace ControlSPI_Test
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret;
            ControlSPI.VSI_INIT_CONFIG pSPI_Config = new ControlSPI.VSI_INIT_CONFIG();
            //Scan connected device
            ret = ControlSPI.VSI_ScanDevice(1);
            if (ret <= 0)
            {
                Console.WriteLine("No device connect!");
                return;
            }
            // Open device
            ret = ControlSPI.VSI_OpenDevice(ControlSPI.VSI_USBSPI, 0, 0);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Open device error!");
                return;
            }
            // Initialize device(Master Mode, Hardware SPI, Half-Duplex)
            // Function VSI_WriteBytes，VSI_ReadBytes，VSI_WriteReadBytes，
            //    VSI_BlockWriteBytes，VSI_BlockReadBytes，VSI_BlockWriteReadBytes can be support in this mode

            pSPI_Config.ControlMode = 1;
            pSPI_Config.MasterMode = 1;
            pSPI_Config.ClockSpeed = 36000000;
            pSPI_Config.CPHA = 0;
            pSPI_Config.CPOL = 0;
            pSPI_Config.LSBFirst = 0;
            pSPI_Config.TranBits = 8;
            ret = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI, 0, ref pSPI_Config);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            Byte[] write_buffer = new Byte[10240];
            Byte[] read_buffer = new Byte[10240];
            // Write data to SPI bus
            for (int i = 0; i < 512; i++)
            {
                write_buffer[i] = (Byte)i;
            }
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 512);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            // Read data from SPI bus
            ret = ControlSPI.VSI_ReadBytes(ControlSPI.VSI_USBSPI, 0, 0, read_buffer, 512);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Read data error!");
                return;
            }
            Console.WriteLine("Read Data:");
            for (int i = 0; i < 512; i++)
            {
                Console.Write(read_buffer[i].ToString("X2")+" ");
            }
            // Write data and then read data(clock separated ), CS is still enable
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer,256,read_buffer, 512);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write&Read data error!");
                return;
            }
            Console.WriteLine("Write&Read Data:");
            for (int i = 0; i < 512; i++)
            {
                Console.Write(read_buffer[i].ToString("X2") + " ");
            }

            //SPI master, Half-Duplex, Block mode, Write data
            UInt16 BlockSize = 4;
            UInt16 BlockNum = 5;
            UInt32 IntervalTime = 10;//in us
            Byte data = 0;
            for (int blockNumIndex = 0; blockNumIndex < BlockNum; blockNumIndex++)
            {
                for (int blockSizeIndex = 0; blockSizeIndex < BlockSize; blockSizeIndex++)
                {
                    write_buffer[blockNumIndex * BlockSize + blockSizeIndex] = data;
                    data++;
                }
            }

            // CS will be enable BlockNum times and write BlockSize bits data after call VSI_BlockWriteBytes(),
            // CS signal interval is IntervalTime(in us)
            ret = ControlSPI.VSI_BlockWriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, BlockSize, BlockNum, IntervalTime);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Block Write data error!");
                return;
            }
            //SPI master, Half-Duplex, Block mode, Read data
            for (int blockNumIndex = 0; blockNumIndex < BlockNum; blockNumIndex++)
            {
                for (int blockSizeIndex = 0; blockSizeIndex < BlockSize; blockSizeIndex++)
                {
                    write_buffer[blockNumIndex * BlockSize + blockSizeIndex] = data;
                    data++;
                }
            }
            // CS will be enable BlockNum times and write BlockSize bits data after call VSI_BlockWriteBytes(),
            // CS signal interval is IntervalTime(in us)
             ret = ControlSPI.VSI_BlockReadBytes(ControlSPI.VSI_USBSPI, 0, 0, read_buffer, BlockSize, BlockNum, IntervalTime);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Block Read data error!");
                return;
            }
            //SPI master, Half-Duplex, Block mode, Write&Read data
            for (int blockNumIndex = 0; blockNumIndex < BlockNum; blockNumIndex++)
            {
                for (int blockSizeIndex = 0; blockSizeIndex < BlockSize; blockSizeIndex++)
                {
                    write_buffer[blockNumIndex * BlockSize + blockSizeIndex] = data;
                    data++;
                }
            }
            // CS will be enable BlockNum times and write BlockSize bits data after call VSI_BlockWriteBytes(),
            // CS signal interval is IntervalTime(in us)
      
            UInt16 WriteBlockSize = 2;
            UInt16 ReadBlockSize = 4;
            ret = ControlSPI.VSI_BlockWriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, WriteBlockSize, read_buffer, ReadBlockSize, BlockNum, IntervalTime);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Block Write&Read data error!");
                return;
            }
 
            // Initialize device(Master Mode, Software SPI, Half-Duplex)
            // function VSI_WriteBytes，VSI_ReadBytes，VSI_WriteReadBytes can be support in software SPI mode
            // Hardware SPI cannot support function VSI_WriteBits,VSI_ReadBits,VSI_WriteReadBits, but can be used in 1-wired mode 
            pSPI_Config.ControlMode = 2;
            pSPI_Config.MasterMode = 1;
            pSPI_Config.ClockSpeed = 100000;
            pSPI_Config.CPHA = 0;
            pSPI_Config.CPOL = 0;
            pSPI_Config.LSBFirst = 0;
            pSPI_Config.TranBits = 8;
            ret = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI, 0, ref pSPI_Config);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            // Write data with binary, up to 10240 bits
            StringBuilder write_buffer_bin;
            StringBuilder read_buffer_bin;
            write_buffer_bin = new StringBuilder("10110100100101");
            ret = ControlSPI.VSI_WriteBits(ControlSPI.VSI_USBSPI, 0, 0, write_buffer_bin);//send 14bit data
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write bit error!");
                return;
            }
            // Read data with binary, up to 10240 bits
            read_buffer_bin = new StringBuilder(10240);
            ret = ControlSPI.VSI_ReadBits(ControlSPI.VSI_USBSPI, 0, 0, read_buffer_bin,19);//read 19bit data
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Read bit error!");
                return;
            }
            else
            {
                Console.WriteLine("Read bits:");
                Console.WriteLine(read_buffer_bin);
            }
            // Read and write data with binary
            write_buffer_bin = new StringBuilder("000011110101001");
            ret = ControlSPI.VSI_WriteReadBits(ControlSPI.VSI_USBSPI, 0, 0, write_buffer_bin,read_buffer_bin, 25);//read 19bit data
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write bit error!");
                return;
            }
            else
            {
                Console.WriteLine("Read bits:");
                Console.WriteLine(read_buffer_bin);
            }
            // Initialize device(Slave Mode, Hardware SPI, Full-Duplex)
            pSPI_Config.ControlMode = 0; // Hardware SPI, Full-Duplex
            pSPI_Config.MasterMode = 0;  // Slave Mode
            pSPI_Config.CPHA = 0;        // Clock Polarity and Phase must be same to master
            pSPI_Config.CPOL = 0;
            pSPI_Config.LSBFirst = 0;
            pSPI_Config.TranBits = 8;    // Support 8bit mode only
            ret = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI, 0, ref pSPI_Config);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Initialize device error!");
                return;
            }
            // Write data in slave mode(call function VSI_SlaveWriteBytes), data will not send out via MISO pin immediately until chip select by master,
            //  function VSI_SlaveWriteBytes return immediately after called, the data stored in adapter memory buffer
            for (int i = 0; i < 8; i++)
            {
                write_buffer[i] = (Byte)i;
            }
            ret = ControlSPI.VSI_SlaveWriteBytes(ControlSPI.VSI_USBSPI, 0, write_buffer, 8);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Write data error!");
                return;
            }
            // Write data in slave mode: last parameter(100) is waiting time(in us),
            //  function will return immediately if time-out(no matter whether read the data or not)
            // Function judge received number of data via parameter read_data_num
            // ATTENTION: After call function VSI_SlaveWriteBytes,
            //  slave will receive the data when master read data(slave in full-duplex)
            //  master can call function VSI_SlaveReadBytes to discard received data after read the data complete from slave
            Int32 read_data_num = 0;
            ret = ControlSPI.VSI_SlaveReadBytes(ControlSPI.VSI_USBSPI, 0, read_buffer, ref read_data_num,100);
            if (ret != ControlSPI.ERROR.SUCCESS)
            {
                Console.WriteLine("Read data error!");
                return;
            }
            else
            {
                if (read_data_num > 0)
                {
                    Console.WriteLine("Read data num:" + read_data_num);
                    Console.WriteLine("Read data(Hex):");
                    for (int i = 0; i < read_data_num; i++)
                    {
                        Console.WriteLine(read_buffer[i].ToString("X2"));
                    }

                }
                else
                {
                    Console.WriteLine("No data!");
                }
            }
        }
    }
}
