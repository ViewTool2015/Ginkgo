#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include "ControlPWM.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int ret;
	VPI_INIT_CONFIG PWM_Config;
    //扫描已经连接的设备
    ret = VPI_ScanDevice(1);
    if (ret <= 0)
    {
		printf("No device connect!\n");
		system("pause");
        return ret;
    }
    //打开设备
    ret = VPI_OpenDevice(VPI_USBPWM, 0, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Open device error!\n");
		system("pause");
        return ret;
    }
    //初始化PWM_CH0通道
    PWM_Config.PWM_ChannelMask = VPI_PWM_CH0;
    PWM_Config.PWM_Frequency = 10000;
    PWM_Config.PWM_Mode = 0;
    PWM_Config.PWM_Polarity = 0;
    PWM_Config.PWM_Pulse = 50;
    ret = VPI_InitPWM(VPI_USBPWM,0,&PWM_Config);
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error!\n");
		system("pause");
        return ret;
    }
    //启动PWM_CH0通道
    ret = VPI_StartPWM(VPI_USBPWM,0,VPI_PWM_CH0);
    if (ret != ERR_SUCCESS)
    {
		printf("Start pwm error!\n");
		system("pause");
        return ret;
    }
    //运行1秒钟
    Sleep(1000);
    //停止PWM_CH0通道
    ret = VPI_StopPWM(VPI_USBPWM, 0, VPI_PWM_CH0);
    if (ret != ERR_SUCCESS)
    {
		printf("Stop pwm error!\n");
		system("pause");
        return ret;
    }
    //初始化所有PWM通道
    PWM_Config.PWM_ChannelMask = VPI_PWM_ALL;
    PWM_Config.PWM_Frequency = 100000;
    PWM_Config.PWM_Mode = 0;
    PWM_Config.PWM_Polarity = 0;
    PWM_Config.PWM_Pulse = 50;
    ret = VPI_InitPWM(VPI_USBPWM, 0, &PWM_Config);
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error!\n");
		system("pause");
        return ret;
    }
    //启动PWM_CH0,PWM_CH2通道
    ret = VPI_StartPWM(VPI_USBPWM, 0, VPI_PWM_CH0 | VPI_PWM_CH2);
    if (ret != ERR_SUCCESS)
    {
		printf("Start pwm error!\n");
		system("pause");
        return ret;
    }
    //运行1秒钟
    Sleep(1000);
    //调整PWM_CH0的时钟频率
	uint32_t Frequency[8] = {0};
    Frequency[0] = 50000;
    ret = VPI_SetPWMPeriod(VPI_USBPWM, 0, VPI_PWM_CH0, Frequency);
    if (ret != ERR_SUCCESS)
    {
		printf("Set frequency error!\n");
		system("pause");
        return ret;
    }
    //调整PWM_CH2的占空比
	uint8_t Pulse[8] = {0};
    Pulse[2] = 80;
    ret = VPI_SetPWMPulse(VPI_USBPWM, 0, VPI_PWM_CH2, Pulse);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pulse error!\n");
		system("pause");
        return ret;
    }
    //运行1秒钟
    Sleep(1000);
    //同时调整PWM_CH0和PWM_CH2的占空比
    Pulse[0] = 20;
    Pulse[2] = 80;
    ret = VPI_SetPWMPulse(VPI_USBPWM, 0, VPI_PWM_CH0|VPI_PWM_CH2, Pulse);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pulse error!\n");
		system("pause");
        return ret;
    }
    //运行1秒钟
    Sleep(1000);
    //注意：PWM_CH0和PWM_CH1的频率不能分开设置，PWM_CH2和PWM_CH3的频率不能分开设置，PWM_CH4和PWM_CH5的频率不能分开设置，PWM_CH6和PWM_CH7的频率不能分开设置
    //每个通道的占空比都可单独设置
    //关闭设备
    ret = VPI_CloseDevice(VPI_USBPWM, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Close device error!\n");
		system("pause");
        return ret;
	}
	system("pause");
	return 0;
}
