Module Module1

    Sub Main()
        Dim ret As Int32
        ' Scan device
        ret = UART_ScanDevice(1)
        If ret <= 0 Then
            System.Console.WriteLine("No device connected")
            Return
        End If
        ' Open device
        ret = UART_OpenDevice(ControlUART.UART_USBUART, 0, 0)
        If ret <> ControlUART.SUCCESS Then
            System.Console.WriteLine("Open device error")
            Return
        End If

        ' Get device information
        Dim UART_Info As ControlUART.UART_BOARD_INFO
        ret = UART_ReadBoardInfo(0, UART_Info)
        If ret < 0 Then
            System.Console.WriteLine("Get device info error")
            Return
        Else
            System.Console.WriteLine("Product Name : " + System.Text.Encoding.GetEncoding("UTF-8").GetString(UART_Info.ProductName))
            System.Console.WriteLine("Firmware Version : v" + UART_Info.FirmwareVersion(1).ToString + "." + UART_Info.FirmwareVersion(2).ToString + "." + UART_Info.FirmwareVersion(3).ToString)
            System.Console.WriteLine("Hardware Version : v" + UART_Info.HardwareVersion(1).ToString + "." + UART_Info.HardwareVersion(2).ToString + "." + UART_Info.HardwareVersion(3).ToString)
        End If
        ' Initialize device
        Dim UART_Config As ControlUART.UART_INIT_CONFIG
        UART_Config.BaudRate = 115200 'band rate
        UART_Config.Parity = 0
        UART_Config.RS485Mode = 232
        UART_Config.StopBits = 0
        UART_Config.WordLength = 8

        ret = UART_InitDevice(UART_USBUART, 0, 0, UART_Config)
        If ret <> ControlUART.SUCCESS Then
            System.Console.WriteLine("Initialize device error!")
            Return
        End If

        Dim WriteBuffer(64) As Byte

        Dim i As UInt32 = 0

        ' Send data
        For i = 0 To 63
            WriteBuffer(i) = CByte(i)
        Next i

        ret = ControlUART.UART_WriteBytes(2, 0, 0, WriteBuffer, 64)
        If ret <> ControlUART.SUCCESS Then
            System.Console.WriteLine("Send data error!")
            Return
        End If

        ' Loop read data
        Dim Len As UInt16 = 0
        Dim ReadBuffer(10240) As Byte
        Dim AllDataNum As Int32 = 0
        While True
            ret = ControlUART.UART_ReadBytes(2, 0, 0, ReadBuffer, Len)
            If ret = ControlUART.READ_NO_DATA Then
                System.Threading.Thread.Sleep(10)
            ElseIf ret = ControlUART.SUCCESS Then
                ' print data if receive data
                If Len > 0 Then
                    AllDataNum += Len
                    Dim read_data As Byte
                    Dim read_data_str As String = ""
                    For i = 0 To (Len - 1)
                        read_data = ReadBuffer(i)
                        read_data_str += read_data.ToString("X2") + " "
                        If ((i + 1) Mod 16) = 0 Then
                            read_data_str += vbLf
                        End If
                    Next i
                    Console.WriteLine(read_data_str)
                    Console.WriteLine("AllDataNum = " + AllDataNum.ToString())
                End If
                System.Threading.Thread.Sleep(10)
            Else
                Console.WriteLine("Read data error!")
                Return
            End If



        End While



    End Sub

End Module
