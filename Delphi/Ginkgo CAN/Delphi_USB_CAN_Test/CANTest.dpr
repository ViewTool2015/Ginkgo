program CANTest;

{$APPTYPE CONSOLE}

{$R *.res}

{$DEFINE CAN_MODE_LOOP_BACK}
{$DEFINE CAN_SEND_DATA}
{$DEFINE CAN_GET_BOARD_INFO}
//{$DEFINE CAN_READ_DATA}
{$DEFINE CAN_CALLBACK_READ_DATA}
{$DEFINE CAN_INIT_EX}
{$DEFINE CAN_GET_STATUS}
uses
  System.SysUtils,
  ControlCAN in 'ControlCAN.pas';
{$ifdef CAN_MODE_LOOP_BACK}
procedure GetDataCallback(DevIndex,CANIndex,Len:integer);stdcall;
Var
i,j,ReadDataNum,DataNum:Integer;
pCAN_ReceiveData:Array Of VCI_CAN_OBJ;
begin
    DataNum := VCI_GetReceiveNum(VCI_USBCAN2, 0, 0);
    SetLength(pCAN_ReceiveData,DataNum);
    if DataNum > 0 then
    begin
      ReadDataNum := VCI_Receive(VCI_USBCAN2, 0, 0, @pCAN_ReceiveData[0], DataNum,0);
      for i := 0 to ReadDataNum-1 do
      begin
        Writeln('');
        Writeln(Format('--CAN_ReceiveData.RemoteFlag = %d',[pCAN_ReceiveData[i].RemoteFlag]));
        Writeln(Format('--CAN_ReceiveData.ExternFlag = %d',[pCAN_ReceiveData[i].ExternFlag]));
        Writeln(Format('--CAN_ReceiveData.ID = 0x%X',[pCAN_ReceiveData[i].ID]));
        Writeln(Format('--CAN_ReceiveData.DataLen = %d',[pCAN_ReceiveData[i].DataLen]));
        Write('--CAN_ReceiveData.Data:');
        for j := 0 to pCAN_ReceiveData[i].DataLen-1 do
          Write(Format('%.2X ',[pCAN_ReceiveData[i].Data[j]]));
        Writeln('');
        Writeln(Format('--CAN_ReceiveData.TimeStamp = %d',[pCAN_ReceiveData[i].TimeStamp]));
      end;
    end;
