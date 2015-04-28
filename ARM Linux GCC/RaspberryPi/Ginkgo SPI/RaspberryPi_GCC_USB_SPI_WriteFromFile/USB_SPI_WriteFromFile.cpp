  /*
  ******************************************************************************
  * @file     : USB_SPI_WriteFromFile.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/18 10:47
  * @brief    : USB_SPI_WriteFromFile demo
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
  Hardware Connection

  No hardware connected

  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ControlSPI.h"


int main(int argc, const char* argv[])
{
	int ret,i,j;
	FILE *fp;
	VSI_INIT_CONFIG	SPI_Init;
	uint8_t	WriteDataTemp[10240]={0};
	char data_buff[40000]={0};
	char *strTemp[10000]={NULL};
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
	SPI_Init.ClockSpeed = 36000000;
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
	// Open file
	fp=fopen("Test File.txt","r");
	if(fp == NULL){
		printf("Open file error!\n");
		return 0;
	}
	// Read file and write to SPI
	do{
		memset(data_buff,0,sizeof(data_buff));
		// Read data one row from file
		fgets(data_buff,sizeof(data_buff)-1,fp);
		i=0;
		// The string every row use space to separate
		strTemp[i]= (char *)strtok(data_buff," ");
		while(strTemp[i++]!=NULL) {
			strTemp[i]=(char *)strtok(NULL," ");
		}
        // The string change to numerical 
		if(i>1){
			printf("Write Data(Hex): ");
		}
		for(j=0;j<(i-1);j++){
			WriteDataTemp[j] = strtol(strTemp[j],NULL,0);
			printf("%02X ",WriteDataTemp[j]);
		}
		printf("\n");
		// Send out the data via USB_SPI
		if(j>0){
			ret = VSI_WriteBytes(VSI_USBSPI,0,0,WriteDataTemp,j);
			if(ret != ERR_SUCCESS){
				printf("Write enable error\n");
				return ret;
			}
		}
	}while(!feof(fp));
	fclose(fp);
	return 0;
}

