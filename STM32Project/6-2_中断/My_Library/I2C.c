#include "I2C.h"

void I2C_Config(I2C_TypeDef* I2Cx,
                uint16_t Mode,
                uint16_t DutyCycle,
                uint16_t OwnAddress1,
                uint16_t Ack,
                uint16_t AcknowledgedAddress,
                uint32_t ClockSpeed)
{
    I2C_InitTypeDef I2C_InitStructure;

    // 填充参数
    I2C_InitStructure.I2C_Mode                = Mode;
    I2C_InitStructure.I2C_DutyCycle           = DutyCycle;
    I2C_InitStructure.I2C_OwnAddress1         = OwnAddress1;
    I2C_InitStructure.I2C_Ack                 = Ack;
    I2C_InitStructure.I2C_AcknowledgedAddress = AcknowledgedAddress;
    I2C_InitStructure.I2C_ClockSpeed          = ClockSpeed;

    // 初始化 I2C 外设
    I2C_Init(I2Cx, &I2C_InitStructure);
}
