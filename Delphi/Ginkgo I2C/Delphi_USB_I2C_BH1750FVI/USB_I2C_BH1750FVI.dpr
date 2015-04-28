program USB_I2C_BH1750FVI;

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
illuminance:Double;
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
    I2C_Config.SubAddrWidth := VII_SUB_ADDR_NONE;
    I2C_Config.ClockSpeed := 100000;
    ret := VII_InitI2C(VII_USBI2C,0,0,@I2C_Config);
    if ret <> ERR_SUCCESS then
      Writeln('Initialize device error!');
    //Send measurement commands
    write_buffer[0] := $01;
    ret := VII_WriteBytes(VII_USBI2C, 0, 0, $46, $00, @write_buffer[0], 1);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    //Start measuring under 1 lx resolution
    write_buffer[0] := $10;
    ret := VII_WriteBytes(VII_USBI2C, 0, 0, $46, $00, @write_buffer[0], 1);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    //Loop read Temperature and humidity data
    while True do
    begin
      //Delay, waiting for the measurement is complete
      Sleep(1000);
      ret := VII_ReadBytes(VII_USBI2C, 0, 0, $46, $00, @read_buffer[0], 2);
      if ret <> ERR_SUCCESS then
        Writeln('Read data error!')
      else
      begin
        illuminance := ((read_buffer[0] shl 8) or read_buffer[1]) / 1.2;
        Writeln(Format('Illuminance: %.2f lx',[illuminance]));
      end;
    end;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