end;
{$endif}
Var
ret:Integer;
i,j,ReadDataNum,DataNum:Integer;
CAN_BoardInfo:VCI_BOARD_INFO_EX;
CAN_InitEx:VCI_INIT_CONFIG_EX;
CAN_FilterConfig:VCI_FILTER_CONFIG;
CAN_Init:VCI_INIT_CONFIG;
CAN_SendData:Array Of VCI_CAN_OBJ;
pCAN_ReceiveData:Array Of VCI_CAN_OBJ;
CAN_Status:VCI_CAN_STATUS;
begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    //Scan device(must call)
    ret := VCI_ScanDevice(1);
    if ret<=0 then
      Writeln('No device connect!')
    else
      Writeln(Format('Have %d device connected!',[ret]));
    //Get board info
{$ifdef CAN_GET_BOARD_INFO}
    ret := VCI_ReadBoardInfoEx(0,@CAN_BoardInfo);
    if ret <> STATUS_OK then
      Writeln('Get device infomation error!')
    else
      Writeln('Device infomation:');
      Writeln('Product Name : '+PAnsiChar(@CAN_BoardInfo.ProductName[0]));
      Writeln('Firmware Version : v'+Format('%d.%d.%d',[CAN_BoardInfo.FirmwareVersion[1],CAN_BoardInfo.FirmwareVersion[2],CAN_BoardInfo.FirmwareVersion[3]]));
      Writeln('Hardware Version : v'+Format('%d.%d.%d',[CAN_BoardInfo.HardwareVersion[1],CAN_BoardInfo.HardwareVersion[2],CAN_BoardInfo.HardwareVersion[3]]));
      Write('Serial Number : ');
      for i := 0 to 11 do
        Write(Format('%.2x',[CAN_BoardInfo.SerialNumber[i]]));
      Writeln('');
{$else}
    //Open device
    ret := VCI_OpenDevice(VCI_USBCAN2,0,0);
    if ret <> STATUS_OK then
      Writeln('Open device failed!')
    else
      Writeln('Open device success!')
{$endif}
{$ifdef CAN_INIT_EX}
    CAN_InitEx.CAN_ABOM := 0;
{$ifdef CAN_MODE_LOOP_BACK}
    CAN_InitEx.CAN_Mode := 1;
{$else}
    CAN_InitEx.CAN_Mode := 0;
{$endif}
    //1Mbps
    CAN_InitEx.CAN_BRP := 6;
    CAN_InitEx.CAN_BS1 := 3;
    CAN_InitEx.CAN_BS2 := 2;
    CAN_InitEx.CAN_SJW := 1;

    CAN_InitEx.CAN_NART := 0;
    CAN_InitEx.CAN_RFLM := 0;
    CAN_InitEx.CAN_TXFP := 1;
    CAN_InitEx.CAN_RELAY := 0;
    ret := VCI_InitCANEx(VCI_USBCAN2,0,0,@CAN_InitEx);
    if ret <> STATUS_OK then
      Writeln('Init device failed!')
    else
      Writeln('Init device success!');
    //Set filter
    CAN_FilterConfig.FilterIndex := 0;
    CAN_FilterConfig.Enable := 1;		//Enable
    CAN_FilterConfig.ExtFrame := 0;
    CAN_FilterConfig.FilterMode := 0;
    CAN_FilterConfig.ID_IDE := 0;
    CAN_FilterConfig.ID_RTR := 0;
    CAN_FilterConfig.ID_Std_Ext := 0;
    CAN_FilterConfig.MASK_IDE := 0;
    CAN_FilterConfig.MASK_RTR := 0;
    CAN_FilterConfig.MASK_Std_Ext := 0;
    ret := VCI_SetFilter(VCI_USBCAN2,0,0,@CAN_FilterConfig);
    if ret <> STATUS_OK then
      Writeln('Set filter failed!')
    else
      Writeln('Set filter success!');
{$else}
    //Config device
    CAN_Init.AccCode := $00000000;
    CAN_Init.AccMask := $FFFFFFFF;
    CAN_Init.Filter := 1;
    CAN_Init.Mode := 0;
    CAN_Init.Timing0 := $00;
    CAN_Init.Timing1 := $14;
    ret := VCI_InitCAN(VCI_USBCAN2,0,0,@CAN_Init);
    if ret <> STATUS_OK then
      Writeln('Init device failed!')
    else
      Writeln('Init device success!');
{$endif}
{$ifdef CAN_CALLBACK_READ_DATA}
    VCI_RegisterReceiveCallback(0,GetDataCallback);
{$endif}
    //Start CAN
    ret := VCI_StartCAN(VCI_USBCAN2,0,0);
    if ret <> STATUS_OK then
      Writeln('Start CAN failed!')
    else
      Writeln('Start CAN success!');
    //Send data
{$ifdef CAN_SEND_DATA}
    SetLength(CAN_SendData,2);
    for j := 0 to 1 do
    begin
      CAN_SendData[j].DataLen := 8;
      for i := 0 to CAN_SendData[j].DataLen-1 do
        CAN_SendData[j].Data[i] := i+j;
      CAN_SendData[j].ExternFlag := 0;
      CAN_SendData[j].RemoteFlag := 0;
      CAN_SendData[j].ID := $155+j;
{$ifdef CAN_MODE_LOOP_BACK}
      CAN_SendData[j].SendType := 2;
{$else}
      CAN_SendData[j].SendType := 0;
{$endif}
    end;
    ret := VCI_Transmit(VCI_USBCAN2,0,0,@CAN_SendData[0],2);
    if ret <> 2 then
      Writeln('Send CAN data failed!')
    else
      Writeln('Send CAN data success!');
{$endif}
    Sleep(20);
{$ifdef CAN_GET_STATUS}
    ret := VCI_ReadCANStatus(VCI_USBCAN2, 0, 0, @CAN_Status);
    if ret <> STATUS_OK then
      Writeln('Get CAN status failed!')
    else
    begin
      Writeln(Format('Buffer Size : %d',[CAN_Status.BufferSize]));
      Writeln(Format('ESR : 0x%.8X',[CAN_Status.regESR]));
      Writeln(Format('------Error warning flag : %d',[(CAN_Status.regESR shr 0) and $01]));
      Writeln(Format('------Error passive flag : %d',[(CAN_Status.regESR shr 1) and $01]));
      Writeln(Format('------Bus-off flag : %d',[(CAN_Status.regESR shr 2) and $01]));
      Write(Format('------Last error code(%d) : ',[(CAN_Status.regESR shr 4) and $07]));
      case ((CAN_Status.regESR shr 4) and $07) of
        0:begin
          Writeln('No Error');
        end;
        1:begin
          Writeln('Stuff Error');
        end;
        2:begin
          Writeln('Form Error');
        end;
        3:begin
          Writeln('Acknowledgment Error');
        end;
        4:begin
          Writeln('Bit recessive Error');
        end;
        5:begin
          Writeln('Bit dominant Error');
        end;
        6:begin
          Writeln('CRC Error');
        end;
        7:begin
          Writeln('Set by software');
        end;
      end;
      Writeln(Format('------Transmit error counter : %d',[(CAN_Status.regESR shr 16) and $FF]));
      Writeln(Format('------Receive error counter : %d',[(CAN_Status.regESR shr 24) and $FF]));
      Writeln(Format('TSR : 0x%.8X',[CAN_Status.regTSR]));
    end;
{$endif}

