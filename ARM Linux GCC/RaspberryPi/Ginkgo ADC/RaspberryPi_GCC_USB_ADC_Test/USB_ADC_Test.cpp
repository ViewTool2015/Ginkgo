  /*
  ******************************************************************************
  * @file     : USB_ADC_Test.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 9:23
  * @brief    : USB_ADC_Test demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ControlADC.h"


int main(int argc, const char* argv[])
{
    int ret;
	uint8_t SamplingCnt = 0;
    //Scan connected device
    ret = VAI_ScanDevice(1);
    if (ret <= 0)
    {
        printf("No device connect!\n");
        return ret;
    }
    // Open device
    ret = VAI_OpenDevice(VAI_USBADC, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    // Initialize ADC_CH0 channel
    ret = VAI_InitADC(VAI_USBADC, 0, VAI_ADC_CH0,0);// Cycle can be set to 0 if each channel transfer a byte of data at a time.
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize ADC error!\n");
        return ret;
    }
    // Get voltage values of ADC_CH0 
	uint16_t adc_datas[4096] = {0};
    ret = VAI_ReadDatas(VAI_USBADC, 0, 1, adc_datas);
    if (ret != ERR_SUCCESS)
    {
        printf("Read ADC data error!\n");
        return ret;
    }
    else
    {
        printf("ADC_CH0 = %.3f\n" ,((adc_datas[0]*3.3)/4095));
        printf("\n");
    }
    // Initialize ADC_CH0 & ADC_CH1
    ret = VAI_InitADC(VAI_USBADC, 0, VAI_ADC_CH0 | VAI_ADC_CH1, 0);// Cycle can be set to 0 if each channel transfer a byte of data at a time.
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize ADC error!\n");
        return ret;
    }
    // Get voltage values of ADC_CH0 & ADC_CH1(each channel transfer a byte of data at a time.)
    ret = VAI_ReadDatas(VAI_USBADC, 0, 1, adc_datas);
    if (ret != ERR_SUCCESS)
    {
        printf("Read ADC data error!\n");
        return ret;
    }
    else
    {
        printf("ADC_CH0 = %.3f\n" , ((adc_datas[0] * 3.3) / 4095));
        printf("ADC_CH1 = %.3f\n" , ((adc_datas[1] * 3.3) / 4095));
		printf("\n");
    }
	// Initialize ADC_CH0 & ADC_CH1, each channel sampling interval is set to 1000us
    ret = VAI_InitADC(VAI_USBADC, 0, VAI_ADC_CH0 | VAI_ADC_CH1, 1000);// Cycle can be set to 0 if each channel transfer a byte of data at a time.
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize ADC error!\n");
        return ret;
    }
	// Get voltage values of ADC_CH0 & ADC_CH1
    ret = VAI_ReadDatas(VAI_USBADC, 0, 10, adc_datas);//Get 10 values each channel, return a total of 20 values
    if (ret != ERR_SUCCESS)
    {
        printf("Read ADC data error!\n");
        return ret;
    }
    else
    {
        for (int i = 0; i < 10; i++)
        {
            printf("ADC_CH0[%d] = %.3f\n" ,i, ((adc_datas[i*2] * 3.3) / 4095));
        }
        printf("\n");
        for (int i = 0; i < 10; i++)
        {
            printf("ADC_CH1[%d] = %.3f\n" ,i, ((adc_datas[i*2+1] * 3.3) / 4095));
        }
    }


	printf("=====================================================================\n");


	while(1)
	{
		// Get voltage values of ADC_CH0 & ADC_CH1
		ret = VAI_ReadDatas(VAI_USBADC, 0, 10, adc_datas);//Get 10 values each channel, return a total of 10 values,1ms*10=10ms
		if (ret != ERR_SUCCESS)
		{
			printf("Read ADC data error!\n");
			return ret; // exit if error ocurred
		}

		// 10ms * 100 =1s in theory,but there is other time-consuming in practice,I choose 50
		if( 50 == ++SamplingCnt ) //
		{
			SamplingCnt = 0;
			printf("ADC_CH0 = %.3f\n" , ((adc_datas[0] * 3.3) / 4095));
		}
	}


    // close device
    ret = VAI_CloseDevice(VAI_USBADC, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Close device error!\n");
        return ret;
    }
	return 0;
}

