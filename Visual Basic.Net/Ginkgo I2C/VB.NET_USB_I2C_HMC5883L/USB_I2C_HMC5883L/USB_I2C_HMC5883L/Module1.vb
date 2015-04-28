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
        I2C_Config.ClockSpeed = 400000
        I2C_Config.ControlMode = ControlI2C.VII_HCTL_MODE
        I2C_Config.MasterMode = ControlI2C.VII_MASTER
        I2C_Config.SubAddrWidth = ControlI2C.VII_SUB_ADDR_NONE
        ret = VII_InitI2C(ControlI2C.VII_USBI2C, 0, 0, I2C_Config)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Initialize device error")
            Return
        End If

        'Put the HMC5883 IC into the correct operating mode
        Dim Address As UInt16
        Address = &H1E  '0011110b, I2C 7bit address of HMC5883
        Address = Address << 1 ' 
        Dim write_buffer(8) As Byte
        Dim read_buffer(8) As Byte
        write_buffer(0) = &H0
        ret = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, Address, &H2, write_buffer, 1)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error!")
        End If

        While True
            'Read status
            ret = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, Address, &H9, read_buffer, 1)
            If ret <> ControlI2C.SUCCESS Then
                System.Console.WriteLine("Read data error!")
            End If
            'Data ready
            If (read_buffer(0) & &H1) > 0 Then

                'Tell the HMC5883 where to begin reading data
                ret = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, Address, &H3, read_buffer, 6)
                If ret <> ControlI2C.SUCCESS Then
                    System.Console.WriteLine("Read data error!")
                End If

                Dim x, y, z As Int32

                x = (read_buffer(0) << 8) Or read_buffer(1)
                y = (read_buffer(2) << 8) Or read_buffer(3)
                z = (read_buffer(4) << 8) Or read_buffer(5)
                Console.WriteLine("------------------------------------------------------\n")
                Console.WriteLine("x = {0:d}", x)
                Console.WriteLine("y = {0:d}", y)
                Console.WriteLine("z = {0:d}", z)
                Console.WriteLine("------------------------------------------------------\n")
            End If
        End While




    End Sub

End Module
