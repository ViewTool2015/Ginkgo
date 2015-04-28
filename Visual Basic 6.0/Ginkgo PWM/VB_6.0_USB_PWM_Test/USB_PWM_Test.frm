VERSION 5.00
Begin VB.Form MainForm 
   Caption         =   "USB-PWM Test"
   ClientHeight    =   3075
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   7035
   LinkTopic       =   "Form1"
   ScaleHeight     =   3075
   ScaleWidth      =   7035
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton StopCommand 
      Caption         =   "Stop"
      Height          =   375
      Left            =   5160
      TabIndex        =   9
      Top             =   2280
      Width           =   1335
   End
   Begin VB.CommandButton StartCommand 
      Caption         =   "Start"
      Height          =   375
      Left            =   3840
      TabIndex        =   8
      Top             =   2280
      Width           =   1215
   End
   Begin VB.TextBox DutyCycle 
      Height          =   285
      Left            =   4920
      TabIndex        =   7
      Text            =   "50"
      Top             =   960
      Width           =   1575
   End
   Begin VB.TextBox Frequency 
      Height          =   285
      Left            =   4920
      TabIndex        =   5
      Text            =   "100000"
      Top             =   360
      Width           =   1575
   End
   Begin VB.ComboBox ChannelCombo 
      Height          =   315
      ItemData        =   "USB_PWM_Test.frx":0000
      Left            =   1680
      List            =   "USB_PWM_Test.frx":001C
      Style           =   2  'Dropdown List
      TabIndex        =   3
      Top             =   840
      Width           =   1575
   End
   Begin VB.ComboBox DeviceCombo 
      Height          =   315
      ItemData        =   "USB_PWM_Test.frx":0048
      Left            =   1680
      List            =   "USB_PWM_Test.frx":006A
      Style           =   2  'Dropdown List
      TabIndex        =   1
      Top             =   360
      Width           =   1575
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Right Justify
      Caption         =   "Duty Cycle:"
      Height          =   255
      Left            =   3480
      TabIndex        =   6
      Top             =   960
      Width           =   1215
   End
   Begin VB.Label TEXT 
      Alignment       =   1  'Right Justify
      Caption         =   "Frequency:"
      Height          =   255
      Left            =   3600
      TabIndex        =   4
      Top             =   480
      Width           =   1095
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Right Justify
      Caption         =   "Channel Index:"
      Height          =   375
      Left            =   360
      TabIndex        =   2
      Top             =   840
      Width           =   1095
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "Device Index:"
      Height          =   255
      Left            =   360
      TabIndex        =   0
      Top             =   360
      Width           =   1095
   End
End
Attribute VB_Name = "MainForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False



Private Sub StartCommand_Click()
    Dim ret As Integer
    'Scanning equipment connections , you must call
    ret = VPI_ScanDevice(1)
    If ret <= 0 Then
        MsgBox ("No device connected!")
        Exit Sub
    End If
    'Open the specified device , you must call
    ret = VPI_OpenDevice(ControlPWM.VPI_USBPWM, DeviceCombo.ListIndex, 0)
    If ret <> ControlPWM.SUCCESS Then
        MsgBox ("Open device error!")
        Exit Sub
    End If
    'Initialize the device
    Dim InitConfig As VPI_INIT_CONFIG
    InitConfig.PWM_ChannelMask = 1
    For i = 0 To (ChannelCombo.ListIndex - 1)
        InitConfig.PWM_ChannelMask = InitConfig.PWM_ChannelMask * 2
    Next i
    InitConfig.PWM_Frequency = Val(Frequency.TEXT)
    InitConfig.PWM_Pulse = Val(DutyCycle.TEXT)
    ret = VPI_InitPWM(ControlPWM.VPI_USBPWM, DeviceCombo.ListIndex, InitConfig)
    If ret <> ControlPWM.SUCCESS Then
        MsgBox ("Init device error!")
        Exit Sub
    End If
    'Start PWM
    ret = VPI_StartPWM(ControlPWM.VPI_USBPWM, DeviceCombo.ListIndex, InitConfig.PWM_ChannelMask)
    If ret <> ControlPWM.SUCCESS Then
        MsgBox ("Start PWM error!")
        Exit Sub
    End If
End Sub


Private Sub StopCommand_Click()
    Dim ret As Integer
    Dim InitConfig As VPI_INIT_CONFIG
    InitConfig.PWM_ChannelMask = 1
    For i = 0 To (ChannelCombo.ListIndex - 1)
        InitConfig.PWM_ChannelMask = InitConfig.PWM_ChannelMask * 2
    Next i
    'Scanning equipment connections , you must call
    ret = VPI_ScanDevice(1)
    If ret <= 0 Then
        MsgBox ("No device connected!")
        Exit Sub
    End If
    'Open the specified device , you must call
    ret = VPI_OpenDevice(ControlPWM.VPI_USBPWM, DeviceCombo.ListIndex, 0)
    If ret <> ControlPWM.SUCCESS Then
        MsgBox ("Open device error!")
        Exit Sub
    End If
    'Stop PWM
    ret = VPI_StopPWM(ControlPWM.VPI_USBPWM, DeviceCombo.ListIndex, InitConfig.PWM_ChannelMask)
    If ret <> ControlPWM.SUCCESS Then
        MsgBox ("Start PWM error!")
        Exit Sub
    End If
End Sub
