program USB_GPIO_Test;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils,
  ControlGPIO in 'ControlGPIO.pas';
var
ret:Integer;
pin_value:Word;
begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    //Scan device(must call)
    ret := VGI_ScanDevice(1);
    if ret<=0 then
      Writeln('No device connect!');
    //Open device(must call)
    ret := VGI_OpenDevice(VGI_USBGPIO,0,0);
    if ret <> ERR_SUCCESS then
      Writeln('Open device error!');
    //将GPIO_7和GPIO_8设置成输出引脚
    ret := VGI_SetOutput(VGI_USBGPIO, 0, VGI_GPIO_PIN7 or VGI_GPIO_PIN8);
    if ret <> ERR_SUCCESS then
      Writeln('Set pin output error!')
    else
      Writeln('Set pin output success!');
    //将GPIO_7和GPIO_8输出高电平
    ret := VGI_SetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN7 or VGI_GPIO_PIN8);
    if ret <> ERR_SUCCESS then
      Writeln('Set pin high error!')
    else
      Writeln('Set pin high success!');
    //将GPIO_7和GPIO_8输出低电平
    ret := VGI_ResetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN7 or VGI_GPIO_PIN8);
    if ret <> ERR_SUCCESS then
      Writeln('Set pin low error!')
    else
      Writeln('Set pin low success!');
    //将GPIO_4和GPIO_5设置成输入引脚
    ret := VGI_SetInput(VGI_USBGPIO, 0, VGI_GPIO_PIN4 or VGI_GPIO_PIN5);
    if ret <> ERR_SUCCESS then
      Writeln('Set pin input error!')
    else
      Writeln('Set pin input success!');
    //获取GPIO_4和GPIO_5引脚状态
    ret := VGI_ReadDatas(VGI_USBGPIO, 0, VGI_GPIO_PIN4 or VGI_GPIO_PIN5, @pin_value);
    if ret <> ERR_SUCCESS then
      Writeln('Get pin data error!')
    else
      begin
        if (pin_value and VGI_GPIO_PIN4)=0 then
          Writeln('GPIO_4 is low-level!')
        else
          Writeln('GPIO_4 is high-level!');
        if (pin_value and VGI_GPIO_PIN5)=0 then
          Writeln('GPIO_5 is low-level!')
        else
          Writeln('GPIO_5 is high-level!');
      end;
    //将GPIO_4和GPIO_5引脚设置成开漏模式（需加上拉电阻，可当双向口）
    ret := VGI_SetOpenDrain(VGI_USBGPIO, 0, VGI_GPIO_PIN4 or VGI_GPIO_PIN5);
    if ret <> ERR_SUCCESS then
      Writeln('Set pin open drain error!')
    else
      Writeln('Set pin open drain success!');
    //将GPIO_4和GPIO_5输出高电平
    ret := VGI_SetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN4 or VGI_GPIO_PIN5);
    if ret <> ERR_SUCCESS then
      Writeln('Set pin high error!')
    else
      Writeln('Set pin high success!');
    //将GPIO_4和GPIO_5输出低电平
    ret := VGI_ResetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN4 or VGI_GPIO_PIN5);
    if ret <> ERR_SUCCESS then
      Writeln('Set pin low error!')
    else
      Writeln('Set pin low success!');
    //获取GPIO_4和GPIO_5引脚状态
    ret := VGI_ReadDatas(VGI_USBGPIO, 0, VGI_GPIO_PIN4 or VGI_GPIO_PIN5, @pin_value);
    if ret <> ERR_SUCCESS then
      Writeln('Get pin data error!')
    else
      begin
        if (pin_value and VGI_GPIO_PIN4)=0 then
          Writeln('GPIO_4 is low-level!')
        else
          Writeln('GPIO_4 is high-level!');
        if (pin_value and VGI_GPIO_PIN5)=0 then
          Writeln('GPIO_5 is low-level!')
        else
          Writeln('GPIO_5 is high-level!');
      end;
    //Close device
    ret := VGI_CloseDevice(VGI_USBGPIO, 0);
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
