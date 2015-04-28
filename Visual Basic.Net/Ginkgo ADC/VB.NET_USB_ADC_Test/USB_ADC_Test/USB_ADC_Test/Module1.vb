'ADC_CH0  ->Pin3
'ADC_CH1  ->pin5

Module Module1

    Sub Main()
        Dim ret As Int32
        ' Scan device
        ret = VAI_ScanDevice(1)
        If ret <= 0 Then
            System.Console.WriteLine("No device connected!")
            Return
        End If
        ' Open device
        ret = VAI_OpenDevice(ControlADC.VAI_USBADC, 0, 0)
        If ret <> ControlADC.SUCCESS Then
            System.Console.WriteLine("Open device error!")
            Return
        End If
        ' Initialize ADC_CH0 channel
        ' Cycle can be set to 0 if each channel transfer a byte of data at a time.
        ret = VAI_InitADC(ControlADC.VAI_USBADC, 0, ControlADC.VAI_ADC_CH0, 0)
        If ret <> ControlADC.SUCCESS Then
            System.Console.WriteLine("Initialize ADC error!")
            Return
        End If
        ' Get ADC_CH0 channel voltage values
        Dim adc_datas(4096) As UInt16
        ret = VAI_ReadDatas(ControlADC.VAI_USBADC, 0, 1, adc_datas)
        If ret <> ControlADC.SUCCESS Then
            System.Console.WriteLine("Read ADC data error!")
            Return
        Else
            System.Console.WriteLine("ADC_CH0 = " + ((adc_datas(0) * 3.3) / 4095).ToString("0.000"))
        End If
        ' Initialize ADC_CH0 and ADC_CH1 channel
        ' Cycle can be set to 0 if each channel transfer a byte of data at a time.
        ret = VAI_InitADC(ControlADC.VAI_USBADC, 0, ControlADC.VAI_ADC_CH0 Or ControlADC.VAI_ADC_CH1, 0)
        If ret <> ControlADC.SUCCESS Then
            System.Console.WriteLine("Initialize ADC error!")
            Return
        End If
        ' Get ADC_CH0 and ADC_CH1 voltage values
        ret = VAI_ReadDatas(ControlADC.VAI_USBADC, 0, 1, adc_datas)
        If ret <> ControlADC.SUCCESS Then
            System.Console.WriteLine("Read ADC data error!")
            Return
        Else
            System.Console.WriteLine("ADC_CH0 = " + ((adc_datas(0) * 3.3) / 4095).ToString("0.000"))
            System.Console.WriteLine("ADC_CH1 = " + ((adc_datas(1) * 3.3) / 4095).ToString("0.000"))
        End If
        ' Initialize ADC_CH0 & ADC_CH1, each channel sampling interval is set to 1000us
        ret = VAI_InitADC(ControlADC.VAI_USBADC, 0, ControlADC.VAI_ADC_CH0 Or ControlADC.VAI_ADC_CH1, 1000)
        If ret <> ControlADC.SUCCESS Then
            System.Console.WriteLine("Initialize ADC error!")
            Return
        End If
        ' Get voltage values of ADC_CH0 & ADC_CH1
        ' Get 10 values each channel, return a total of 20 values
        ret = VAI_ReadDatas(ControlADC.VAI_USBADC, 0, 10, adc_datas)
        If ret <> ControlADC.SUCCESS Then
            System.Console.WriteLine("Read ADC data error!")
            Return
        Else
            Dim i As Integer
            For i = 0 To 9
                System.Console.WriteLine("ADC_CH0({0}) = {1}", i, ((adc_datas(i * 2) * 3.3) / 4095).ToString("0.000"))
            Next
            For i = 0 To 9
                System.Console.WriteLine("ADC_CH1({0}) = {1}", i, ((adc_datas(i * 2 + 1) * 3.3) / 4095).ToString("0.000"))
            Next
        End If
        ' Close device
        ret = VAI_CloseDevice(ControlADC.VAI_USBADC, 0)
        If ret <> ControlADC.SUCCESS Then
            System.Console.WriteLine("Close device error!")
            Return
        End If
    End Sub

End Module
