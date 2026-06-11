#include "stm32f10x.h"
/**
 * @brief  通用串口配置函数（不配置引脚、不开启时钟）
 * @param  USARTx     : USART1、USART2 或 USART3
 * @param  BaudRate   : 波特率，例如 115200
 * @param  WordLength : 数据位，USART_WordLength_8b / 9b
 * @param  StopBits   : 停止位，USART_StopBits_1 / 0.5 / 2 / 1.5
 * @param  Parity     : 校验位，USART_Parity_No / Even / Odd
 * @param  EnableIT   : 1 = 使能接收中断(RXNE)，0 = 不使能
 * @note   调用前需手动：
 *         - 用 RCC_APBxPeriphClockCmd() 使能 USART 时钟
 *         - 初始化对应的 TX/RX 引脚（GPIO_Mode_AF_PP / GPIO_Mode_IN_FLOATING）
 *         - 若使用中断，还需配置 NVIC
 */
void USART_Config(USART_TypeDef* USARTx, uint32_t BaudRate,
                  uint16_t WordLength, uint16_t StopBits, uint16_t Parity,
                  uint8_t EnableIT)
{
    USART_InitTypeDef USART_InitStructure;

    // 仅配置串口参数，不涉及 GPIO
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = WordLength;
    USART_InitStructure.USART_StopBits = StopBits;
    USART_InitStructure.USART_Parity = Parity;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USARTx, &USART_InitStructure);
	
	USART_Cmd(USARTx, ENABLE);
}

//@作用：使用串口一次性发送多个字节数据
//参数：USARTx：串口外设地址；pData：待发送数据的首地址；Size：待发送数据的字节数
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t*pData, uint16_t Size)
{
	for(uint16_t i = 0; i < Size; i++)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		//等待发送数据寄存器空
		USART_SendData(USARTx, pData[i]);
		//将待发送数据写入发送数据寄存器
	}
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	//等待数据发送完成
}
