  /*
  ******************************************************************************
  * @file     : USB_I2C_AT24C02_SCTL.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 11:00
  * @brief    : USB_I2C_AT24C02_SCTL demo
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
  Hardware Connection  (This is for your reference only)
  AT24C02        Ginkgo USB-I2C Adapter
  1.A0      <-->  GND(Pin19/Pin20)
  2.A1	    <-->  GND(Pin19/Pin20)
  3.A2      <-->  GND(Pin19/Pin20)
  4.GND     <-->  GND(Pin19/Pin20)
  5.SDA	    <-->  SI2C_SDA0(Pin6)
  6.SCL	    <-->  SI2C_SCL0 (Pin4)
  7.WP	    <-->  GND(Pin19/Pin20)
  8.VCC	    <-->  VCC(Pin1/Pin2)
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ControlI2C.h"

int main(int argc, const char* argv[])
{
	int ret,i;
	VII_INIT_CONFIG I2C_Config;
	VII_TIME_CONFIG I2C_TimeConfig;
	uint8_t write_buffer[8]={0};
	uint8_t	read_buffer[8]={0};
	//Scan device
	ret = VII_ScanDevice(1);
	if(ret <= 0)
	{
		printf("No device connect!\n");
		return ret;
	}
    //Open device
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    //Initializes the device
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 400000;
    I2C_Config.ControlMode = VII_SCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize device error!\n");
        return ret;
    }
    //Config timing,Unit of time for microseconds
    I2C_TimeConfig.tSU_STA = 5;
    I2C_TimeConfig.tHD_STA = 4;
    I2C_TimeConfig.tLOW = 5; 
    I2C_TimeConfig.tHIGH = 5;
    I2C_TimeConfig.tSU_DAT = 1;
    I2C_TimeConfig.tSU_STO = 4;
    I2C_TimeConfig.tBuf = 5;  
    ret = VII_TimeConfig(VII_USBI2C, 0, 0, &I2C_TimeConfig);
    if (ret != ERR_SUCCESS)
    {
        printf("Set time error!\n");
        return ret;
    }
	//Write 8 byte data to 0x00
	for (i = 0; i < 8; i++)
    {
        write_buffer[i] = i;
    }
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xA0, 0x00, write_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
        printf("Write data error!\n");
        return ret;
    }
    //Delay
    usleep(10*1000);
	//Read 8 byte data from 0x00
    ret = VII_ReadBytes(VII_USBI2C, 0, 0, 0xA0, 0x00, read_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
        printf("Read data error!\n");
        return ret;
    }
	else
	{
		printf("Read Data:\n");
		for(i=0;i<8;i++)
		{
			printf("%02X ",read_buffer[i]);
		}
		printf("\n");
	}
	return 0;
}

