#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus

#endif


#include "main.h"
#include "sys.h"

#define USART_REC_LEN3  200  
#define RXBUFFERSIZE3   1

extern u8  USART_RX_BUF3[USART_REC_LEN3]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA3; 
extern u8 aRxBuffer3[RXBUFFERSIZE3];
extern UART_HandleTypeDef huart3;

#define RS485_TX_EN	PEout(10)	

typedef struct _TMODBUS_MSG
{
	uint8_t iSlave;//从机号
	uint8_t iCMD;//命令值
	uint8_t iRegNum;//寄存器号
	uint16_t iRegV1;//寄存器值
    uint16_t iRegV2;//调试	
}TMODBUS_MSG,*PTMODBUS_MSG;

void RS485_Init(void);
void RS485_Send( uint8_t *pData, uint16_t Size);
int GetCRC(const unsigned char *pcRec,unsigned int iLen,unsigned char *pcCRC);
int8_t ReadRegFromUartArray(unsigned char *pcRec,unsigned char cLen,PTMODBUS_MSG ptModbusMsg);











#ifdef __cplusplus

#endif
#endif 


