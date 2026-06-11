#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"   // 标准外设库主头文件，包含了 GPIO、USART 等所需定义

/**
 * @brief  通用串口配置函数（不开启时钟，需在调用前手动使能 GPIO 和 USART 时钟）
 * @param  USARTx     : 要配置的 USART 外设，如 USART1、USART2、USART3
 * @param  BaudRate   : 波特率，例如 115200
 * @param  WordLength : 数据位长度，可选 USART_WordLength_8b 或 USART_WordLength_9b
 * @param  StopBits   : 停止位，可选 USART_StopBits_1 / 0.5 / 2 / 1.5
 * @param  Parity     : 校验位，可选 USART_Parity_No / Even / Odd
 * @param  EnableIT   : 是否使能接收中断(RXNE)，1 = 使能，0 = 不使能
 * @note   调用本函数前，必须使用 RCC_APBxPeriphClockCmd() 使能对应 GPIO 和 USART 的时钟。
 *         若使能了中断，还需在主程序中配置 NVIC 中断通道（如 USART1_IRQn）。
 */
void USART_Config(USART_TypeDef* USARTx,
                  uint32_t BaudRate,
                  uint16_t WordLength,
                  uint16_t StopBits,
                  uint16_t Parity,
                  uint8_t EnableIT);

/**
 * @brief  使用串口一次性发送多个字节数据
 * @param  USARTx : 串口外设地址，如 USART1
 * @param  pData  : 待发送数据的首地址
 * @param  Size   : 待发送数据的字节数
 */
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t *pData, uint16_t Size);

// 函数声明
//@作用：使用串口一次性发送多个字节数据
//参数：USARTx：串口外设地址；pData：待发送数据的首地址；Size：待发送数据的字节数
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t* pData, uint16_t Size);

//@作用：使用串口发送字符串
//参数：USARTx：串口外设地址；str：待发送字符串的首地址（自动识别'\0'结束符）
void USART_SendString(USART_TypeDef* USARTx, char *str);


#endif /* __USART_CONFIG_H */
