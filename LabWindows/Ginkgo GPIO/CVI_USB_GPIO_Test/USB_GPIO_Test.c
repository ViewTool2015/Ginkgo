//==============================================================================
//
// Title:		USB_GPIO_Test.c
// Purpose:		A short description of the implementation.
//
// Created on:	2014/3/7 at 17:11:13 by Win7.
// Copyright:	Win7. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "ControlGPIO.h"
#include <ansi_c.h>
#include <stdio.h>

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
int main (void)
{
    int ret;
    //扫描已经连接的设备
    ret = VGI_ScanDevice(1);
    if (ret <= 0)
    {
		printf("No device connect!\n");
		system("cmd /C pause");
        return ret;
    }
    //打开设备
    ret = VGI_OpenDevice(VGI_USBGPIO, 0, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Open device error!\n");
		system("cmd /C pause");
        return ret;
    }
    //将GPIO_7和GPIO_8设置成输出引脚
    ret = VGI_SetOutput(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pin output error!\n");
		system("cmd /C pause");
        return ret;
    }
    //将GPIO_7和GPIO_8输出高电平
    ret = VGI_SetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pin high error!\n");
		system("cmd /C pause");
        return ret;
    }
    //将GPIO_7和GPIO_8输出低电平
    ret = VGI_ResetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pin low error!\n");
		system("cmd /C pause");
        return ret;
    }
    //将GPIO_4和GPIO_5设置成输入引脚
    ret = VGI_SetInput(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pin input error!\n");
		system("cmd /C pause");
        return ret;
    }
    //获取GPIO_4和GPIO_5引脚状态
    uint16_t pin_value = 0;
    ret = VGI_ReadDatas(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5, &pin_value);
    if (ret != ERR_SUCCESS)
    {
		printf("Get pin data error!\n");
		system("cmd /C pause");
        return ret;
    }
    else
    {
        if ((pin_value & VGI_GPIO_PIN4) != 0)
        {
            printf("GPIO_4 is high-level!\n");
        }
        else
        {
            printf("GPIO_4 is low-level!\n");
        }
        if ((pin_value & VGI_GPIO_PIN5) != 0)
        {
            printf("GPIO_5 is high-level!\n");
        }
        else
        {
            printf("GPIO_5 is low-level!\n");
        }
    }
    //将GPIO_4和GPIO_5引脚设置成开漏模式（需加上拉电阻，可当双向口）
    ret = VGI_SetOpenDrain(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pin open drain error!\n");
		system("cmd /C pause");
        return ret;
    }
    //将GPIO_4和GPIO_5输出高电平
    ret = VGI_SetOutput(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pin high error!\n");
		system("cmd /C pause");
        return ret;
    }
    //将GPIO_4和GPIO_5输出低电平
    ret = VGI_ResetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS)
    {
		printf("Set pin high error!\n");
		system("cmd /C pause");
        return ret;
    }
    //获取GPIO_4和GPIO_5引脚状态
    ret = VGI_ReadDatas(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5, &pin_value);
    if (ret != ERR_SUCCESS)
    {
		printf("Get pin data error!\n");
		system("cmd /C pause");
        return ret;
    }
    else
    {
        if ((pin_value & VGI_GPIO_PIN4) != 0)
        {
            printf("GPIO_4 is high-level!\n");
        }
        else
        {
            printf("GPIO_4 is low-level!\n");
        }
        if ((pin_value & VGI_GPIO_PIN5) != 0)
        {
            printf("GPIO_5 is high-level!\n");
        }
        else
        {
            printf("GPIO_5 is low-level!\n");
        }
    }
    //关闭设备
    ret = VGI_CloseDevice(VGI_USBGPIO, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Close device error!\n");
		system("cmd /C pause");
        return ret;
	}
	system("cmd /C pause");
	return 0;
}
