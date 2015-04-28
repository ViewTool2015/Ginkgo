#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    WavePlotInit(ui->waveQwtPlot);
    showWave();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
  * @brief  初始化曲线显示
  * @param  None
  * @retval None
  */
void MainWindow::WavePopulate(void)
{
    WaveCurve = new QwtPlotCurve( "Wave" );
    WaveCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    WaveCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    WaveCurve->setPen( QPen( QColor(0,255,0) ) );
    WaveCurve->attach( ui->waveQwtPlot );
}
/**
  * @brief  显示波形曲线
  * @param  pCurve 曲线指针
  * @param  pCurveData Y轴数据
  * @param  dataNum 数据个数
  * @retval None
  */
void MainWindow::displayCurve(QwtPlot *plot,QwtPlotCurve *pCurve,double *pCurveData,int dataNum)
{
    double *pCurveXData = (double*)malloc(dataNum*sizeof(double));
    for(int i=0;i<dataNum;i++){
        pCurveXData[i] = i;
    }
    pCurve->setSamples(pCurveXData,pCurveData,dataNum);
    plot->setAutoReplot(true);
    plot->replot();
    free(pCurveXData);
}

void MainWindow::showWave(void)
{
    ui->amplitudeSpinBox->setMaximum(3300-ui->dcVoltageSpinBox->value());
    int dcVoltage = ui->dcVoltageSpinBox->value();
    int amplitude = ui->amplitudeSpinBox->value();
    if((ui->waveTypeComboBox->currentText()!="DC Voltage")&&(ui->waveTypeComboBox->currentText()!="Noise Wave")){
        ui->frequencySpinBox->setMinimum(1);
    }
    if(ui->waveTypeComboBox->currentText()=="DC Voltage"){
        for(int i=0;i<1000;i++){
            WaveData[i] = amplitude+dcVoltage;
        }
        displayCurve(ui->waveQwtPlot,WaveCurve,WaveData,1000);
    }
    if(ui->waveTypeComboBox->currentText()=="Square Wave"){
        for(int i=0;i<5;i++){
            for(int j=0;j<100;j++){
                WaveData[i*200+j] = dcVoltage;
            }
            for(int j=0;j<100;j++){
                WaveData[i*200+100+j] = dcVoltage+amplitude;
            }
        }
        displayCurve(ui->waveQwtPlot,WaveCurve,WaveData,1000);
    }
    if(ui->waveTypeComboBox->currentText()=="Sawtooth Wave"){
        for(int i=0;i<5;i++){
            for(int j=0;j<200;j++){
                WaveData[i*200+j] = dcVoltage+(j)*amplitude/200;
            }
        }
        displayCurve(ui->waveQwtPlot,WaveCurve,WaveData,1000);
    }
    if(ui->waveTypeComboBox->currentText()=="Triangular Wave"){
        for(int i=0;i<5;i++){
            for(int j=0;j<100;j++){
                WaveData[i*200+j] = dcVoltage+(j)*amplitude/100;;
            }
            for(int j=0;j<100;j++){
                WaveData[i*200+100+j] = dcVoltage+(100-j)*amplitude/100;;
            }
        }
        displayCurve(ui->waveQwtPlot,WaveCurve,WaveData,1000);
    }
    if(ui->waveTypeComboBox->currentText()=="Sine Wave"){
        for(int i=0;i<5;i++){
            for(int j=0;j<200;j++){
                WaveData[i*200+j] = dcVoltage+amplitude*(1+sin(2*3.141592653589793*j/200))/2.0;
            }
        }
        displayCurve(ui->waveQwtPlot,WaveCurve,WaveData,1000);
    }
    if(ui->waveTypeComboBox->currentText()=="Noise Wave"){
        QTime time;
        time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        for(int i=0;i<1000;i++){
            WaveData[i] = dcVoltage+(qrand()%(amplitude+1));
        }
        displayCurve(ui->waveQwtPlot,WaveCurve,WaveData,1000);
    }
}

