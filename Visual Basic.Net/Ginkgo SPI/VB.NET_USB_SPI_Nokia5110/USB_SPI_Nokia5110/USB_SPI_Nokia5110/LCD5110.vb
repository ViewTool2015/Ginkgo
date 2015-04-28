Imports Microsoft.VisualBasic
Imports System.Threading
Imports System.String

Module LCD5110
    Public Structure ControlPin
        Public Const LCD_RST_PIN As UInt16 = ControlGPIO.VGI_GPIO_PIN10
        Public Const LCD_DC_PIN As UInt16 = ControlGPIO.VGI_GPIO_PIN11
        Public Const LCD_BL_PIN As UInt16 = ControlGPIO.VGI_GPIO_PIN9
    End Structure



    Public Function ConfigAdapter() As Int32
        Dim ret As Int32
        Dim pSPI_Config As VSI_INIT_CONFIG = New VSI_INIT_CONFIG
        'Scan device connected
        ret = ControlGPIO.VGI_ScanDevice(1)
        If ret <= 0 Then
            Console.WriteLine("No device connect!")
            Return ret
        End If

        ' Open device
        ret = ControlSPI.VSI_OpenDevice(ControlSPI.VSI_USBSPI, 0, 0)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Open device error!")
            Return ret
        End If
        ' Initialize device
        pSPI_Config.ControlMode = 1
        pSPI_Config.MasterMode = 1
        pSPI_Config.ClockSpeed = 4500000
        pSPI_Config.CPHA = 0
        pSPI_Config.CPOL = 0
        pSPI_Config.LSBFirst = 0
        pSPI_Config.TranBits = 8
        pSPI_Config.SelPolarity = 0
        ret = ControlSPI.VSI_InitSPI(ControlSPI.VSI_USBSPI, 0, pSPI_Config)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Initialize device error!")
        End If
        ' Config RST,DC,BL pin
        ret = ControlGPIO.VGI_SetOutput(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_BL_PIN Or ControlPin.LCD_DC_PIN Or ControlPin.LCD_RST_PIN)
        If ret <> ControlSPI.SUCCESS Then
            Console.WriteLine("Config GPIO error!")
            Return ret
        End If
        Return 0
    End Function

    ' Set back light state
    Public Function LCD_SetBacklightState(ByVal state)
        If state <> 0 Then
            ControlGPIO.VGI_SetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_BL_PIN)
        Else
            ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_BL_PIN)
        End If
        Return 0
    End Function

    ' reset led
    Public Function LCD_Reset()
        ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_RST_PIN)
        Thread.Sleep(10)
        ControlGPIO.VGI_SetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_RST_PIN)
        Return 0
    End Function

    ' Write 1 byte data to 5110
    Public Function LCD_WriteByte(ByVal Data As Byte, ByVal Command As Byte)
        If Command <> 0 Then
            ControlGPIO.VGI_SetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_DC_PIN)
        Else
            ControlGPIO.VGI_ResetPins(ControlGPIO.VGI_USBGPIO, 0, ControlPin.LCD_DC_PIN)
        End If
        Dim temp(1) As Byte
        temp(0) = Data
        ControlSPI.VSI_WriteBytes(ControlSPI.VSI_USBSPI, 0, 0, temp, 1)
        Return 0
    End Function

    ' Initialize 5110
    Public Function LCD_Init()
        ConfigAdapter()
        LCD_Reset()
        LCD_SetBacklightState(1)
        LCD_WriteByte(&H21, 0)
        LCD_WriteByte(&HC0, 0)
        LCD_WriteByte(&H6, 0)
        LCD_WriteByte(&H13, 0)
        LCD_WriteByte(&H20, 0)
        LCD_WriteByte(&HC, 0)
        Return 0
    End Function

    ' Set display position
    Public Function LCD_SetPosition(ByVal x As Byte, ByVal y As Byte)
        y = y Or &H40
        x = x Or &H80

        LCD_WriteByte(y, 0)
        LCD_WriteByte(x, 0)
        Return 0
    End Function

    ' Clear screen
    Public Function LCD_Clear()
        Dim t As Byte
        Dim k As Byte
        LCD_SetPosition(0, 0)
        For t = 0 To 5
            For k = 0 To 83
                LCD_WriteByte(0, 1)
            Next k
        Next t
        Return 0
    End Function


    ' Display a character
    Public Function LCD_DisplayChar(ByVal c As Char)
        Dim line As Byte
        Dim cc As Byte
        cc = Asc(c) - &H20
        For line = 0 To 5
            LCD_WriteByte(font6x8(cc, line), 1)
        Next line

        Return 0
    End Function


    Public Function LCD_WriteString(ByVal x As Byte, ByVal y As Byte, ByVal str As String)
        Console.WriteLine("Display {0} at(x={1},y={2})", str, x, y)
        LCD_SetPosition(x, y)

        For i = 0 To (str.Length - 1)
            LCD_DisplayChar(str.ToCharArray()(i))
        Next i

        Return 0
    End Function


    Dim font6x8 As Byte(,) =
                        {{&H0, &H0, &H0, &H0, &H0, &H0},
                        {&H0, &H0, &H0, &H2F, &H0, &H0},
                        {&H0, &H0, &H7, &H0, &H7, &H0},
                        {&H0, &H14, &H7F, &H14, &H7F, &H14},
                        {&H0, &H24, &H2A, &H7F, &H2A, &H12},
                        {&H0, &H62, &H64, &H8, &H13, &H23},
                        {&H0, &H36, &H49, &H55, &H22, &H50},
                        {&H0, &H0, &H5, &H3, &H0, &H0},
                        {&H0, &H0, &H1C, &H22, &H41, &H0},
                        {&H0, &H0, &H41, &H22, &H1C, &H0},
                        {&H0, &H14, &H8, &H3E, &H8, &H14},
                        {&H0, &H8, &H8, &H3E, &H8, &H8},
                        {&H0, &H0, &H0, &HA0, &H60, &H0},
                        {&H0, &H8, &H8, &H8, &H8, &H8},
                        {&H0, &H0, &H60, &H60, &H0, &H0},
                        {&H0, &H20, &H10, &H8, &H4, &H2},
                        {&H0, &H3E, &H51, &H49, &H45, &H3E},
                        {&H0, &H0, &H42, &H7F, &H40, &H0},
                        {&H0, &H42, &H61, &H51, &H49, &H46},
                        {&H0, &H21, &H41, &H45, &H4B, &H31},
                        {&H0, &H18, &H14, &H12, &H7F, &H10},
                        {&H0, &H27, &H45, &H45, &H45, &H39},
                        {&H0, &H3C, &H4A, &H49, &H49, &H30},
                        {&H0, &H1, &H71, &H9, &H5, &H3},
                        {&H0, &H36, &H49, &H49, &H49, &H36},
                        {&H0, &H6, &H49, &H49, &H29, &H1E},
                        {&H0, &H0, &H36, &H36, &H0, &H0},
                        {&H0, &H0, &H56, &H36, &H0, &H0},
                        {&H0, &H8, &H14, &H22, &H41, &H0},
                        {&H0, &H14, &H14, &H14, &H14, &H14},
                        {&H0, &H0, &H41, &H22, &H14, &H8},
                        {&H0, &H2, &H1, &H51, &H9, &H6},
                        {&H0, &H32, &H49, &H59, &H51, &H3E},
                        {&H0, &H7C, &H12, &H11, &H12, &H7C},
                        {&H0, &H7F, &H49, &H49, &H49, &H36},
                        {&H0, &H3E, &H41, &H41, &H41, &H22},
                        {&H0, &H7F, &H41, &H41, &H22, &H1C},
                        {&H0, &H7F, &H49, &H49, &H49, &H41},
                        {&H0, &H7F, &H9, &H9, &H9, &H1},
                        {&H0, &H3E, &H41, &H49, &H49, &H7A},
                        {&H0, &H7F, &H8, &H8, &H8, &H7F},
                        {&H0, &H0, &H41, &H7F, &H41, &H0},
                        {&H0, &H20, &H40, &H41, &H3F, &H1},
                        {&H0, &H7F, &H8, &H14, &H22, &H41},
                        {&H0, &H7F, &H40, &H40, &H40, &H40},
                        {&H0, &H7F, &H2, &HC, &H2, &H7F},
                        {&H0, &H7F, &H4, &H8, &H10, &H7F},
                        {&H0, &H3E, &H41, &H41, &H41, &H3E},
                        {&H0, &H7F, &H9, &H9, &H9, &H6},
                        {&H0, &H3E, &H41, &H51, &H21, &H5E},
                        {&H0, &H7F, &H9, &H19, &H29, &H46},
                        {&H0, &H46, &H49, &H49, &H49, &H31},
                        {&H0, &H1, &H1, &H7F, &H1, &H1},
                        {&H0, &H3F, &H40, &H40, &H40, &H3F},
                        {&H0, &H1F, &H20, &H40, &H20, &H1F},
                        {&H0, &H3F, &H40, &H38, &H40, &H3F},
                        {&H0, &H63, &H14, &H8, &H14, &H63},
                        {&H0, &H7, &H8, &H70, &H8, &H7},
                        {&H0, &H61, &H51, &H49, &H45, &H43},
                        {&H0, &H0, &H7F, &H41, &H41, &H0},
                        {&H0, &H55, &H2A, &H55, &H2A, &H55},
                        {&H0, &H0, &H41, &H41, &H7F, &H0},
                        {&H0, &H4, &H2, &H1, &H2, &H4},
                        {&H0, &H40, &H40, &H40, &H40, &H40},
                        {&H0, &H0, &H1, &H2, &H4, &H0},
                        {&H0, &H20, &H54, &H54, &H54, &H78},
                        {&H0, &H7F, &H48, &H44, &H44, &H38},
                        {&H0, &H38, &H44, &H44, &H44, &H20},
                        {&H0, &H38, &H44, &H44, &H48, &H7F},
                        {&H0, &H38, &H54, &H54, &H54, &H18},
                        {&H0, &H8, &H7E, &H9, &H1, &H2},
                        {&H0, &H18, &HA4, &HA4, &HA4, &H7C},
                        {&H0, &H7F, &H8, &H4, &H4, &H78},
                        {&H0, &H0, &H44, &H7D, &H40, &H0},
                        {&H0, &H40, &H80, &H84, &H7D, &H0},
                        {&H0, &H7F, &H10, &H28, &H44, &H0},
                        {&H0, &H0, &H41, &H7F, &H40, &H0},
                        {&H0, &H7C, &H4, &H18, &H4, &H78},
                        {&H0, &H7C, &H8, &H4, &H4, &H78},
                        {&H0, &H38, &H44, &H44, &H44, &H38},
                        {&H0, &HFC, &H24, &H24, &H24, &H18},
                        {&H0, &H18, &H24, &H24, &H18, &HFC},
                        {&H0, &H7C, &H8, &H4, &H4, &H8},
                        {&H0, &H48, &H54, &H54, &H54, &H20},
                        {&H0, &H4, &H3F, &H44, &H40, &H20},
                        {&H0, &H3C, &H40, &H40, &H20, &H7C},
                        {&H0, &H1C, &H20, &H40, &H20, &H1C},
                        {&H0, &H3C, &H40, &H30, &H40, &H3C},
                        {&H0, &H44, &H28, &H10, &H28, &H44},
                        {&H0, &H1C, &HA0, &HA0, &HA0, &H7C},
                        {&H0, &H44, &H64, &H54, &H4C, &H44},
                        {&H14, &H14, &H14, &H14, &H14, &H14}
                }

End Module
