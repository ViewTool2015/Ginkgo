program USB_SPI_AT45DB161D;

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
    SPI_Config.ClockSpeed := 36000000;
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
    ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 1, @read_buffer[0], 4);
    if ret <> ERR_SUCCESS then
      Writeln('Read flash ID error!')
    else
      Writeln(Format('Flash ID = 0x%.8X',[(read_buffer[0] shl 24)or(read_buffer[1] shl 16)or(read_buffer[2] shl 8)or(read_buffer[3])]));
    //Wait Busy
    repeat
      write_buffer[0] := $D7;
      ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 1, @read_buffer[0], 1);
      if ret <> ERR_SUCCESS then
        Writeln('Write data error!');
    until (read_buffer[0]and$80)<>0;
    //Page Earse
    write_buffer[0] := $81;
    write_buffer[1] := $00;
    write_buffer[2] := $00;
    write_buffer[3] := $00;
    ret := VSI_WriteBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 4);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    //Wait Busy
    repeat
      write_buffer[0] := $D7;
      ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 1, @read_buffer[0], 1);
      if ret <> ERR_SUCCESS then
        Writeln('Write data error!');
    until (read_buffer[0]and$80)<>0;
    //Write data to buffer
    write_buffer[0] := $87;
    write_buffer[1] := $00;
    write_buffer[2] := $00;
    write_buffer[3] := $00;
    for i := 0 to 527 do
      write_buffer[i+4] := Byte(i);
    ret := VSI_WriteBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 528+4);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    //Buffer to Main Memory Page Program without Built-in Erase
    write_buffer[0] := $89;
    write_buffer[1] := $00;
    write_buffer[2] := $00;
    write_buffer[3] := $00;
    ret := VSI_WriteBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 4);
    if ret <> ERR_SUCCESS then
      Writeln('Write data to main memory error!')
    else
      Writeln('Write data to main memory success!');
    //Wait Busy
    repeat
      write_buffer[0] := $D7;
      ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 1, @read_buffer[0], 1);
      if ret <> ERR_SUCCESS then
        Writeln('Write data error!');
    until (read_buffer[0]and$80)<>0;
    //Main Memory Page Read
    write_buffer[0] := $D2;
    write_buffer[1] := $00;
    write_buffer[2] := $00;
    write_buffer[3] := $00;
    write_buffer[4] := $00;
    ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 8, @read_buffer[0], 528);
    if ret <> ERR_SUCCESS then
      Writeln('Read data to buffer error!')
    else
    begin
      Writeln('Read Data(Hex):');
      for i := 0 to 527 do
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
