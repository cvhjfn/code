#include "stm32f10x.h"

/**
  * 功能：通用GPIO引脚初始化函数（标准库）
  * 参数：
  *   GPIOx    : GPIO端口号 (GPIOA~GPIOG)
  *   GPIO_Pin : 引脚号    (GPIO_Pin_0 ~ GPIO_Pin_15)
  *   mode     : 引脚模式 (推挽输出/上拉输入/复用功能等)
  *   speed    : 引脚速度 (GPIO_Speed_10MHz/50MHz)
  */
void GPIO_Init_Config(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed)
{
    // 1. 定义GPIO初始化结构体
    GPIO_InitTypeDef GPIO_InitStruct;
    // 3. 配置结构体参数
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin;    // 选择引脚
    GPIO_InitStruct.GPIO_Mode = mode;       // 引脚模式
    GPIO_InitStruct.GPIO_Speed = speed;     // 引脚速度（仅输出模式有效）

    // 4. 初始化GPIO引脚
    GPIO_Init(GPIOx, &GPIO_InitStruct);
}
