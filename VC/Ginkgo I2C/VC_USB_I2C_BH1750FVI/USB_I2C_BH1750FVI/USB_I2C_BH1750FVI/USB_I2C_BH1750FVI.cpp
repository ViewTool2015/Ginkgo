  /*
  ******************************************************************************
  * @file     : USB_I2C_BH1750FVI.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 11:03
  * @brief    : USB_I2C_BH1750FVI demo
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
  BH1750FVI      Ginkgo USB-I2C Adapter
  1.VCC	    <-->  VCC(Pin2)
  2.ADDR    <-->  GND(Pin19/Pin20)
  3.GND     <-->  GND(Pin19/Pin20)
  4.SDA	    <-->  HI2C_SDA0(Pin8)
  5.DVI     <-->  VCC(Pin2)
  6.SCL	    <-->  HI2C_SCL0 (Pin6)
  */


#include "stdafx.h"
#include "ControlI2C.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int ret,i;
	VII_INIT_CONFIG I2C_Config;
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
    I2C_Config.ClockSpeed = 100000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_NONE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize device error!\n");
        return ret;
    }
    //Send command
    write_buffer[0] = 0x01;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x46, 0x00, write_buffer, 1);
    if (ret != ERR_SUCCESS)
    {
        printf("Write data error!\n");
        return ret;
    }
    //Send command
    write_buffer[0] = 0x10;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x46, 0x00, write_buffer, 1);
    if (ret != ERR_SUCCESS)
    {
        printf("Write data error!\n");
        return ret;
    }
    //Loop measurement
	while(1)
	{
        //Delay
        Sleep(1000);
        //Get the measurement data
        ret = VII_ReadBytes(VII_USBI2C, 0, 0, 0x46, 0x00, read_buffer, 2);
        if (ret != ERR_SUCCESS)
        {
            printf("Read data error!\n");
            return ret;
        }
        else
        {
            double illuminance = ((read_buffer[0] << 8) | read_buffer[1]) / 1.2;
            system("cls");
            printf("Light intensity: %.2f lx\n" ,illuminance);
        }
	}
	return 0;
}

