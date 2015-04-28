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
        ' Write 8 bits data to 0x00
        Dim write_buffer(8) As Byte
        Dim device_addr As UInt16
        Dim sub_addr As UInt32
        Dim i As Integer
        For i = 0 To 7
            write_buffer(i) = CType(i, Byte)
        Next i
        device_addr = &HA0
        sub_addr = &H8
        ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, device_addr, sub_addr, write_buffer, 8)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error")
            Return
        Else
            System.Console.WriteLine("Write data success")
        End If
        'Delay to ensure write complete
        Threading.Thread.Sleep(10)
        ' Read data from 0x00
        Dim read_buffer(8) As Byte
        ret = VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, device_addr, sub_addr, read_buffer, 8)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Read data error" + ret.ToString)
            Return
        Else
            System.Console.WriteLine("Read data:")
            For i = 0 To 7
                System.Console.Write(" " + read_buffer(i).ToString)
            Next i
        End If
    End Sub

End Module
