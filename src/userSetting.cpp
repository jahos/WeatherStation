/*
 * userSetting.cpp
 *
 *  Created on: 27.12.2016
 *      Author: Jan Cwiecek
 */

#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "userSettings.h"
#include "Sensors/MiCS6814.h"
#define CLK_FREQ 24000000

volatile uint16_t ADCVal[4];

void init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	initUsart();
	initSPI();
	initADC();

	SysTick_Config(13000000);
}

void initUsart()
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO , ENABLE);
	GPIO_InitTypeDef usartPinTx;
	usartPinTx.GPIO_Pin = USART1_TX;
	usartPinTx.GPIO_Mode = GPIO_Mode_AF_PP;
	usartPinTx.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&usartPinTx);

	GPIO_InitTypeDef usartPinRx;
	usartPinRx.GPIO_Pin = USART1_RX;
	usartPinRx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&usartPinRx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef usartCtrPin;
	usartCtrPin.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	usartCtrPin.GPIO_Mode = GPIO_Mode_Out_PP;
	usartCtrPin.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&usartCtrPin);

	USART_InitTypeDef initStruct;
	initStruct.USART_BaudRate 				= 19200;
//	initStruct.USART_BaudRate 				= 115200;
	initStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	initStruct.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;
	initStruct.USART_Parity 				= USART_Parity_No;
	initStruct.USART_StopBits 				= USART_StopBits_2;
	initStruct.USART_WordLength 			= USART_WordLength_8b;
	USART_Init(USART1,&initStruct);
	USART_Cmd(USART1, ENABLE);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(USART1_IRQn);
}

void initSPI()
{

	/*------------------SPI1------------------------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);


	GPIO_InitTypeDef spiGpio;
	spiGpio.GPIO_Mode =		GPIO_Mode_AF_PP;
	spiGpio.GPIO_Pin = 		SPI1_MOSI_PIN | SPI1_CLK_PIN ;
	spiGpio.GPIO_Speed = 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&spiGpio);

	spiGpio.GPIO_Mode = 	GPIO_Mode_Out_PP;
	spiGpio.GPIO_Pin =		SPI1_RES_PIN;
	spiGpio.GPIO_Speed = 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&spiGpio);

	spiGpio.GPIO_Mode = 	GPIO_Mode_Out_PP;
	spiGpio.GPIO_Pin =		SPI1_D_C_PIN | SPI1_CS_PIN ;
	spiGpio.GPIO_Speed = 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&spiGpio);

	GPIO_SetBits(GPIOC, SPI1_RES_PIN);
	GPIO_SetBits(GPIOA, SPI1_D_C_PIN | SPI1_CS_PIN );

	SPI_InitTypeDef spiConfig;
	SPI_StructInit(&spiConfig);
	spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	spiConfig.SPI_Mode = SPI_Mode_Master;
	spiConfig.SPI_NSS = SPI_NSS_Soft;
	spiConfig.SPI_DataSize = SPI_DataSize_8b;
	spiConfig.SPI_Direction = SPI_Direction_1Line_Tx;
	spiConfig.SPI_FirstBit = SPI_FirstBit_MSB;
	spiConfig.SPI_CPOL = SPI_CPOL_High;
	spiConfig.SPI_CPHA = SPI_CPHA_2Edge;
	spiConfig.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&spiConfig);
	SPI_SSOutputCmd(SPI1,ENABLE);
	SPI_Cmd(SPI1,ENABLE);


	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_EnableIRQ(SPI1_IRQn);

	/*------------------SPI2------------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	spiGpio.GPIO_Mode =	GPIO_Mode_AF_PP;
	spiGpio.GPIO_Pin = 	SPI2_MOSI_PIN | SPI2_CLK_PIN | SPI2_MISO_PIN;
	spiGpio.GPIO_Speed = 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&spiGpio);

	spiGpio.GPIO_Mode = 	GPIO_Mode_Out_PP;
	spiGpio.GPIO_Pin =		SPI2_CS_PIN;
	spiGpio.GPIO_Speed = 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&spiGpio);

	GPIO_SetBits(GPIOB, SPI2_CS_PIN);

	SPI_StructInit(&spiConfig);
	spiConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	spiConfig.SPI_Mode = SPI_Mode_Master;
	spiConfig.SPI_NSS = SPI_NSS_Soft;
	spiConfig.SPI_DataSize = SPI_DataSize_8b;
	spiConfig.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spiConfig.SPI_FirstBit = SPI_FirstBit_MSB;
	spiConfig.SPI_CPOL = SPI_CPOL_High;
	spiConfig.SPI_CPHA = SPI_CPHA_2Edge;
	spiConfig.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&spiConfig);
	SPI_SSOutputCmd(SPI2,ENABLE);
	SPI_Cmd(SPI2,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_EnableIRQ(SPI2_IRQn);
	/*------------------SPI3------------------------------------------------*/
}

void initADC()
{
	GPIO_InitTypeDef gpio;
	 DMA_InitTypeDef dma;
	 ADC_InitTypeDef adc;

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	 RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	 gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	 gpio.GPIO_Mode = GPIO_Mode_AIN;
	 GPIO_Init(GPIOA, &gpio);

	 DMA_StructInit(&dma);
	 dma.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	 dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	 dma.DMA_MemoryBaseAddr = (uint32_t)MiCS_6814::getBuffer();
	 dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	 dma.DMA_DIR = DMA_DIR_PeripheralSRC;
	 dma.DMA_BufferSize = ADC_CHANNELS;
	 dma.DMA_Mode = DMA_Mode_Circular;
	 DMA_Init(DMA1_Channel1, &dma);
	 DMA_Cmd(DMA1_Channel1, ENABLE);

	 ADC_StructInit(&adc);
	 adc.ADC_ScanConvMode = ENABLE;
	 adc.ADC_ContinuousConvMode = ENABLE;
	 adc.ADC_NbrOfChannel = ADC_CHANNELS;
	 adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	 ADC_Init(ADC1, &adc);

	 ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_17,4, ADC_SampleTime_239Cycles5);

	 ADC_DMACmd(ADC1, ENABLE);
	 ADC_Cmd(ADC1, ENABLE);

	 ADC_ResetCalibration(ADC1);
	 while (ADC_GetResetCalibrationStatus(ADC1));

	 ADC_StartCalibration(ADC1);
	 while(ADC_GetCalibrationStatus(ADC1));

	 ADC_TempSensorVrefintCmd(ENABLE);
	 ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



