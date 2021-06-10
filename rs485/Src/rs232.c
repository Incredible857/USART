#include "rs232.h"

UART_HandleTypeDef huart2; 

u8 USART_RX_BUF2[USART_REC_LEN2];    

u16 USART_RX_STA2=0;       	

u8 aRxBuffer2[RXBUFFERSIZE2];

   		
void RS232_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
	
   __HAL_RCC_USART2_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
	
	 GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart2);			        
    
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);  
	HAL_NVIC_EnableIRQ(USART2_IRQn);				        
	HAL_NVIC_SetPriority(USART2_IRQn,0,0);			        
	
		
}

void USART2_IRQHandler(void)                	
{ 

	HAL_UART_IRQHandler(&huart2);	//调用HAL库中断处理公用函数
		
}





