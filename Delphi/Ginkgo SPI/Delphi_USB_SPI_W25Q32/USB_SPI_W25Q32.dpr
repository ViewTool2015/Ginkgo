program USB_SPI_W25Q32;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils,
  ControlSPI in 'ControlSPI.pas';
var
ret,i:Integer;
SPI_Config:VSI_INIT_CONFIG;
write_buffer:Array[0..1024] Of Byte;
read_buffer:Array[0..1024] Of Byte;
begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    //Scan device(must call)
    ret := VSI_ScanDevice(1);
    if ret<=0 then
      Writeln('No device connect!');
    //Open device(must call)
    ret := VSI_OpenDevice(VSI_USBSPI,0,0);
    if ret <> ERR_SUCCESS then
      Writeln('Open device error!');
    //Device initialization
    SPI_Config.ControlMode := 1;
    SPI_Config.MasterMode := 1;
    SPI_Config.ClockSpeed := 1125000;
    SPI_Config.CPHA := 0;
    SPI_Config.CPOL := 0;
    SPI_Config.LSBFirst := 0;
    SPI_Config.TranBits := 8;
    SPI_Config.SelPolarity := 0;
    ret := VSI_InitSPI(VSI_USBSPI, 0, @SPI_Config);
    if ret <> ERR_SUCCESS then
      Writeln('Initialize device error!')
    else
      Writeln('Initialize device success!');
    //Get JEDEC ID
    write_buffer[0] := $9F;
    ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 1, @read_buffer[0], 3);
    if ret <> ERR_SUCCESS then
      Writeln('Read flash ID error!')
    else
      Writeln(Format('Flash ID = 0x%.6X',[(read_buffer[0] shl 16)or(read_buffer[1] shl 8)or(read_buffer[2])]));
    //Write Enable
    write_buffer[0] := $06;
    ret := VSI_WriteBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 1);
    if ret <> ERR_SUCCESS then
      Writeln('Flash write enable error!')
    else
      Writeln('Write enable success!');
    //Sector Erase (4KB)
    write_buffer[0] := $20;
    write_buffer[1] := $00;
    write_buffer[2] := $00;
    write_buffer[3] := $00;
    ret := VSI_WriteBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 4);
    if ret <> ERR_SUCCESS then
      Writeln('Sector Erase start error!')
    else
      Writeln('Sector erase start success!');
    //Check the operation to complete
    repeat
      write_buffer[0] := $05;
      ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 1, @read_buffer[0], 1);
      if ret <> ERR_SUCCESS then
        Writeln('Write data error!');
    until (read_buffer[0]and$01)=0;
    //Write Enable
    write_buffer[0] := $06;
    ret := VSI_WriteBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 1);
    if ret <> ERR_SUCCESS then
      Writeln('Flash write enable error!')
    else
      Writeln('Write enable success!');
    //Page Program
    write_buffer[0] := $02;
    write_buffer[1] := $00;
    write_buffer[2] := $00;
    write_buffer[3] := $00;
    for i := 0 to 255 do
      write_buffer[i+4] := Byte(i);
    ret := VSI_WriteBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 256+4);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    //Check the operation to complete
    repeat
      write_buffer[0] := $05;
      ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 1, @read_buffer[0], 1);
      if ret <> ERR_SUCCESS then
        Writeln('Write data error!');
    until (read_buffer[0]and$01)=0;
    //Read Data
    write_buffer[0] := $03;
    write_buffer[1] := $00;
    write_buffer[2] := $00;
    write_buffer[3] := $00;
    ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 4, @read_buffer[0], 256);
    if ret <> ERR_SUCCESS then
      Writeln('Read data to buffer error!')
    else
    begin
      Writeln('Read Data(Hex):');
      for i := 0 to 255 do
      begin
        Write(Format('%.2X ',[read_buffer[i]]));
        if ((i+1) Mod 16)=0 then
          Writeln('')
      end;
    end;
    //Close device
    ret := VSI_CloseDevice(VSI_USBSPI, 0);
    if ret <> ERR_SUCCESS then
      Writeln('Close device error!')
    else
      Writeln('Close device success!');
    Readln;

  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
