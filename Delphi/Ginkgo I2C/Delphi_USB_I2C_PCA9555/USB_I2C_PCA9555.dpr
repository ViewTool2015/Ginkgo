program USB_I2C_PCA9555;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils,
  ControlI2C in 'ControlI2C.pas';
var
ret,i:Integer;
I2C_Config:VII_INIT_CONFIG;
write_buffer:Array[0..1024] Of Byte;
read_buffer:Array[0..1024] Of Byte;
begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    //Scan device(must call)
    ret := VII_ScanDevice(1);
    if ret<=0 then
      Writeln('No device connect!');
    //Open device(must call)
    ret := VII_OpenDevice(VII_USBI2C,0,0);
    if ret <> ERR_SUCCESS then
      Writeln('Open device error!');
    //Initialize the device (hardware control mode)
    I2C_Config.MasterMode := VII_MASTER;
    I2C_Config.ControlMode := VII_HCTL_MODE;
    I2C_Config.AddrType := VII_ADDR_7BIT;
    I2C_Config.SubAddrWidth := VII_SUB_ADDR_1BYTE;
    I2C_Config.ClockSpeed := 400000;
    ret := VII_InitI2C(VII_USBI2C,0,0,@I2C_Config);
    if ret <> ERR_SUCCESS then
      Writeln('Initialize device error!');
    //Set all ports to output mode
    write_buffer[0] := $00;
    write_buffer[1] := $00;
    ret := VII_WriteBytes(VII_USBI2C, 0, 0, $40, $06, @write_buffer[0], 2);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    while True do
    begin
      //Control all the port output low level
      write_buffer[0] := $FF;
      write_buffer[1] := $FF;
      ret := VII_WriteBytes(VII_USBI2C, 0, 0, $40, $02, @write_buffer[0], 2);
      if ret <> ERR_SUCCESS then
        Writeln('Write data error!');
      Sleep(500);
      //Control all the port output low level
      write_buffer[0] := $00;
      write_buffer[1] := $00;
      ret := VII_WriteBytes(VII_USBI2C, 0, 0, $40, $02, @write_buffer[0], 2);
      if ret <> ERR_SUCCESS then
        Writeln('Write data error!');
      Sleep(500);
    end;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
