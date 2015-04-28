Module Module1

    Sub Main()
        Dim ret As Int32

        'Scan device
        ret = VII_ScanDevice(1)
        If ret <= 0 Then
            System.Console.WriteLine("No device connected")
            Return
        End If
        'Open device
        ret = VII_OpenDevice(ControlI2C.VII_USBI2C, 0, 0)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Open device error")
            Return
        End If
        'Get device information
        Dim I2C_Info As ControlI2C.VII_BOARD_INFO
        ret = VII_ReadBoardInfo(0, I2C_Info)
        If ret < 0 Then
            System.Console.WriteLine("Get device info error")
            Return
        Else
            System.Console.WriteLine("Product Name : " + System.Text.Encoding.GetEncoding("UTF-8").GetString(I2C_Info.ProductName))
            System.Console.WriteLine("Firmware Version : v" + I2C_Info.FirmwareVersion(1).ToString + "." + I2C_Info.FirmwareVersion(2).ToString + "." + I2C_Info.FirmwareVersion(3).ToString)
            System.Console.WriteLine("Hardware Version : v" + I2C_Info.HardwareVersion(1).ToString + "." + I2C_Info.HardwareVersion(2).ToString + "." + I2C_Info.HardwareVersion(3).ToString)
        End If
        'Initialize device(hardware mode)
        Dim I2C_Config As ControlI2C.VII_INIT_CONFIG
        I2C_Config.AddrType = ControlI2C.VII_ADDR_7BIT
        I2C_Config.ClockSpeed = 400000
        I2C_Config.ControlMode = ControlI2C.VII_HCTL_MODE
        I2C_Config.MasterMode = ControlI2C.VII_MASTER
        I2C_Config.SubAddrWidth = ControlI2C.VII_SUB_ADDR_1BYTE
        ret = VII_InitI2C(ControlI2C.VII_USBI2C, 0, 0, I2C_Config)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Initialize device error")
            Return
        End If


        Dim write_buffer(8) As Byte
        Dim read_buffer(8) As Byte
        Dim cmdString As String
        Dim pll As Int32
        Dim frequency As Int32
        Dim dotNum As Int32
        Dim i As Int32
        Dim chartemp(20) As Char

        While True
            Console.WriteLine("Please input the frequency of radio(The unit is MHz):")
            cmdString = Console.ReadLine()
            If cmdString = "quit" Then
                Console.WriteLine("Exit")
                Exit While
            Else
                dotNum = 0
                For i = 0 To (cmdString.Length - 1)
                    If (Asc(cmdString.ToCharArray()(i)) <> &H2E) And (Asc(cmdString.ToCharArray()(i)) < &H30) And (Asc(cmdString.ToCharArray()(i)) > &H39) Then
                        Console.WriteLine("Input error!")
                        Exit While
                    End If
                    If Asc(cmdString.ToCharArray()(i)) = &H2E Then
                        dotNum = dotNum + 1
                    End If
                Next i

                If dotNum > 1 Then
                    Console.WriteLine("Input error!")
                    Exit While
                End If
                If dotNum > 0 Then
                    frequency = CInt(Single.Parse(cmdString) * 1000)
                Else
                    frequency = Int32.Parse(cmdString) * 1000
                End If
                chartemp = cmdString.ToCharArray()
                pll = CInt((CSng(frequency + 225)) / (8.192))
                write_buffer(0) = CByte(pll >> 8)
                write_buffer(1) = CByte(pll And &HFF)
                write_buffer(2) = &H70
                write_buffer(3) = &H16
                write_buffer(4) = &H80
                ret = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &HC0, &H0, write_buffer, 5)
                If ret <> ControlI2C.SUCCESS Then
                    Console.WriteLine("Set frequency error:{0}", ret)
                    exit While
                Else
                    Console.WriteLine("Set frequency success!")
                End If




            End If

        End While




    End Sub

End Module
