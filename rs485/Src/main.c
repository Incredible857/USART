#include "main.h"
#include "rs485.h"
#include "gpio.h"
#include "ttl.h"
#include "string.h"
#include "rs232.h"


#include "stdio.h"


int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		if((USART_RX_STA2&0x8000)==0)
		{
			if(USART_RX_STA2&0x4000)
			{
				if(aRxBuffer2[0]!=0x0a)USART_RX_STA2=0;
				else USART_RX_STA2|=0x8000;	
			}
			else 
			{	
				if(aRxBuffer2[0]==0x0d)USART_RX_STA2|=0x4000;
				else
				{
					USART_RX_BUF2[USART_RX_STA2&0X3FFF]=aRxBuffer2[0];
					USART_RX_STA2++;
					if(USART_RX_STA2>(USART_REC_LEN2-1))USART_RX_STA2=0;
				}		 
			}
		} 
		HAL_UART_Receive_IT(&huart2, (u8 *)aRxBuffer2, RXBUFFERSIZE2);
	}		

	if(huart->Instance==USART1)
	{
		if((USART_RX_STA1&0x8000)==0)
		{
			if(USART_RX_STA1&0x4000)
			{
				if(aRxBuffer1[0]!=0x0a)USART_RX_STA1=0;
				else USART_RX_STA1|=0x8000;	
			}
			else 
			{	
				if(aRxBuffer1[0]==0x0d)USART_RX_STA1|=0x4000;
				else
				{
					USART_RX_BUF1[USART_RX_STA1&0X3FFF]=aRxBuffer1[0];
					USART_RX_STA1++;
					if(USART_RX_STA1>(USART_REC_LEN1-1))USART_RX_STA1=0;  
				}		 
			}
		} 
		HAL_UART_Receive_IT(&huart1, (u8 *)aRxBuffer1, RXBUFFERSIZE1);
	}		

	if(huart->Instance==USART3)
	{
		if((USART_RX_STA3&0x8000)==0)
		{
			if(USART_RX_STA3&0x4000)
			{
				if(aRxBuffer3[0]!=0x0a)USART_RX_STA3=0;
				else USART_RX_STA3|=0x8000;	
			}
			else 
			{	
				if(aRxBuffer3[0]==0x0d)USART_RX_STA3|=0x4000;
				else
				{
					USART_RX_BUF3[USART_RX_STA3&0X3FFF]=aRxBuffer3[0];
					USART_RX_STA3++;
					if(USART_RX_STA3>(USART_REC_LEN3-1))USART_RX_STA3=0;  
				}		 
			}
		} 
		HAL_UART_Receive_IT(&huart3, (u8 *)aRxBuffer3, RXBUFFERSIZE3);
	}		

}



//AB CD 01 03 09 05 27 9D AD 


void SystemClock_Config(void);




int main(void)
{
  u8 len;
  TMODBUS_MSG tModbus={0};
	
  HAL_Init();
  SystemClock_Config();
	
  MX_GPIO_Init();
  RS485_Init();
  TTL_Init();
  RS232_Init();
  
  printf("/********************************\r\n");
  printf("电子系统设计与分析\r\n");
  printf("实验四：通信接口应用设计\r\n");
  printf("班级：信息工程1901\r\n");
  printf("作者：刘尚宇1941900120\r\n");
  printf("日期：2021年6月9日\r\n");
  printf("/********************************\r\n");

  HAL_UART_Receive_IT(&huart2, (u8 *)aRxBuffer2, RXBUFFERSIZE2);
 
	
  RS485_TX_EN=0;
  HAL_UART_Receive_IT(&huart3, (u8 *)aRxBuffer3, RXBUFFERSIZE3);
  
  while (1)
  {
	  		
       if(USART_RX_STA2&0x8000)
		{					   
			len=USART_RX_STA2&0x3fff;
			HAL_UART_Transmit(&huart2,(uint8_t*)"\r\n串口2发送的消息为:\r\n",24,1000);
			HAL_UART_Transmit(&huart2,(uint8_t*)USART_RX_BUF2,len,1000);	
			while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)!=SET);		
			HAL_UART_Transmit(&huart2,(uint8_t*)"\r\n\r\n",8,1000);
			USART_RX_STA2=0;
		}else if(USART_RX_STA1&0x8000)
		{					   
			len=USART_RX_STA1&0x3fff;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\r\n串口1发送的消息为:\r\n",24,1000);
			HAL_UART_Transmit(&huart1,(uint8_t*)USART_RX_BUF1,len,1000);	
			while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);		
			HAL_UART_Transmit(&huart1,(uint8_t*)"\r\n\r\n",8,1000);
			USART_RX_STA1=0;
		}else if(USART_RX_STA3&0x8000)
		{					   
			len=USART_RX_STA3&0x3fff;
			tModbus.iRegNum=0;
		    tModbus.iRegV1=0;
			tModbus.iRegV2=0;
		    ReadRegFromUartArray(USART_RX_BUF3,len,&tModbus);
			RS485_Send(&tModbus.iRegNum,1);
		    RS485_Send((uint8_t *)&tModbus.iRegV1,1);
			RS485_Send((uint8_t *)&tModbus.iRegV2,1);
			USART_RX_STA3=0;
			
//			len=USART_RX_STA3&0x3fff;
//			RS485_Send((uint8_t*)"\r\n串口3发送的消息为:\r\n",24);
//			RS485_Send((uint8_t*)USART_RX_BUF3,len);	
//			while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)!=SET);		
//			RS485_Send((uint8_t*)"\r\n\r\n",8);
//			USART_RX_STA3=0;
		}
  
  
	
		
		
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
