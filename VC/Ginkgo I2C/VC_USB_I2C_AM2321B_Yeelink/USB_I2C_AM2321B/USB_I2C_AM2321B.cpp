  /*
  ******************************************************************************
  * @file     : USB_I2C_AM2321B.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 10:17
  * @brief    : USB_I2C_AM2321B demo
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
#include "yeelink.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int ret,i;
	VII_INIT_CONFIG I2C_Config;
	uint8_t write_buffer[8]={0};
	uint8_t	read_buffer[8]={0};
	ret = Yeelink_GetApiKey("viewtool","viewtool2013");// Your Yeelink ID & password
	if(ret != ERR_SUCCESS){
		printf("Get api key error!");
		return ret;
	}
	// Scan connected device
	ret = VII_ScanDevice(1);
	if(ret <= 0)
	{
		printf("No device connect!\n");
		return ret;
	}
    // Open device
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    // Initialize device (Hardware I2C)
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
	// Loop reading  temperature & humidity sensor data 
	while(1)
	{
		uint8_t write_buffer[8] = {0};
        //Wake up AM2311 sensor
        VII_WriteBytes(VII_USBI2C, 0, 0, 0xB8, 0x00, write_buffer, 1);
        //Send out read temperature and humidity command
        write_buffer[0] = 0x03;
        write_buffer[1] = 0x00;
        write_buffer[2] = 0x04;
        ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xB8, 0x00, write_buffer, 3);
        if (ret != ERR_SUCCESS)
        {
            printf("Write data error!\n");
            return ret;
        }
        // Read out temperature and humidity
		uint8_t read_buffer[8] = {0};
        ret = VII_ReadBytes(VII_USBI2C, 0, 0, 0xB8, 0x00, read_buffer, 8);
        if (ret != ERR_SUCCESS)
        {
            printf("Read data error!\n");
            return ret;
        }
        else
        {
            double t = ((read_buffer[4] << 8) | read_buffer[5]) / 10.0;
            system("cls");
            printf("温度值：%.1f ℃\n",t);
            double h = ((read_buffer[2] << 8) | read_buffer[3]) / 10.0;
            printf("湿度值：%.1f ％\n",h);
			Sleep(10000);
			char StrTmp[1024]={0};
			sprintf(StrTmp,"%.1f",t);
			ret = Yeelink_PostData("9433","14860",StrTmp);// Your device ID & sensor ID on Yeelink, and sensor data
			if(ret != ERR_SUCCESS){
				printf("Post data error!");
			}
			sprintf(StrTmp,"%.1f",h);
			ret = Yeelink_PostData("9433","14861",StrTmp);// Your device ID & sensor ID on Yeelink, and sensor data
			if(ret != ERR_SUCCESS){
				printf("Post data error!");
			}
        }
	}
	return 0;
}

