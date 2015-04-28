  /*
  ******************************************************************************
  * @file     : USB_SPI_SpeedTest.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/18 9:15
  * @brief    : USB_SPI_SpeedTest demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#include <stdio.h>
#include <sys/time.h>
#include "ControlSPI.h"

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000
#endif

int main(int argc, const char* argv[])
{
	int ret,i;
	uint8_t write_buffer[10240]={0};
	uint8_t read_buffer[10240]={0};
	VSI_INIT_CONFIG SPI_Config;
    // Scan connected device 
    ret = VSI_ScanDevice(1);
    if (ret <= 0)
    {
        printf("No device connect!\n");
        return ret;
    }
    // Open device
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    // Initialize device(Master Mode, Hardware SPI, Half-Duplex)
    SPI_Config.ControlMode = 1;
    SPI_Config.MasterMode = 1;
    SPI_Config.ClockSpeed = 36000000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize device error!\n");
        return ret;
    }
	// Start Write Data Speed Test
	printf("Start Write Data Speed Test...\n");
	// Write Data Speed Test
#ifdef OS_UNIX
    struct timeval StartTime_t,EndTime_t;
    long long  StartTime,EndTime;
    int sendedDataNum;
    double dfFreq = CLOCKS_PER_SEC;
    gettimeofday(&StartTime_t,NULL);//Start time
#else
    LARGE_INTEGER litmp;
    LONGLONG StartTime,EndTime;
    double dfFreq;
    QueryPerformanceFrequency(&litmp);// Get the performance counter frequency, in n/s
    dfFreq = (double)litmp.QuadPart;
    QueryPerformanceCounter(&litmp);  // Get the current value of the performance counter
    StartTime = litmp.QuadPart;       // Start time
#endif
	for(i=0;i<400;i++)
	{
		ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,10000);
		if(ret != ERR_SUCCESS){
			printf("Write data error!\n");
			break;
		}
	}
#ifdef OS_UNIX
    gettimeofday(&EndTime_t,NULL); //Stop time
    StartTime = StartTime_t.tv_sec*1000000+StartTime_t.tv_usec;
    EndTime = EndTime_t.tv_sec*1000000+EndTime_t.tv_usec;
#else
    QueryPerformanceCounter(&litmp);// Get the current value of the performance counter
    EndTime = litmp.QuadPart; // Stop time
#endif
	// Print the write data speed information
	printf("Write Data Numbers: %d Bytes\n",i*10000);
	printf("Write Data Elapsed Time: %f\n",(EndTime-StartTime)/dfFreq);
	printf("Write Data Speed: %f KByte/s\n",((i*10000)/1024)/((EndTime-StartTime)/dfFreq));
	// Start Write Data Speed Test
	printf("\nStart Read Data Speed Test...\n");
#ifdef OS_UNIX
	gettimeofday(&StartTime_t,NULL);//Start time
#else
    QueryPerformanceCounter(&litmp);// Get the current value of the performance counter
    StartTime = litmp.QuadPart; // Start time
#endif
	for(i=0;i<400;i++)
	{
		ret = VSI_ReadBytes(VSI_USBSPI,0,0,read_buffer,10000);
		if(ret != ERR_SUCCESS){
			printf("Read data error!\n");
			break;
		}
	}
#ifdef OS_UNIX
    gettimeofday(&EndTime_t,NULL); //Stop time
    StartTime = StartTime_t.tv_sec*1000000+StartTime_t.tv_usec;
    EndTime = EndTime_t.tv_sec*1000000+EndTime_t.tv_usec;
#else
    QueryPerformanceCounter(&litmp);// Get the current value of the performance counter
    EndTime = litmp.QuadPart; // Stop time
#endif
	// Print the write data speed information
	printf("Read Data Numbers: %d Bytes\n",i*10000);
	printf("Read Data Elapsed Time: %f\n",(EndTime-StartTime)/dfFreq);
	printf("Read Data Speed: %f KByte/s\n",((i*10000)/1024)/((EndTime-StartTime)/dfFreq));
	getchar();
	return 0;
}

