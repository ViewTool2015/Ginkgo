#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ControlI2C.h"
/**
  * @brief  硬件连接
  * @param  VCC->3.3,GND->GND,SDA->SDA,SCL->SCL,AD0->GND
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
    deviceNum = 0;
    memset(AccelXData,0,sizeof(AccelXData));
    memset(AccelYData,0,sizeof(AccelYData));
    memset(AccelZData,0,sizeof(AccelZData));
    memset(GyroXData,0,sizeof(GyroXData));
    memset(GyroYData,0,sizeof(GyroYData));
    memset(GyroZData,0,sizeof(GyroZData));
    memset(TempData,0,sizeof(TempData));
    TempPlotInit(ui->TempQwtPlot);
    GyroPlotInit(ui->GyroQwtPlot);
    AccelPlotInit(ui->AccelQwtPlot);

    QObject::connect(ui->aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    QObject::connect(ui->contactUsAction, SIGNAL(triggered()), this, SLOT(contactus()));
    QObject::connect(ui->quitAction, SIGNAL(triggered()), this, SLOT(quit()));

    autoScanDeviceTimerID=startTimer(1000);
}
/**
  * @brief  定时器时间函数
  * @param  event 定时器指针
  * @retval None
  */
void MainWindow::timerEvent(QTimerEvent *event)
{
    int vt_board_num,i;
    char deviceName[100]={0};
    QStringList addDeviceList;
    QString stateStr;
    //寻找设备
    if(event->timerId()==autoScanDeviceTimerID){
        vt_board_num=VII_ScanDevice(1);
        if(vt_board_num!=deviceNum){//增加一个设备
            for(i=0;i<vt_board_num;i++){
                sprintf(deviceName,"DEVICE %d",i);
                addDeviceList.push_back(deviceName);
            }
            ui->deviceComboBox->clear();
            ui->deviceComboBox->addItems(addDeviceList);
            if(vt_board_num>0){
            }else{
                ui->deviceComboBox->addItem("NO DEVICE");
            }
        }
        deviceNum=vt_board_num;
        stateStr.sprintf("设备连接数：%d",deviceNum);
        ui->statusBar->showMessage(stateStr);
    }
    //定时采样数据并显示
    if(event->timerId()==mesureTimerID){
        int ret;
        uint8_t dataBuff[512]={0};
        ret = VII_ReadBytes(VII_USBI2C,//设备类型
                            ui->deviceComboBox->currentIndex(),//设备索引号
                            ui->channelComboBox->currentIndex(),//通道号
                            0xD0,//地址
                            0x3B,//子地址
                            dataBuff,//数据储存缓冲区
                            14);//读数据字节数
        if(ret==ERR_SUCCESS){
            QString dataStrTemp;
            short AccelX,AccelY,AccelZ,Temp;
            double realTemp;
            AccelX = (dataBuff[0]<<8)|dataBuff[1];
            AccelY = (dataBuff[2]<<8)|dataBuff[3];
            AccelZ = (dataBuff[4]<<8)|dataBuff[5];
            Temp = ((dataBuff[6]<<8)|dataBuff[7]);
            realTemp = Temp/340.0 + 36.53;
            for(int i=0;i<SIMPLE_NUMS-1;i++){
                AccelXData[i]=AccelXData[i+1];
                AccelYData[i]=AccelYData[i+1];
                AccelZData[i]=AccelZData[i+1];
                TempData[i]=TempData[i+1];
            }
            AccelXData[SIMPLE_NUMS-1]=4.0*AccelX/32768.0;
            AccelYData[SIMPLE_NUMS-1]=4.0*AccelY/32768.0;
            AccelZData[SIMPLE_NUMS-1]=4.0*AccelZ/32768.0;
            TempData[SIMPLE_NUMS-1]=realTemp;
            displayCurve(ui->AccelQwtPlot,AccelXCurve,AccelXData,SIMPLE_NUMS);//显示波形
            displayCurve(ui->AccelQwtPlot,AccelYCurve,AccelYData,SIMPLE_NUMS);
            displayCurve(ui->AccelQwtPlot,AccelZCurve,AccelZData,SIMPLE_NUMS);
            displayCurve(ui->TempQwtPlot,TempCurve,TempData,SIMPLE_NUMS);
            dataStrTemp.sprintf("%.2f",4.0*AccelX/32768.0);
            ui->acceleratXLabel->setText(dataStrTemp);
            dataStrTemp.sprintf("%.2f",4.0*AccelY/32768.0);
            ui->acceleratYLabel->setText(dataStrTemp);
            dataStrTemp.sprintf("%.2f",4.0*AccelZ/32768.0);
            ui->acceleratZLabel->setText(dataStrTemp);
            dataStrTemp.sprintf("%.2f",realTemp);
            ui->tempLabel->setText(dataStrTemp);

            short GyroX,GyroY,GyroZ;
            GyroX = (dataBuff[8]<<8)|dataBuff[9];
            GyroY = (dataBuff[10]<<8)|dataBuff[11];
            GyroZ = (dataBuff[12]<<8)|dataBuff[13];
            for(int i=0;i<SIMPLE_NUMS-1;i++){
                GyroXData[i]=GyroXData[i+1];
                GyroYData[i]=GyroYData[i+1];
                GyroZData[i]=GyroZData[i+1];
            }
            GyroXData[SIMPLE_NUMS-1]=GyroX*500.0/32768.0;
            GyroYData[SIMPLE_NUMS-1]=GyroY*500.0/32768.0;
            GyroZData[SIMPLE_NUMS-1]=GyroZ*500.0/32768.0;
            displayCurve(ui->GyroQwtPlot,GyroXCurve,GyroXData,SIMPLE_NUMS);//显示波形
            displayCurve(ui->GyroQwtPlot,GyroYCurve,GyroYData,SIMPLE_NUMS);
            displayCurve(ui->GyroQwtPlot,GyroZCurve,GyroZData,SIMPLE_NUMS);
            dataStrTemp.sprintf("%.2f",GyroX*500.0/32768.0);
            ui->angularXLabel->setText(dataStrTemp);
            dataStrTemp.sprintf("%.2f",GyroY*500.0/32768.0);
            ui->angularYLabel->setText(dataStrTemp);
            dataStrTemp.sprintf("%.2f",GyroZ*500.0/32768.0);
            ui->angularZLabel->setText(dataStrTemp);
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
void MainWindow::on_initDevicePushButton_clicked()
{
    int ret;
    VII_INIT_CONFIG InitConfig;
    //选择需要发送数据的设备
    if(ui->deviceComboBox->currentText()!="NO DEVICE"){
        ret = VII_OpenDevice(VII_USBI2C,
                             ui->deviceComboBox->currentIndex(),
                             0);
        if(ret!=ERR_SUCCESS){
            QMessageBox::warning(this,"警告","打开设备失败！");
            return;
        }else{
            InitConfig.AddrType = VII_ADDR_7BIT;
            InitConfig.ClockSpeed = 400000;
            InitConfig.ControlMode = VII_HCTL_MODE;
            InitConfig.MasterMode = VII_MASTER;
            InitConfig.SubAddrWidth = VII_SUB_ADDR_1BYTE;
            ret = VII_InitI2C(VII_USBI2C,
                              ui->deviceComboBox->currentIndex(),
                              ui->channelComboBox->currentIndex(),
                              &InitConfig);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"警告","初始化设备失败！");
                return;
            }else{
                MPU6050_Init();
                qDebug()<<"初始化设备成功！";
                QMessageBox::information(this,"消息","初始化设备成功",QMessageBox::Ok);
                return;
            }
        }
    }
}

/**
  * @brief  启动采样数据或停止采样数据
  * @param  None
  * @retval None
  */
void MainWindow::on_startPushButton_clicked()
{
    if(ui->startPushButton->text()=="开始测量"){
        mesureTimerID = startTimer(50);
        ui->startPushButton->setText("停止测量");
    }else{
        killTimer(mesureTimerID);
        ui->startPushButton->setText("开始测量");
    }
}
/**
  * @brief  初始化传感器
  * @param  None
  * @retval None
  */
void MainWindow::MPU6050_Init(void)
{
    uint8_t dataBuff[8];
    //初始化陀螺仪相关参数
    //This register allows the user to configure the power mode and clock source.
    dataBuff[0]=0x00;
    VII_WriteBytes(VII_USBI2C,
                   ui->deviceComboBox->currentIndex(),
                   ui->channelComboBox->currentIndex(),
                   0xD0,
                   0x6B,
                   dataBuff,
                   1);
    Sleep(10);
    //This register allows the user to configure the frequency of wake-ups in Accelerometer Only Low Power Mode.
    dataBuff[0]=0x00;
    VII_WriteBytes(VII_USBI2C,
                   ui->deviceComboBox->currentIndex(),
                   ui->channelComboBox->currentIndex(),
                   0xD0,
                   0x6C,
                   dataBuff,
                   1);
    Sleep(10);
    //This register is used to trigger gyroscope self-test and configure the gyroscopes’ full scale range.
    dataBuff[0]=0x08;//± 500 °/s
    VII_WriteBytes(VII_USBI2C,
                   ui->deviceComboBox->currentIndex(),
                   ui->channelComboBox->currentIndex(),
                   0xD0,
                   0x1B,
                   dataBuff,
                   1);
    Sleep(10);
    //This register configures the external Frame Synchronization (FSYNC) pin sampling and the Digital Low Pass Filter (DLPF) setting for both the gyroscopes and accelerometers.
    dataBuff[0]=0x01;
    VII_WriteBytes(VII_USBI2C,
                   ui->deviceComboBox->currentIndex(),
                   ui->channelComboBox->currentIndex(),
                   0xD0,
                   0x1A,
                   dataBuff,
                   1);
    Sleep(10);
    //This register is used to trigger accelerometer self test and configure the accelerometer full scale range.
    dataBuff[0]=0x08;//± 4g
    VII_WriteBytes(VII_USBI2C,
                   ui->deviceComboBox->currentIndex(),
                   ui->channelComboBox->currentIndex(),
                   0xD0,
                   0x1C,
                   dataBuff,
                   1);
    Sleep(10);

}
/**
  * @brief  初始化曲线显示
  * @param  None
  * @retval None
  */
void MainWindow::AccelPopulate(void)
{
    // Insert new curves
    AccelXCurve = new QwtPlotCurve( "加速度X轴" );
    AccelXCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    AccelXCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    AccelXCurve->setPen( QPen( QColor(255,255,0) ) );
    AccelXCurve->attach(ui->AccelQwtPlot);

    AccelYCurve = new QwtPlotCurve( "加速度Y轴" );
    AccelYCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    AccelYCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    AccelYCurve->setPen( QPen( QColor(255,0,0) ) );
    AccelYCurve->attach( ui->AccelQwtPlot );

    AccelZCurve = new QwtPlotCurve( "加速度Z轴" );
    AccelZCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    AccelZCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    AccelZCurve->setPen( QPen( QColor(255,0,255) ) );
    AccelZCurve->attach( ui->AccelQwtPlot );
}
/**
  * @brief  初始化曲线显示
  * @param  None
  * @retval None
  */
void MainWindow::GyroPopulate(void)
{
    GyroXCurve = new QwtPlotCurve( "角速度X轴" );
    GyroXCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    GyroXCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    GyroXCurve->setPen( QPen( QColor(0,0,255) ) );
    GyroXCurve->attach( ui->GyroQwtPlot );

    GyroYCurve = new QwtPlotCurve( "角速度Y轴" );
    GyroYCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    GyroYCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    GyroYCurve->setPen( QPen( QColor(0,255,255) ) );
    GyroYCurve->attach( ui->GyroQwtPlot );

    GyroZCurve = new QwtPlotCurve( "角速度Z轴" );
    GyroZCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    GyroZCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    GyroZCurve->setPen( QPen( QColor(0,255,0) ) );
    GyroZCurve->attach( ui->GyroQwtPlot );
}

/**
  * @brief  初始化曲线显示
  * @param  None
  * @retval None
  */
void MainWindow::TempPopulate(void)
{
    TempCurve = new QwtPlotCurve( "温度" );
    TempCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    TempCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    TempCurve->setPen( QPen( QColor(0,255,0) ) );
    TempCurve->attach( ui->TempQwtPlot );
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
void MainWindow::AccelPlotInit(QwtPlot *pPlot)
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

    pPlot->setAxisScale( pPlot->yLeft, -8.0, 8.0 );

    // canvas
    //pPlot->canvas()->setLineWidth( 1 );
    //pPlot->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    //pPlot->canvas()->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::black );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    pPlot->canvas()->setPalette( canvasPalette );

    AccelPopulate();
}
/**
  * @brief  初始化波形显示控件
  * @param  pPlot 显示波形的控件
  * @retval None
  */
void MainWindow::GyroPlotInit(QwtPlot *pPlot)
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

    pPlot->setAxisScale( pPlot->yLeft, -600.0, 600.0 );

    // canvas
    //pPlot->canvas()->setLineWidth( 1 );
    //pPlot->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    //pPlot->canvas()->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::black );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    pPlot->canvas()->setPalette( canvasPalette );

    GyroPopulate();
}

