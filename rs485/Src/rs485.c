#include "rs485.h"
#include "sys.h"
#include "tim.h"


  		 	 
UART_HandleTypeDef huart3;

u8 USART_RX_BUF3[USART_REC_LEN3],USART_TX_BUF3[USART_REC_LEN3];    

u16 USART_RX_STA3=0;       	

u8 aRxBuffer3[RXBUFFERSIZE3];

  
void RS485_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}



void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  
    __HAL_RCC_USART3_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
   
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

   
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
 
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
 
    __HAL_RCC_USART3_CLK_DISABLE();
  
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    HAL_NVIC_DisableIRQ(USART3_IRQn);
  }
} 

void USART3_IRQHandler(void)                	
{ 

	HAL_UART_IRQHandler(&huart3);	//调用HAL库中断处理公用函数
		
}

void RS485_Send( uint8_t *pData, uint16_t Size)
{
	RS485_TX_EN=1;
	HAL_UART_Transmit(&huart3,pData,Size,1000);
	RS485_TX_EN=0;
	
}

int GetCRC(const unsigned char *pcRec,unsigned int iLen,unsigned char *pcCRC)
{
		unsigned int  IX,IY,iCRC;	
        iCRC=0xFFFF;//set all 1
		if (iLen<=0)
		{
				pcCRC[0] =0;//高位置
				pcCRC[1] =0;  //低位置
				return -1;
		}
		else
		{
				iLen--;
				for (IX=0;IX<=iLen;IX++)
				{
					iCRC=iCRC^(unsigned int)(pcRec[IX]);
					for(IY=0;IY<=7;IY++)
					{
					   if ((iCRC&1)!=0 )
						{
							iCRC=(iCRC>>1)^0xA001;
						}else
						{
							iCRC=iCRC>>1;    //
						}
						}
				}

		}
		pcCRC[0] = (unsigned char)((iCRC & 0xff00)>>8);//高位置
		pcCRC[1] = (unsigned char)(iCRC & 0x00ff);  //低位置
		return 0;
}

int8_t ReadRegFromUartArray(unsigned char *pcRec,unsigned char cLen,PTMODBUS_MSG ptModbusMsg)
{
	unsigned char acCRC[2]={0};

  //unsigned short iTmp=0;
	//check length
	if(cLen<9)
	{
		return -1;
	}
//	//check start
	if(pcRec[0]!=0xAB || pcRec[1]!=0xCD)
	{
		return -1;
	}
//	//check CRC
////	GetCRC(pcRec,7,acCRC);
////	if(acCRC[0]!=pcRec[8] || acCRC[1]!=pcRec[7])
////	{			
////			return -1;
////	}
	ptModbusMsg->iSlave=pcRec[2];
	ptModbusMsg->iCMD=pcRec[3];
	ptModbusMsg->iRegNum=pcRec[4];
	ptModbusMsg->iRegV1=pcRec[5];
	ptModbusMsg->iRegV2=pcRec[6];
	
  return 0;
}	










