  /*
  ******************************************************************************
  * @file       USB_SPI_AT45DB161D.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/17 17:08
  * @brief    : USB_SPI_AT45DB161D demo
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
  AT45DB161D      Ginkgo USB-SPI Adapter
  1.SI      <-->  SPI_MOSI(Pin17)
  2.SCK	    <-->  SPI_SCK(Pin13)
  3.RESET   <-->  VCC(Pin2)
  4.CS      <-->  SPI_SEL0(Pin11)
  5.WP	    <-->  VCC(Pin2)
  6.VCC	    <-->  VCC(Pin2)
  7.GND	    <-->  GND(Pin19/Pin20)
  8.SO	    <-->  SPI_MISO(Pin15)
  */


#include "stdafx.h"
#include "ErrorType.h"
#include "ControlSPI.h"

#define FLASH_CHREAD		0x0B
#define FLASH_CLREAD		0x03
#define FLASH_PREAD			0xD2

#define FLASH_BUFWRITE1		0x84
#define FLASH_IDREAD		0x9F
#define FLASH_STATUS		0xD7
#define PAGE_ERASE			0x81
#define PAGE_READ			0xD2
#define MM_PAGE_TO_B1_XFER	0x53					// main memory specified page load into buffer 1
#define BUFFER_2_WRITE		0x87					// buffer 2 write
#define BUFFER_1_READ		0xD4					// buffer 1 read
#define B2_TO_MM_PAGE_PROG_WITH_ERASE		0x86	// buffer to main memory page program with build-in erase
#define B2_TO_MM_PAGE_PROG_WITHOUT_ERASE	0x89	// buffer to main memory page program without build-in erase

int _tmain(int argc, _TCHAR* argv[])
{
	int ret,i;
	VSI_INIT_CONFIG	SPI_Init;
	uint8_t	WriteDataTemp[1024]={0};
	uint8_t	ReadDataTemp[1024]={0};
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
	WriteDataTemp[0] = FLASH_IDREAD;
	ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,1,ReadDataTemp,4);
	if(ret != ERR_SUCCESS){
		printf("Read flash ID error :%d\n",ret);
		return ret;
	}else{
		printf("Flash ID = 0x%08X\n",(ReadDataTemp[0]<<24)|(ReadDataTemp[1]<<16)|(ReadDataTemp[2]<<8)|(ReadDataTemp[3]));
	}
	// Wait Busy
	do{
		WriteDataTemp[0] = FLASH_STATUS;
		ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,1,ReadDataTemp,1);
	}while(((ReadDataTemp[0]&0x80)==0)&&(ret == ERR_SUCCESS));
	if(ret != ERR_SUCCESS){
		printf("Sector Erase error :%d\n",ret);
		return ret;
	}else{
		printf("Sector erase success!\n");
	}
	// Page Erase
	WriteDataTemp[0] = PAGE_ERASE;
	WriteDataTemp[1] = 0x00;
	WriteDataTemp[2] = 0x00;
	WriteDataTemp[3] = 0x00;
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,WriteDataTemp,4);
	if(ret != ERR_SUCCESS){
		printf("Sector Erase start error :%d\n",ret);
		return ret;
	}else{
		printf("Sector erase start success!\n");
	}
	// Wait Busy
	do{
		WriteDataTemp[0] = FLASH_STATUS;
		ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,1,ReadDataTemp,1);
	}while(((ReadDataTemp[0]&0x80)==0)&&(ret == ERR_SUCCESS));
	if(ret != ERR_SUCCESS){
		printf("Sector Erase error :%d\n",ret);
		return ret;
	}else{
		printf("Sector erase success!\n");
	}
	// Write data to buffer
	WriteDataTemp[0] = BUFFER_2_WRITE;
	WriteDataTemp[1] = 0x00;
	WriteDataTemp[2] = 0x00;
	WriteDataTemp[3] = 0x00;
	for(i=0;i<528;i++){
		WriteDataTemp[4+i] = (char)i;
	}
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,WriteDataTemp,528+4);
	if(ret != ERR_SUCCESS){
		printf("Write data to buffer error :%d\n",ret);
		return ret;
	}else{
		printf("Write data to buffer success!\n");
	}
	// Buffer to Main Memory Page Program without Built-in Erase
	WriteDataTemp[0] = B2_TO_MM_PAGE_PROG_WITHOUT_ERASE;
	WriteDataTemp[1] = 0x00;
	WriteDataTemp[2] = 0x00;
	WriteDataTemp[3] = 0x00;
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,WriteDataTemp,4);
	if(ret != ERR_SUCCESS){
		printf("Write data to main memory error :%d\n",ret);
		return ret;
	}else{
		printf("Write data to main memory success!\n");
	}
	// Wait Busy
	do{
		WriteDataTemp[0] = FLASH_STATUS;
		ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,1,ReadDataTemp,1);
	}while(((ReadDataTemp[0]&0x80)==0)&&(ret == ERR_SUCCESS));
	// Main Memory Page Read
	WriteDataTemp[0] = PAGE_READ;
	WriteDataTemp[1] = 0x00;
	WriteDataTemp[2] = 0x00;
	WriteDataTemp[3] = 0x00;
	WriteDataTemp[4] = 0x00;
	ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,8,ReadDataTemp,528);
	if(ret != ERR_SUCCESS){
		printf("Read data to buffer error :%d\n",ret);
		return ret;
	}else{
		printf("Read data to buffer success!\n");
		for(i=0;i<528;i++){
			if((i%25)==0){
				printf("\n");
			}
			printf("%02X ",ReadDataTemp[i]);
		}
		printf("\n");
	}

	return 0;
}

