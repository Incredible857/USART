#ifndef __TTL_H
#define __TTL_H
#include "sys.h"

extern UART_HandleTypeDef huart1;

#define USART_REC_LEN1  	200  

#define RXBUFFERSIZE1   1

extern u8  USART_RX_BUF1[USART_REC_LEN1]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 

extern u16 USART_RX_STA1; 

extern u8 aRxBuffer1[RXBUFFERSIZE1];



void TTL_Init(void);
	
#endif
