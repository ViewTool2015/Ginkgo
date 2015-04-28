Imports System.String
Module Module1

    Sub Main()
        Dim ret As Int32
        ' Scan device
        ret = VDA_ScanDevice(1)
        If ret <= 0 Then
            System.Console.WriteLine("No device connected!")
            Return
        End If
        ' Open device
        ret = VDA_OpenDevice(ControlDAC.VDA_USBDAC, 0, 0)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Open device error!")
            Return
        End If

        Dim WaveDataBuff(1024) As UShort
        Dim j As Int32 = 0

        ' Configuration DAC to output dc voltage
        WaveDataBuff(0) = 1532 ' Vout = (3.3V/4095)*1532 = 1.23V
        System.Console.WriteLine("Output DC Voltage: {0}", (1532 * 3.3 / 4095))
        ret = VDA_InitDAC(VDA_USBDAC, 0, VDA_DAC_CH0, WaveDataBuff, 1, 0)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.Write("Config DAC error!")
            Return
        End If
        ' Start  DAC to output 
        ret = VDA_StartDAC(VDA_USBDAC, 0, VDA_DAC_CH0)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Start DAC error!")
            Return
        End If
        System.Console.WriteLine("Press ""Enter"" to continue")
        System.Console.ReadLine()
        VDA_StopDAC(VDA_USBDAC, 0, VDA_DAC_CH0)


        Dim i As Int32
        ' Configuration DAC to output square wave
        For i = 0 To 63
            WaveDataBuff(i) = &HFFF
            WaveDataBuff(64 + i) = &H0
        Next i
        ' Square wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
        System.Console.WriteLine("Output Square Wave!")
        ret = VDA_InitDAC(VDA_USBDAC, 0, VDA_DAC_CH0, WaveDataBuff, 128, 100)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Config DAC error!")
            Return
        End If
        ' Start  DAC to output 
        ret = VDA_StartDAC(VDA_USBDAC, 0, VDA_DAC_CH0)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Start DAC error!")
            Return
        End If
        System.Console.WriteLine("Press ""Enter"" to continue")
        System.Console.ReadLine()
        VDA_StopDAC(VDA_USBDAC, 0, VDA_DAC_CH0)


        ' Configuration DAC to output sawtooth wave 
        For i = 0 To 127
            WaveDataBuff(i) = i * &HFFF / 128
        Next i
        ' Square wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
        System.Console.WriteLine("Output Sawtooth Wave!")
        ret = VDA_InitDAC(VDA_USBDAC, 0, VDA_DAC_CH0, WaveDataBuff, 128, 100)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Config DAC error!")
            Return
        End If
        ' Start  DAC to output 
        ret = VDA_StartDAC(VDA_USBDAC, 0, VDA_DAC_CH0)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Start DAC error!")
            Return
        End If
        System.Console.WriteLine("Press ""Enter"" to continue")
        System.Console.ReadLine()
        VDA_StopDAC(VDA_USBDAC, 0, VDA_DAC_CH0)


        ' Configuration DAC to output triangle wave 
        For i = 0 To 64
            WaveDataBuff(i) = i * &HFFF / 64
            WaveDataBuff(i + 64) = (64 - i) * &HFFF / 64
        Next i
        ' Triangle wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
        System.Console.WriteLine("Output Triangle Wave!")
        ret = VDA_InitDAC(VDA_USBDAC, 0, VDA_DAC_CH0, WaveDataBuff, 128, 100)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Config DAC error!")
            Return
        End If
        ' Start  DAC to output 
        ret = VDA_StartDAC(VDA_USBDAC, 0, VDA_DAC_CH0)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Start DAC error!")
            Return
        End If
        System.Console.WriteLine("Press ""Enter"" to continue")
        System.Console.ReadLine()
        VDA_StopDAC(VDA_USBDAC, 0, VDA_DAC_CH0)


        ' Configuration DAC to output sine wave 
        For i = 0 To 127
            WaveDataBuff(i) = CUShort(&HFFF * (1 + Math.Sin(2 * 3.14159 * i / 128)) / 2.0)
        Next i
        ' sine wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
        System.Console.WriteLine("Output Triangle Wave!")
        ret = VDA_InitDAC(VDA_USBDAC, 0, VDA_DAC_CH0, WaveDataBuff, 128, 100)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Config DAC error!")
            Return
        End If
        ' Start  DAC to output 
        ret = VDA_StartDAC(VDA_USBDAC, 0, VDA_DAC_CH0)
        If ret <> ControlDAC.SUCCESS Then
            System.Console.WriteLine("Start DAC error!")
            Return
        End If
        System.Console.WriteLine("Press ""Enter"" to continue")
        System.Console.ReadLine()
        VDA_StopDAC(VDA_USBDAC, 0, VDA_DAC_CH0)


        ' Close device
        VDA_CloseDevice(VDA_USBDAC, 0)
    End Sub

End Module