/**
  * @brief  初始化波形显示控件
  * @param  pPlot 显示波形的控件
  * @retval None
  */
void MainWindow::WavePlotInit(QwtPlot *pPlot)
{
    // panning with the left mouse button
    //( void ) new QwtPlotPanner( pPlot->canvas() );

    // zoom in/out with the wheel
    //( void ) new QwtPlotMagnifier( pPlot->canvas() );

    pPlot->setAutoFillBackground( true );
    pPlot->setPalette( QPalette( QColor( 255, 255, 255 ) ) );
    //pPlot->updateGradient();

    //pPlot->setTitle( "A Simple QwtPlot Demonstration" );
    pPlot->insertLegend( new QwtLegend(), QwtPlot::RightLegend );

    // axes
    pPlot->setAxisScale( pPlot->xBottom, 0.0, 1000 );

    pPlot->setAxisScale( pPlot->yLeft, 0, 4000);


    QPalette canvasPalette( Qt::black );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    pPlot->canvas()->setPalette( canvasPalette );

    WavePopulate();
}

void MainWindow::on_startPushButton_clicked()
{
    int dcVoltage = ui->dcVoltageSpinBox->value();
    int amplitude = ui->amplitudeSpinBox->value();
    uint16_t waveData[1024];
    int ret;
    //Scan device
    ret = VDA_ScanDevice(1);
    if(ret <= 0){
        QMessageBox::warning(this,"Warning","No device connected!");
        return;
    }
    //Open device
    ret = VDA_OpenDevice(VDA_USBDAC,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device error!");
        return ;
    }
    if(ui->waveTypeComboBox->currentText()=="DC Voltage"){
        waveData[0] = (dcVoltage+amplitude)*4095/3300;
        ret = VDA_InitDAC(VDA_USBDAC,
                          ui->deviceIndexComboBox->currentIndex(),
                          ui->channelIndexComboBox->currentIndex(),
                          waveData,
                          1,
                          0);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Config DAC error!");
            return;
        }
    }
    if(ui->waveTypeComboBox->currentText()=="Square Wave"){
        int Period = 1000000/ui->frequencySpinBox->value();
        int dataNum;
        int Time = 2;
        do{
            dataNum = Period/Time;
            if(dataNum > 1024){
                Time += 2;
            }else{
                break;
            }
        }while(dataNum > 1024);
        for(int j=0;j<(dataNum>>1);j++){
            waveData[j] = dcVoltage*4095/3300;
        }
        for(int j=0;j<(dataNum>>1);j++){
            waveData[(dataNum>>1)+j] = (dcVoltage+amplitude)*4095/3300;
        }
        ret = VDA_InitDAC(VDA_USBDAC,
                          ui->deviceIndexComboBox->currentIndex(),
                          ui->channelIndexComboBox->currentIndex(),
                          waveData,
                          dataNum,
                          Time);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Config DAC error!");
            return;
        }
    }
    if(ui->waveTypeComboBox->currentText()=="Sawtooth Wave"){
        int Period = 1000000/ui->frequencySpinBox->value();
        int dataNum;
        int Time = 2;
        do{
            dataNum = Period/Time;
            if(dataNum > 1024){
                Time += 2;
            }else{
                break;
            }
        }while(dataNum > 1024);
        for(int j=0;j<dataNum;j++){
            waveData[j] = (dcVoltage+j*amplitude/dataNum)*4095/3300;
        }
        ret = VDA_InitDAC(VDA_USBDAC,
                          ui->deviceIndexComboBox->currentIndex(),
                          ui->channelIndexComboBox->currentIndex(),
                          waveData,
                          dataNum,
                          Time);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Config DAC error!");
            return;
        }
    }
    if(ui->waveTypeComboBox->currentText()=="Triangular Wave"){
        int Period = 1000000/ui->frequencySpinBox->value();
        int dataNum;
        int Time = 2;
        do{
            dataNum = Period/Time;
            if(dataNum > 1024){
                Time += 2;
            }else{
                break;
            }
        }while(dataNum > 1024);
        for(int j=0;j<(dataNum>>1);j++){
            waveData[j] = (dcVoltage+amplitude*j/(dataNum>>1))*4095/3300;
        }
        for(int j=0;j<(dataNum>>1);j++){
            waveData[(dataNum>>1)+j] = (dcVoltage+amplitude*((dataNum>>1)-j)/(dataNum>>1))*4095/3300;
        }
        ret = VDA_InitDAC(VDA_USBDAC,
                          ui->deviceIndexComboBox->currentIndex(),
                          ui->channelIndexComboBox->currentIndex(),
                          waveData,
                          dataNum,
                          Time);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Config DAC error!");
            return;
        }
    }
    if(ui->waveTypeComboBox->currentText()=="Sine Wave"){
        int Period = 1000000/ui->frequencySpinBox->value();
        int dataNum;
        int Time = 2;
        do{
            dataNum = Period/Time;
            if(dataNum > 1024){
                Time += 2;
            }else{
                break;
            }
        }while(dataNum > 1024);
        for(int j=0;j<dataNum;j++){
            waveData[j] = (dcVoltage+amplitude*(1+sin(2*3.141592653589793*j/dataNum))/2.0)*4095/3300;
        }
        ret = VDA_InitDAC(VDA_USBDAC,
                          ui->deviceIndexComboBox->currentIndex(),
                          ui->channelIndexComboBox->currentIndex(),
                          waveData,
                          dataNum,
                          Time);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Config DAC error!");
            return;
        }
    }
    if(ui->waveTypeComboBox->currentText()=="Noise Wave"){
        int dataNum = 1024;
        QTime time;
        time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        for(int i=0;i<dataNum;i++){
            waveData[i] = (dcVoltage+(qrand()%amplitude))*4095/3300;
        }
        ret = VDA_InitDAC(VDA_USBDAC,
                          ui->deviceIndexComboBox->currentIndex(),
                          ui->channelIndexComboBox->currentIndex(),
                          waveData,
                          dataNum,
                          2);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Config DAC error!");
            return;
        }
    }
    //Start DAC to output
    ret = VDA_StartDAC(VDA_USBDAC,
                       ui->deviceIndexComboBox->currentIndex(),
                       ui->channelIndexComboBox->currentIndex());
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Start DAC error!");
        return;
    }
}