/**
  * @brief  初始化波形显示控件
  * @param  pPlot 显示波形的控件
  * @retval None
  */
void MainWindow::TempPlotInit(QwtPlot *pPlot)
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

    pPlot->setAxisScale( pPlot->yLeft, -20.0, 100.0 );

    // canvas
    //pPlot->canvas()->setLineWidth( 1 );
    //pPlot->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    //pPlot->canvas()->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::black );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    pPlot->canvas()->setPalette( canvasPalette );

    TempPopulate();
}
void MainWindow::about()
{
    QString AboutStr;
    AboutStr = "Ginkgo I2C Adapter MPU6050 3.0.0\n";
    AboutStr.append("内江纬图虚拟仪器有限公司 2009-2013 版权所有\n");
    AboutStr.append("支持硬件：Ginkgo 2 I2C Adapter");
    QMessageBox::about(this,"关于Ginkgo I2C Adapter Classic",AboutStr);
}
void MainWindow::contactus()
{
    QString AboutStr;
    AboutStr.append("官方网站<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://www.viewtool.com\">www.viewtool.com</a><br>");
    AboutStr.append("官方论坛<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://www.viewtool.com/bbs\">www.viewtool.com/bbs</a><br>");
    AboutStr.append("官方淘宝店<span style=\"font-size:9px;\">&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://viewtool.taobao.com/\">viewtool.taobao.com</a><br>");
    AboutStr.append("技术支持QQ：1713047813<br>");
    AboutStr.append("产品咨询QQ：191431236");
    QMessageBox::about(this,"联系我们",AboutStr);
}
void MainWindow::quit()
{
    this->close();
}
