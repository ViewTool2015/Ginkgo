Module Module1

    Sub Main()
        Dim ret As Int32
        ' Scan device
        ret = VGI_ScanDevice(1)
        If ret <= 0 Then
            System.Console.WriteLine("No device connected!")
            Return
        End If
        ' Open device
        ret = VGI_OpenDevice(ControlGPIO.VGI_USBGPIO, 0, 0)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Open device error!")
            Return
        End If
        ' Set GPIO_7 and GPIO_8 to output
        ret = VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN7 Or ControlGPIO.VGI_GPIO_PIN8)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Set pin output error!")
            Return
        End If
        ' Set GPIO_7 and GPIO_8 to high level
        ret = VGI_SetPins(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN7 Or ControlGPIO.VGI_GPIO_PIN8)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Set pin high error!")
            Return
        End If
        ' Set GPIO_7 and GPIO_8 to low level
        ret = VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN7 Or ControlGPIO.VGI_GPIO_PIN8)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Set pin low error!")
            Return
        End If
        ' Set GPIO_4 and GPIO_5 to input
        ret = VGI_SetInput(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 Or ControlGPIO.VGI_GPIO_PIN5)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Set pin input error!")
            Return
        End If
        ' Get GPIO_4 and GPIO_5 value
        Dim pin_value As UInt16
        pin_value = 0
        ret = VGI_ReadDatas(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 Or ControlGPIO.VGI_GPIO_PIN5, pin_value)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Get pin data error!")
            Return
        Else
            If pin_value And ControlGPIO.VGI_GPIO_PIN4 Then
                System.Console.WriteLine("GPIO_4 is high-level!")
            Else
                System.Console.WriteLine("GPIO_4 is low-level!")
            End If
            If pin_value And ControlGPIO.VGI_GPIO_PIN5 Then
                System.Console.WriteLine("GPIO_5 is high-level!")
            Else
                System.Console.WriteLine("GPIO_5 is low-level!")
            End If
        End If
        ' Set GPIO_4 and GPIO_5 to OD(Bi-directional, need pull-up resistor)
        ret = VGI_SetOpenDrain(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 Or ControlGPIO.VGI_GPIO_PIN5)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Set pin input error!")
            Return
        End If
        ' Set GPIO_4 and GPIO_5 to high level 
        ret = VGI_SetPins(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 Or ControlGPIO.VGI_GPIO_PIN5)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Set pin high error!")
            Return
        End If
        ' Set GPIO_4 and GPIO_5 to low level
        ret = VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 Or ControlGPIO.VGI_GPIO_PIN5)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Set pin low error!")
            Return
        End If
        ' Get GPIO_4 and GPIO_5 value
        pin_value = 0
        ret = VGI_ReadDatas(ControlGPIO.VGI_USBGPIO, 0, ControlGPIO.VGI_GPIO_PIN4 Or ControlGPIO.VGI_GPIO_PIN5, pin_value)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Get pin data error!")
            Return
        Else
            If pin_value And ControlGPIO.VGI_GPIO_PIN4 Then
                System.Console.WriteLine("GPIO_4 is high-level!")
            Else
                System.Console.WriteLine("GPIO_4 is low-level!")
            End If
            If pin_value And ControlGPIO.VGI_GPIO_PIN5 Then
                System.Console.WriteLine("GPIO_5 is high-level!")
            Else
                System.Console.WriteLine("GPIO_5 is low-level!")
            End If
        End If
        ret = VGI_CloseDevice(ControlGPIO.VGI_USBGPIO, 0)
        If ret <> ControlGPIO.SUCCESS Then
            System.Console.WriteLine("Close device error!")
            Return
        End If
    End Sub
End Module
