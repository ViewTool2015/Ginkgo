#include <iostream>
#include <stdio.h>
#include "ControlGPIO.h"

using namespace std;

int main()
{
    int ret;
    //扫描已经连接的设备
    ret = VGI_ScanDevice(1);
    if (ret <= 0) {
        printf("No device connect!\n");
        getchar();
        return ret;
    }
    //打开设备
    ret = VGI_OpenDevice(VGI_USBGPIO, 0, 0);
    if (ret != ERR_SUCCESS) {
        printf("Open device error!\n");
        getchar();
        return ret;
    }
    //将GPIO_7和GPIO_8设置成输出引脚
    ret = VGI_SetOutput(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS) {
        printf("Set pin output error!\n");
        getchar();
        return ret;
    }
    //将GPIO_7和GPIO_8输出高电平
    ret = VGI_SetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS) {
        printf("Set pin high error!\n");
        getchar();
        return ret;
    }
    //将GPIO_7和GPIO_8输出低电平
    ret = VGI_ResetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS) {
        printf("Set pin low error!\n");
        getchar();
        return ret;
    }
    //将GPIO_4和GPIO_5设置成输入引脚
    ret = VGI_SetInput(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS) {
        printf("Set pin input error!\n");
        getchar();
        return ret;
    }
    //获取GPIO_4和GPIO_5引脚状态
    uint16_t pin_value = 0;
    ret = VGI_ReadDatas(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5, &pin_value);
    if (ret != ERR_SUCCESS) {
        printf("Get pin data error!\n");
        getchar();
        return ret;
    } else {
        if ((pin_value & VGI_GPIO_PIN4) != 0) {
            printf("GPIO_4 is high-level!\n");
        } else {
            printf("GPIO_4 is low-level!\n");
        }
        if ((pin_value & VGI_GPIO_PIN5) != 0) {
            printf("GPIO_5 is high-level!\n");
        } else {
            printf("GPIO_5 is low-level!\n");
        }
    }
    //将GPIO_4和GPIO_5引脚设置成开漏模式（需加上拉电阻，可当双向口）
    ret = VGI_SetOpenDrain(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS) {
        printf("Set pin open drain error!\n");
        getchar();
        return ret;
    }
    //将GPIO_4和GPIO_5输出高电平
    ret = VGI_SetOutput(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS) {
        printf("Set pin high error!\n");
        getchar();
        return ret;
    }
    //将GPIO_4和GPIO_5输出低电平
    ret = VGI_ResetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS) {
        printf("Set pin high error!\n");
        getchar();
        return ret;
    }
    //获取GPIO_4和GPIO_5引脚状态
    ret = VGI_ReadDatas(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5, &pin_value);
    if (ret != ERR_SUCCESS) {
        printf("Get pin data error!\n");
        getchar();
        return ret;
    } else {
        if ((pin_value & VGI_GPIO_PIN4) != 0) {
            printf("GPIO_4 is high-level!\n");
        } else {
            printf("GPIO_4 is low-level!\n");
        }
        if ((pin_value & VGI_GPIO_PIN5) != 0) {
            printf("GPIO_5 is high-level!\n");
        } else {
            printf("GPIO_5 is low-level!\n");
        }
    }
    //关闭设备
    ret = VGI_CloseDevice(VGI_USBGPIO, 0);
    if (ret != ERR_SUCCESS) {
        printf("Close device error!\n");
        getchar();
        return ret;
    }
    getchar();
    return 0;
}
