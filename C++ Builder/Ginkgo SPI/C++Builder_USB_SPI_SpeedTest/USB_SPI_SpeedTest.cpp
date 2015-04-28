#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include "ControlSPI.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int ret,i;
	uint8_t write_buffer[10240]={0};
	uint8_t read_buffer[10240]={0};
	VSI_INIT_CONFIG SPI_Config;
    //Scan device(must call)
    ret = VSI_ScanDevice(1);
    if (ret <= 0)
    {
		printf("No device connect!\n");
		system("pause");
        return ret;
    }
    //Open device(must call)
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Open device error!\n");
		system("pause");
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
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error!\n");
		system("pause");
        return ret;
    }
	//Start to write the data speed test
	printf("Start Write Data Speed Test...\n");
    LARGE_INTEGER litmp;
    LONGLONG StartTime,EndTime;
	double dfFreq;
	//To obtain high precision run counter frequency f, is how many times per second (n/s)
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;
	//To obtain the high precision run counter value
	QueryPerformanceCounter(&litmp);
	//Start the time
    StartTime = litmp.QuadPart;
	for(i=0;i<400;i++)
	{
		ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,10000);
		if(ret != ERR_SUCCESS){
			printf("Write data error!\n");
			break;
		}
	}
	//To obtain the high precision run counter value
	QueryPerformanceCounter(&litmp);
	//Stop timing
    EndTime = litmp.QuadPart;
	//Print write data speed test information
	printf("Write Data Numbers: %d Bytes\n",i*10000);
	printf("Write Data Elapsed Time: %f\n",(EndTime-StartTime)/dfFreq);
	printf("Write Data Speed: %f KByte/s\n",((i*10000)/1024)/((EndTime-StartTime)/dfFreq));
	//Start reading data speed test
	printf("\nStart Read Data Speed Test...\n");
	//To obtain the high precision run counter value
	QueryPerformanceCounter(&litmp);
	//Start the time
    StartTime = litmp.QuadPart;
	for(i=0;i<400;i++)
	{
		ret = VSI_ReadBytes(VSI_USBSPI,0,0,read_buffer,10000);
		if(ret != ERR_SUCCESS){
			printf("Read data error!\n");
			break;
		}
	}
	//To obtain the high precision run counter value
	QueryPerformanceCounter(&litmp);
	//Stop timing
	EndTime = litmp.QuadPart;
	//Print reading data speed test information
	printf("Read Data Numbers: %d Bytes\n",i*10000);
	printf("Read Data Elapsed Time: %f\n",(EndTime-StartTime)/dfFreq);
	printf("Read Data Speed: %f KByte/s\n",((i*10000)/1024)/((EndTime-StartTime)/dfFreq));
	system("pause");
	return 0;
}
