#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"

/**
 * @brief  I2C 外设初始化（仅配置 I2C 参数，不碰时钟、GPIO、外设使能）
 * @param  I2Cx                : I2C1 或 I2C2
 * @param  Mode                :I2C_Mode_I2C			0x0000	标准 I2C 模式
								I2C_Mode_SMBusDevice	0x0002	SMBus 设备模式
								I2C_Mode_SMBusHost		0x000A	SMBus 主机模式
 * @param  DutyCycle           :I2C_DutyCycle_16_9	0x4000	Tlow/Thigh = 16/9（快速模式）
								I2C_DutyCycle_2		0xBFFF	Tlow/Thigh = 2（快速模式）
								仅在 ClockSpeed > 100000 时有效，标准模式自动忽略。
 * @param  OwnAddress1         :0x00 ~ 0x3F		
								7 位地址（实际值是 7 位左移 1 位，例如 0x0A 对应实际地址 0x05）
								主机模式下通常设为 0x00，从机模式需设置有效地址。
 * @param  Ack                 :I2C_Ack_Enable	0x0400	使能应答
								I2C_Ack_Disable	0x0000	禁止应答
								主机接收数据时必须使能应答（除最后一个字节外），从机通常使能。
 * @param  AcknowledgedAddress :宏定义							数值	说明
								I2C_AcknowledgedAddress_7bit	0x4000	7 位地址模式
								I2C_AcknowledgedAddress_10bit	0xC000	10 位地址模式
								绝大多数外设使用 7 位地址。
 * @param  ClockSpeed          :常用值	说明
								100000	标准模式（100 kHz）
								400000	快速模式（400 kHz）
								可设置任意 ≤400000 的值，I2C 外设根据 APB1 时钟自动计算分频系数。
 * @note   
 *   调用前需手动使能 I2C 时钟、配置 SCL/SDA 引脚为复用开漏输出。
 *   调用后需手动调用 I2C_Cmd(I2Cx, ENABLE) 开启外设。
 */
void I2C_Config(I2C_TypeDef* I2Cx,
                uint16_t Mode,
                uint16_t DutyCycle,
                uint16_t OwnAddress1,
                uint16_t Ack,
                uint16_t AcknowledgedAddress,
                uint32_t ClockSpeed);

/*使用示例

// 在 main 中手动使能时钟、配置引脚后：
I2C_Config(I2C1,
           I2C_Mode_I2C,                  // 模式
           I2C_DutyCycle_2,              // 占空比
           0x00,                          // 本机地址
           I2C_Ack_Enable,               // 应答
           I2C_AcknowledgedAddress_7bit, // 地址位宽
           100000);                       // 速率 100kHz

I2C_Cmd(I2C1, ENABLE);   // 启动外设*/
#endif
