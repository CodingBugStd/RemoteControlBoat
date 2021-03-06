#include "bsp_adc.h"

static void ADC_GPIO_Init(void);
static void ADC_Config(void);

const static Pin ADC_Pin[2] = {
    {GPIO_Pin_0,GPIOC},
    {GPIO_Pin_3,GPIOC}
};

const static uint8_t ADC_Channel[2] = 
{
    ADC_Channel_10,
    ADC_Channel_13
};

void BSP_ADC_Init(void)
{
    ADC_GPIO_Init();
    ADC_Config();
}

void ADC_GPIO_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;

    ADC_GPIO_CLK();

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    for(uint8_t temp=0;temp<2;temp++)
    {
        GPIO_InitStruct.GPIO_Pin = ADC_Pin[temp].Pin;
        GPIO_Init(ADC_Pin[temp].GPIO,&GPIO_InitStruct);
    }
}

void ADC_Config(void)
{
    ADC_InitTypeDef ADC_InitStruct;

    ADC_CLK();

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //配置ADC时钟 72/6 = 12MHz (最大不超过14MHz)

    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;    //关闭连续转换
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; //转换数据右对齐
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;    //软件触发转换
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;     //独立模式
    ADC_InitStruct.ADC_NbrOfChannel = 1;                //规则转换通道数
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;          //扫描模式关闭

    ADC_Init(ADC1,&ADC_InitStruct);      //初始化ADC1
    ADC_Cmd(ADC1,ENABLE);               //启动ADC1

    //等待校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));

    ADC_Init(ADC2,&ADC_InitStruct);      //初始化ADC2
    ADC_Cmd(ADC2,ENABLE);               //启动ADC2

    //等待校准
    ADC_ResetCalibration(ADC2);
    while(ADC_GetResetCalibrationStatus(ADC2));
    ADC_StartCalibration(ADC2);
    while(ADC_GetResetCalibrationStatus(ADC2));
}

float ADC_ReadVoltage(ADC_TypeDef*ADC,uint8_t channel_num)
{
    //开始转换
    ADC_SoftwareStartConvCmd(ADC,DISABLE);
    //给规则通道1绑定ADC捕获口
    ADC_RegularChannelConfig(ADC,ADC_Channel[channel_num],1,ADC_SampleTime_55Cycles5);
    //开始转换
    ADC_SoftwareStartConvCmd(ADC,ENABLE);
    //等待转换结束
    while(!ADC_GetFlagStatus(ADC,ADC_FLAG_EOC));

    return (float)ADC_GetConversionValue(ADC)*3.3f/4096;
}

