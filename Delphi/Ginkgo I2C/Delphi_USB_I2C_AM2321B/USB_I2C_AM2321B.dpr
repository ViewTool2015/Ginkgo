program USB_I2C_AM2321B;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils,
  ControlI2C in 'ControlI2C.pas';
Var
ret,i:Integer;
I2C_Config:VII_INIT_CONFIG;
write_buffer:Array[0..1024] Of Byte;
read_buffer:Array[0..1024] Of Byte;
t,h:Double;
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
    //Loop read Temperature and humidity data
    while True do
    begin
        //Wake up AM2311 sensor
        VII_WriteBytes(VII_USBI2C, 0, 0, $B8, $00, @write_buffer[0], 1);
        //Send out read temperature and huminity command
        write_buffer[0] := $03;
        write_buffer[1] := $00;
        write_buffer[2] := $04;
        ret := VII_WriteBytes(VII_USBI2C, 0, 0, $B8, $00, @write_buffer[0], 3);
        if ret <> ERR_SUCCESS then
          Writeln('Write data error!');
        // Read out temperature and huminity
        ret := VII_ReadBytes(VII_USBI2C, 0, 0, $B8, $00, @read_buffer[0], 8);
        if ret <> ERR_SUCCESS then
          Writeln('Read data error!')
        else
        begin
          t := ((read_buffer[4] shl 8) or read_buffer[5]) / 10.0;
          h := ((read_buffer[2] shl 8) or read_buffer[3]) / 10.0;
          Writeln(Format('Temperature: %.1f',[t]));
          Writeln(Format('Humidity: %.1f',[h]));
        end;
        Sleep(1000);
    end;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
