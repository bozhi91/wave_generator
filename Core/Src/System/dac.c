/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dac.c
  * @brief   This file provides code for the configuration
  *          of the DAC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "dac.h"

/* USER CODE BEGIN 0 */
#include "tim.h"
#include "math.h"
#include "signalGen.h"
#include <string.h>

/* USER CODE END 0 */

DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac1_ch1;

/* DAC1 init function */
void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */
  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */
  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  sConfig.DAC_OutputSwitch = DAC_OUTPUTSWITCH_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */
  /* USER CODE END DAC1_Init 2 */

}

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dacHandle->Instance==DAC1)
  {
  /* USER CODE BEGIN DAC1_MspInit 0 */
  /* USER CODE END DAC1_MspInit 0 */
    /* DAC1 clock enable */
    __HAL_RCC_DAC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**DAC1 GPIO Configuration
    PA4     ------> DAC1_OUT1
    PA5     ------> DAC1_OUT2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* DAC1 DMA Init */
    /* DAC1_CH1 Init */
    hdma_dac1_ch1.Instance = DMA1_Channel3;
    hdma_dac1_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_dac1_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dac1_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dac1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_dac1_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dac1_ch1.Init.Mode = DMA_CIRCULAR;
    hdma_dac1_ch1.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_dac1_ch1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_DMA_REMAP_CHANNEL_ENABLE(HAL_REMAPDMA_TIM6_DAC1_CH1_DMA1_CH3);

    __HAL_LINKDMA(dacHandle,DMA_Handle1,hdma_dac1_ch1);

  /* USER CODE BEGIN DAC1_MspInit 1 */
  /* USER CODE END DAC1_MspInit 1 */
  }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle)
{

  if(dacHandle->Instance==DAC1)
  {
  /* USER CODE BEGIN DAC1_MspDeInit 0 */
  /* USER CODE END DAC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC1_CLK_DISABLE();

    /**DAC1 GPIO Configuration
    PA4     ------> DAC1_OUT1
    PA5     ------> DAC1_OUT2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5);

    /* DAC1 DMA DeInit */
    HAL_DMA_DeInit(dacHandle->DMA_Handle1);
  /* USER CODE BEGIN DAC1_MspDeInit 1 */
  /* USER CODE END DAC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

	//Initialize the DAC channel
	void startDAC(uint16_t *sine_table, unsigned int n_samples, unsigned int freq){

		HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
		HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)sine_table, n_samples, DAC_ALIGN_12B_R);
		HAL_TIM_Base_Start( getTimInst(6) );

		//Start with a default frequency of 100Hz
		Set_DAC_Frequency(freq);
	}

	//Write a fixed value to the DAC channel
	void DAC_Write(int value){
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R,  value);   // ~Vref/2
		//DAC1->DHR12R1 = value;
	}

	void toggleDAC(char status){

		if(status == 0){
		//	Set_DAC_Frequency(0);
			HAL_TIM_Base_Stop( getTimInst(6) );
		}
		else if(status == 1){
			HAL_TIM_Base_Start( getTimInst(6) );
		}
	}

	//Change the DAC frequency
	void Set_DAC_Frequency(uint32_t f_out){

		unsigned int pclk1 = HAL_RCC_GetPCLK1Freq()*2;
		uint32_t psc = ceil(pclk1/(f_out*65536.0f))-1; /// for high frequencies, we can use a low PSC(like 0)
		//No dynamic PSC is needed

		uint32_t arr = ceil(pclk1 / (f_out * N_SAMPLES)) - 1;

		//Stop the Tim_6, set the registers values and start the Tim_6 again
		HAL_TIM_Base_Stop( getTimInst(6) );
		__HAL_TIM_SET_PRESCALER( getTimInst(6) , psc);
		__HAL_TIM_SET_AUTORELOAD( getTimInst(6) , arr);
		HAL_TIM_Base_Start( getTimInst(6) );

		/*if(freq_hz == 0) return;
		uint32_t timer_base = CLOCK_FREQ_MHZ*1000000 / (TIM6_PSC + 1UL);
		uint32_t arr        = (timer_base / (freq_hz * N_SAMPLES)) - 1UL;

		if(arr < 1UL) arr = 1UL;
		if(arr > 0xFFFFUL) arr = 0xFFFFUL;   // límite 16 bits TIM6

		__HAL_TIM_SET_AUTORELOAD(&htim6, arr);
		__HAL_TIM_SET_COUNTER(&htim6, 0);*/
	}

	/*uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
uint32_t timclk;

if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1)
{
    timclk = pclk1 * 2;
}
else
{
    timclk = pclk1;
}*/

/* USER CODE END 1 */
