/**
  ******************************************************************************
  * @file    stm32f1xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11-February-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_it.h"
#include "stm32f10x.h"
#include "string.h"

#include "common.h"
#include <stdio.h>

/** @addtogroup IO_Toggle
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define ENTER 				13
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global Variable -----------------------------------------------------------*/


static BufferS_t outBuffer;
static BufferS_t inBuffer;

void (*addJobFunPtr)();
void (*setSensorFunPtr)(int i);

/******************************************************************************/
/*            Cortex-M Processor Exceptions Handlers                          */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */


uint8_t getBufferCapacity(BufferS_t* buf)
{
    uint8_t cap = BUFFER_SIZE;
    if(buf->startIndex > buf->endIntex)
    {
        cap = buf->startIndex - (buf->endIntex + 1);
    }
    else if(buf->startIndex < buf->endIntex)
    {
        cap = (buf->startIndex + BUFFER_SIZE) - (buf->endIntex + 1);
    }
    return cap;
}


int _write(int fd, char *str, int len)
{
	while(len)
	{
		int capacity = getBufferCapacity(&outBuffer);
		capacity = (capacity > len) ? len : capacity;
		int i = 0;

		for(i = 0; i < capacity; ++i)
		{
			outBuffer.endIntex = (outBuffer.endIntex % BUFFER_SIZE);
			outBuffer.buffer[outBuffer.endIntex++] = *(str++);
			outBuffer.wordCount++;
			len--;
		}
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
	return 0;
}

void SysTick_Handler(void)
{
	addJobFunPtr();
}

void USART1_IRQHandler()
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
    	char tmpChar;
    	tmpChar = USART_ReceiveData(USART1);

    	if((uint16_t)tmpChar == ENTER)
    	{
    		_write(0,inBuffer.buffer,inBuffer.wordCount);
    		inBuffer.wordCount = 0;
    		inBuffer.endIntex = 0;
    		inBuffer.startIndex = 0;
    	}
    	else
    	{
    		inBuffer.endIntex = (inBuffer.endIntex % BUFFER_SIZE);
    		inBuffer.buffer[inBuffer.endIntex++] = tmpChar;
    		inBuffer.wordCount++;

    		USART_SendData(USART1,tmpChar);
    	}
    }
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
    {
    	if(outBuffer.wordCount > 0)
    	{
    		USART_SendData(USART1,outBuffer.buffer[outBuffer.startIndex++]);
    		outBuffer.startIndex = (outBuffer.startIndex % BUFFER_SIZE);
    		outBuffer.wordCount--;
    	}
    	else
    	{
    		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    	}

    }
}

void EXTI9_5_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line8))
	{
		EXTI_ClearFlag(EXTI_Line8);
		TIM_Cmd(TIM3, ENABLE);
	}
}



void TIM3_IRQHandler()
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		TIM_Cmd(TIM3, DISABLE);
		TIM_ITConfig(TIM3,TIM_IT_CC3,DISABLE);
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2,ENABLE);
	}
}

void TIM2_IRQHandler()
{
	static int counter = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8))
		{
			TIM_Cmd(TIM2,DISABLE);
			TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
			TIM_SetCounter(TIM3,0);
			if((counter > 2) && (counter <=55))
			{
				setSensorFunPtr(MiCS6814e);
			}
			else if((counter > 55) && (counter < 200))
			{
				setSensorFunPtr(HIH6030e);
			}
			else
			{
//				printf("czas:%dms\n\r",counter*5);
			}
			counter = 0;
		}
		else
		{
			counter++;
		}
	}
}



/******************************************************************************/
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_md.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
