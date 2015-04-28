VERSION 5.00
Begin VB.Form MainForm 
   Caption         =   "USB-I2C 24C02"
   ClientHeight    =   2400
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   7395
   LinkTopic       =   "USB-I2C 24C02"
   ScaleHeight     =   2400
   ScaleWidth      =   7395
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton ReadBoardInfoCommand 
      Caption         =   "ReadInfo"
      Height          =   375
      Left            =   6240
      TabIndex        =   10
      Top             =   240
      Width           =   1095
   End
   Begin VB.CommandButton ReadCommand 
      Caption         =   "Read"
      Height          =   375
      Left            =   6240
      TabIndex        =   9
      Top             =   1440
      Width           =   1095
   End
   Begin VB.CommandButton WriteCommand 
      Caption         =   "Write"
      Height          =   375
      Left            =   6240
      TabIndex        =   8
      Top             =   840
      Width           =   1095
   End
   Begin VB.TextBox ReadText 
      Height          =   405
      Left            =   1560
      TabIndex        =   7
      Top             =   1440
      Width           =   4455
   End
   Begin VB.TextBox WriteText 
      Height          =   405
      Left            =   1560
      TabIndex        =   4
      Text            =   "11 22 33 44 55 66 77 88"
      Top             =   840
      Width           =   4455
   End
   Begin VB.ComboBox ChannelCombo 
      Height          =   315
      ItemData        =   "MainForm.frx":0000
      Left            =   4560
      List            =   "MainForm.frx":000A
      Style           =   2  'Dropdown List
      TabIndex        =   1
      Top             =   240
      Width           =   1455
   End
   Begin VB.ComboBox DeviceCombo 
      Height          =   315
      ItemData        =   "MainForm.frx":0014
      Left            =   1560
      List            =   "MainForm.frx":0036
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   240
      Width           =   1455
   End
   Begin VB.Label Label3 
      Alignment       =   1  'Right Justify
      Caption         =   "Read Data(Hex):"
      Height          =   375
      Left            =   120
      TabIndex        =   6
      Top             =   1440
      Width           =   1335
   End
   Begin VB.Label Label2 
      Alignment       =   1  'Right Justify
      Caption         =   "Write Data(Hex):"
      Height          =   375
      Left            =   120
      TabIndex        =   5
      Top             =   840
      Width           =   1335
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "Channel Index:"
      Height          =   255
      Index           =   1
      Left            =   3360
      TabIndex        =   3
      Top             =   240
      Width           =   1095
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "Device Index:"
      Height          =   255
      Index           =   0
      Left            =   360
      TabIndex        =   2
      Top             =   240
      Width           =   1095
   End
End
Attribute VB_Name = "MainForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub ReadBoardInfoCommand_Click()
    Dim ret As Integer
    'Scanning equipment connections , you must call
    ret = VII_ScanDevice(1)
    If ret <= 0 Then
        MsgBox ("No device connected")
        Exit Sub
    End If
    'Open the specified device , you must call
    ret = VII_OpenDevice(ControlI2C.VII_USBI2C, DeviceCombo.ListIndex, 0)
    If ret <> ControlI2C.SUCCESS Then
        MsgBox ("Open device error")
        Exit Sub
    End If
    'Get information about the device
    Dim I2C_Info As ControlI2C.VII_BOARD_INFO
    ret = VII_ReadBoardInfo(DeviceCombo.ListIndex, I2C_Info)
    If ret <> ControlI2C.SUCCESS Then
        MsgBox ("Get device info error")
        Exit Sub
    Else
        Dim ProductInfo As String
        Dim FirmwareVersion As String
        Dim HardwareVersion As String
        Dim ProductName As String
        ProductName = "Product Name : " + StrConv(I2C_Info.ProductName, vbUnicode) + vbNewLine
        FirmwareVersion = "Firmware Version : " + CStr(I2C_Info.FirmwareVersion(1)) + "." + CStr(I2C_Info.FirmwareVersion(2)) + "." + CStr(I2C_Info.FirmwareVersion(3)) + vbNewLine
        HardwareVersion = "Hardware Version : " + CStr(I2C_Info.HardwareVersion(1)) + "." + CStr(I2C_Info.HardwareVersion(2)) + "." + CStr(I2C_Info.HardwareVersion(3)) + vbNewLine
        ProductInfo = ProductName + FirmwareVersion + HardwareVersion
        MsgBox (FirmwareVersion + HardwareVersion + ProductName)
    End If
