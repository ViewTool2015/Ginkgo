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
        ' Write data to SPI bus
        Dim write_buffer(1024) As Byte
        Dim i As Integer
        For i = 0 To 7
            write_buffer(i) = CByte(i)
        Next
        ret = VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 8)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If
        ' Read data from SPI bus
        Dim read_buffer(1024) As Byte
        ret = VSI_ReadBytes(ControlSPI.VSI_USBSPI, 0, 0, read_buffer, 8)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Read data error!")
            Return
        Else
            System.Console.WriteLine("Read data(Hex):")
            For i = 0 To 7
                System.Console.WriteLine(read_buffer(i).ToString("X2"))
            Next
        End If
        ' Write data and then read data(clock separated ), CS is still enable
        For i = 0 To 4
            write_buffer(i) = CByte(i)
        Next
        ret = VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 5, read_buffer, 8)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("WriteRead data error!")
            Return
        Else
            System.Console.WriteLine("Read data(Hex):")
            For i = 0 To 7
                System.Console.WriteLine(read_buffer(i).ToString("X2"))
            Next
        End If
        ' Initialize device(Master Mode, Software SPI, Half-Duplex)
        ' function VSI_WriteBytes，VSI_ReadBytes，VSI_WriteReadBytes can be support in software SPI mode
        ' Hardware SPI cannot support function VSI_WriteBits,VSI_ReadBits,VSI_WriteReadBits, but can be used in 1-wired mode 
        SPI_Config.ClockSpeed = 100000
        SPI_Config.ControlMode = 2
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
        ' Send data with binary, up to 10240 bits
        Dim write_buffer_bin(1024) As Byte
        Dim write_data_str As String
        write_data_str = "10110100100101"
        For i = 0 To (write_data_str.Length - 1)
            write_buffer_bin(i) = AscW(write_data_str.Chars(i))
        Next
        write_buffer_bin(write_data_str.Length) = 0
        ret = VSI_WriteBits(ControlSPI.VSI_USBSPI, 0, 0, write_buffer_bin)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Write bits error!")
            Return
        End If
        ' Write data with binary, up to 10240 bits
        Dim read_buffer_bin(50) As Byte
        ret = VSI_ReadBits(ControlSPI.VSI_USBSPI, 0, 0, read_buffer_bin, 20)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Read bits error!")
            Return
        Else
            System.Console.WriteLine("Read bits:")
            System.Console.WriteLine(System.Text.Encoding.GetEncoding("UTF-8").GetString(read_buffer_bin))
        End If
        ' Read data with binary, up to 10240 bits
        For i = 0 To (read_buffer_bin.Length - 1)
            read_buffer_bin(i) = 0
        Next
        ret = VSI_WriteReadBits(ControlSPI.VSI_USBSPI, 0, 0, write_buffer_bin, read_buffer_bin, 10)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("WriteRead bits error!")
            Return
        Else
            System.Console.WriteLine("Read bits:")
            System.Console.WriteLine(System.Text.Encoding.GetEncoding("UTF-8").GetString(read_buffer_bin))
        End If
        ' Initialize device(Slave Mode, Hardware SPI, Full-Duplex)
        SPI_Config.ControlMode = 0  ' Hardware SPI, Full-Duplex
        SPI_Config.CPHA = 0         ' Clock Polarity and Phase should be same to master
        SPI_Config.CPOL = 0
        SPI_Config.LSBFirst = 0
        SPI_Config.MasterMode = 0   ' Slave Mode
        SPI_Config.SelPolarity = 0
        SPI_Config.TranBits = 8
        ret = VSI_InitSPI(ControlSPI.VSI_USBSPI, 0, SPI_Config)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Initialize device error!")
            Return
        End If
        ' Write data in slave mode(call function VSI_SlaveWriteBytes), data will not send out via MISO pin immediately until chip select by master,
        ' function VSI_SlaveWriteBytes return immediately after called, the data stored in adapter memory buffer
        For i = 0 To 7
            write_buffer(i) = CByte(i)
        Next
        ret = VSI_SlaveWriteBytes(ControlSPI.VSI_USBSPI, 0, write_buffer, 8)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Slave Write data error!")
            Return
        End If
        ' Write data in slave mode: last parameter(100) is waiting time(in us),
        '  function will return immediately if time-out(no matter whether read the data or not)
        ' Function judge received number of data via parameter read_data_num
        ' ATTENTION: After call function VSI_SlaveWriteBytes,
        '  slave will receive the data when master read data(slave in full-duplex)
        '  master can call function VSI_SlaveReadBytes to discard received data after read the data complete from slave
        Dim read_data_num As Int32
        read_data_num = 0
        ret = VSI_SlaveReadBytes(ControlSPI.VSI_USBSPI, 0, read_buffer, read_data_num, 100)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Slave Read data error!")
            Return
        Else
            If read_data_num > 0 Then
                System.Console.WriteLine("Read data num:" + read_data_num.ToString)
                System.Console.WriteLine("Read data(Hex):")
                For i = 0 To read_data_num
                    System.Console.WriteLine(read_buffer(i).ToString("X2"))
                Next
            Else
                System.Console.WriteLine("No data!")
            End If
        End If
    End Sub

End Module
