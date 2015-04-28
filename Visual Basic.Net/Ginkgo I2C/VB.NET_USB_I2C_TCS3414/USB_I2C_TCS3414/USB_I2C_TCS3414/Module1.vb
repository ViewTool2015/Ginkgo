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
        'Initialize device
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

        ' Write control register
        Dim cmd As UInt32
        Dim ControlRegister(8) As Byte
        ControlRegister(0) = &H3 ' POWER = 1,ADC_EN = 1
        cmd = &H80 'Select command register,Byte protocol,Register Address.
        ret = ControlI2C.VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &HA0, cmd, ControlRegister, 8)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error!")
            Return
        End If

        'Delay
        System.Threading.Thread.Sleep(10)

        'Get ADC data
        Dim adc_buffer(2) As Byte
        cmd = &H80 Or &H20 Or &H10 'Select command register,Word protocol,Register Address.
        ret = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, &HA0, cmd, adc_buffer, 2)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Read data error!!")
            Return
        Else
            Console.WriteLine("")
            Console.WriteLine("Get Color Data:")
            Console.WriteLine("  Green ADC Data : " + String.Format("{0:D}", adc_buffer(0) Or (adc_buffer(1) << 8)))
        End If

        cmd = &H80 Or &H20 Or &H12 'Select command register,Word protocol,Register Address.
        ret = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, &HA0, cmd, adc_buffer, 2)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Read data error!!")
            Return
        Else
            Console.WriteLine("  Green ADC Data : " + String.Format("{0:D}", adc_buffer(0) Or (adc_buffer(1) << 8)))
        End If

        cmd = &H80 Or &H20 Or &H14 'Select command register,Word protocol,Register Address.
        ret = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, &HA0, cmd, adc_buffer, 2)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Read data error!!")
            Return
        Else
            Console.WriteLine("  Green ADC Data : " + String.Format("{0:D}", adc_buffer(0) Or (adc_buffer(1) << 8)))
        End If

        cmd = &H80 Or &H20 Or &H16 'Select command register,Word protocol,Register Address.
        ret = ControlI2C.VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, &HA0, cmd, adc_buffer, 2)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Read data error!!")
            Return
        Else
            Console.WriteLine("  Green ADC Data : " + String.Format("{0:D}", adc_buffer(0) Or (adc_buffer(1) << 8)))
        End If


    End Sub

End Module
