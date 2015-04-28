VERSION 5.00
Begin VB.Form MainForm 
   Caption         =   "USB-GPIO Test"
   ClientHeight    =   1740
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   3270
   LinkTopic       =   "Form1"
   ScaleHeight     =   1740
   ScaleWidth      =   3270
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   15
      Left            =   240
      TabIndex        =   17
      Top             =   240
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   14
      Left            =   600
      TabIndex        =   16
      Top             =   240
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   13
      Left            =   960
      TabIndex        =   15
      Top             =   240
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   12
      Left            =   1320
      TabIndex        =   14
      Top             =   240
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   11
      Left            =   1680
      TabIndex        =   13
      Top             =   240
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   10
      Left            =   2040
      TabIndex        =   12
      Top             =   240
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   9
      Left            =   2400
      TabIndex        =   11
      Top             =   240
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   8
      Left            =   2760
      TabIndex        =   10
      Top             =   240
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   7
      Left            =   240
      TabIndex        =   9
      Top             =   720
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   6
      Left            =   600
      TabIndex        =   8
      Top             =   720
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   5
      Left            =   960
      TabIndex        =   7
      Top             =   720
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   4
      Left            =   1320
      TabIndex        =   6
      Top             =   720
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   3
      Left            =   1680
      TabIndex        =   5
      Top             =   720
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   2
      Left            =   2040
      TabIndex        =   4
      Top             =   720
      Width           =   255
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   1
      Left            =   2400
      TabIndex        =   3
      Top             =   720
      Width           =   255
   End
   Begin VB.CommandButton ReadCommand 
      Caption         =   "Read"
      Height          =   375
      Left            =   1920
      TabIndex        =   2
      Top             =   1200
      Width           =   1095
   End
   Begin VB.CommandButton WriteCommand 
      Caption         =   "Write"
      Height          =   375
      Left            =   240
      TabIndex        =   1
      Top             =   1200
      Width           =   1095
   End
   Begin VB.CheckBox GPIOCheck 
      Caption         =   "GPIO_0"
      Height          =   255
      Index           =   0
      Left            =   2760
      TabIndex        =   0
      Top             =   720
      Width           =   255
   End
End
Attribute VB_Name = "MainForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub ReadCommand_Click()
    Dim ret As Integer
    'Scanning equipment connections , you must call
    ret = VGI_ScanDevice(1)
    If ret <= 0 Then
        MsgBox ("No device connected!")
        Exit Sub
    End If
    'Open the specified device , you must call
    ret = VGI_OpenDevice(ControlGPIO.VPI_USBGPIO, 0, 0)
    If ret <> ControlGPIO.SUCCESS Then
        MsgBox ("Open device error!")
        Exit Sub
    End If
    'Get Data
    Dim PinData As Integer
    Dim ShiftData As Long
    PinData = 0
    ShiftData = &H1
    For i = 0 To 15
        If GPIOCheck(i).Value = vbChecked Then
            PinData = PinData And ShiftData
        End If
        ShiftData = ShiftData * 2
    Next i
    'Read Data
    ret = VGI_SetInput(ControlGPIO.VPI_USBGPIO, 0, &HFFFF)
    If ret <> ControlGPIO.SUCCESS Then
        MsgBox ("Init device error!")
        Exit Sub
    End If
    ret = VGI_ReadDatas(ControlGPIO.VPI_USBGPIO, 0, &HFFFF, PinData)
    If ret <> ControlGPIO.SUCCESS Then
        MsgBox ("Read Pin Data error!")
        Exit Sub
    End If
    ShiftData = &H1
    For i = 0 To 15
        If (PinData And ShiftData) = ShiftData Then
            GPIOCheck(i).Value = vbChecked
        Else
            GPIOCheck(i).Value = vbUnchecked
        End If
        ShiftData = ShiftData * 2
    Next i
End Sub

Private Sub WriteCommand_Click()
    Dim ret As Integer
    'Scanning equipment connections , you must call
    ret = VGI_ScanDevice(1)
    If ret <= 0 Then
        MsgBox ("No device connected!")
        Exit Sub
    End If
    'Open the specified device , you must call
    ret = VGI_OpenDevice(ControlGPIO.VPI_USBGPIO, 0, 0)
    If ret <> ControlGPIO.SUCCESS Then
        MsgBox ("Open device error!")
        Exit Sub
    End If
    'Get Data
    Dim PinData As Integer
    Dim ShiftData As Long
    PinData = 0
    ShiftData = &H1
    For i = 0 To 15
        If GPIOCheck(i).Value = vbChecked Then
            If i = 15 Then
                PinData = PinData Or &H8000
            Else
                PinData = PinData Or ShiftData
            End If
        End If
        ShiftData = ShiftData * 2
    Next i
    'Write Data
    ret = VGI_SetOutput(ControlGPIO.VPI_USBGPIO, 0, &HFFFF)
    If ret <> ControlGPIO.SUCCESS Then
        MsgBox ("Init device error!")
        Exit Sub
    End If
    ret = VGI_SetPins(ControlGPIO.VPI_USBGPIO, 0, PinData)
    If ret <> ControlGPIO.SUCCESS Then
        MsgBox ("Write Pin Data error!")
        Exit Sub
    End If
    ret = VGI_ResetPins(ControlGPIO.VPI_USBGPIO, 0, Not PinData)
    If ret <> ControlGPIO.SUCCESS Then
        MsgBox ("Write Pin Data error!")
        Exit Sub
    End If
End Sub
