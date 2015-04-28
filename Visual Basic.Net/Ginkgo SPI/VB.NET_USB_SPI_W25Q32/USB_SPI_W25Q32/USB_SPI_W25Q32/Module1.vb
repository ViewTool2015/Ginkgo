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
        '  Initialize device(hardware mode)
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
        ' Get JEDEC ID
        Dim write_buffer(1024) As Byte
        Dim read_buffer(1024) As Byte
        write_buffer(0) = &H9F
        ret = VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, read_buffer, 3)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Get JEDEC ID error!")
            Return
        Else
            Dim JEDEC_ID As UInt32
            JEDEC_ID = (CInt(read_buffer(0)) << 16) + (CInt(read_buffer(1)) << 8) + CInt(read_buffer(2))
            System.Console.WriteLine("JEDEC ID(Hex):{0}", JEDEC_ID.ToString("X6"))
        End If

        ' Write enable
        write_buffer(0) = &H6 'SPI Flash write enable command
        ret = VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If

        ' Erase first sector
        write_buffer(0) = &H20  ' SPI Flash sector erase command
        write_buffer(1) = &H0   ' sector address - high
        write_buffer(2) = &H0   ' sector address - mid
        write_buffer(3) = &H0   ' sector address - low
        ret = VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 3)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If
        ' Delay to ensure the operation to complete
        System.Threading.Thread.Sleep(100)
        ' Write enable
        write_buffer(0) = &H6 'SPI Flash write enable command
        ret = VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If
        ' Page Program
        write_buffer(0) = &H2   ' SPI Flash write data command
        write_buffer(1) = &H0   ' sector address - high
        write_buffer(2) = &H0   ' sector address - mid
        write_buffer(3) = &H0   ' sector address - low
        For i As Integer = 0 To 255
            write_buffer(4 + i) = CByte(i)
        Next
        ret = VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4 + 256)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        Else
            System.Console.WriteLine("Write data succeed!")
        End If
        ' Delay to ensure the operation to complete
        System.Threading.Thread.Sleep(100)
        ' Read data
        write_buffer(0) = &H3   ' SPI Flash read data command
        write_buffer(1) = &H0   ' sector address - high
        write_buffer(2) = &H0   ' sector address - mid
        write_buffer(3) = &H0   ' sector address - low
        ret = VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4, read_buffer, 256)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Read data error!")
            Return
        Else
            System.Console.WriteLine("Get Data:")
            Dim read_data_str As String
            read_data_str = ""
            For i As Integer = 0 To 255
                read_data_str += read_buffer(i).ToString("X2") + " "
                If (((i + 1) Mod 24) = 0) Then
                    read_data_str += vbNewLine
                End If
            Next
            Console.WriteLine(read_data_str)
        End If
        ' Close device
        ret = VSI_CloseDevice(ControlSPI.VSI_USBSPI, 0)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Close device error!")
            Return
        End If
    End Sub

End Module
