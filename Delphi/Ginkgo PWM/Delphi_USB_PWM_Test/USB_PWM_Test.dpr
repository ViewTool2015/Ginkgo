program USB_PWM_Test;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils,
  ControlPWM in 'ControlPWM.pas';
var
ret,i:Integer;
PWM_Config:VPI_INIT_CONFIG;
Frequency:Array[0..7] Of Integer;
Pulse:Array[0..7] Of Byte;
begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    //Scan device(must call)
    ret := VPI_ScanDevice(1);
    if ret<=0 then
      Writeln('No device connect!');
    //Open device(must call)
    ret := VPI_OpenDevice(VPI_USBPWM,0,0);
    if ret <> ERR_SUCCESS then
      Writeln('Open device error!');
    //Device initialization
    PWM_Config.PWM_ChannelMask := VPI_PWM_CH0;
    PWM_Config.PWM_Frequency := 10000;
    PWM_Config.PWM_Mode := 0;
    PWM_Config.PWM_Polarity := 0;
    PWM_Config.PWM_Pulse := 50;
    ret := VPI_InitPWM(VPI_USBPWM, 0, @PWM_Config);
    if ret <> ERR_SUCCESS then
      Writeln('Initialize device error!')
    else
      Writeln('Initialize device success!');
    //Start PWM_CH0
    ret := VPI_StartPWM(VPI_USBPWM,0,VPI_PWM_CH0);
    if ret <> ERR_SUCCESS then
      Writeln('Start pwm error!')
    else
      Writeln('Start pwm success!');
    //To run a second
    Sleep(1000);
    //Stop PWM_CH0
    ret := VPI_StopPWM(VPI_USBPWM,0,VPI_PWM_CH0);
    if ret <> ERR_SUCCESS then
      Writeln('Stop pwm error!')
    else
      Writeln('Stop pwm success!');
    //Initialization all pwm channels
    PWM_Config.PWM_ChannelMask := VPI_PWM_ALL;
    PWM_Config.PWM_Frequency := 10000;
    PWM_Config.PWM_Mode := 0;
    PWM_Config.PWM_Polarity := 0;
    PWM_Config.PWM_Pulse := 50;
    ret := VPI_InitPWM(VPI_USBPWM, 0, @PWM_Config);
    if ret <> ERR_SUCCESS then
      Writeln('Initialize device error!')
    else
      Writeln('Initialize device success!');
    //Start PWM_CH0 and PWM_CH2
    ret := VPI_StartPWM(VPI_USBPWM,0,VPI_PWM_CH0 or VPI_PWM_CH2);
    if ret <> ERR_SUCCESS then
      Writeln('Start pwm error!')
    else
      Writeln('Start pwm success!');
    //To run a second
    Sleep(1000);
    //Adjust the PWM_CH0 clock frequency
    Frequency[0] := 50000;
    ret := VPI_SetPWMPeriod(VPI_USBPWM, 0, VPI_PWM_CH0, @Frequency[0]);
    if ret <> ERR_SUCCESS then
      Writeln('Set frequency error!')
    else
      Writeln('Set frequency success!');
    //Adjust PWM_CH2 duty ratio
    Pulse[2] := 80;
    ret := VPI_SetPWMPulse(VPI_USBPWM, 0, VPI_PWM_CH0, @Pulse[0]);
    if ret <> ERR_SUCCESS then
      Writeln('Set pulse error!')
    else
      Writeln('Set pulse success!');
    //To run a second
    Sleep(1000);
    //At the same time adjust PWM_CH0 and PWM_CH2 duty ratio
    Pulse[0] := 80;
    Pulse[2] := 20;
    ret := VPI_SetPWMPulse(VPI_USBPWM, 0, VPI_PWM_CH0, @Pulse[0]);
    if ret <> ERR_SUCCESS then
      Writeln('Set pulse error!')
    else
      Writeln('Set pulse success!');
    //To run a second
    Sleep(1000);
    //Stop PWM_ALL
    ret := VPI_StopPWM(VPI_USBPWM,0,VPI_PWM_ALL);
    if ret <> ERR_SUCCESS then
      Writeln('Stop pwm error!')
    else
      Writeln('Stop pwm success!');
    //Close device
    ret := VPI_CloseDevice(VPI_USBPWM, 0);
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
