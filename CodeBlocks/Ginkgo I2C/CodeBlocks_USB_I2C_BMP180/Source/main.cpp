#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "ControlI2C.h"
#include "bmp180.h"

using namespace std;

#define I2C_BUFFER_LEN  64

int DeviceIndex=0,ChannelIndex=0;


signed char BMP180_I2C_bus_read(unsigned char device_addr, unsigned char reg_addr, unsigned char *reg_data, unsigned char cnt)
{
    int iError=0;
    unsigned char DtaTmp[I2C_BUFFER_LEN];
    unsigned char stringpos;
    iError = VII_ReadBytes(VII_USBI2C,DeviceIndex,ChannelIndex,device_addr<<1,reg_addr,DtaTmp,cnt);
    for(stringpos=0; stringpos<cnt; stringpos++) {
        *(reg_data + stringpos) = DtaTmp[stringpos];
    }
    return (signed char)iError;
}

signed char BMP180_I2C_bus_write(unsigned char device_addr, unsigned char reg_addr, unsigned char *reg_data, unsigned char cnt)
{
    int iError=0;
    unsigned char DtaTmp[I2C_BUFFER_LEN];
    unsigned char stringpos;
    for(stringpos=0; stringpos<cnt; stringpos++) {
        DtaTmp[stringpos] = *(reg_data + stringpos);
    }
    iError = VII_WriteBytes(VII_USBI2C,DeviceIndex,ChannelIndex,device_addr<<1,reg_addr,DtaTmp,cnt);
    return (signed char)iError;
}

void BMP180_delay_msek(unsigned int msek) //delay in milliseconds
{
#ifndef OS_UNIX
    Sleep(msek);
#else
    usleep(msek*1000);
#endif
}

int main()
{
    int ret;
    bmp180_t bmp180;
    VII_INIT_CONFIG I2C_Config;
    //scan device
    ret = VII_ScanDevice(1);
    if(ret <= 0) {
        printf("No device connect!\n");
        return ret;
    }
    //open device
    ret = VII_OpenDevice(VII_USBI2C,DeviceIndex,ChannelIndex);
    if (ret != ERR_SUCCESS) {
        printf("Open device error!\n");
        return ret;
    }
    //initialize device
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 100000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    ret = VII_InitI2C(VII_USBI2C,DeviceIndex,ChannelIndex,&I2C_Config);
    if (ret != ERR_SUCCESS) {
        printf("Initialize device error!\n");
        return ret;
    }
    bmp180.bus_read = BMP180_I2C_bus_read;   // assign read function
    bmp180.bus_write = BMP180_I2C_bus_write; // assign write function
    bmp180.delay_msec = BMP180_delay_msek;   // assign wait function
    bmp180.oversampling_setting = 3;
    //initialize bmp180
    bmp180_init(&bmp180);
#ifndef OS_UNIX
    Sleep(100);
#else
    usleep(100000);
#endif // OS_UNIX
    //Get data
    while(1) {

        double t = bmp180_get_temperature(bmp180_get_ut())/10.0;//temperature
        double p = bmp180_get_pressure(bmp180_get_up())/1000.0;//pressure
        double a = (44330.0 * (1.0-pow(p*1000 / 101325.0, 1.0/5.255)) );//Altitude
#ifndef OS_UNIX
        system("cls");
#else
        system("clear");
#endif // OS_UNIX
        printf("Altitude = %.2f\n",a);
        printf("Temperature = %.2f\n",t);
        printf("Pressure = %.2f\n",p);
#ifndef OS_UNIX
        Sleep(1000);
#else
        usleep(1000000);
#endif // OS_UNIX
    }
    getchar();
    return 0;
}
