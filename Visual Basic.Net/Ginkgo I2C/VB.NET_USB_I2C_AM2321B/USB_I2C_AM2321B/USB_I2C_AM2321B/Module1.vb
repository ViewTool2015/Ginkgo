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
        'Initialize device(hardware mode)
        Dim I2C_Config As ControlI2C.VII_INIT_CONFIG
        I2C_Config.AddrType = ControlI2C.VII_ADDR_7BIT
        I2C_Config.ClockSpeed = 100000
        I2C_Config.ControlMode = ControlI2C.VII_HCTL_MODE
        I2C_Config.MasterMode = ControlI2C.VII_MASTER
        I2C_Config.SubAddrWidth = ControlI2C.VII_SUB_ADDR_NONE
        ret = VII_InitI2C(ControlI2C.VII_USBI2C, 0, 0, I2C_Config)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Initialize device error")
            Return
        End If
        While True
            'Write data buffer
            Dim write_buffer(8) As Byte
            'Wake up AM2311 sensor
            VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &HB8, &H0, write_buffer, 0)
            'Send out read temperature and huminity command
            write_buffer(0) = &H3
            write_buffer(1) = &H0
            write_buffer(2) = &H4
            ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &HB8, &H0, write_buffer, 3)
            If ret <> ControlI2C.SUCCESS Then
                System.Console.WriteLine("Write data error")
                Return
            End If
            'Read data buffer
            Dim read_buffer(8) As Byte
            'Read out temperature and huminity
            ret = VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, &HB8, &H0, read_buffer, 8)
            If ret <> ControlI2C.SUCCESS Then
                System.Console.WriteLine("Read data error" + ret.ToString)
                Return
            Else
                System.Console.Clear()
                Dim Temperature As Double
                Temperature = ((CShort(read_buffer(4)) << 8) + read_buffer(5)) / 10.0
                Dim Humidity As Double
                Humidity = ((CShort(read_buffer(2)) << 8) + read_buffer(3)) / 10.0
                System.Console.WriteLine("Temperature : " + Temperature.ToString("0.0") + " ℃")
                System.Console.WriteLine("Humidity : " + Humidity.ToString("0.0") + " ％")
            End If
            Threading.Thread.Sleep(1000)
        End While
    End Sub

End Module
