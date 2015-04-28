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

        Dim i As Byte
        Dim SlaveReadLen As UInt16 : SlaveReadLen = 0
        Dim write_buffer(512) As Byte
        Dim read_buffer(512) As Byte

        'Slave write data
        For i = 0 To 15
            write_buffer(i) = i
        Next i
        ret = ControlI2C.VII_SlaveWriteBytes(ControlI2C.VII_USBI2C, 0, 0, write_buffer, 16)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Slave write data error!")
            Return
        Else
            Console.WriteLine("Write Data:\n")
            For i = 0 To 15
                Console.WriteLine("%02X ", write_buffer(i))
                If Not ((i + 1) Mod 16) Then
                    Console.WriteLine("\n")
                End If
            Next i
        End If


        'Slave read data
        While (True)
            System.Threading.Thread.Sleep(50)
            ret = ControlI2C.VII_SlaveReadBytes(ControlI2C.VII_USBI2C, 0, 0, read_buffer, SlaveReadLen)
            If (ret = ControlI2C.SUCCESS) And (SlaveReadLen > 0) Then
                Console.WriteLine("Read Data:\n")
                For i = 0 To SlaveReadLen
                    Console.WriteLine("%02X ", read_buffer(i))
                    If (i + 1) Mod 16 = 0 Then
                        Console.WriteLine("\n")
                    End If
                Next i
                Console.WriteLine("\n")
            End If

            If ret = ControlI2C.READ_NO_DATA Then
                Continue While
            Else
                Console.WriteLine("Slave read data error!\n")
            End If

        End While


    End Sub

End Module