{$ifdef CAN_READ_DATA}
    DataNum := VCI_GetReceiveNum(VCI_USBCAN2, 0, 0);
    SetLength(pCAN_ReceiveData,DataNum);
    if DataNum > 0 then
    begin
      ReadDataNum := VCI_Receive(VCI_USBCAN2, 0, 0, @pCAN_ReceiveData[0], DataNum,0);
      for i := 0 to ReadDataNum-1 do
      begin
        Writeln('');
        Writeln(Format('--CAN_ReceiveData.RemoteFlag = %d',[pCAN_ReceiveData[i].RemoteFlag]));
        Writeln(Format('--CAN_ReceiveData.ExternFlag = %d',[pCAN_ReceiveData[i].ExternFlag]));
        Writeln(Format('--CAN_ReceiveData.ID = 0x%X',[pCAN_ReceiveData[i].ID]));
        Writeln(Format('--CAN_ReceiveData.DataLen = %d',[pCAN_ReceiveData[i].DataLen]));
        Write('--CAN_ReceiveData.Data:');
        for j := 0 to pCAN_ReceiveData[i].DataLen-1 do
          Write(Format('%.2X ',[pCAN_ReceiveData[i].Data[j]]));
        Writeln('');
        Writeln(Format('--CAN_ReceiveData.TimeStamp = %d',[pCAN_ReceiveData[i].TimeStamp]));
      end;
    end;
{$endif}
    Readln;
{$ifdef CAN_CALLBACK_READ_DATA}
	VCI_LogoutReceiveCallback(0);
	Writeln('VCI_LogoutReceiveCallback');
{$endif}
  Sleep(10);
	//Stop receive can data
	ret := VCI_ResetCAN(VCI_USBCAN2,0,0);
	Writeln(Format('VCI_ResetCAN %d',[ret]));
	VCI_CloseDevice(VCI_USBCAN2,0);
	Writeln('VCI_CloseDevice');
  Readln;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
