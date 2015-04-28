#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include "ControlPWM.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int ret;
	VPI_INIT_CONFIG PWM_Config;
    //ɨ���Ѿ����ӵ��豸
    ret = VPI_ScanDevice(1);
    if (ret <= 0)
    {
		printf("No device connect!\n");
		system("pause");
        return ret;
    }
    //���豸
    ret = VPI_OpenDevice(VPI_USBPWM, 0, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Open device error!\n");
		system("pause");
        return ret;
    }
    //��ʼ��PWM_CH0ͨ��
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
    //����PWM_CH0ͨ��
    ret = VPI_StartPWM(VPI_USBPWM,0,VPI_PWM_CH0);
    if (ret != ERR_SUCCESS)
    {
		printf("Start pwm error!\n");
		system("pause");
        return ret;
    }
    //����1����
    Sleep(1000);
    //ֹͣPWM_CH0ͨ��
    ret = VPI_StopPWM(VPI_USBPWM, 0, VPI_PWM_CH0);
    if (ret != ERR_SUCCESS)
    {
		printf("Stop pwm error!\n");
		system("pause");
        return ret;
    }
    //��ʼ������PWMͨ��
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
    //����PWM_CH0,PWM_CH2ͨ��
    ret = VPI_StartPWM(VPI_USBPWM, 0, VPI_PWM_CH0 | VPI_PWM_CH2);
    if (ret != ERR_SUCCESS)
    {
		printf("Start pwm error!\n");
		system("pause");
        return ret;
    }
    //����1����
    Sleep(1000);
    //����PWM_CH0��ʱ��Ƶ��
	uint32_t Frequency[8] = {0};
    Frequency[0] = 50000;
    ret = VPI_SetPWMPeriod(VPI_USBPWM, 0, VPI_PWM_CH0, Frequency);
    if (ret != ERR_SUCCESS)
    {
		printf("Set frequency error!\n");
		system("pause");
        return ret;
    }
    //����PWM_CH2��ռ�ձ�
	uint8_t Pulse[8] = {0};
    Pulse[2] = 80;
    ret = VPI_SetPWMPulse(VPI_USBPWM, 0, VPI_PWM_CH2, Pulse);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pulse error!\n");
		system("pause");
        return ret;
    }
    //����1����
    Sleep(1000);
    //ͬʱ����PWM_CH0��PWM_CH2��ռ�ձ�
    Pulse[0] = 20;
    Pulse[2] = 80;
    ret = VPI_SetPWMPulse(VPI_USBPWM, 0, VPI_PWM_CH0|VPI_PWM_CH2, Pulse);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pulse error!\n");
		system("pause");
        return ret;
    }
    //����1����
    Sleep(1000);
    //ע�⣺PWM_CH0��PWM_CH1��Ƶ�ʲ��ֿܷ����ã�PWM_CH2��PWM_CH3��Ƶ�ʲ��ֿܷ����ã�PWM_CH4��PWM_CH5��Ƶ�ʲ��ֿܷ����ã�PWM_CH6��PWM_CH7��Ƶ�ʲ��ֿܷ�����
    //ÿ��ͨ����ռ�ձȶ��ɵ�������
    //�ر��豸
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