// USB_I2C_HMC5883L.cpp : Defines the entry point for the console application.
//
/*Hardware connected
HMC5883		USB-I2C
1.VCC		3.3V
2.GND		GND
3.SCL		HI2C_SCL0(P6)
4.SDA		HI2C_SDA0(P8)
*/


#include "stdafx.h"
#include "ControlI2C.h"

#define Address 0x1E				//0011110b, I2C 7bit address of HMC5883

int _tmain(int argc, _TCHAR* argv[])
{
	int ret,i;
	VII_INIT_CONFIG I2C_Config;
	VII_BOARD_INFO BoardInfo;
	uint8_t write_buffer[8]={0};
	uint8_t	read_buffer[8]={0};
	//Scan device
	ret = VII_ScanDevice(1);
	if(ret <= 0){
		printf("No device connect!\n");
		return ret;
	}
    //Open device
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS){
        printf("Open device error!\n");
        return ret;
    }
	//Get product information
	ret = VII_ReadBoardInfo(0,&BoardInfo);
    if (ret != ERR_SUCCESS){
        printf("Read board information error!\n");
        return ret;
    }else{
		printf("Product Name:%s\n",BoardInfo.ProductName);
		printf("Firmware Version:V%d.%d.%d\n",BoardInfo.FirmwareVersion[1],BoardInfo.FirmwareVersion[2],BoardInfo.FirmwareVersion[3]);
		printf("Hardware Version:V%d.%d.%d\n",BoardInfo.HardwareVersion[1],BoardInfo.HardwareVersion[2],BoardInfo.HardwareVersion[3]);
		printf("Serial Number:");
		for(i=0;i<12;i++){
			printf("%02X",BoardInfo.SerialNumber[i]);
		}
		printf("\n");
	}
    //Initializes the device
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 400000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS){
        printf("Initialize device error!\n");
        return ret;
    }
	//Put the HMC5883 IC into the correct operating mode
	write_buffer[0] = 0x00;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, Address << 1, 0x02, write_buffer, 1);
    if (ret != ERR_SUCCESS){
        printf("Write data error!\n");
        return ret;
    }
	while(1)
	{
		//Read status
		ret = VII_ReadBytes(VII_USBI2C, 0, 0, Address << 1, 0x09, read_buffer, 1);
		if (ret != ERR_SUCCESS){
			printf("Read data error!\n");
			return ret;
		}
		//Data ready?
		if(read_buffer[0]&0x01){
			//Tell the HMC5883 where to begin reading data
			ret = VII_ReadBytes(VII_USBI2C, 0, 0, Address << 1, 0x03, read_buffer, 6);
			if (ret != ERR_SUCCESS){
				printf("Read data error!\n");
				return ret;
			}
			int x,y,z;
			x = (read_buffer[0] << 8)|read_buffer[1];
			y = (read_buffer[2] << 8)|read_buffer[3];
			z = (read_buffer[4] << 8)|read_buffer[5];
			printf("------------------------------------------------------\n");
			printf("x = %d\n",x);
			printf("y = %d\n",y);
			printf("z = %d\n",z);
			printf("------------------------------------------------------\n");
		}
#ifndef OS_UNIX
		Sleep(1000);
#else
		sleep(100);
#endif
	}
	return 0;
}

