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

        ' Start write data speed test
        Console.WriteLine("Start write data speed test.....")
        ' Write Data Speed Test
        Dim write_buffer(10240) As Byte
        Dim read_buffer(10240) As Byte
        Dim litmp As Long
        Dim StartTime As Double
        Dim EndTime As Double
        Dim dfFreq As Double
        Dim i As Int16 = 0


        timer.QueryPerformanceFrequency(litmp) 'Get the performance counter frequency, in n/s
        dfFreq = litmp
        timer.QueryPerformanceCounter(litmp)  ' Get the current value of the performance counter
        StartTime = litmp                ' Start time
        For i = 0 To 399
            ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 10000)
            If ret <> ControlSPI.SUCCESS Then
                Console.WriteLine("write data error!")
                Return
            End If
        Next i

        timer.QueryPerformanceCounter(litmp)   ' Get the current value of the performance counter
        EndTime = litmp

        ' Print the write data speed information
        Console.WriteLine("Write Data Numbers: {0} Bytes", i * 10000)
        Console.WriteLine("Write Data Elapsed Time: {0:###.000}", (EndTime - StartTime) / dfFreq)
        Console.WriteLine("Write Data Speed: {0:###.000} KByte/s", ((i * 10000) / 1024) / ((EndTime - StartTime) / dfFreq))
        ' Start Write Data Speed Test
        Console.WriteLine("Start Read Data Speed Test...")
        timer.QueryPerformanceCounter(litmp) ' Get the current value of the performance counter
        StartTime = litmp ' Start time
        For i = 0 To 399
            ret = ControlSPI.VSI_ReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 10000)
            If ret <> ControlSPI.SUCCESS Then
                Console.WriteLine("read data error!")
                Return
            End If
        Next i

        timer.QueryPerformanceCounter(litmp) ' Get the current value of the performance counter
        EndTime = litmp ' Stop time
        ' Print the write data speed information
        Console.WriteLine("Read Data Numbers: {0:d} Bytes", i * 10000)
        Console.WriteLine("Read Data Elapsed Time: {0:f}", (EndTime - StartTime) / dfFreq)
        Console.WriteLine("Read Data Speed: {0:f} KByte/s", ((i * 10000) / 1024) / ((EndTime - StartTime) / dfFreq))


    End Sub

End Module
