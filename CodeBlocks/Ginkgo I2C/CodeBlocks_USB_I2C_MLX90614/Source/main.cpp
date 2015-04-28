#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "ControlSMBus.h"
#include "ControlGPIO.h"

using namespace std;

#define SCL0    VGI_GPIO_PIN1
#define SCL1    VGI_GPIO_PIN5

//  --------------------------------------------------------------------------
//  Sleep for a number of milliseconds

int main()
{
    int ret;
    int DeviceIndex = 0,ChannelIndex = 0;
    //scan device
    ret = SMBus_ScanDevice(1);
    if(ret <= 0) {
        printf("No device connect!\n");
        return ret;
    }
    //open device
    ret = SMBus_OpenDevice(VII_USBI2C,DeviceIndex,ChannelIndex);
    if (ret != SMBUS_OK) {
        printf("Open device error!\n");
        return ret;
    }
    //Switching from PWM mode to SMBus
    VGI_SetOutput(VGI_USBGPIO,DeviceIndex,ChannelIndex==0?SCL0:SCL1);
    VGI_SetPins(VGI_USBGPIO,DeviceIndex,ChannelIndex==0?SCL0:SCL1);
    VGI_ResetPins(VGI_USBGPIO,DeviceIndex,ChannelIndex==0?SCL0:SCL1);
#ifndef OS_UNIX
    Sleep(100);
#else
    usleep(100000);
#endif
    VGI_SetPins(VGI_USBGPIO,DeviceIndex,ChannelIndex==0?SCL0:SCL1);
    //Configure device
    ret = SMBus_HardInit(VII_USBI2C,DeviceIndex,ChannelIndex,100000,0x00);
    if (ret != SMBUS_OK) {
        printf("Initialize device error!\n");
        return ret;
    }
    while(1) {
#ifndef OS_UNIX
        system("cls");
#else
        system("clear");
#endif // OS_UNIX
        uint16_t dataTmp;
        //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
        double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
        double tempData = 0x0000; // zero out the data
        float celcius;
        ret = SMBus_ReadWordProtocol(VII_USBI2C,DeviceIndex,ChannelIndex,0x5A<<1,0x06,&dataTmp,1);
        if (ret != SMBUS_OK) {
            printf("Read data error!\n");
            return ret;
        }

        // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
        tempData = (dataTmp * tempFactor)-0.01;
        celcius = tempData - 273.15;
        printf("TA = %.2f\n",celcius);

        ret = SMBus_ReadWordProtocol(VII_USBI2C,DeviceIndex,ChannelIndex,0x5A<<1,0x07,&dataTmp,1);
        if (ret != SMBUS_OK) {
            printf("Read data error!\n");
            return ret;
        }

        // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
        tempData = (dataTmp * tempFactor)-0.01;
        celcius = tempData - 273.15;
        printf("OBJ1 = %.2f\n",celcius);
#ifndef OS_UNIX
        Sleep(500);
#else
        usleep(500000);
#endif
    }
    getchar();
    return 0;
}
