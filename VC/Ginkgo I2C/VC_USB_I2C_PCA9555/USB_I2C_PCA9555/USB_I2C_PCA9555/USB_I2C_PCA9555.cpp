  /*
  ******************************************************************************
  * @file     : USB_I2C_PCA9555.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 11:26
  * @brief    : USB_I2C_PCA9555 demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
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
    I2C_Config.ClockSpeed = 400000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize device error!\n");
        return ret;
    }
    //Set all ports to output mode
    write_buffer[0] = 0;
    write_buffer[1] = 0;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x40, 0x06, write_buffer, 2);
    if (ret != ERR_SUCCESS)
    {
        printf("Write data error!\n");
        return ret;
    }
	while(1)
	{
        //Control all the port output low level
        write_buffer[0] = 0;
        write_buffer[1] = 0;
        ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x40, 0x02, write_buffer, 2);
        if (ret != ERR_SUCCESS)
        {
            printf("Write data error!\n");
            return ret;
        }
        Sleep(500);
        //Control of all port output high level
        write_buffer[0] = 0xFF;
        write_buffer[1] = 0xFF;
        ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x40, 0x02, write_buffer, 2);
        if (ret != ERR_SUCCESS)
        {
            printf("Write data error!\n");
            return ret;
        }
        Sleep(500);
	}
	return 0;
}

