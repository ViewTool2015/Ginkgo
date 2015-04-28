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
        'Set all ports to output mode
        Dim write_buffer(8) As Byte
        write_buffer(0) = 0
        write_buffer(1) = 0
        ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &H40, &H6, write_buffer, 2)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If
        While True
            'Control all the port output low level
            write_buffer(0) = 0
            write_buffer(1) = 0
            ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &H40, &H2, write_buffer, 2)
            If ret <> ControlI2C.SUCCESS Then
                System.Console.WriteLine("Write data error!")
                Return
            End If
            System.Threading.Thread.Sleep(500)
            'Control of all port output high level
            write_buffer(0) = &HFF
            write_buffer(1) = &HFF
            ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &H40, &H2, write_buffer, 2)
            If ret <> ControlI2C.SUCCESS Then
                System.Console.WriteLine("Write data error!")
                Return
            End If
            System.Threading.Thread.Sleep(500)
        End While
    End Sub

End Module
