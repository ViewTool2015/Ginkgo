
'  Hardware Connection
'  AT45DB041D      Ginkgo USB-SPI Adapter
'  1.SI      <-->  SPI_MOSI(Pin17)
'  2.SCK     <-->  SPI_SCK(Pin13)
'  3.RESET   <-->  VCC(Pin2)
'  4.CS      <-->  SPI_SEL0(Pin11)
'  5.WP	     <-->  VCC(Pin2)
'  6.VCC     <-->  VCC(Pin2)
'  7.GND     <-->  GND(Pin19/Pin20)
'  8.SO	     <-->  SPI_MISO(Pin15)


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
        Dim i As UInt32 = 0

        ' Get JEDEC ID
        write_buffer(0) = &H9F 'SPI Flash get JEDEC ID command
        ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, read_buffer, 3)
        If ret <> ControlSPI.SUCCESS Then
            System.Console.WriteLine("Get JEDEC ID error!")
            Return
        Else
            Dim JEDEC_ID As Int32 = (CInt(read_buffer(0)) << 16) Or (CInt(read_buffer(1)) << 8) Or CInt(read_buffer(2))
            Console.WriteLine("JEDEC ID(Hex):{0}", JEDEC_ID.ToString("X6"))
        End If



        ' Wait Busy
        Do
            write_buffer(0) = &HD7 'SPI Flash read status register
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, read_buffer, 1)
            If ret <> ControlSPI.SUCCESS Then
                Console.WriteLine("Get status error!")
                Return
            End If
        Loop While ((read_buffer(0) And &H80) = &H0)

        ' Page Erase
        write_buffer(0) = &H81
        write_buffer(1) = &H0
        write_buffer(2) = &H0
        write_buffer(3) = &H0
        ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Sector Erase start error!")
            Return
        Else
            Console.WriteLine("Sector erase start success!")
        End If

        ' Wait Busy
        Do
            write_buffer(0) = &HD7 'SPI Flash read status register
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, read_buffer, 1)
            If ret <> ControlSPI.SUCCESS Then
                Console.WriteLine("Get status error!")
                Return
            End If
        Loop While ((read_buffer(0) And &H80) = &H0)

        ' Write data to buffer
        write_buffer(0) = &H87
        write_buffer(1) = &H0
        write_buffer(2) = &H0
        write_buffer(3) = &H0
        For i = 0 To 527
            write_buffer(4 + i) = CByte(i And &HFF)
        Next i
        ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4 + 528)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Write data error!!")
            Return
        Else
            Console.WriteLine("Write data success!")
        End If

        ' Buffer to Main Memory Page Program without Built-in Erase
        write_buffer(0) = &H89
        write_buffer(1) = &H0
        write_buffer(2) = &H0
        write_buffer(3) = &H0
        ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 4)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Write data to main memory error!")
            Return
        Else
            Console.WriteLine("Write data to main memory success!")
        End If

        ' Wait Busy
        Do
            write_buffer(0) = &HD7 'SPI Flash read status register
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 1, read_buffer, 1)
            If ret <> ControlSPI.SUCCESS Then
                Console.WriteLine("Get status error!")
                Return
            End If
        Loop While ((read_buffer(0) And &H80) = &H0)

        ' Main Memory Page Read
        write_buffer(0) = &HD2
        write_buffer(1) = &H0
        write_buffer(2) = &H0
        write_buffer(3) = &H0
        ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, write_buffer, 8, read_buffer, 528)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Read data error!")
            Return
        Else
            Dim read_data As Int32
            Dim read_data_str As String = ""
            Console.WriteLine("Get Data:")
            For i = 0 To 527
                read_data = read_buffer(i)
                read_data_str += read_data.ToString("X2") + " "
                If (((i + 1) Mod 16) = 0) Then
                    read_data_str += vbLf
                End If
            Next i
            Console.WriteLine(read_data_str)
        End If

        ' Close device
        ret = ControlSPI.VSI_CloseDevice(ControlSPI.VSI_USBSPI, 0)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Close device error!")
            Return
        Else
            Console.WriteLine("Close device success!")
        End If



    End Sub

End Module
