#include "stm32f10x.h"
#include "self_stm32f10x.h"

#include "BSP\bsp_usart.h"
#include "BSP\bsp_spi.h"
#include "BSP\bsp_adc.h"

#include "HARDWARE\nrf24l01.h"

#include "SOFTWARE\user.h"

#pragma	diag_suppress	870	//屏蔽汉字警告

//nRF24L01初始化结构体
static nRF24L01_Cfg nRF24_Cfg;
static uint8_t TxAddr[5] = {0x43,0x16,'R','C',0xFF};	//遥控器地址
static uint8_t RxAddr[5] = {0x43,0x16,'B','T',0xFF};	//船地址

static uint8_t Sbuffer[32];

int main(void)
{
	//中断优先级分组 4位主优先级 0位子优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	//关闭JTAG调试接口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	//串口初始化
	BSP_Usart_Init();

	//nRF24L01初始化
	if(nRF24L01_Init())
		printf("nRF24L01_Err\r\n");
	else
		printf("nRF24L01_Pass\r\n");
	//nRF24L01 相关配置
	nRF24_Cfg.Channel = 50;	//2.45GHz 通讯频段
	nRF24_Cfg.retry = 5;	//最大重发次数
	nRF24_Cfg.retry_cycle = 1;	//重发周期
	nRF24_Cfg.Rx_Length = 32;	//结束长度
	MemCopy(TxAddr,nRF24_Cfg.TX_Addr,5);
	MemCopy(RxAddr,nRF24_Cfg.RX_Addr,5);
	nRF24L01_Config(&nRF24_Cfg);	//配置nRF24L01
	nRF24L01_Rx_Mode();

	for(uint8_t temp=0;temp<32;temp++)
	{
		Sbuffer[temp] = temp;
	}

	while(1)
	{
		soft_delay_ms(500);
		printf("nRF24L01 Send 32Byte\r\n");
		nRF24L01_Send(Sbuffer,32);
	}
}

void SysTick_Handler(void)
{

}

void nRF24L01_NoACK_ISR(void)
{
	printf("NoACK_ISR Run\r\n");
}

void nRF24L01_Tx_ISR(void)
{
	printf("Tx_ISR Run\r\n");
}

void nRF24L01_Rx_ISR(void)
{
	printf("Rx_ISR Run\r\n");
}
