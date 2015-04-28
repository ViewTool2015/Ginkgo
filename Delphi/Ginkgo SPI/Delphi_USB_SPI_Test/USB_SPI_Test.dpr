program USB_SPI_Test;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils,
  ControlSPI in 'ControlSPI.pas';
var
ret,i:Integer;
SPI_Config:VSI_INIT_CONFIG;
DeviceInfo:VSI_BOARD_INFO;
write_buffer:Array[0..1024] Of Byte;
read_buffer:Array[0..1024] Of Byte;
write_bit_str:PChar;
read_data_num:Integer;
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
    //Get device infomation
    ret := VSI_ReadBoardInfo(0,@DeviceInfo);
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
    //初始化设备(主机硬件半双工模式)
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
    //SPI主机半双工模式写8字节数据
    for i := 0 to 7 do
      write_buffer[i] := i;
    ret := VSI_WriteBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 8);
    if ret <> ERR_SUCCESS then
      Writeln('Write data error!')
    else
      Writeln('Write data success!');
    //SPI主机半双工模式读8字节数据
    ret := VSI_ReadBytes(VSI_USBSPI, 0, 0, @read_buffer[0], 8);
    if ret <> ERR_SUCCESS then
      Writeln('Read data error!')
    else
    begin
      Writeln('Read data(Hex):');
      for i := 0 to 7 do
        Write(Format('%.2X ',[read_buffer[i]]));
      Writeln('');
    end;
    //SPI主机半双工模式发送读取数据,发送5字节数据后立即读8字节数据，整个操作期间片选信号一直有效
    for i := 0 to 4 do
      write_buffer[i] := i;
    ret := VSI_WriteReadBytes(VSI_USBSPI, 0, 0, @write_buffer[0], 5, @read_buffer[0], 8);
    if ret <> ERR_SUCCESS then
      Writeln('Read data error!')
    else
    begin
      Writeln('Read data(Hex):');
      for i := 0 to 7 do
        Write(Format('%.2X ',[read_buffer[i]]));
      Writeln('');
    end;
    //初始化设备(主机半双工模式)，软件模式下也支持VSI_WriteBytes，VSI_ReadBytes，VSI_WriteReadBytes这3个函数
    //VSI_WriteBits,VSI_ReadBits,VSI_WriteReadBits这三个函数不能在硬件件模式下使用，在单总线模式下也能使用
    SPI_Config.ControlMode := 2;
    SPI_Config.MasterMode := 1;
    SPI_Config.ClockSpeed := 100000;
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
    //二进制模式发送数据，发送bit数最大可达10240bit
    write_bit_str := '10110100100101';
    FillChar(write_buffer,SizeOf(write_buffer),0);
    for i := 0 to Length(write_bit_str)-1 do
      write_buffer[i] := Byte(write_bit_str[i]);
    ret := VSI_WriteBits(VSI_USBSPI, 0, 0, @write_buffer[0]);//发送14bit数据
    if ret <> ERR_SUCCESS then
      Writeln('Write bits error!')
    else
      Writeln('Write bits success!');
    //二进制模式读取数据,读取最大bit数可达10240bit
    FillChar(read_buffer,SizeOf(read_buffer),0);
    ret := VSI_ReadBits(VSI_USBSPI, 0, 0, @read_buffer[0],19);//读取19bit数据
    if ret <> ERR_SUCCESS then
      Writeln('Read bits error!')
    else
    begin
      Writeln('Read bits(bin):');
      Writeln(PAnsiChar(@read_buffer[0]));
    end;
    //二进制模式发送读取数据
    write_bit_str := '11010111';
    FillChar(write_buffer,SizeOf(write_buffer),0);
    FillChar(read_buffer,SizeOf(read_buffer),0);
    for i := 0 to Length(write_bit_str)-1 do
      write_buffer[i] := Byte(write_bit_str[i]);
    ret := VSI_WriteReadBits(VSI_USBSPI, 0, 0,@write_buffer[0],@read_buffer[0], 8);//读取8bit数据
    if ret <> ERR_SUCCESS then
      Writeln('Read bits error!')
    else
    begin
      Writeln('Read bits(bin):');
      Writeln(PAnsiChar(@read_buffer[0]));
    end;
    //初始化设备(从机模式)
    SPI_Config.ControlMode := 0;
    SPI_Config.MasterMode := 0;
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
    //从机模式写数据，数据不会立即从MISO引脚发送出去，必须要等到主机发送片选信号（CS低有效）并发送时钟后才将数据移位发送出去
    //但是调用该函数后会立即返回，数据存放到适配器内存缓冲区中
    ret := VSI_SlaveWriteBytes(VSI_USBSPI, 0, @write_buffer[0], 8);
    if ret <> ERR_SUCCESS then
      Writeln('Slave write data error!')
    else
      Writeln('Slave write data success!');
    //从机模式读数据,最后一个参数为等待时间，单位为微秒，超过等待时间后不管是否是否读到数据函数都立即返回
    //调用该函数后可以通过判断读数据字节数这个参数判断接收了多少字节数据
    //注意：在调用VSI_SlaveWriteBytes后，主机主动读取数据的同时，从机也会接收到数据（从机工作于全双工模式）
    //若主机在读取数据的时候不想要从机接收到的数据，可以在主机读取数据完成后调用VSI_SlaveReadBytes函数并丢弃返回的数据
    ret := VSI_SlaveReadBytes(VSI_USBSPI, 0, @read_buffer[0], @read_data_num,100);
    if ret <> ERR_SUCCESS then
      Writeln('Slave Read data error!')
    else
    begin
      if read_data_num > 0 then
      begin
        Writeln(Format('Read data num: %d ',[read_data_num]));
        Writeln('Read data(Hex):');
        for i := 0 to read_data_num-1 do
          Write(Format('%.2X ',[read_buffer[i]]));
      end
      else
        Writeln('No data!');
    end;
    //关闭设备
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
