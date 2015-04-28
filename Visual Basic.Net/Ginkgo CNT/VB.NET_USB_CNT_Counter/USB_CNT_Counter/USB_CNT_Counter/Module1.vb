Imports System.Timers

Module Module1

    Sub Main()
        Dim ret As Int32
        ' Scan device
        ret = CNT_ScanDevice(1)
        If ret <= 0 Then
            System.Console.WriteLine("No device connected")
            Return
        End If
        ' Open device
        ret = CNT_OpenDevice(ControlCNT.CNT_USBCNT, 0, 0)
        If ret <> ControlCNT.SUCCESS Then
            System.Console.WriteLine("Open device error")
            Return
        End If

        ' Config counter
        Dim CNT_Config As ControlCNT.CNT_INIT_CONFIG
        CNT_Config.CounterBitWide = 32
        CNT_Config.CounterMode = 0
        CNT_Config.CounterPolarity = 0
        ret = ControlCNT.CNT_InitCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNTpin.CNT_CH0, CNT_Config)

        ret = CNT_InitCounter(CNT_USBCNT, 0, 0, CNT_Config)
        If ret <> ControlCNT.SUCCESS Then
            System.Console.WriteLine("Initialize device error!")
            Return
        End If

        ' Clear counter
        Dim counterValue(4) As Int32
        counterValue(0) = 0
        ret = ControlCNT.CNT_SetCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNTpin.CNT_CH0, counterValue)
        If ret <> ControlCNT.SUCCESS Then
            System.Console.WriteLine("Set counter value error!")
            Return
        End If
        ' Start counter
        ret = ControlCNT.CNT_StartCounter(ControlCNT.CNT_USBCNT, 0, ControlCNT.CNTpin.CNT_CH0)
        If ret <> ControlCNT.SUCCESS Then
            System.Console.WriteLine("Start counter error!")
            Return
        End If

        While True
            ' get counter value
            ret = CNT_GetCounter(CNT_USBCNT, 0, ControlCNT.CNTpin.CNT_CH0, counterValue)
            If ret <> ControlCNT.SUCCESS Then
                System.Console.WriteLine("Get counter value error!!")
                Return
            Else
                System.Console.WriteLine("Counter Value :{0}", counterValue(0))
            End If
            System.Threading.Thread.Sleep(1000)
        End While




    End Sub

End Module

