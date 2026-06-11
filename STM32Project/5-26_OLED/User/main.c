#include "stm32f10x.h"
#include"Delay.h"
#include "GPIO.h"
#include"USART.h"
#include "I2C.h"
#include "OLED.h"

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_Init_Config(GPIOA, GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);//引脚初始化

	GPIO_Init_Config(GPIOA, GPIO_Pin_5, GPIO_Mode_IPD, GPIO_Speed_2MHz);//引脚初始化

	GPIO_Init_Config(GPIOA, GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_10MHz);//引脚初始化

	GPIO_Init_Config(GPIOA, GPIO_Pin_10, GPIO_Mode_IN_FLOATING, GPIO_Speed_10MHz);//引脚初始化

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART1时钟

	USART_Config(USART1, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, 0);//USART1配置

	uint8_t bytesToSend[] = {1, 2, 3, 4, 5};//要发送的数据

	USART_SendBytes(USART1, bytesToSend, 5);//发送数据

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能 AFIO 时钟（复用功能）

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); // 使能 I2C1 时钟

    I2C_Config(I2C1,
           I2C_Mode_I2C,                  // 模式
           I2C_DutyCycle_2,              // 占空比
           0x00,                          // 本机地址
           I2C_Ack_Enable,               // 应答
           I2C_AcknowledgedAddress_7bit, // 地址位宽
           100000);                       // 速率 100kHz

    I2C_Cmd(I2C1, ENABLE);   // 启动外设*/

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

    GPIO_Init_Config(GPIOB, GPIO_Pin_8, GPIO_Mode_AF_OD, GPIO_Speed_50MHz);
    GPIO_Init_Config(GPIOB, GPIO_Pin_9, GPIO_Mode_AF_OD, GPIO_Speed_50MHz);

    OLED_Init(); // OLED 初始化
    OLED_ShowString(1, 1, "Hello, World!"); // 在 OLED 上显示字符串
    OLED_ShowNum(2, 1, 12345, 5); // 在 OLED 上显示数字
    OLED_ShowSignedNum(3, 1, -12345, 5); // 在 OLED 上显示带符号数字
    OLED_ShowHexNum(4, 1, 0x1A2B3C, 6); // 在 OLED 上显示十六进制数字
    
   while(1)
{
    static uint8_t last_key_state = 0;   // 保持按键上一次的稳定状态

    uint8_t current_key = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);

    if(current_key != last_key_state)    // 比较的是“本次读取”和“上次稳定状态”
    {
        Delay_ms(20);                    // 消抖
        current_key = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);  // 再次读取

        if(current_key != last_key_state) // 确认状态确实变了
        {
            // 更新状态（记住这次已经稳定变化的电平）
            last_key_state = current_key;

            // 只有按下（上升沿）才翻转，松开不管
            if(current_key == 1)
            {
                if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
                {
                    GPIO_SetBits(GPIOA, GPIO_Pin_0);   // 原低变高
                }
                else
                {
                    GPIO_ResetBits(GPIOA, GPIO_Pin_0); // 原高变低
                }
            }
        }
    }
}
}