void MainWindow::on_stopPushButton_clicked()
{
    VDA_StopDAC(VDA_USBDAC,
              ui->deviceIndexComboBox->currentIndex(),
              ui->channelIndexComboBox->currentIndex());

}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{
    QString AboutStr;
    AboutStr = "Ginkgo USB-DAC Waveform Generator 1.0.0\n";
    AboutStr.append("ViewTool Virtual Instrument 2009-2014 Copyright\n");
    AboutStr.append("Hardware Support: Ginkgo 2 USB-I2C/USB-SPI Adapter");
    QMessageBox::about(this,"About Ginkgo USB-DAC Waveform Generator",AboutStr);
}

void MainWindow::on_actionContuct_Us_triggered()
{
    QString AboutStr;
    AboutStr.append("Official website<span style=\"font-size:12px;\">&nbsp;</span>:&nbsp;<a href=\"http://www.viewtool.com\">www.viewtool.com</a><br>");
    AboutStr.append("Official BBS<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>:&nbsp;<a href=\"http://www.viewtool.com/bbs\">www.viewtool.com/bbs</a><br>");
    AboutStr.append("Official Taobao<span style=\"font-size: 9px;\">&nbsp;</span>:&nbsp;<a href=\"http://viewtool.taobao.com/\">viewtool.taobao.com</a><br>");
    AboutStr.append("Technical Suuport QQ&nbsp;:&nbsp;1713047813<br>");
    AboutStr.append("Product Consultant QQ&nbsp;:&nbsp;191431236");
    QMessageBox::about(this,"Contact Us",AboutStr);
}
