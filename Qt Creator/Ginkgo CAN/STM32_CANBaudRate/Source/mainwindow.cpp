#include "mainwindow.h"
#include "ui_mainwindow.h"


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

void MainWindow::on_calculateBaudRatePushButton_clicked()
{
    float BaudRateTemp;
    float BaudRate = ui->baudRateLineEdit->text().toFloat(NULL);
    float pclk = ui->PCLKLineEdit->text().toFloat(NULL);
    float error = (ui->errorComboBox->currentIndex()+1)/100.0;
    float SamplePoint = (ui->samplePointComboBox->currentIndex()*10+40);
    QTableWidgetItem *item;
    ui->baudRateTableWidget->verticalHeader()->hide();
    ui->baudRateTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->baudRateTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->baudRateTableWidget->setRowCount(0);
    for(int BS1=1;BS1<17;BS1++){
        for(int BS2=1;BS2<9;BS2++){
            for(int BRP=1;BRP<1025;BRP++){
                float site = (float)(100.0*(1+BS1))/(1+BS1+BS2);
                BaudRateTemp = (pclk*1000.0)/(BRP*(1+BS1+BS2));
                if((BaudRate <= (BaudRateTemp+BaudRateTemp*error))&&(BaudRate >= (BaudRateTemp-BaudRateTemp*error))&&(site>=SamplePoint)){
                    ui->baudRateTableWidget->setRowCount(ui->baudRateTableWidget->rowCount()+1);
                    ui->baudRateTableWidget->setRowHeight(ui->baudRateTableWidget->rowCount()-1,20);
                    QString str;
                    str.sprintf("CAN_BS1_%dtq",BS1);
                    item = new QTableWidgetItem(str);
                    ui->baudRateTableWidget->setItem(ui->baudRateTableWidget->rowCount()-1,0,item);
                    str.sprintf("CAN_BS2_%dtq",BS2);
                    item = new QTableWidgetItem(str);
                    ui->baudRateTableWidget->setItem(ui->baudRateTableWidget->rowCount()-1,1,item);
                    str.sprintf("%d",BRP);
                    item = new QTableWidgetItem(str);
                    ui->baudRateTableWidget->setItem(ui->baudRateTableWidget->rowCount()-1,2,item);
                    str.sprintf("%.1f%%",site);
                    item = new QTableWidgetItem(str);
                    ui->baudRateTableWidget->setItem(ui->baudRateTableWidget->rowCount()-1,3,item);
                    str.sprintf("%.1f",BaudRateTemp);
                    item = new QTableWidgetItem(str);
                    ui->baudRateTableWidget->setItem(ui->baudRateTableWidget->rowCount()-1,4,item);
                    str.sprintf("%.1f%%",qAbs(BaudRateTemp-BaudRate)*100.0/BaudRate);
                    item = new QTableWidgetItem(str);
                    ui->baudRateTableWidget->setItem(ui->baudRateTableWidget->rowCount()-1,5,item);
                    QCoreApplication::processEvents(QEventLoop::AllEvents);
                }
            }
        }
    }
}
