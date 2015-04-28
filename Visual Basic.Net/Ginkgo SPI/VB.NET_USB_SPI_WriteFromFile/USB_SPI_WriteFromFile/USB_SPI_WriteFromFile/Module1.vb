Imports System.IO


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

        Dim WriteDataTemp(512) As Byte
        Dim data_buufer(4000) As Char
        Dim strBuff As String
        Dim strTmp(1000) As String
        Dim j As UInt16

        ' Open file
        Dim fs As FileStream = New FileStream("data.txt", FileMode.Open, FileAccess.Read)
        Dim ftmp As StreamReader = New StreamReader(fs)

        ' Read file and write to SPI and console
        Do While (Not ftmp.EndOfStream)
            ' Read data one row from file
            strBuff = ftmp.ReadLine()

            ' The string every row use space to separate
            strTmp = strBuff.Split(" ")
            ' The string convert to numerical 
            If strBuff.Length > 1 Then
                Console.WriteLine("Write Data(Hex): ")
            End If



            For j = 0 To (strTmp.Length - 1)
                WriteDataTemp(j) = CByte(Integer.Parse(strTmp(j))) ' attention data overflow
                Console.Write("{0,3:d}  ", Integer.Parse(strTmp(j)))
            Next j
            Console.WriteLine()

            ' Send out the data via USB_SPI

            If j > 0 Then
                ret = ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, WriteDataTemp, j)
                If ret <> ControlSPI.SUCCESS Then
                    System.Console.WriteLine("Write enable error")
                    Return
                End If
            End If

        Loop
        fs.Close()

    End Sub

End Module
