#include <iostream>
#include <stdio.h>
#ifdef OS_UNIX
#include <sys/time.h>
#else
#endif
#include "ControlSPI.h"
using namespace std;

int main()
{
    int ret,i;
    uint8_t write_buffer[10240]= {0};
    uint8_t read_buffer[10240]= {0};
    VSI_INIT_CONFIG SPI_Config;
    //Scan device(must call)
    ret = VSI_ScanDevice(1);
    if (ret <= 0) {
        printf("No device connect!\n");
        getchar();
        return ret;
    }
    //Open device(must call)
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS) {
        printf("Open device error!\n");
        getchar();
        return ret;
    }
    //Initialize the device (host hardware half-duplex mode)
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
    //Start to write the data speed test
    printf("Start Write Data Speed Test...\n");
#ifdef OS_UNIX
    struct timeval StartTime_t,EndTime_t;
    long long  StartTime,EndTime;
    double dfFreq = CLOCKS_PER_SEC;
    gettimeofday(&StartTime_t,NULL);//开始计时
#else
    LARGE_INTEGER litmp;
    LONGLONG StartTime,EndTime;
    double dfFreq;
    int sendedDataNum;
    QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s），
    dfFreq = (double)litmp.QuadPart;
    QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
    StartTime = litmp.QuadPart; //开始计时
#endif
    for(i=0; i<400; i++) {
        ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,10000);
        if(ret != ERR_SUCCESS) {
            printf("Write data error!\n");
            break;
        }
    }
#ifdef OS_UNIX
    gettimeofday(&EndTime_t,NULL); //终止计时
    StartTime = StartTime_t.tv_sec*1000000+StartTime_t.tv_usec;
    EndTime = EndTime_t.tv_sec*1000000+EndTime_t.tv_usec;
#else
    QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
    EndTime = litmp.QuadPart; //终止计时
#endif
    //Print write data speed test information
    printf("Write Data Numbers: %d Bytes\n",i*10000);
    printf("Write Data Elapsed Time: %f\n",(EndTime-StartTime)/dfFreq);
    printf("Write Data Speed: %f KByte/s\n",((i*10000)/1024)/((EndTime-StartTime)/dfFreq));
    //Start reading data speed test
    printf("\nStart Read Data Speed Test...\n");
#ifdef OS_UNIX
    gettimeofday(&StartTime_t,NULL);//开始计时
#else
    StartTime = litmp.QuadPart; //开始计时
#endif
    for(i=0; i<400; i++) {
        ret = VSI_ReadBytes(VSI_USBSPI,0,0,read_buffer,10000);
        if(ret != ERR_SUCCESS) {
            printf("Read data error!\n");
            break;
        }
    }
#ifdef OS_UNIX
    gettimeofday(&EndTime_t,NULL); //终止计时
    StartTime = StartTime_t.tv_sec*1000000+StartTime_t.tv_usec;
    EndTime = EndTime_t.tv_sec*1000000+EndTime_t.tv_usec;
#else
    QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
    EndTime = litmp.QuadPart; //终止计时
#endif
    //Print reading data speed test information
    printf("Read Data Numbers: %d Bytes\n",i*10000);
    printf("Read Data Elapsed Time: %f\n",(EndTime-StartTime)/dfFreq);
    printf("Read Data Speed: %f KByte/s\n",((i*10000)/1024)/((EndTime-StartTime)/dfFreq));
    getchar();
    return 0;
}