End Sub

Private Sub ReadCommand_Click()
    Dim ret As Integer
    'Scanning equipment connections , you must call
    ret = VII_ScanDevice(1)
    If ret <= 0 Then
        MsgBox ("No device connected")
        Exit Sub
    End If
    'Open the specified device , you must call
    ret = VII_OpenDevice(ControlI2C.VII_USBI2C, DeviceCombo.ListIndex, 0)
    If ret <> ControlI2C.SUCCESS Then
        MsgBox ("Open device error")
        Exit Sub
    End If
    'Initialize the device ( hardware control mode )
    Dim I2C_Config As ControlI2C.VII_INIT_CONFIG
    I2C_Config.AddrType = ControlI2C.VII_ADDR_7BIT
    I2C_Config.ClockSpeed = 400000
    I2C_Config.ControlMode = ControlI2C.VII_HCTL_MODE
    I2C_Config.MasterMode = ControlI2C.VII_MASTER
    I2C_Config.SubAddrWidth = ControlI2C.VII_SUB_ADDR_1BYTE
    ret = VII_InitI2C(ControlI2C.VII_USBI2C, DeviceCombo.ListIndex, ChannelCombo.ListIndex, I2C_Config)
    If ret <> ControlI2C.SUCCESS Then
        MsgBox ("Initialize device error")
        Exit Sub
    End If
    'Subaddress 0x00 to read 8 bytes of data
    Dim read_buffer(10240) As Byte
    Dim device_addr As Integer
    Dim sub_addr As Long
    Dim i As Integer
    Dim StrTmp As String
    device_addr = &HA0
    sub_addr = &H0
    ret = VII_ReadBytes(ControlI2C.VII_USBI2C, DeviceCombo.ListIndex, ChannelCombo.ListIndex, device_addr, sub_addr, VarPtr(read_buffer(0)), 8)
    If ret <> ControlI2C.SUCCESS Then
        MsgBox ("Read data error")
    Else
        For i = 0 To 7
            StrTmp = StrTmp + Hex(read_buffer(i)) + " "
        Next i
        ReadText.Text = StrTmp
    End If
End Sub

Private Sub WriteCommand_Click()
    Dim ret As Integer
    'Scanning equipment connections , you must call
    ret = VII_ScanDevice(1)
    If ret <= 0 Then
        MsgBox ("No device connected")
        Exit Sub
    End If
    'Open the specified device , you must call
    ret = VII_OpenDevice(ControlI2C.VII_USBI2C, DeviceCombo.ListIndex, 0)
    If ret <> ControlI2C.SUCCESS Then
        MsgBox ("Open device error")
        Exit Sub
    End If
    'Initialize the device ( hardware control mode )
    Dim I2C_Config As ControlI2C.VII_INIT_CONFIG
    I2C_Config.AddrType = ControlI2C.VII_ADDR_7BIT
    I2C_Config.ClockSpeed = 400000
    I2C_Config.ControlMode = ControlI2C.VII_HCTL_MODE
    I2C_Config.MasterMode = ControlI2C.VII_MASTER
    I2C_Config.SubAddrWidth = ControlI2C.VII_SUB_ADDR_1BYTE
    ret = VII_InitI2C(ControlI2C.VII_USBI2C, DeviceCombo.ListIndex, ChannelCombo.ListIndex, I2C_Config)
    If ret <> ControlI2C.SUCCESS Then
        MsgBox ("Initialize device error")
        Exit Sub
    End If
    'Subaddress 0x00 to write 8 bytes of data
    Dim write_buffer(10240) As Byte
    Dim device_addr As Integer
    Dim sub_addr As Long
    Dim i As Integer
    Dim StrTmp() As String
    StrTmp = Split(WriteText, " ")
    For i = 0 To UBound(StrTmp)
        write_buffer(i) = Val("&H" + StrTmp(i))
    Next i
    device_addr = &HA0
    sub_addr = &H0
    ret = VII_WriteBytes(ControlI2C.VII_USBI2C, DeviceCombo.ListIndex, ChannelCombo.ListIndex, device_addr, sub_addr, VarPtr(write_buffer(0)), UBound(StrTmp) + 1)
    If ret <> ControlI2C.SUCCESS Then
        MsgBox ("Write data error")
    Else
        MsgBox ("Write data success")
    End If
End Sub
