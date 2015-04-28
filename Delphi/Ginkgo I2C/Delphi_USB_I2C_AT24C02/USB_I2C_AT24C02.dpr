program USB_I2C_AT24C02;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils,
  ControlI2C in 'ControlI2C.pas';
Var
ret,i:Integer;
I2C_Config:VII_INIT_CONFIG;
DeviceInfo:VII_BOARD_INFO;
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
    //Get device infomation
    ret := VII_ReadBoardInfo(0,@DeviceInfo);
    if ret <> ERR_SUCCESS then
      Writeln('Get device infomation error!')
    else
      Writeln('Device infomation:');
      Writeln('Product Name : '+PAnsiChar(@DeviceInfo.ProductName[0]));
      Writeln('Firmware Version : v'+Format('%d.%d.%d',[DeviceInfo.FirmwareVersion[1],DeviceInfo.FirmwareVersion[2],DeviceInfo.FirmwareVersion[3]]));
      Writeln('Hardware Version : v'+Format('%d.%d.%d',[DeviceInfo.HardwareVersion[1],DeviceInfo.HardwareVersion[2],DeviceInfo.HardwareVersion[3]]));
      Write('Serial Number : ');
      for i := 0 to 11 do
        Write(Format('%.2x',[DeviceInfo.SerialNumber[i]]));
      Writeln('');

    //Initialize the device (hardware control mode)
    I2C_Config.MasterMode := VII_MASTER;
    I2C_Config.ControlMode := VII_HCTL_MODE;
    I2C_Config.AddrType := VII_ADDR_7BIT;
    I2C_Config.SubAddrWidth := VII_SUB_ADDR_1BYTE;
    I2C_Config.ClockSpeed := 400000;
    ret := VII_InitI2C(VII_USBI2C,0,0,@I2C_Config);
    if ret <> ERR_SUCCESS then
      Writeln('Initialize device error!');
    //Write data
    for i:=0 to 7 do
      write_buffer[i] := i;
    ret := VII_WriteBytes(VII_USBI2C, 0, 0, $A0, $08, @write_buffer[0], 8);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    //Delay,to ensure that the data to complete
    Sleep(20);
    //Read data
    ret := VII_ReadBytes(VII_USBI2C, 0, 0, $A0, $08, @read_buffer[0], 8);
    if ret <> ERR_SUCCESS then
      Writeln('Read data error!')
    else
      Writeln('Read Data(Hex):');
      for i := 0 to 7 do
        Write(Format('%.2x ',[read_buffer[i]]));
      Writeln('');
    Readln;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
