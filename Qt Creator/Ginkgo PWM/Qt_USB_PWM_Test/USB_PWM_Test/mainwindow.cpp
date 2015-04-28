#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ControlPWM.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startCh1PushButton_clicked()
{
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    VPI_INIT_CONFIG PWM_Config;
    PWM_Config.PWM_ChannelMask = 0x01<<(ui->pwmIndexComboBox->currentIndex()*2);
    PWM_Config.PWM_Frequency = ui->freqSpinBox->value();
    PWM_Config.PWM_Mode = ui->pwmModeCh1ComboBox->currentIndex();
    PWM_Config.PWM_Polarity = ui->polarityCh1ComboBox->currentIndex();
    PWM_Config.PWM_Pulse = ui->dutyCycleCh1SpinBox->value();
    ret = VPI_InitPWM(VPI_USBPWM,
                      ui->deviceIndexComboBox->currentIndex(),
                      &PWM_Config);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialize device faild!");
        return;
    }
    ret = VPI_StartPWM(VPI_USBPWM,
                       ui->deviceIndexComboBox->currentIndex(),
                       PWM_Config.PWM_ChannelMask);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Start PWM faild!");
        return;
    }else{
        QMessageBox::information(this,"Information","Start PWM success!");
    }
}

void MainWindow::on_freqSpinBox_valueChanged(int arg1)
{
    ui->freqHorizontalSlider->setValue(arg1);
    uint8_t PWM_ChannelMask = 0x03<<(ui->pwmIndexComboBox->currentIndex()*2);
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    uint32_t pwm_period[8]={0};
    for(int i=0;i<8;i++){
        if((PWM_ChannelMask>>i)&0x01){
            pwm_period[i] = arg1;
        }
    }
    ret = VPI_SetPWMPeriod(VPI_USBPWM,
                          ui->deviceIndexComboBox->currentIndex(),
                          PWM_ChannelMask,
                          pwm_period);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Set duty cycle faild!");
        return;
    }
}

void MainWindow::on_freqHorizontalSlider_valueChanged(int value)
{
    ui->freqSpinBox->setValue(value);
    uint8_t PWM_ChannelMask = 0x03<<(ui->pwmIndexComboBox->currentIndex()*2);
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    uint32_t pwm_period[8]={0};
    for(int i=0;i<8;i++){
        if((PWM_ChannelMask>>i)&0x01){
            pwm_period[i] = value;
        }
    }
    ret = VPI_SetPWMPeriod(VPI_USBPWM,
                          ui->deviceIndexComboBox->currentIndex(),
                          PWM_ChannelMask,
                          pwm_period);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Set duty cycle faild!");
        return;
    }
}

void MainWindow::on_dutyCycleCh1SpinBox_valueChanged(int arg1)
{
    ui->dutyCycleCh1HorizontalSlider->setValue(arg1);
    uint8_t PWM_ChannelMask = 0x01<<(ui->pwmIndexComboBox->currentIndex()*2);
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    uint8_t pwm_pulse[8]={0};
    for(int i=0;i<8;i++){
        if((PWM_ChannelMask>>i)&0x01){
            pwm_pulse[i] = arg1;
        }
    }
    ret = VPI_SetPWMPulse(VPI_USBPWM,
                          ui->deviceIndexComboBox->currentIndex(),
                          PWM_ChannelMask,
                          pwm_pulse);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Set duty cycle faild!");
        return;
    }
}

void MainWindow::on_dutyCycleCh1HorizontalSlider_valueChanged(int value)
{
    ui->dutyCycleCh1SpinBox->setValue(value);
    uint8_t PWM_ChannelMask = 0x01<<(ui->pwmIndexComboBox->currentIndex()*2);
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    uint8_t pwm_pulse[8]={0};
    for(int i=0;i<8;i++){
        if((PWM_ChannelMask>>i)&0x01){
            pwm_pulse[i] = value;
        }
    }
    ret = VPI_SetPWMPulse(VPI_USBPWM,
                          ui->deviceIndexComboBox->currentIndex(),
                          PWM_ChannelMask,
                          pwm_pulse);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Set duty cycle faild!");
        return;
    }
}

void MainWindow::on_dutyCycleCh2SpinBox_valueChanged(int arg1)
{
    ui->dutyCycleCh2HorizontalSlider->setValue(arg1);
    uint8_t PWM_ChannelMask = 0x02<<(ui->pwmIndexComboBox->currentIndex()*2);
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    uint8_t pwm_pulse[8]={0};
    for(int i=0;i<8;i++){
        if((PWM_ChannelMask>>i)&0x01){
            pwm_pulse[i] = arg1;
        }
    }
    ret = VPI_SetPWMPulse(VPI_USBPWM,
                          ui->deviceIndexComboBox->currentIndex(),
                          PWM_ChannelMask,
                          pwm_pulse);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Set duty cycle faild!");
        return;
    }
}

