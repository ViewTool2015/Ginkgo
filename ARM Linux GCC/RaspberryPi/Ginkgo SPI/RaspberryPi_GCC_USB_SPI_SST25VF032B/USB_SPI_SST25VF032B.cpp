  /*
  ******************************************************************************
  * @file     : USB_SPI_SST25VF032B.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/18 9:44
  * @brief    : USB_SPI_SST25VF032B demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

  /*
  Hardware Connection   (for reference only)
  SST25VF032B    Ginkgo USB-SPI Adapter
  1.CE       <-->  SPI_SEL0(Pin11)
  2.SO	     <-->  SPI_MISO(Pin15)
  3.WP       <-->  VCC(Pin2)
  4.Vss(GND) <-->  GND(Pin19/Pin20)
  5.SI	     <-->  VCC(Pin2)SPI_MOSI(Pin17)
  6.SCK	     <-->  VCC(Pin2)SPI_SCK(Pin13)
  7.HOLD	 <-->  VCC(Pin2)
  8.Vdd	     <-->  VCC(Pin2)
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ControlSPI.h"


int main(int argc, const char* argv[])
{
	int ret,i;
	FILE *fp;
	int StartAddr = 0x000000;// Read data start address
	int EndAddr = 0x400000;// Read data end address
	int Addr = 0;
	VSI_INIT_CONFIG	SPI_Init;
	uint8_t	WriteDataTemp[512]={0};
	uint8_t	ReadDataTemp[10240]={0};
	char strTmp[1024]={0};
	char str[100]={0};
	// Check the device number of connections
	ret = VSI_ScanDevice();
	if(ret>0){
		printf("The connected device number is:%d\n",ret);
	}else{
		printf("No device to connect!\n");
		return 0;
	}
	// Open Device
	ret = VSI_OpenDevice(VSI_USBSPI,0,0);
	if(ret != ERR_SUCCESS){
		printf("Open device error :%d\n",ret);
		return ret;
	}else{
		printf("Open device success!\n");
	}
	// Device initialization
	SPI_Init.ClockSpeed = 1125000;
	SPI_Init.ControlMode = 1;
	SPI_Init.CPHA = 0;
	SPI_Init.CPOL = 0;
	SPI_Init.LSBFirst = 0;
	SPI_Init.MasterMode = 1;
	SPI_Init.SelPolarity = 0;
	SPI_Init.TranBits = 8;
	ret = VSI_InitSPI(VSI_USBSPI,0,&SPI_Init);
	if(ret != ERR_SUCCESS){
		printf("Initialization device error :%d\n",ret);
		return ret;
	}else{
		printf("Initialization device success!\n");
	}
	// JEDEC ID
	WriteDataTemp[0] = 0x9F;
	ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,1,ReadDataTemp,3);
	if(ret != ERR_SUCCESS){
		printf("Read flash ID error :%d\n",ret);
		return ret;
	}else{
		printf("Flash ID = 0x%06X\n",(ReadDataTemp[0]<<16)|(ReadDataTemp[1]<<8)|(ReadDataTemp[2]));
	}
	// Open file
	fp=fopen("Data.txt","w");
	if(fp == NULL){
		printf("Open file error!\n");
		return 0;
	}
	printf("Start read data,please wait...\n");
	// Read data
	for(Addr = StartAddr;Addr < EndAddr;Addr += 4096){
		WriteDataTemp[0] = 0x0B;
		WriteDataTemp[1] = Addr >> 16;
		WriteDataTemp[2] = Addr >> 8;
		WriteDataTemp[3] = Addr >> 0;
		WriteDataTemp[4] = 0x00;
		ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,4,ReadDataTemp,4096);
		if(ret != ERR_SUCCESS){
			printf("Read data error :%d\n",ret);
			return ret;
		}else{
			for(int j=0;j<4096;j+=64){
				memset(strTmp,0,1024);
				for(i=0;i<32;i++){
					sprintf(str,"%d,",(ReadDataTemp[j+i*2]<<8)|ReadDataTemp[j+i*2+1]);
					strcat(strTmp,str);
				}
				fprintf(fp,"%s\n",strTmp);
			}
		}
	}
	printf("Read data end!\n");
	return 0;
}

