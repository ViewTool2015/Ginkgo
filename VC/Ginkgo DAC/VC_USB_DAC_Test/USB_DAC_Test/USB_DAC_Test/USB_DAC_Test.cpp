// USB_DAC_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ControlDAC.h"
#include <math.h>


int _tmain(int argc, _TCHAR* argv[])
{
	int ret;
	uint16_t WaveDataBuff[1024];
	//Scan device
	ret = VDA_ScanDevice(1);
	if(ret <= 0){
		printf("No device connected!\n");
		return ret;
	}
	//Open device
	ret = VDA_OpenDevice(VDA_USBDAC,0,0);
	if(ret != ERR_SUCCESS){
		printf("Open device error!\n");
		return ret;
	}
	//Configuration DAC to output dc voltage
	WaveDataBuff[0] = 1532;//Vout = (3.3V/4095)*1532 = 1.23V
	printf("Output DC Voltage: %f\n",(1532*3.3/4095));
	ret = VDA_InitDAC(VDA_USBDAC,0,VDA_DAC_CH0,WaveDataBuff,1,0);
	if(ret != ERR_SUCCESS){
		printf("Config DAC error!\n");
		return ret;
	}
	//Start DAC to output
	ret = VDA_StartDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	if(ret != ERR_SUCCESS){
		printf("Start DAC error!\n");
		return ret;
	}
	getchar();
	VDA_StopDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	//Configuration DAC to output square wave
	for(int i=0;i<64;i++){
		WaveDataBuff[i] = 0xFFF;
	}
	for(int i=0;i<64;i++){
		WaveDataBuff[64+i] = 0x00;
	}
	//Square wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
	printf("Output Square Wave!\n");
	ret = VDA_InitDAC(VDA_USBDAC,0,VDA_DAC_CH0,WaveDataBuff,128,100);
	if(ret != ERR_SUCCESS){
		printf("Config DAC error!\n");
		return ret;
	}
	//Start DAC to output
	ret = VDA_StartDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	if(ret != ERR_SUCCESS){
		printf("Start DAC error!\n");
		return ret;
	}
	getchar();
	VDA_StopDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	//Configuration DAC to output sawtooth wave 
	for(int i=0;i<128;i++){
		WaveDataBuff[i] = i*0xFFF/128;
	}
	//Sawtooth wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
	printf("Output Sawtooth Wave!\n");
	ret = VDA_InitDAC(VDA_USBDAC,0,VDA_DAC_CH0,WaveDataBuff,128,100);
	if(ret != ERR_SUCCESS){
		printf("Config DAC error!\n");
		return ret;
	}
	//Start DAC to output
	ret = VDA_StartDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	if(ret != ERR_SUCCESS){
		printf("Start DAC error!\n");
		return ret;
	}
	getchar();
	VDA_StopDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	//Configuration DAC to output triangle wave
	for(int i=0;i<64;i++){
		WaveDataBuff[i] = i*0xFFF/64;
	}
	for(int i=0;i<64;i++){
		WaveDataBuff[64+i] = (64-i)*0xFFF/64;
	}
	//Triangle wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
	printf("Output Triangle Wave!\n");
	ret = VDA_InitDAC(VDA_USBDAC,0,VDA_DAC_CH0,WaveDataBuff,128,100);
	if(ret != ERR_SUCCESS){
		printf("Config DAC error!\n");
		return ret;
	}
	//Start DAC to output
	ret = VDA_StartDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	if(ret != ERR_SUCCESS){
		printf("Start DAC error!\n");
		return ret;
	}
	getchar();
	VDA_StopDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	//Configuration DAC to output Sine  wave
	for(int i=0;i<128;i++){
		WaveDataBuff[i] = 0xFFF*(1+sin(2*3.141592653589793*i/128))/2.0;
	}
	//Sine wave frequency = 1/(Period*WaveDataNum) = 1/(100us * 128) = 78.125Hz
	printf("Output Sine Wave!\n");
	ret = VDA_InitDAC(VDA_USBDAC,0,VDA_DAC_CH0,WaveDataBuff,128,100);
	if(ret != ERR_SUCCESS){
		printf("Config DAC error!\n");
		return ret;
	}
	//Start DAC to output
	ret = VDA_StartDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	if(ret != ERR_SUCCESS){
		printf("Start DAC error!\n");
		return ret;
	}
	getchar();
	VDA_StopDAC(VDA_USBDAC,0,VDA_DAC_CH0);
	//Close device
	VDA_CloseDevice(VDA_USBDAC,0);
	return 0;
}

