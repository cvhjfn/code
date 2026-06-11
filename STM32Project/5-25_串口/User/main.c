#include "stm32f10x.h"
#include"Delay.h"
#include "GPIO.h"
#include"USART.h"
void My_USART_SendBytes(USART_TypeDef* USARTx, uint8_t*pData, uint16_t Size);
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_Init_Config(GPIOA, GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);

	GPIO_Init_Config(GPIOA, GPIO_Pin_5, GPIO_Mode_IPD, GPIO_Speed_2MHz);

	GPIO_Init_Config(GPIOA, GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_10MHz);

	GPIO_Init_Config(GPIOA, GPIO_Pin_10, GPIO_Mode_IN_FLOATING, GPIO_Speed_10MHz);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_Config(USART1, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, 0);

	uint8_t bytesToSend[] = {1, 2, 3, 4, 5};

	My_USART_SendBytes(USART1, bytesToSend, 5);

	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
		}
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		}
	}
}

//@作用：使用串口一次性发送多个字节数据
//参数：USARTx：串口外设地址；pData：待发送数据的首地址；Size：待发送数据的字节数
void My_USART_SendBytes(USART_TypeDef* USARTx, uint8_t*pData, uint16_t Size)
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

