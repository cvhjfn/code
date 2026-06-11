#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"

void OLED_Init(void);//OLED初始化
void OLED_Clear(void);//OLED清屏
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);//(行, 列, 字符)
/*使用示例
OLED_ShowChar(1, 1, 'A');
OLED_ShowChar(2, 5, '5');
OLED_ShowChar(3, 10, '@');*/
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);//(行, 起始列, 字符串)
/*
使用示例
OLED_ShowString(1, 1, "Hello, World!");
*/
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);//(行, 列, 数字, 位数)
/*使用示例
OLED_ShowNum(1, 1, 12345, 5);*/
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);//(行, 列, 数字, 位数)
/*使用示例
OLED_ShowSignedNum(1, 1, -12345, 5);*/
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);//(行, 列, 数字, 位数)
/*使用示例
OLED_ShowHexNum(1, 1, 0x1A2B3C, 6);*/
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);//(行, 列, 数字, 位数)
/*使用示例
OLED_ShowBinNum(1, 1, 0b101010, 6);*/

#endif
