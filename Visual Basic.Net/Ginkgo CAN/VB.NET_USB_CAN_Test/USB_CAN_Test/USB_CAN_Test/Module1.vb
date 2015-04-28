#Const CAN_GET_BOARD_INFO = True
#Const CAN_INIT_EX = True
#Const CAN_SEND_DATA = True
#Const CAN_GET_STATUS = True
#Const CAN_READ_DATA = False
#Const CAN_MODE_LOOP_BACK = True
#Const CAN_CALLBACK_READ_DATA = True

Module Module1
#If CAN_CALLBACK_READ_DATA Then
    'Get data callback
    Public Sub GetDataCallback(ByVal DevIndex As UInt32, ByVal CANIndex As UInt32, ByVal Len As UInt32)
        Dim CAN_ReceiveData(Len) As VCI_CAN_OBJ
        For i = 0 To Len - 1
            ReDim CAN_ReceiveData(i).Data(7)
        Next
        Dim ReadDataNum As UInt32
        Dim DataNum As UInt32
        DataNum = VCI_GetReceiveNum(VCI_USBCAN2, 0, 0)
        If DataNum > 0 Then
            ReadDataNum = VCI_Receive(VCI_USBCAN2, 0, 0, CAN_ReceiveData, Len, 0)
            For i = 0 To ReadDataNum - 1
                Console.WriteLine("")
                Console.WriteLine("--CAN_ReceiveData.RemoteFlag = " + CAN_ReceiveData(i).RemoteFlag.ToString())
                Console.WriteLine("--CAN_ReceiveData.ExternFlag = " + CAN_ReceiveData(i).ExternFlag.ToString())
                Console.WriteLine("--CAN_ReceiveData.ID = 0x" + CAN_ReceiveData(i).ID.ToString("X4"))
                Console.WriteLine("--CAN_ReceiveData.DataLen = " + CAN_ReceiveData(i).DataLen.ToString())
                Console.Write("--CAN_ReceiveData.Data:")
                For j = 0 To CAN_ReceiveData(i).DataLen - 1
                    Console.Write(CAN_ReceiveData(i).Data(j).ToString("X2") + " ")
                Next
                Console.WriteLine("")
                Console.WriteLine("--CAN_ReceiveData.TimeStamp = " + CAN_ReceiveData(i).TimeStamp.ToString())
            Next
        End If
    End Sub
#End If
    Sub Main()
        Dim ret As Int32
        'Scan device 
        ret = VCI_ScanDevice(1)
        If ret <= 0 Then
            Console.WriteLine("No device connected")
            Return
        End If
#If CAN_GET_BOARD_INFO Then
        'Get device infomation
        Dim CAN_BoardInfo As VCI_BOARD_INFO_EX
        CAN_BoardInfo = New VCI_BOARD_INFO_EX
        ret = VCI_ReadBoardInfoEx(0, CAN_BoardInfo)
        If ret <> STATUS_OK Then
            Console.WriteLine("Get board info failed!")
        Else
            Console.WriteLine("--CAN_BoardInfo.ProductName = " + Text.Encoding.GetEncoding("UTF-8").GetString(CAN_BoardInfo.ProductName))
            Console.WriteLine("--CAN_BoardInfo.FirmwareVersion = v" + CAN_BoardInfo.FirmwareVersion(1).ToString() + "." + CAN_BoardInfo.FirmwareVersion(2).ToString() + "." + CAN_BoardInfo.FirmwareVersion(3).ToString())
            Console.WriteLine("--CAN_BoardInfo.HardwareVersion = v" + CAN_BoardInfo.HardwareVersion(1).ToString() + "." + CAN_BoardInfo.HardwareVersion(2).ToString() + "." + CAN_BoardInfo.HardwareVersion(3).ToString())
            Console.Write("--CAN_BoardInfo.SerialNumber(Hex) = ")
            For i = 0 To 11
                Console.Write(CAN_BoardInfo.SerialNumber(i).ToString("X2"))
            Next
            Console.WriteLine("")
        End If
#Else
        ret = VCI_OpenDevice(VCI_USBCAN2, 0, 0)
        If ret <> STATUS_OK Then
            Console.WriteLine("Open device failed!")
        Else
            Console.WriteLine("Open device success!")
        End If
