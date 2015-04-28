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


        ' Write enable
        write_buffer(0) = &H6
        ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, read_buffer, 3)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Flash write enable error {0}", ret)
            Return
        Else
            Console.WriteLine("Write enable success!")
        End If

        ' Write data to address 0
        write_buffer(0) = &H2
        write_buffer(1) = &H0

        For i = 0 To 15
            write_buffer(i + 2) = CByte(i)
        Next i
        ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 18)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Write enable success!")
        Else
            Console.WriteLine("Write 16 byte success!")
        End If

        ' Delay to ensure data write completely
        System.Threading.Thread.Sleep(100)

        ' Read the written data
        write_buffer(0) = &H3
        write_buffer(1) = &H0 ' low 8bits address, please see m95040 datasheet for detail
        ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 2, read_buffer, 16)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Read Data error {0}", ret)
        Else
            Console.WriteLine("Read Data success")
        End If

        For i = 0 To 15
            Console.Write("{0,  2}  ", read_buffer(i))
        Next i

    End Sub

End Module
