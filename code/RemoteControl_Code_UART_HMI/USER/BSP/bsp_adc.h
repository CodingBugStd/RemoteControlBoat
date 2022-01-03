#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#include "self_stm32f10x.h"

/*************************************************
 * 软件触发 多通道 非扫描 ADC捕获
 * 
 * Create by: 庞碧璋
 * Github: https://github.com/CodingBugStd
 * csdn:   https://blog.csdn.net/RampagePBZ
 * Encoding: utf-8
 * create date: 21年电赛
 * last date:   2021/12/20
*************************************************/

#define ADC_GPIO_CLK()  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); \
                        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE)
#define ADC_CLK()       RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE)

#define ADC_IN_0    0
#define ADC_IN_1    1
#define ADC_IN_8    2
#define ADC_IN_9    3    
#define ADC_IN_4    4

const static Pin ADC_Pin[5] = {
    {GPIO_Pin_0,GPIOA},{GPIO_Pin_1,GPIOA},
    {GPIO_Pin_0,GPIOB},{GPIO_Pin_1,GPIOB},
    {GPIO_Pin_4,GPIOA}
};

const static uint8_t ADC_Channel[5] = 
{
    ADC_Channel_0,ADC_Channel_1,
    ADC_Channel_8,ADC_Channel_9,
    ADC_Channel_4
};

void BSP_ADC_Init(void);
float ADC_ReadVoltage(uint8_t channel_Num);

#endif

