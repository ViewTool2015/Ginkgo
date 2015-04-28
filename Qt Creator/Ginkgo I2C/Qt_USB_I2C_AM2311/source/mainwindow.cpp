#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pTemperatureThermo = new ThermoMeter();
    pTemperatureThermo->setMinimum(-40);
    pTemperatureThermo->setMaximum(100);
    pTemperatureThermo->setSuffix(" [C]");
    pTemperatureThermo->setValueOffset(280);
    ui->temperatureGroupBox->layout()->addWidget(pTemperatureThermo);
    pHumidityThermo = new ThermoMeter();
    pHumidityThermo->setMinimum(0);
    pHumidityThermo->setMaximum(100);
    pHumidityThermo->setSuffix(" [%]");
    pHumidityThermo->setValueOffset(280);
    ui->humidityGroupBox->layout()->addWidget(pHumidityThermo);
    plotInit(ui->qwtPlot);
    for(int i=0;i<SIMPLE_NUMS;i++){
        HumidityData[i]=0;
        TemperatureData[i]=0;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
  * @brief  初始化波形显示控件
  * @param  pPlot 显示波形的控件
  * @retval None
  */
void MainWindow::plotInit(QwtPlot *pPlot)
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
    pPlot->setAxisScale( pPlot->yLeft, -40, 100 );

    QPalette canvasPalette( Qt::black );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    pPlot->canvas()->setPalette( canvasPalette );

    populate();
}

/**
  * @brief  初始化曲线显示
  * @param  None
  * @retval None
  */
void MainWindow::populate(void)
{
    // Insert new curves
    pHumidityCurve = new QwtPlotCurve( "Humidity" );
    pHumidityCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    pHumidityCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    pHumidityCurve->setPen( QPen( QColor(255,0,0) ) );
    pHumidityCurve->attach( ui->qwtPlot );

    pTemperatureCurve = new QwtPlotCurve( "Temperature" );
    pTemperatureCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    pTemperatureCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    pTemperatureCurve->setPen( QPen( QColor(0,255,0) ) );
    pTemperatureCurve->attach( ui->qwtPlot );

}

/**
  * @brief  显示波形曲线
  * @param  pCurve 曲线指针
  * @param  pCurveData Y轴数据
  * @param  dataNum 数据个数
  * @retval None
  */
void MainWindow::displayCurve(QwtPlotCurve *pCurve,double *pCurveData,int dataNum)
{
    double *pCurveXData = (double*)malloc(dataNum*sizeof(double));
    for(int i=0;i<dataNum;i++){
        pCurveXData[i] = i;
    }
    pCurve->setSamples(pCurveXData,pCurveData,dataNum);
    ui->qwtPlot->setAutoReplot(true);
    ui->qwtPlot->replot();
    free(pCurveXData);
}

bool MainWindow::AM2311_Init(void)
{
    int ret;
    VII_INIT_CONFIG I2C_Config;
    //scan device
    ret = VII_ScanDevice(1);
    if(ret <= 0){
        QMessageBox::warning(this,"Warning","No device connect!");
        return false;
    }
    //open device
    ret = VII_OpenDevice(VII_USBI2C,
                         ui->deviceIndexComboBox->currentIndex(),
                         ui->channelIndexComboBox->currentIndex());
    if (ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device error!");
        return false;
    }
    //初始化设备(硬件控制模式)
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 100000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_NONE;
    ret = VII_InitI2C(VII_USBI2C,
                      ui->deviceIndexComboBox->currentIndex(),
                      ui->channelIndexComboBox->currentIndex(),
                      &I2C_Config);
    if (ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialize device error!");
        return false;
    }
    return true;
}

void MainWindow::on_startPushButton_clicked()
{
    if(ui->startPushButton->text()=="Start"){
        if(AM2311_Init()){
            mesureTimerID = startTimer(500);
            ui->startPushButton->setText("Stop");
        }
    }else{
        killTimer(mesureTimerID);
        ui->startPushButton->setText("Start");
    }
}

/**
  * @brief  定时器时间函数
  * @param  event 定时器指针
  * @retval None
  */
void MainWindow::timerEvent(QTimerEvent *event)
{
    uint8_t write_buffer[8]={0};
    uint8_t	read_buffer[8]={0};
    int ret;
    //定时采样数据并显示
    if(event->timerId()==mesureTimerID){
        //Wake up AM2311 sensor
        VII_WriteBytes(VII_USBI2C,
                       ui->deviceIndexComboBox->currentIndex(),
                       ui->channelIndexComboBox->currentIndex(),
                       0xB8,
                       0x00,
                       write_buffer,
                       1);
        //Send out read temperature and huminity command
        write_buffer[0] = 0x03;
        write_buffer[1] = 0x00;
        write_buffer[2] = 0x04;
        ret = VII_WriteBytes(VII_USBI2C,
                             ui->deviceIndexComboBox->currentIndex(),
                             ui->channelIndexComboBox->currentIndex(),
                             0xB8,
                             0x00,
                             write_buffer,
                             3);
        if (ret != ERR_SUCCESS){
            qDebug()<<"ret = "<<ret;
            QMessageBox::warning(this,"Warning","Write data error!");
            killTimer(mesureTimerID);
            ui->startPushButton->setText("Start");
            return ;
        }
        // Read out temperature and huminity
        ret = VII_ReadBytes(VII_USBI2C,
                            ui->deviceIndexComboBox->currentIndex(),
                            ui->channelIndexComboBox->currentIndex(),
                            0xB8,
                            0x00,
                            read_buffer,
                            8);
        if (ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Read data error!");
            killTimer(mesureTimerID);
            ui->startPushButton->setText("Start");
            return ;
        }else{
            double t;
            uint16_t t_tmp = (read_buffer[4] << 8) | read_buffer[5];
            if(t_tmp & 0x8000){
                t = -((t_tmp&0x7FFF)/10.0);
            }else{
                t = ((t_tmp&0x7FFF)/10.0);
            }
            double h = ((read_buffer[2] << 8) | read_buffer[3]) / 10.0;
            for(int i=0;i<SIMPLE_NUMS-1;i++){
                HumidityData[i]=HumidityData[i+1];
                TemperatureData[i]=TemperatureData[i+1];
            }
            HumidityData[SIMPLE_NUMS-1]=h;
            TemperatureData[SIMPLE_NUMS-1]=t;
            displayCurve(pHumidityCurve,HumidityData,SIMPLE_NUMS);//显示波形
            displayCurve(pTemperatureCurve,TemperatureData,SIMPLE_NUMS);//显示波形
            pTemperatureThermo->setValue(t);
            pHumidityThermo->setValue(h);
        }
    }
}
