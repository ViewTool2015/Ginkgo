Module Module1

    Sub Main()
        Dim ret As Int32
        ' Scan device
        ret = VPI_ScanDevice(1)
        If ret <= 0 Then
            System.Console.WriteLine("No device connected!")
            Return
        End If
        ' Open device
        ret = VPI_OpenDevice(ControlPWM.VPI_USBPWM, 0, 0)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Open device error!")
            Return
        End If
        ' Initialize configure  - CH0 channel
        Dim PWM_Config As ControlPWM.VII_INIT_CONFIG
        PWM_Config.PWM_ChannelMask = ControlPWM.VPI_PWM_CH0
        PWM_Config.PWM_Frequency = 10000    ' Frequency = 10kHz
        PWM_Config.PWM_Mode = 0
        PWM_Config.PWM_Polarity = 0        
        PWM_Config.PWM_Pulse = 50  
        ret = VPI_InitPWM(ControlPWM.VPI_USBPWM, 0, PWM_Config)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Initialize device error!")
            Return
        End If
        ' Start PWM_CH0 channel
        ret = VPI_StartPWM(ControlPWM.VPI_USBPWM, 0, ControlPWM.VPI_PWM_CH0)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Start pwm error!")
            Return
        End If
        ' Run 1s
        System.Threading.Thread.Sleep(1000)
        ' Stop PWM_CH0 channel 
        ret = VPI_StopPWM(ControlPWM.VPI_USBPWM, 0, ControlPWM.VPI_PWM_CH0)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Stop pwm error!")
            Return
        End If
        ' Initialize all PWM channel 
        PWM_Config.PWM_ChannelMask = ControlPWM.VPI_PWM_ALL
        PWM_Config.PWM_Frequency = 100000   ' Frequency = 100kHz
        PWM_Config.PWM_Mode = 0
        PWM_Config.PWM_Polarity = 0    
        PWM_Config.PWM_Pulse = 50     
        ret = VPI_InitPWM(ControlPWM.VPI_USBPWM, 0, PWM_Config)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Initialize device error!")
            Return
        End If
        ' Start PWM_CH0 and PWM_CH2 channel
        ret = VPI_StartPWM(ControlPWM.VPI_USBPWM, 0, ControlPWM.VPI_PWM_CH0 Or ControlPWM.VPI_PWM_CH2)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Start pwm error!")
            Return
        Else
            System.Console.WriteLine("Start pwm success!")
        End If
        ' Run 1S
        System.Threading.Thread.Sleep(1000)
        ' Adjust PWM_CH0 frequency
        Dim Frequency(8) As Int32
        Frequency(0) = 50000
        ret = VPI_SetPWMPeriod(ControlPWM.VPI_USBPWM, 0, ControlPWM.VPI_PWM_CH0, Frequency)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Set frequency error!")
            Return
        Else
            System.Console.WriteLine("Set frequency success!")
        End If
        ' Adjust PWM_CH2 Pulse
        Dim Pulse(8) As Byte
        Pulse(2) = CByte(80)
        ret = VPI_SetPWMPulse(ControlPWM.VPI_USBPWM, 0, ControlPWM.VPI_PWM_CH2, Pulse)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Set pulse error!")
            Return
        Else
            System.Console.WriteLine("Set pulse success!")
        End If
        ' Run 1S
        System.Threading.Thread.Sleep(1000)
        ' Adjust PWM_CH0 and PWM_CH2 pulse
        Pulse(0) = CByte(20)
        Pulse(2) = CByte(80)
        ret = VPI_SetPWMPulse(ControlPWM.VPI_USBPWM, 0, ControlPWM.VPI_PWM_CH0 Or ControlPWM.VPI_PWM_CH2, Pulse)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Set pulse error!")
            Return
        Else
            System.Console.WriteLine("Set pulse success!")
        End If
        ' Run 1S
        System.Threading.Thread.Sleep(1000)
        ' Attention: PWM_CH(2n) and PWM_CH(2n+1) cannot separate set
        ' Each channel duty ratio can separate set
        ' Close  device
        ' Close device
        ret = VPI_CloseDevice(ControlPWM.VPI_USBPWM, 0)
        If ret <> ControlPWM.SUCCESS Then
            System.Console.WriteLine("Close device error!")
        Else
            System.Console.WriteLine("Close device success!")
            Return
        End If
    End Sub

End Module