void MainWindow::on_dutyCycleCh2HorizontalSlider_valueChanged(int value)
{
    ui->dutyCycleCh2SpinBox->setValue(value);
    uint8_t PWM_ChannelMask = 0x02<<(ui->pwmIndexComboBox->currentIndex()*2);
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    uint8_t pwm_pulse[8]={0};
    for(int i=0;i<8;i++){
        if((PWM_ChannelMask>>i)&0x01){
            pwm_pulse[i] = value;
        }
    }
    ret = VPI_SetPWMPulse(VPI_USBPWM,
                          ui->deviceIndexComboBox->currentIndex(),
                          PWM_ChannelMask,
                          pwm_pulse);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Set duty cycle faild!");
        return;
    }
}

void MainWindow::on_startCh2PushButton_clicked()
{
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    VPI_INIT_CONFIG PWM_Config;
    PWM_Config.PWM_ChannelMask = 0x02<<(ui->pwmIndexComboBox->currentIndex()*2);
    PWM_Config.PWM_Frequency = ui->freqSpinBox->value();
    PWM_Config.PWM_Mode = ui->pwmModeCh2ComboBox->currentIndex();
    PWM_Config.PWM_Polarity = ui->polarityCh2ComboBox->currentIndex();
    PWM_Config.PWM_Pulse = ui->dutyCycleCh2SpinBox->value();
    ret = VPI_InitPWM(VPI_USBPWM,
                      ui->deviceIndexComboBox->currentIndex(),
                      &PWM_Config);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialize device faild!");
        return;
    }
    ret = VPI_StartPWM(VPI_USBPWM,
                       ui->deviceIndexComboBox->currentIndex(),
                       PWM_Config.PWM_ChannelMask);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Start PWM faild!");
        return;
    }else{
        QMessageBox::information(this,"Information","Start PWM success!");
    }
}

void MainWindow::on_stopCh1PushButton_clicked()
{
    uint8_t PWM_ChannelMask = 0x01<<(ui->pwmIndexComboBox->currentIndex()*2);
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    ret = VPI_StopPWM(VPI_USBPWM,
                      ui->deviceIndexComboBox->currentIndex(),
                      PWM_ChannelMask);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Stop PWM faild!");
        return;
    }else{
        QMessageBox::information(this,"Information","Stop PWM success!");
    }
}

void MainWindow::on_stopCh2PushButton_clicked()
{
    uint8_t PWM_ChannelMask = 0x02<<(ui->pwmIndexComboBox->currentIndex()*2);
    int ret;
    ret = VPI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    ret = VPI_OpenDevice(VPI_USBPWM,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device faild!");
        return;
    }
    ret = VPI_StopPWM(VPI_USBPWM,
                      ui->deviceIndexComboBox->currentIndex(),
                      PWM_ChannelMask);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Stop PWM faild!");
        return;
    }else{
        QMessageBox::information(this,"Information","Stop PWM success!");
    }
}

void MainWindow::on_actionContact_Us_triggered()
{
    QString AboutStr;
    AboutStr.append("Official website<span style=\"font-size:12px;\">&nbsp;</span>:&nbsp;<a href=\"http://www.viewtool.com\">www.viewtool.com</a><br>");
    AboutStr.append("Official BBS<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>:&nbsp;<a href=\"http://www.viewtool.com/bbs\">www.viewtool.com/bbs</a><br>");
    AboutStr.append("Official Taobao<span style=\"font-size: 9px;\">&nbsp;</span>:&nbsp;<a href=\"http://viewtool.taobao.com/\">viewtool.taobao.com</a><br>");
    AboutStr.append("Technical Suuport QQ&nbsp;:&nbsp;1713047813<br>");
    AboutStr.append("Product Consultant QQ&nbsp;:&nbsp;191431236");
    QMessageBox::about(this,"Contact us",AboutStr);
}

void MainWindow::on_actionAbout_triggered()
{
    QString AboutStr;
    AboutStr = "Ginkgo PWM Adapter PWM Tester 1.0.0\n";
    AboutStr.append("ViewTool Virtual Instrument 2009-2013 Copyright\n");
    AboutStr.append("Hardware Suuport:Ginkgo 2 USB-SPI Adapter");
    QMessageBox::about(this,"About Ginkgo USB PWM Tester",AboutStr);
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
