object SDIAppForm: TSDIAppForm
  Left = 197
  Top = 111
  Caption = 'Ginkgo USB-I2C AT24C02'
  ClientHeight = 158
  ClientWidth = 400
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'System'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 16
  object Label1: TLabel
    Left = 0
    Top = 40
    Width = 107
    Height = 16
    Caption = 'Write Data(Hex):'
  end
  object Label2: TLabel
    Left = 0
    Top = 88
    Width = 106
    Height = 16
    Caption = 'Read Data(Hex):'
  end
  object WriteDataEdit: TEdit
    Left = 113
    Top = 37
    Width = 184
    Height = 24
    ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    TabOrder = 0
    Text = '00 11 22 33 44 55 66 77'
  end
  object ReadDataEdit: TEdit
    Left = 112
    Top = 88
    Width = 185
    Height = 24
    ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    TabOrder = 1
  end
  object WriteDataButton: TButton
    Left = 303
    Top = 37
    Width = 75
    Height = 25
    Caption = 'Write'
    TabOrder = 2
    OnClick = WriteDataButtonClick
  end
  object ReadDataButton: TButton
    Left = 303
    Top = 85
    Width = 75
    Height = 25
    Caption = 'Read'
    TabOrder = 3
    OnClick = ReadDataButtonClick
  end
end
