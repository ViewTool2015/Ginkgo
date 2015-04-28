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

        For j = 0 To 63
            write_buffer(j) = CByte(j)
        Next j

        ret = ControlSPI.VSI_SlaveWriteBytes(ControlSPI.VSI_USBSPI, 0, write_buffer, 64)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Slave write data error!!")
            Return
        End If

        While True
            Dim read_data_num As Int32 = 0
            ret = ControlSPI.VSI_SlaveReadBytes(ControlSPI.VSI_USBSPI, 0, read_buffer, read_data_num, 100)
            If ret <> ControlSPI.SUCCESS Then
                Console.WriteLine("Slave write data error!!")
                Return
            ElseIf read_data_num > 0 Then
                Console.WriteLine("Read data ")

                For i = 0 To (read_data_num - 1)
                    Console.Write("{0,-3:x}", read_buffer(i))
                Next i
                Console.WriteLine()

            End If
        End While
    End Sub

End Module
