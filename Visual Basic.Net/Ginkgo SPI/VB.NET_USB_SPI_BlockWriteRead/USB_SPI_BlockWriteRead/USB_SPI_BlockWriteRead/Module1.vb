Module Module1

    Sub Main()
        Dim ret As Int32
        ' Scan device
        ret = VSI_ScanDevice(1)
        If ret <= 0 Then
            System.Console.WriteLine("No device connected")
            Return
        End If
        ' Open device
        ret = VSI_OpenDevice(ControlSPI.VSI_USBSPI, 0, 0)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Open device error")
            Return
        End If

        ' Get device information
        Dim SPI_Info As ControlSPI.VSI_BOARD_INFO
        ret = VSI_ReadBoardInfo(0, SPI_Info)
        If ret < 0 Then
            System.Console.WriteLine("Get device info error")
            Return
        Else
            System.Console.WriteLine("Product Name : " + System.Text.Encoding.GetEncoding("UTF-8").GetString(SPI_Info.ProductName))
            System.Console.WriteLine("Firmware Version : v" + SPI_Info.FirmwareVersion(1).ToString + "." + SPI_Info.FirmwareVersion(2).ToString + "." + SPI_Info.FirmwareVersion(3).ToString)
            System.Console.WriteLine("Hardware Version : v" + SPI_Info.HardwareVersion(1).ToString + "." + SPI_Info.HardwareVersion(2).ToString + "." + SPI_Info.HardwareVersion(3).ToString)
        End If
        ' Initialize device(Master Mode, Hardware SPI, Half-Duplex)
        Dim SPI_Config As ControlSPI.VSI_INIT_CONFIG
        SPI_Config.ClockSpeed = 36000000
        SPI_Config.ControlMode = 1
        SPI_Config.CPHA = 0
        SPI_Config.CPOL = 0
        SPI_Config.LSBFirst = 0
        SPI_Config.MasterMode = 1
        SPI_Config.SelPolarity = 0
        SPI_Config.TranBits = 8
        ret = VSI_InitSPI(ControlSPI.VSI_USBSPI, 0, SPI_Config)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Initialize device error!")
            Return
        End If

        Dim write_buffer(10240) As Byte
        Dim read_buffer(10240) As Byte
        Dim BlockNum As Byte = 5
        Dim BlockSize As Byte = 2
        Dim IntervalTime As UInt32 = 100
        ' Block mode write data, data will be send out BlockNum time(s), send out BlockSize bytes of data every time
        ' Set CS to low before send data and set CS to high after send data complete when every time send data
        ' Each time send data, interval is IntervalTime us, that is CS set to high and hold IntervalTime us
        For i = 0 To (BlockNum - 1)
            For j = 0 To (BlockSize - 1)
                write_buffer(i * BlockSize + j) = CByte(i * j)
            Next j
        Next i
        ret = ControlSPI.VSI_BlockWriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, BlockSize, BlockNum, IntervalTime)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Block write data error!\n")
            Return
        End If

        ' Block mode read data: same as write data
        ret = ControlSPI.VSI_BlockReadBytes(ControlSPI.VSI_USBSPI, 0, 0, read_buffer, BlockSize, BlockNum, IntervalTime)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Block write data error!\n")
            Return
        Else
            Console.WriteLine("Read data(Hex):")
            For i = 0 To (BlockNum - 1)
                For j = 0 To (BlockSize - 1)
                    Console.Write("{0,-4:x}", read_buffer(i * BlockSize + j))
                Next j
            Next i
        End If
        Console.WriteLine()

        ' Block mode: write and read data
        Dim write_BlockSize As UInt16 = 1
        Dim read_BlockSize As UInt16 = 2

        For i = 0 To (BlockNum - 1)
            For j = 0 To (BlockSize - 1)
                write_buffer(i * write_BlockSize + j) = CByte(i * j)
            Next j
        Next i
        ret = ControlSPI.VSI_BlockWriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, write_BlockSize, read_buffer, read_BlockSize, BlockNum, IntervalTime)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Block write read data error!")
            Return
        Else
            Console.WriteLine("Read data(Hex):")
            For i = 0 To (BlockNum - 1)
                For j = 0 To (BlockSize - 1)
                    Console.Write("{0,-4:x}", read_buffer(i * BlockSize + j))
                Next j
            Next i

        End If

    End Sub

End Module
