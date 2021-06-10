#ifndef __RS232_H
#define __RS232_H
#include "sys.h"

#define USART_REC_LEN2  	200  

#define RXBUFFERSIZE2   1

extern u8  USART_RX_BUF2[USART_REC_LEN2]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 

extern u16 USART_RX_STA2; 

extern u8 aRxBuffer2[RXBUFFERSIZE2];

extern UART_HandleTypeDef huart2;

void RS232_Init(void);
	
#endif
