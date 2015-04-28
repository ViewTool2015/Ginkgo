
  'Hardware Connection  (This is for your reference only)
  'BH1750FVI      Ginkgo USB-I2C Adapter
  '1.VCC	    <-->  VCC(Pin2)
  '2.ADDR    <-->  GND(Pin19/Pin20)
  '3.GND     <-->  GND(Pin19/Pin20)
  '4.SDA	    <-->  HI2C_SDA0(Pin8)
  '5.DVI     <-->  VCC(Pin2)
  '6.SCL	    <-->  HI2C_SCL0 (Pin6)

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

        'Write data buffer
        Dim write_buffer(8) As Byte
        ' Send wait for measuring instructions
        write_buffer(0) = &H1
        ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &H46, &H0, write_buffer, 1)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error")
            Return
        End If
        ' Start measurement in 11x resolution ratio
        write_buffer(0) = &H10
        ret = VII_WriteBytes(ControlI2C.VII_USBI2C, 0, 0, &H46, &H0, write_buffer, 1)
        If ret <> ControlI2C.SUCCESS Then
            System.Console.WriteLine("Write data error")
            Return
        End If
        While True
            'Waiting for measurement complete, at least 120ms
            Threading.Thread.Sleep(1000)
            'Get measurement data
            'Read data buffer
            Dim read_buffer(8) As Byte
            'Read out temperature and humidity
            ret = VII_ReadBytes(ControlI2C.VII_USBI2C, 0, 0, &H46, &H0, read_buffer, 2)
            If ret <> ControlI2C.SUCCESS Then
                System.Console.WriteLine("Read data error" + ret.ToString)
                Return
            Else
                System.Console.Clear()
                Dim Illuminance As Double
                Illuminance = ((CShort(read_buffer(0)) << 8) + read_buffer(1)) / 1.2
                System.Console.WriteLine("Luminance : " + Illuminance.ToString("0.0") + " lx")
            End If
        End While
    End Sub

End Module
