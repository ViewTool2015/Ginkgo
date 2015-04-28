#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ControlUART.h"
#include "ControlADC.h"
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
    deviceNum = 0;
    memset(PM25ADData,0,sizeof(PM25ADData));
    memset(PM25Data,0,sizeof(PM25Data));
    PM25PlotInit(ui->qwtPlot);

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
        vt_board_num=UART_ScanDevice(1);
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
        uint16_t Data;
        uint8_t dataTmp[1024];
        uint16_t Len = 0;
        ret = VAI_ReadDatas(1,
                            ui->deviceComboBox->currentIndex(),//设备索引号
                            1,
                            &Data
                            );
        if(ret==ERR_SUCCESS){
            double PM25DataTmp = (Data*3.3/4096)*0.2*1000;
            QString dataStrTemp;
            for(int i=0;i<SIMPLE_NUMS-1;i++){
                PM25ADData[i]=PM25ADData[i+1];
            }
            PM25ADData[SIMPLE_NUMS-1]=PM25DataTmp;
            displayCurve(ui->qwtPlot,PM25ADCurve,PM25ADData,SIMPLE_NUMS);//显示波形
            dataStrTemp.sprintf("%.2f",PM25DataTmp);
            ui->PM25ADLabel->setText(dataStrTemp);
        }
        ret = UART_ReadBytes(ui->deviceComboBox->currentIndex(),
                             0,
                             dataTmp,
                             &Len);
        if(Len > 0){
            qDebug()<<"Len = "<<Len;
            PM25ADDataBuffer.append((const char*)dataTmp,Len);
            for(int i=0;(i<PM25ADDataBuffer.length())&&(PM25ADDataBuffer.length()>=7);){
                if((PM25ADDataBuffer.at(i)==(char)0xAA)&&(PM25ADDataBuffer.at(i+6)==(char)0xFF)){
                    double PM25DataTmp = (((uint8_t)PM25ADDataBuffer.at(i+1)*256+(uint8_t)PM25ADDataBuffer.at(i+2))*5/1024.0)*0.2*1000;
                    qDebug()<<"PM25DataTmp = "<<PM25DataTmp;
                    QString dataStrTemp;
                    for(int i=0;i<SIMPLE_NUMS-1;i++){
                        PM25Data[i]=PM25Data[i+1];
                    }
                    PM25Data[SIMPLE_NUMS-1]=PM25DataTmp;
                    displayCurve(ui->qwtPlot,PM25Curve,PM25Data,SIMPLE_NUMS);//显示波形
                    dataStrTemp.sprintf("%.2f",PM25DataTmp);
                    ui->PM25Label->setText(dataStrTemp);
                    PM25ADDataBuffer.clear();
                    break;
                }else{
                    i++;
                }
            }
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
void MainWindow::initDevice()
{
    int ret;
    UART_INIT_CONFIG InitConfig;
    //选择需要发送数据的设备
    if(ui->deviceComboBox->currentText()!="NO DEVICE"){
        ret = UART_OpenDevice(ui->deviceComboBox->currentIndex(),0);
        if(ret!=ERR_SUCCESS){
            QMessageBox::warning(this,"警告","打开设备失败！");
            return;
        }else{
            InitConfig.BaudRate = 2400;
            InitConfig.Parity = 0;
            InitConfig.RS485Mode = 232;
            InitConfig.StopBits = 0;
            InitConfig.WordLength = 8;
            ret = UART_InitDevice(ui->deviceComboBox->currentIndex(),
                                  0,
                                  &InitConfig);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"警告","初始化设备失败！");
                return;
            }
        }
        ret = VAI_InitADC(1,
                          ui->deviceComboBox->currentIndex(),
                          VAI_ADC_CH0,
                          0);
        if(ret!=ERR_SUCCESS){
            QMessageBox::warning(this,"警告","初始化设备失败！");
            return;
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
        initDevice();
        mesureTimerID = startTimer(50);
        ui->startPushButton->setText("停止测量");
    }else{
        killTimer(mesureTimerID);
        ui->startPushButton->setText("开始测量");
    }
}
/**
  * @brief  初始化曲线显示
  * @param  None
  * @retval None
  */
void MainWindow::PM25Populate(void)
{
    // Insert new curves
    PM25Curve = new QwtPlotCurve( "数字值" );
    PM25Curve->setRenderHint( QwtPlotItem::RenderAntialiased );
    PM25Curve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    PM25Curve->setPen( QPen( QColor(255,0,0) ) );
    PM25Curve->attach(ui->qwtPlot);

    PM25ADCurve = new QwtPlotCurve( "模拟值" );
    PM25ADCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    PM25ADCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    PM25ADCurve->setPen( QPen( QColor(0,255,0) ) );
    PM25ADCurve->attach( ui->qwtPlot );
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
void MainWindow::PM25PlotInit(QwtPlot *pPlot)
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

    pPlot->setAxisScale( pPlot->yLeft, 0, 700);

    // canvas
    //pPlot->canvas()->setLineWidth( 1 );
    //pPlot->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    //pPlot->canvas()->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::black );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    pPlot->canvas()->setPalette( canvasPalette );

    PM25Populate();
}

void MainWindow::about()
{
    QString AboutStr;
    AboutStr = "Ginkgo Adapter GP2Y1050AU0F 1.0.0\n";
    AboutStr.append("内江纬图虚拟仪器有限公司 2009-2013 版权所有\n");
    AboutStr.append("支持硬件：Ginkgo 2 Adapter");
    QMessageBox::about(this,"关于Ginkgo Adapter GP2Y1050AU0F",AboutStr);
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
