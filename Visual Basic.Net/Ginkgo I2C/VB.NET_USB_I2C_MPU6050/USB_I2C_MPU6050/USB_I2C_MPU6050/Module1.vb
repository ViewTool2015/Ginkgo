Module Module1

    Sub Main()
        Dim ret As Int32
        'Scan device
        ret = VII_ScanDevice(1)
        If ret <= 0 Then
            System.Console.WriteLine("No device connected!")
            Return
        End If
        'Open device
        ret = VII_OpenDevice(ControlI2C.VII_USBI2C, 0, 0)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Open device error!")
            Return
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
            System.Console.WriteLine("Initialize device error!")
            Return
        End If
        'Initialize MPU6050 sensor
        Dim write_buffer(8) As Byte
        write_buffer(0) = 0
        ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &HD0, &H6B, write_buffer, 1)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If
        System.Threading.Thread.Sleep(10)
        write_buffer(0) = 0
        ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &HD0, &H6C, write_buffer, 1)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If
        System.Threading.Thread.Sleep(10)
        write_buffer(0) = &H8
        ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &HD0, &H1B, write_buffer, 1)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If
        System.Threading.Thread.Sleep(10)
        write_buffer(0) = &H1
        ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &HD0, &H1A, write_buffer, 1)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If
        System.Threading.Thread.Sleep(10)
        write_buffer(0) = &H8
        ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &HD0, &H1C, write_buffer, 1)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If
        System.Threading.Thread.Sleep(10)
        While True
            'Get data
            Dim read_buffer(14) As Byte
            ret = VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, &HD0, &H3B, read_buffer, 14)
            If ret <> ControlI2C.SUCCESS Then
                System.Console.WriteLine("Read data error!")
                Return
            End If
            'Process data
            Dim AccelX As Int16
            Dim AccelY As Int16
            Dim AccelZ As Int16
            AccelX = (CShort(read_buffer(0)) << 8) + read_buffer(1)
            AccelY = (CShort(read_buffer(2)) << 8) + read_buffer(3)
            AccelZ = (CShort(read_buffer(4)) << 8) + read_buffer(5)
            Dim GyroX As Int16
            Dim GyroY As Int16
            Dim GyroZ As Int16
            GyroX = (CShort(read_buffer(6)) << 8) + read_buffer(7)
            GyroY = (CShort(read_buffer(8)) << 8) + read_buffer(9)
            GyroZ = (CShort(read_buffer(10)) << 8) + read_buffer(11)
            'Show data
            System.Console.Clear()
            System.Console.WriteLine("AccelX = " + AccelX.ToString())
            System.Console.WriteLine("AccelY = " + AccelY.ToString())
            System.Console.WriteLine("AccelZ = " + AccelZ.ToString())
            System.Console.WriteLine("GyroX = " + GyroX.ToString())
            System.Console.WriteLine("GyroY = " + GyroY.ToString())
            System.Console.WriteLine("GyroZ = " + GyroZ.ToString())
            System.Threading.Thread.Sleep(100)
        End While
    End Sub

End Module
