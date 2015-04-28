#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ControlSMBus.h"
/**
  * @brief  硬件连接
  * @param  VCC->5V(PIN1),GND->GND(PIN19),VO->ADC_CH0(PIN3),TXD->UART1_RX(PIN8)
  * @retval None
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::addLibraryPath("./plugins");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    memset(redData,0,sizeof(redData));
    memset(greenData,0,sizeof(greenData));
    memset(blueData,0,sizeof(blueData));
    memset(clearData,0,sizeof(clearData));
    colorPlotInit(ui->qwtPlot);

    ui->colorWidget->setAutoFillBackground(true);
    ui->colorWidget->setPalette(QPalette( QColor( 0, 0, 0 )));
}

/**
  * @brief  定时器时间函数
  * @param  event 定时器指针
  * @retval None
  */
void MainWindow::timerEvent(QTimerEvent *event)
{
    int ret;
    //定时采样数据并显示
    if(event->timerId()==mesureTimerID){
        uint8_t readingdata[32];
        uint8_t ByteCount;
        ret = SMBus_BlockRead(VII_USBI2C,
                              ui->deviceIndexComboBox->currentIndex(),
                              ui->channelIndexComboBox->currentIndex(),
                              COLOR_SENSOR_ADDR,
                              REG_BLOCK_READ,
                              readingdata,
                              &ByteCount,
                              0);
        if (ret != SMBUS_OK){
            QMessageBox::warning(this,"Warning","Read data error!");
            on_startPushButton_clicked();
            return ;
        }
        if(ByteCount >= 8){
            uint16_t green=readingdata[1]*256+readingdata[0];
            uint16_t red=readingdata[3]*256+readingdata[2];
            uint16_t blue=readingdata[5]*256+readingdata[4];
            uint16_t clr=readingdata[7]*256+readingdata[6];

            float X=(-0.14282)*red+(1.54924)*green+(-0.95641)*blue;
            float Y=(-0.32466)*red+(1.57837)*green+(-0.73191)*blue;
            float Z=(-0.68202)*red+(0.77073)*green+(0.56332)*blue;
            int r,g,b;
            CCIE *ccie = new CCIE();
            ccie->XYZ2RGB(X,Y,Z,r,g,b);
            qDebug()<<"green = "<<g;
            qDebug()<<"red = "<<r;
            qDebug()<<"blue = "<<b;
            ui->colorWidget->setPalette(QPalette( QColor( r, g, b )));
            QString dataStrTemp;
            for(int i=0;i<SIMPLE_NUMS-1;i++){
                redData[i]=redData[i+1];
                greenData[i]=greenData[i+1];
                blueData[i]=blueData[i+1];
                clearData[i]=clearData[i+1];
            }
            redData[SIMPLE_NUMS-1]=red;
            greenData[SIMPLE_NUMS-1]=green;
            blueData[SIMPLE_NUMS-1]=blue;
            clearData[SIMPLE_NUMS-1]=clr;
            displayCurve(ui->qwtPlot,redCurve,redData,SIMPLE_NUMS);//显示波形
            displayCurve(ui->qwtPlot,greenCurve,greenData,SIMPLE_NUMS);//显示波形
            displayCurve(ui->qwtPlot,blueCurve,blueData,SIMPLE_NUMS);//显示波形
            displayCurve(ui->qwtPlot,clearCurve,clearData,SIMPLE_NUMS);//显示波形

            dataStrTemp.sprintf("%d",r);
            ui->redValueLabel->setText(dataStrTemp);
            dataStrTemp.sprintf("%d",g);
            ui->greenValueLabel->setText(dataStrTemp);
            dataStrTemp.sprintf("%d",b);
            ui->blueValueLabel->setText(dataStrTemp);
            dataStrTemp.sprintf("%d",clr);
            ui->clearValueLabel->setText(dataStrTemp);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
/**
  * @brief  初始化设备
  * @param  None
  * @retval None
  */
bool MainWindow::initDevice()
{
    int ret;
    //scan device
    ret = SMBus_ScanDevice(1);
    if(ret <= 0){
        QMessageBox::warning(this,"Warning","No device connect!");
        return false;
    }
    //open device
    ret = SMBus_OpenDevice(VII_USBI2C,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex());
    if (ret != SMBUS_OK){
        QMessageBox::warning(this,"Warning","Open device error!");
        return false;
    }
    //初始化设备(硬件控制模式)
    ret = SMBus_HardInit(VII_USBI2C,
                         ui->deviceIndexComboBox->currentIndex(),
                         ui->channelIndexComboBox->currentIndex(),
                         100000,
                         0x00);
    if (ret != SMBUS_OK){
        QMessageBox::warning(this,"Warning","Initialize device error!");
        return false;
    }
    ret = setTimingReg(INTEG_MODE_FREE);//Set trigger mode.Including free mode,manually mode,single synchronizition mode or so.
    if (ret != SMBUS_OK){
        QMessageBox::warning(this,"Warning","Write data error!");
        return false;
    }
    ret = setInterruptSourceReg(INT_SOURCE_GREEN); //Set interrupt source
    if (ret != SMBUS_OK){
        QMessageBox::warning(this,"Warning","Write data error!");
        return false;
    }
    ret = setInterruptControlReg(INTR_LEVEL|INTR_PERSIST_EVERY);//Set interrupt mode
    if (ret != SMBUS_OK){
        QMessageBox::warning(this,"Warning","Write data error!");
        return false;
    }
    ret = setGain(GAIN_1|PRESCALER_4);//Set gain value and prescaler value
    if (ret != SMBUS_OK){
        QMessageBox::warning(this,"Warning","Write data error!");
        return false;
    }
    ret = setEnableADC();//Start ADC of the color sensor
    if (ret != SMBUS_OK){
        QMessageBox::warning(this,"Warning","Write data error!");
        return false;
    }
    return true;
}

/**
  * @brief  启动采样数据或停止采样数据
  * @param  None
  * @retval None
  */
void MainWindow::on_startPushButton_clicked()
{
    if(ui->startPushButton->text()=="Start"){
        if(initDevice()){
            mesureTimerID = startTimer(50);
            ui->startPushButton->setText("Stop");
        }
    }else{
        killTimer(mesureTimerID);
        ui->startPushButton->setText("Start");
    }
}
/**
  * @brief  初始化曲线显示
  * @param  None
  * @retval None
  */
void MainWindow::colorPopulate(void)
{
    // Insert new curves
    redCurve = new QwtPlotCurve( "Red" );
    redCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    redCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    redCurve->setPen( QPen( QColor(255,0,0) ) );
    redCurve->attach(ui->qwtPlot);

    greenCurve = new QwtPlotCurve( "Green" );
    greenCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    greenCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    greenCurve->setPen( QPen( QColor(0,255,0) ) );
    greenCurve->attach( ui->qwtPlot );

    blueCurve = new QwtPlotCurve( "Blue" );
    blueCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    blueCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    blueCurve->setPen( QPen( QColor(0,0,255) ) );
    blueCurve->attach( ui->qwtPlot );

    clearCurve = new QwtPlotCurve( "Clear" );
    clearCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    clearCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    clearCurve->setPen( QPen( QColor(255,255,255) ) );
    clearCurve->attach( ui->qwtPlot );
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
/**
  * @brief  初始化波形显示控件
  * @param  pPlot 显示波形的控件
  * @retval None
  */
void MainWindow::colorPlotInit(QwtPlot *pPlot)
{
    // panning with the left mouse button
    ( void ) new QwtPlotPanner( pPlot->canvas() );

    // zoom in/out with the wheel
    ( void ) new QwtPlotMagnifier( pPlot->canvas() );

    pPlot->setAutoFillBackground( true );
    pPlot->setPalette( QPalette( QColor( 255, 255, 255 ) ) );
    //pPlot->updateGradient();

    //pPlot->setTitle( "A Simple QwtPlot Demonstration" );
    pPlot->insertLegend( new QwtLegend(), QwtPlot::RightLegend );

    // axes
    pPlot->setAxisScale( pPlot->xBottom, 0.0, SIMPLE_NUMS );

    pPlot->setAxisScale( pPlot->yLeft, 0, 255);

    // canvas
    //pPlot->canvas()->setLineWidth( 1 );
    //pPlot->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    //pPlot->canvas()->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::black );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    pPlot->canvas()->setPalette( canvasPalette );

    colorPopulate();
}


int MainWindow::setTimingReg(int x)
{
    return
    SMBus_WriteByteProtocol(VII_USBI2C,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex(),
                           COLOR_SENSOR_ADDR,
                           REG_TIMING,
                           x,
                           0);
}
int MainWindow::setInterruptSourceReg(int x)
{
    return
    SMBus_WriteByteProtocol(VII_USBI2C,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex(),
                           COLOR_SENSOR_ADDR,
                           REG_INT_SOURCE,
                           x,
                           0);
}
int MainWindow::setInterruptControlReg(int x)
{
    return
    SMBus_WriteByteProtocol(VII_USBI2C,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex(),
                           COLOR_SENSOR_ADDR,
                           REG_INT,
                           x,
                           0);
}
int MainWindow::setGain(int x)
{
    return
    SMBus_WriteByteProtocol(VII_USBI2C,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex(),
                           COLOR_SENSOR_ADDR,
                           REG_GAIN,
                           x,
                           0);
}
int MainWindow::setEnableADC()
{
    return
    SMBus_WriteByteProtocol(VII_USBI2C,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex(),
                           COLOR_SENSOR_ADDR,
                           REG_CTL,
                           CTL_DAT_INIITIATE,
                           0);
}
int MainWindow::clearInterrupt()
{
    return
    SMBus_WriteByte(VII_USBI2C,
                    ui->deviceIndexComboBox->currentIndex(),
                    ui->channelIndexComboBox->currentIndex(),
                    COLOR_SENSOR_ADDR,
                    CLR_INT,
                    0);
}