#End If
        'Config device
#If CAN_INIT_EX Then
        Dim CAN_InitEx As VCI_INIT_CONFIG_EX
        CAN_InitEx.CAN_ABOM = 0
#If CAN_MODE_LOOP_BACK Then
        CAN_InitEx.CAN_Mode = 1
#Else
        CAN_InitEx.CAN_Mode = 0
#End If
        'Baud rate
        CAN_InitEx.CAN_BRP = 6
        CAN_InitEx.CAN_BS1 = 3
        CAN_InitEx.CAN_BS2 = 2
        CAN_InitEx.CAN_SJW = 1

        CAN_InitEx.CAN_NART = 1 'No automatic retransmission
        CAN_InitEx.CAN_RFLM = 0 'Receive FIFO locked mode
        CAN_InitEx.CAN_TXFP = 0 'Transmit FIFO priority
        CAN_InitEx.CAN_RELAY = 0
        ret = VCI_InitCANEx(VCI_USBCAN2, 0, 0, CAN_InitEx)
        If ret <> STATUS_OK Then
            Console.WriteLine("Init device failed!")
        Else
            Console.WriteLine("Init device success!")
        End If
        'Set filter
        Dim CAN_FilterConfig As VCI_FILTER_CONFIG
        CAN_FilterConfig.FilterIndex = 0
        CAN_FilterConfig.Enable = 1
        CAN_FilterConfig.ExtFrame = 0
        CAN_FilterConfig.FilterMode = 0
        CAN_FilterConfig.ID_IDE = 0
        CAN_FilterConfig.ID_RTR = 0
        CAN_FilterConfig.ID_Std_Ext = 0
        CAN_FilterConfig.MASK_IDE = 0
        CAN_FilterConfig.MASK_RTR = 0
        CAN_FilterConfig.MASK_Std_Ext = 0
        ret = VCI_SetFilter(VCI_USBCAN2, 0, 0, CAN_FilterConfig)
        If ret <> STATUS_OK Then
            Console.WriteLine("Set filter failed!")
        Else
            Console.WriteLine("Set filter success!")
        End If
#Else
        'Config device
        Dim CAN_Init As VCI_INIT_CONFIG
        CAN_Init.AccCode = &H0
        CAN_Init.AccMask = &HFFFFFFFFUI
        CAN_Init.Filter = 1
        CAN_Init.Mode = 0
        CAN_Init.Timing0 = &H0
        CAN_Init.Timing1 = &H14
        ret = VCI_InitCAN(VCI_USBCAN2, 0, 0, CAN_Init)
        If ret <> STATUS_OK Then
            Console.WriteLine("Init device failed!")
        Else
            Console.WriteLine("Init device success!")
        End If
#End If
        'Start CAN
        ret = VCI_StartCAN(VCI_USBCAN2, 0, 0)
        If ret <> STATUS_OK Then
            Console.WriteLine("Start CAN failed!")
        Else
            Console.WriteLine("Start CAN success!")
        End If
        'Register recevie callback
#If CAN_CALLBACK_READ_DATA Then
        ControlCAN.VCI_RegisterReceiveCallback(0, AddressOf GetDataCallback)
#End If
        'Send data
#If CAN_SEND_DATA Then
        Dim CAN_SendData(2) As VCI_CAN_OBJ
        For i = 0 To 1
            CAN_SendData(i).DataLen = 8
            ReDim CAN_SendData(i).Data(7)
            For j = 0 To CAN_SendData(i).DataLen - 1
                CAN_SendData(i).Data(j) = CType(i + j, Byte)
            Next
            CAN_SendData(i).ExternFlag = 0
            CAN_SendData(i).RemoteFlag = 0
            CAN_SendData(i).ID = &H123 + i
#If CAN_MODE_LOOP_BACK Then
            CAN_SendData(i).SendType = 2
#Else
            CAN_SendData(i).SendType = 0
#End If
        Next
        ret = VCI_Transmit(VCI_USBCAN2, 0, 0, CAN_SendData, 2)
        If ret = STATUS_ERR Then
            Console.WriteLine("Send CAN data failed!")
        Else
            Console.WriteLine("Send CAN data success!")
        End If
