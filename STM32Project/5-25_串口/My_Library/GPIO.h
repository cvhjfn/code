#ifndef __GPIO_H
#define __GPIO_H

// 必须包含！STM32标准库核心头文件（解决所有类型未定义问题）
#include "stm32f10x.h"

// 函数声明（头文件只写这一行！）
void GPIO_Init_Config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed);

#endif
