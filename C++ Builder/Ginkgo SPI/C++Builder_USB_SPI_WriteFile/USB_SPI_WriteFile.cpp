#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include "ControlSPI.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int ret,i,j;
	FILE *fp;
	VSI_INIT_CONFIG	SPI_Init;
	uint8_t	WriteDataTemp[10240]={0};
	uint8_t	ReadDataTemp[10240]={0};
	char data_buff[40000]={0};
	char *strTemp[10000]={NULL};
	//Check the device number of connections
	ret = VSI_ScanDevice(1);
	if(ret>0){
		printf("The connected device number is:%d\n",ret);
	}else{
		printf("No device to connect!\n");
		system("pause");
		return 0;
	}
	//open Device
	ret = VSI_OpenDevice(VSI_USBSPI,0,0);
	if(ret != ERR_SUCCESS){
		printf("Open device error :%d\n",ret);
		system("pause");
		return ret;
	}else{
		printf("Open device success!\n");
	}
	//Device initialization
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
		system("pause");
		return ret;
	}else{
		printf("Initialization device success!\n");
	}
	//Open file
	fp=fopen("Test File.txt","r");
	if(fp == NULL){
		printf("Open file error!\n");
		system("pause");
		return 0;
	}
	//read file and write to spi
	int data_num = 0;
	do{
		memset(data_buff,0,sizeof(data_buff));
		//Read a line of data
		fgets(data_buff,sizeof(data_buff)-1,fp);
		i=0;
		//To separate a row string with Spaces
		strTemp[i]= (char *)strtok(data_buff," ");
		while(strTemp[i++]!=NULL) {
			strTemp[i]=(char *)strtok(NULL," ");
		}
		//Converts each separate data string values
		if(i>1){
			printf("Write Data(Hex): ");
		}
		for(j=0;j<(i-1);j++){
			WriteDataTemp[j] = strtol(strTemp[j],NULL,0);
			printf("%02X ",WriteDataTemp[j]);
		}
		printf("\n");
		//The data sent via USB - SPI
		if(j>0){
			ret = VSI_WriteBytes(VSI_USBSPI,0,0,WriteDataTemp,j);
			if(ret != ERR_SUCCESS){
				printf("Write enable error\n",ret);
				system("pause");
				return ret;
			}
		}
	}while(!feof(fp));
	fclose(fp);
	system("pause");
	return 0;
}