#End If
        'Delay
        Threading.Thread.Sleep(50)
#If CAN_GET_STATUS Then
        Dim CAN_Status As VCI_CAN_STATUS
        ret = VCI_ReadCANStatus(VCI_USBCAN2, 0, 0, CAN_Status)
        If ret <> STATUS_OK Then
            Console.WriteLine("Get CAN status failed!")
            Return
        Else
            Console.WriteLine("Buffer Size : " + CAN_Status.BufferSize.ToString())
            Console.WriteLine("ESR : 0x" + CAN_Status.regESR.ToString("X8"))
            Console.WriteLine("------Error warning flag : " + ((CAN_Status.regESR) And &H1).ToString())
            Console.WriteLine("------Error passive flag : " + ((CAN_Status.regESR / 2) And &H1).ToString())
            Console.WriteLine("------Bus-off flag : " + ((CAN_Status.regESR / 4) And &H1).ToString())
            Console.Write("------Last error code(" + ((CAN_Status.regESR / 16) And &H7).ToString() + ") : ")
            Select Case ((CAN_Status.regESR / 16) And &H7)
                Case 0
                    Console.WriteLine("No Error")
                Case 1
                    Console.WriteLine("Stuff Error")
                Case 2
                    Console.WriteLine("Form Error")
                Case 3
                    Console.WriteLine("Acknowledgment Error")
                Case 4
                    Console.WriteLine("Bit recessive Error")
                Case 5
                    Console.WriteLine("Bit dominant Error")
                Case 6
                    Console.WriteLine("CRC Error")
                Case 7
                    Console.WriteLine("Set by software")
            End Select
            Console.WriteLine("------Transmit error counter : " + ((CAN_Status.regESR / 65536) And &HFF).ToString())
            Console.WriteLine("------Receive error counter : " + ((CAN_Status.regESR / 16777216) And &HFF).ToString())
            Console.WriteLine("TSR : 0x" + CAN_Status.regTSR.ToString("X8"))
        End If
#End If
        'Read data
#If CAN_READ_DATA Then
        Dim CAN_ReceiveData(2) As VCI_CAN_OBJ
        For i = 0 To 1
            ReDim CAN_ReceiveData(i).Data(7)
        Next
        Dim ReadDataNum As UInt32
        Dim DataNum As UInt32
        DataNum = VCI_GetReceiveNum(VCI_USBCAN2, 0, 0)
        If DataNum > 0 Then
            ReadDataNum = VCI_Receive(VCI_USBCAN2, 0, 0, CAN_ReceiveData, 2, 0)
            For i = 0 To ReadDataNum - 1
                Console.WriteLine("")
                Console.WriteLine("--CAN_ReceiveData.RemoteFlag = " + CAN_ReceiveData(i).RemoteFlag.ToString())
                Console.WriteLine("--CAN_ReceiveData.ExternFlag = " + CAN_ReceiveData(i).ExternFlag.ToString())
                Console.WriteLine("--CAN_ReceiveData.ID = 0x" + CAN_ReceiveData(i).ID.ToString("X4"))
                Console.WriteLine("--CAN_ReceiveData.DataLen = " + CAN_ReceiveData(i).DataLen.ToString())
                Console.Write("--CAN_ReceiveData.Data:")
                For j = 0 To CAN_ReceiveData(i).DataLen - 1
                    Console.Write(CAN_ReceiveData(i).Data(j).ToString("X2") + " ")
                Next
                Console.WriteLine("")
                Console.WriteLine("--CAN_ReceiveData.TimeStamp = " + CAN_ReceiveData(i).TimeStamp.ToString())
            Next
        End If
#End If
        Console.WriteLine("")
#If CAN_CALLBACK_READ_DATA Then
        ControlCAN.VCI_LogoutReceiveCallback(0)
#End If
        'Stop receive can data
        ControlCAN.VCI_ResetCAN(VCI_USBCAN2, 0, 0)
        Console.WriteLine("VCI_ResetCAN")
        'Close Device
        ControlCAN.VCI_CloseDevice(VCI_USBCAN2, 0)
        Console.WriteLine("VCI_CloseDevice")
    End Sub

End Module
