#pragma hdrstop
#pragma argsused

#include <cstdlib>
#include <tchar.h>
#include <stdio.h>
#include "ControlI2C.h"

int _tmain(int argc, _TCHAR* argv[])
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
		system("pause");
		return ret;
	}
    //Open device
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Open device error!\n");
		system("pause");
        return ret;
    }
    //Initialize the device (software control mode)
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 400000;
	I2C_Config.ControlMode = VII_SCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS)
	{
		printf("Initialize device error!\n");
		system("pause");
        return ret;
    }
	//Configuration time parameters, software mode must be configured, unit of time for microseconds
    I2C_TimeConfig.tSU_STA = 5;
	I2C_TimeConfig.tHD_STA = 4;
	//The following two parameters determine the clock frequency, here to set the clock frequency of 100 KHZ
	I2C_TimeConfig.tLOW = 5;
    I2C_TimeConfig.tHIGH = 5;
    I2C_TimeConfig.tSU_DAT = 1;
    I2C_TimeConfig.tSU_STO = 4;
	I2C_TimeConfig.tBuf = 5;
	//Other time parameters can not be set
    ret = VII_TimeConfig(VII_USBI2C, 0, 0, &I2C_TimeConfig);
    if (ret != ERR_SUCCESS)
    {
		printf("Set time error!\n");
		system("pause");
        return ret;
	}
	//Write 8 bytes of data to address 0x00
	for (i = 0; i < 8; i++)
    {
        write_buffer[i] = i;
    }
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xA0, 0x00, write_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("pause");
        return ret;
    }
    //Delay for a period of time, to ensure that data to complete
    Sleep(10);
	//Read 8 bytes of data from address 0x00
    ret = VII_ReadBytes(VII_USBI2C, 0, 0, 0xA0, 0x00, read_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
		printf("Read data error!\n");
		system("pause");
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
	system("pause");
	return 0;
}
