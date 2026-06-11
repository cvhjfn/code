
/*
 * main.c
 * 功能：
 * - 使用 PA0/PA1/PA2 驱动三色指示灯（PA0=红, PA1=绿, PA2=黄）
 * - 使用 PA4/PA5/PA6 接入三个按键，按下切换对应 LED 的开/关状态（按键接地，配置上拉）
 * - 在 OLED 上显示当前三灯状态（OLED 硬件线接在 PB8/PB9）
 * - 使用轮询 + 简单去抖（30ms）检测按键上升沿（按下瞬间切换）
 */

#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include <stdio.h>

int main(void)
{
	/* 打开 GPIOA 时钟（LED 与按键都使用 GPIOA 引脚） */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	/*
	 * 配置 LED 引脚：PA0/PA1/PA2
	 * - 这些引脚接三色指示灯（或外接 LED）
	 * - 推挽输出，50MHz
	 * - 约定：PA0 = 红灯，PA1 = 绿灯，PA2 = 黄灯
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /* 推挽输出 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*
	 * 配置按键引脚：PA4/PA5/PA6
	 * - 按键一端接引脚，另一端接 GND（按下到 GND）
	 * - 使用内部上拉（IPU），空闲时为高电平，按下时为低（0）
	 * - 代码中以按下为 1 处理（通过判断 Bit_RESET）
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 输入上拉 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	OLED_Init();
	OLED_Clear();
	

	/*
	 * 状态变量说明：
	 * - led_state 的位映射：bit0 -> PA0(红), bit1 -> PA1(绿), bit2 -> PA2(黄)
	 * - last_btn 保存上一次采样的按键按下位，用于检测按键边沿
	 * - prev_led_state 用于记录上次 OLED 显示时的 LED 状态，只有变化时才更新 OLED
	 */
	uint8_t led_state = 0;
	uint8_t last_btn = 0; /* 上次采样的按键按下位（按下=1） */
	uint8_t prev_led_state = 0xFF; /* 启动时强制刷新 OLED */

	/* ensure LEDs off initially */
	GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);

	while (1)
	{
		/*
		 * 读取按键电平并标准化为按下=1、释放=0
		 * 由于按键接地并启用上拉，按下时读取到 Bit_RESET
		 */
		uint8_t b0 = (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) ? 1 : 0; /* 按键0 -> 控制红灯(PA0) */
		uint8_t b1 = (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) ? 1 : 0; /* 按键1 -> 控制绿灯(PA1) */
		uint8_t b2 = (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == Bit_RESET) ? 1 : 0; /* 按键2 -> 控制黄灯(PA2) */

		uint8_t btn = (b0 << 0) | (b1 << 1) | (b2 << 2); /* btn 的位映射与 led_state 对应 */

		/*
		 * 检测按键上升沿：
		 * - 因为我们把按下视为 1，按键由 0->1 的瞬间表示“刚被按下”
		 * - 使用 last_btn 保存上次采样，当前为 1 且上次为 0 时认为发生一次按下事件
		 * - 按下一次切换对应 LED 状态（XOR）
		 */
		if ((btn & 0x01) && !(last_btn & 0x01)) led_state ^= (1 << 0); /* 切换红灯 */
		if ((btn & 0x02) && !(last_btn & 0x02)) led_state ^= (1 << 1); /* 切换绿灯 */
		if ((btn & 0x04) && !(last_btn & 0x04)) led_state ^= (1 << 2); /* 切换黄灯 */

		/* 记录本次按键状态以便下次比较 */
		last_btn = btn;

		/*
		 * 根据 led_state 更新物理 LED 引脚输出：
		 * - 对应位为 1 则置位（点亮 LED），为 0 则复位（熄灭 LED）
		 */
		if (led_state & 0x01) GPIO_SetBits(GPIOA, GPIO_Pin_0); else GPIO_ResetBits(GPIOA, GPIO_Pin_0); /* 红 */
		if (led_state & 0x02) GPIO_SetBits(GPIOA, GPIO_Pin_1); else GPIO_ResetBits(GPIOA, GPIO_Pin_1); /* 绿 */
		if (led_state & 0x04) GPIO_SetBits(GPIOA, GPIO_Pin_2); else GPIO_ResetBits(GPIOA, GPIO_Pin_2); /* 黄 */

		/* Update OLED when LED state changes */
		/*
		 * 当 LED 状态发生变化时，更新 OLED 显示：
		 * - 在第 4 行显示类似 "R:ON G:OFF Y:ON" 的状态文本
		 * - 仅在状态变化时更新，减少不必要的屏幕刷新
		 * - 注意：OLED 驱动与 I2C/SPI 的初始化在 OLED_Init() 内完成，硬件上 OLED 已接到 PB8/PB9
		 */
		if (led_state != prev_led_state)
		{
		    char buf[32];
		    sprintf(buf, "R:%s G:%s Y:%s",
			    (led_state & 0x01) ? "ON" : "OFF",
			    (led_state & 0x02) ? "ON" : "OFF",
			    (led_state & 0x04) ? "ON" : "OFF");
		    OLED_ShowString(1, 1, buf);
		    prev_led_state = led_state;
		}

		/*
		 * 简单去抖与轮询间隔：
		 * - 使用 30ms 延时，既能去掉短抖动，又能保证按键响应流畅
		 * - 对于更严格的去抖或按键长按识别，可改用定时器/中断策略
		 */
		Delay_ms(30);
	}

}

