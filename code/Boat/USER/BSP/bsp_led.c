#include "bsp_led.h"

void BSP_LED_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    for(uint8_t temp=0;temp<2;temp++)
    {
        GPIO_InitStruct.GPIO_Pin = LED_Pin[temp].Pin;
        GPIO_Init(LED_Pin[temp].GPIO,&GPIO_InitStruct);
        LED_CTR(temp,LED_OFF);
    }
}

void LED_CTR(uint8_t num,uint8_t status)
{
    switch(status)
    {
        case LED_OFF:
            Pin_Set(LED_Pin[num]);
            break;
        case LED_ON:
            Pin_Reset(LED_Pin[num]);
            break;
        default:
            Pin_Reversal(LED_Pin[num]);
            break;
    }
}

