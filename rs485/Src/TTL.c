#include "ttl.h"

UART_HandleTypeDef huart1; 

u8 USART_RX_BUF1[USART_REC_LEN1];    

u16 USART_RX_STA1=0;       	

u8 aRxBuffer1[RXBUFFERSIZE1];
   		
 void TTL_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
	
   __HAL_RCC_USART1_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	

	huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);			        
    
  

	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);  
	HAL_NVIC_EnableIRQ(USART1_IRQn);				        
	HAL_NVIC_SetPriority(USART1_IRQn,1,0);			        
											
}

void USART1_IRQHandler(void)                	
{ 

	HAL_UART_IRQHandler(&huart1);	//调用HAL库中断处理公用函数
		
}

 
