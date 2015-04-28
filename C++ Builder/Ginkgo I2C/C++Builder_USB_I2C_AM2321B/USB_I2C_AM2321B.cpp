#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include "ControlI2C.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int ret,i;
	VII_INIT_CONFIG I2C_Config;
	uint8_t write_buffer[8]={0};
	uint8_t	read_buffer[8]={0};
	//Scan device(must call)
	ret = VII_ScanDevice(1);
	if(ret <= 0)
	{
		printf("No device connect!\n");
		return ret;
	}
    //Open device(must call)
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    //Initialize the device (hardware control mode)
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
	//Loop read Temperature and humidity data
	while(1)
	{
		uint8_t write_buffer[8] = {0};
        //Wake up AM2311 sensor
        VII_WriteBytes(VII_USBI2C, 0, 0, 0xB8, 0x00, write_buffer, 1);
        //Send out read temperature and huminity command
        write_buffer[0] = 0x03;
        write_buffer[1] = 0x00;
        write_buffer[2] = 0x04;
        ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xB8, 0x00, write_buffer, 3);
        if (ret != ERR_SUCCESS)
        {
            printf("Write data error!\n");
            return ret;
        }
        // Read out temperature and huminity
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
			printf("Temperature: %.1f ¡æ\n",t);
            double h = ((read_buffer[2] << 8) | read_buffer[3]) / 10.0;
			printf("Humidity: %.1f £¥\n",h);
        }
        Sleep(1000);
	}
	return 0;
}
