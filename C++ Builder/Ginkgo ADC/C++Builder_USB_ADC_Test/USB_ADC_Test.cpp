#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include "ControlADC.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int ret;
    //扫描已经连接的设备
    ret = VAI_ScanDevice(1);
    if (ret <= 0)
    {
		printf("No device connect!\n");
		system("pause");
        return ret;
    }
    //打开设备
    ret = VAI_OpenDevice(VAI_USBADC, 0, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Open device error!\n");
		system("pause");
        return ret;
    }
    //初始化ADC的ADC_CH0通道
    ret = VAI_InitADC(VAI_USBADC, 0, VAI_ADC_CH0,0);//如果每次每个通道只读一个数据，其周期可设置为0
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize ADC error!\n");
		system("pause");
        return ret;
    }
    //获取ADC_CH0通道的电压值
	uint16_t adc_datas[4096] = {0};
    ret = VAI_ReadDatas(VAI_USBADC, 0, 1, adc_datas);
    if (ret != ERR_SUCCESS)
    {
		printf("Read ADC data error!\n");
		system("pause");
        return ret;
    }
    else
    {
		printf("ADC_CH0 = %.3f\n" ,((adc_datas[0]*3.3)/4095));
        printf("\n");
    }
    //初始化ADC的ADC_CH0,ADC_CH1通道
    ret = VAI_InitADC(VAI_USBADC, 0, VAI_ADC_CH0 | VAI_ADC_CH1, 0);//如果每次每个通道只读一个数据，其周期可设置为0
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize ADC error!\n");
		system("pause");
        return ret;
    }
    //获取ADC_CH0,ADC_CH1通道的电压值（每个通道获取一个值）
    ret = VAI_ReadDatas(VAI_USBADC, 0, 1, adc_datas);
    if (ret != ERR_SUCCESS)
    {
		printf("Read ADC data error!\n");
		system("pause");
        return ret;
    }
    else
    {
        printf("ADC_CH0 = %.3f\n" , ((adc_datas[0] * 3.3) / 4095));
        printf("ADC_CH1 = %.3f\n" , ((adc_datas[1] * 3.3) / 4095));
		printf("\n");
    }
    //初始化ADC的ADC_CH0,ADC_CH1通道,每个通道采样时间间隔设置为1000微秒
    ret = VAI_InitADC(VAI_USBADC, 0, VAI_ADC_CH0 | VAI_ADC_CH1, 1000);//如果每次每个通道只读一个数据，其周期可设置为0
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize ADC error!\n");
		system("pause");
        return ret;
    }
    //获取ADC_CH0,ADC_CH1通道的电压值
    ret = VAI_ReadDatas(VAI_USBADC, 0, 10, adc_datas);//每个通道获取10个值，总共返回20个值，耗时1000us*10 = 10ms
    if (ret != ERR_SUCCESS)
    {
		printf("Read ADC data error!\n");
		system("pause");
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
    //关闭设备
    ret = VAI_CloseDevice(VAI_USBADC, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Close device error!\n");
		system("pause");
        return ret;
	}
	system("pause");
	return 0;
}
