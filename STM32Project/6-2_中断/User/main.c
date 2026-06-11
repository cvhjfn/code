#include "stm32f10x.h"
#include"Delay.h"
#include "GPIO.h"
#include"USART.h"
#include "I2C.h"
#include "OLED.h"

uint32_t blinkInterval = 1000;// 定义一个全局变量来控制LED闪烁的间隔时间（单位：毫秒）

void App_OnBoardLED_Init(void);

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA时钟

	GPIO_Init_Config(GPIOA, GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_2MHz);
	GPIO_Init_Config(GPIOA, GPIO_Pin_10, GPIO_Mode_IPU, GPIO_Speed_2MHz);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // 使能USART1时钟

	USART_Config(USART1, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, 1); // 配置USART1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能USART1接收中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置NVIC优先级分组
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	App_OnBoardLED_Init();
	while(1)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // 点亮LED
		Delay_ms(blinkInterval); // 延时
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); // 熄灭LED
		Delay_ms(blinkInterval); // 延时
	
	}
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 检查是否是接收中断
	{
		uint8_t receivedByte = USART_ReceiveData(USART1); // 读取接收到的数据

		if(receivedByte == '1') // 如果接收到字符 '1'
		{
			blinkInterval = 500; // 将LED闪烁间隔设置为500ms
		}
		else if(receivedByte == '2') // 如果接收到字符 '2'
		{
			blinkInterval = 1000; // 将LED闪烁间隔设置为1000ms
		}
		else if(receivedByte == '3') // 如果接收到字符 '3'
		{
			blinkInterval = 2000; // 将LED闪烁间隔设置为2000ms
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志位
	}
}

void App_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能GPIOC时钟

	GPIO_Init_Config(GPIOC, GPIO_Pin_13, GPIO_Mode_Out_OD, GPIO_Speed_2MHz);
}
	
