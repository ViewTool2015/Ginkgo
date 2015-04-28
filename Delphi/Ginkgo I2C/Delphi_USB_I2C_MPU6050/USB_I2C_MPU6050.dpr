program USB_I2C_MPU6050;

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
AccelX, AccelY, AccelZ:Smallint;
GyroX, GyroY, GyroZ:Smallint;
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
    //Initialize MPU6050 sensor
    write_buffer[0] := $00;
    ret := VII_WriteBytes(VII_USBI2C, 0, 0, $D0, $6B, @write_buffer[0], 1);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    Sleep(10);
    write_buffer[0] := $00;
    ret := VII_WriteBytes(VII_USBI2C, 0, 0, $D0, $6C, @write_buffer[0], 1);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    Sleep(10);
      write_buffer[0] := $08;
    ret := VII_WriteBytes(VII_USBI2C, 0, 0, $D0, $1B, @write_buffer[0], 1);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    Sleep(10);
    write_buffer[0] := $01;
    ret := VII_WriteBytes(VII_USBI2C, 0, 0, $D0, $1A, @write_buffer[0], 1);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    Sleep(10);
      write_buffer[0] := $08;
    ret := VII_WriteBytes(VII_USBI2C, 0, 0, $D0, $1C, @write_buffer[0], 1);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    Sleep(10);
    while True do
    begin
      //Get data
      ret := VII_ReadBytes(VII_USBI2C, 0, 0, $D0, $3B, @read_buffer[0], 14);
      if ret <> ERR_SUCCESS then
        Writeln('Read data error!')
      else
        begin
        //Process the data
        AccelX := ((read_buffer[0] shl 8) or read_buffer[1]);
        AccelY := ((read_buffer[2] shl 8) or read_buffer[3]);
        AccelZ := ((read_buffer[4] shl 8) or read_buffer[5]);
        GyroX := ((read_buffer[8] shl 8) or read_buffer[9]);
        GyroY := ((read_buffer[10] shl 8) or read_buffer[11]);
        GyroZ := ((read_buffer[12] shl 8) or read_buffer[13]);
        //Display the data
        Writeln(Format('AccelX = %d',[AccelX]));
        Writeln(Format('AccelY = %d',[AccelY]));
        Writeln(Format('AccelZ = %d',[AccelZ]));
        Writeln(Format('GyroX = %d',[GyroX]));
        Writeln(Format('GyroY = %d',[GyroY]));
        Writeln(Format('GyroZ = %d',[GyroZ]));
        Writeln('');
        Writeln('');
        Sleep(100);
        end;
    end;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
