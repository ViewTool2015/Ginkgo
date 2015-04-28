Imports System.IO
Imports System.Text

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
        Dim ReadDataTemp(10240) As Byte
        Dim StartAddr As Int32 = 0
        Dim EndAddr As Int32 = &H400000
        Dim addr As Int32 = 0
        Dim charTmp(1024) As Char
        Dim byteTmp(1024) As Byte
        Dim strTmp As String = New String(" ", 1024)
        Dim temp As Int64

        ' JEDEC ID
        WriteDataTemp(0) = &H9F
        ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, WriteDataTemp, 1, ReadDataTemp, 3)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Read flash ID error :%d\n", ret)
        Else
             Console.WriteLine("Flash ID = {0, -6:x}", (ReadDataTemp(0) << 16) Or (ReadDataTemp(1) << 8) Or (ReadDataTemp(2)))
        End If

        ' Open file  
        Dim fs As FileStream = New FileStream("Data.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite)
        ' FileStream aFile = File.OpenRead("Data.txt");
        Console.WriteLine("Start read data,please wait...")
        ' Read data
        For addr = StartAddr To (EndAddr - 1) Step 4096
            WriteDataTemp(0) = &HB
            WriteDataTemp(1) = CByte((addr >> 16) And &HFF)
            WriteDataTemp(2) = CByte((addr >> 8) And &HFF)
            WriteDataTemp(3) = CByte((addr >> 0) And &HFF)
            WriteDataTemp(4) = &H0
            ret = ControlSPI.VSI_WriteReadBytes(ControlSPI.VSI_USBSPI, 0, 0, WriteDataTemp, 4, ReadDataTemp, 4096)
            If ret <> ControlSPI.SUCCESS Then
                Console.WriteLine("Read data error :%d", ret)
                Return
            Else
                For j = 0 To 4095 Step 64
                    strTmp = New String(" ", 1024)
                    For i = 0 To 31
                        temp = (ReadDataTemp(j + i * 2) << 8) Or ReadDataTemp(j + i * 2 + 1)
                        strTmp = strTmp + temp.ToString()
                        strTmp = strTmp.Replace(" ", "")
                    Next i
                    charTmp = strTmp.ToCharArray()
                    byteTmp = Encoding.Default.GetBytes(charTmp)
                    fs.Write(byteTmp, 0, strTmp.Length)

                Next j
            End If

        Next addr
        fs.Close()
        Console.WriteLine("Read data end!\n")
        Console.ReadLine()

    End Sub

End Module
