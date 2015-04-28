#include <iostream>
#include <stdio.h>
#include "ControlSPI.h"

using namespace std;

int main()
{
    int ret;
    VSI_INIT_CONFIG SPI_Config;
    //扫描已经连接的设备
    ret = VSI_ScanDevice(1);
    if (ret <= 0) {
        printf("No device connect!\n");
        getchar();
        return ret;
    }
    //打开设备
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS) {
        printf("Open device error!\n");
        getchar();
        return ret;
    }
    //初始化设备(主机硬件半双工模式)
    SPI_Config.ControlMode = 1;
    SPI_Config.MasterMode = 1;
    SPI_Config.ClockSpeed = 36000000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
    SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS) {
        printf("Initialize device error!\n");
        getchar();
        return ret;
    }
    uint8_t write_buffer[10240];
    uint8_t read_buffer[10240];
    //SPI主机半双工模式写8字节数据
    for (int i = 0; i < 8; i++) {
        write_buffer[i] = i;
    }
    ret = VSI_WriteBytes(VSI_USBSPI, 0, 0, write_buffer, 8);
    if (ret != ERR_SUCCESS) {
        printf("Write data error!\n");
        getchar();
        return ret;
    }
    //SPI主机半双工模式读8字节数据
    ret = VSI_ReadBytes(VSI_USBSPI, 0, 0, read_buffer, 8);
    if (ret != ERR_SUCCESS) {
        printf("Read data error!\n");
        getchar();
        return ret;
    } else {
        printf("Read data(Hex):");
        for (int i = 0; i < 8; i++) {
            printf("%02X ",read_buffer[i]);
        }
        printf("\n");
    }
    //SPI主机半双工模式发送读取数据,发送5字节数据后立即读8字节数据，整个操作期间片选信号一直有效
    for (int i = 0; i < 5; i++) {
        write_buffer[i] = i;
    }
    ret = VSI_WriteReadBytes(VSI_USBSPI, 0, 0, write_buffer, 5, read_buffer, 8);
    if (ret != ERR_SUCCESS) {
        printf("WriteRead data error!\n");
        getchar();
        return ret;
    } else {
        printf("Read data(Hex):");
        for (int i = 0; i < 8; i++) {
            printf("%02X ",read_buffer[i]);
        }
        printf("\n");
    }
    //初始化设备(主机半双工模式)，软件模式下也支持VSI_WriteBytes，VSI_ReadBytes，VSI_WriteReadBytes这3个函数
    //VSI_WriteBits,VSI_ReadBits,VSI_WriteReadBits这三个函数不能在硬件件模式下使用，在单总线模式下也能使用
    SPI_Config.ControlMode = 2;
    SPI_Config.MasterMode = 1;
    SPI_Config.ClockSpeed = 100000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
    SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS) {
        printf("Initialize device error!\n");
        getchar();
        return ret;
    }
    //二进制模式发送数据，发送bit数最大可达10240bit
    ret = VSI_WriteBits(VSI_USBSPI, 0, 0, (uint8_t *)"10110100100101");//发送14bit数据
    if (ret != ERR_SUCCESS) {
        printf("Write bit error!\n");
        getchar();
        return ret;
    }
    //二进制模式读取数据,读取最大bit数可达10240bit
    uint8_t read_buffer_bin[10240] = {0};
    ret = VSI_ReadBits(VSI_USBSPI, 0, 0, read_buffer_bin,19);//读取19bit数据
    if (ret != ERR_SUCCESS) {
        printf("Read bit error!\n");
        getchar();
        return ret;
    } else {
        printf("Read bits:");
        printf("%s",read_buffer_bin);
        printf("\n");
    }
    //二进制模式发送读取数据
    ret = VSI_WriteReadBits(VSI_USBSPI, 0, 0,(uint8_t *)"000011110101001",read_buffer_bin, 25);//读取19bit数据
    if (ret != ERR_SUCCESS) {
        printf("Write bit error!\n");
        getchar();
        return ret;
    } else {
        printf("Read bits:");
        printf("%s\n",read_buffer_bin);
    }
    //初始化设备(从机模式)
    SPI_Config.ControlMode = 0; //从机模式工作在硬件全双工模式下
    SPI_Config.MasterMode = 0;  //从机模式
    SPI_Config.CPHA = 0;        //时钟极性和相位必须和主机设置一样
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;    //只支持8bit模式
    SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS) {
        printf("Initialize device error!\n");
        getchar();
        return ret;
    }
    //从机模式写数据，数据不会立即从MISO引脚发送出去，必须要等到主机发送片选信号（CS低有效）并发送时钟后才将数据移位发送出去
    //但是调用该函数后会立即返回，数据存放到适配器内存缓冲区中
    for (int i = 0; i < 8; i++) {
        write_buffer[i] = i;
    }
    ret = VSI_SlaveWriteBytes(VSI_USBSPI, 0, write_buffer, 8);
    if (ret != ERR_SUCCESS) {
        printf("Write data error!\n");
        getchar();
        return ret;
    }
    //从机模式读数据,最后一个参数为等待时间，单位为微秒，超过等待时间后不管是否是否读到数据函数都立即返回
    //调用该函数后可以通过判断读数据字节数这个参数判断接收了多少字节数据
    //注意：在调用VSI_SlaveWriteBytes后，主机主动读取数据的同时，从机也会接收到数据（从机工作于全双工模式）
    //若主机在读取数据的时候不想要从机接收到的数据，可以在主机读取数据完成后调用VSI_SlaveReadBytes函数并丢弃返回的数据
    int read_data_num = 0;
    ret = VSI_SlaveReadBytes(VSI_USBSPI, 0, read_buffer, &read_data_num,100);
    if (ret != ERR_SUCCESS) {
        printf("Read data error!\n");
        getchar();
        return ret;
    } else {
        if (read_data_num > 0) {
            printf("Read data num: %d\n",read_data_num);
            printf("Read data(Hex):");
            for (int i = 0; i < read_data_num; i++) {
                printf("%02X ",read_buffer[i]);
            }

        } else {
            printf("No data!\n");
        }
    }
    getchar();
    return 0;
}
