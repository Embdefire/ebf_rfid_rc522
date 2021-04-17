/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   RFID-RC522ģ��ʵ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart1.h"
#include "rc522_config.h"
#include "rc522_function.h"
#include <stdbool.h>


/**
  * @brief  IC���Ժ���
  * @param  ��
  * @retval ��
  */
uint8_t KeyValue[]={0xFF ,0xFF, 0xFF, 0xFF, 0xFF, 0xFF};   // ��A��Կ
void IC_test ( void )
{
	uint32_t writeValue = 100;
	uint32_t readValue;
	char cStr [ 30 ];
  uint8_t ucArray_ID [ 4 ];    /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/                                                                                         
	uint8_t ucStatusReturn;      /*����״̬*/                                                                                           
  while ( 1 )
  {    
    /*Ѱ��*/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQIDL, ucArray_ID ) ) != MI_OK )  
       /*��ʧ���ٴ�Ѱ��*/
			ucStatusReturn = PcdRequest ( PICC_REQIDL, ucArray_ID );		                                                

		if ( ucStatusReturn == MI_OK  )
		{
      /*����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				PcdSelect(ucArray_ID);			
		
				PcdAuthState( PICC_AUTHENT1A, 0x11, KeyValue, ucArray_ID );//У������ 
        WriteAmount(0x11,writeValue); //д����
        if(ReadAmount(0x11,&readValue) == MI_OK)	//��ȡ���
				{		
					writeValue +=100;
					sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X",ucArray_ID [0], ucArray_ID [1], ucArray_ID [2],ucArray_ID [3] );
					printf ( "%s\r\n",cStr );  //��ӡ��ƬID

					
					printf ("���Ϊ��%d\r\n",readValue);
          PcdHalt();
				}				
			}				
		}		
		    
  }	
}
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main ( void )
{
  /*�δ�ʱ�ӳ�ʼ��*/
	SysTick_Init ();   
	
  /*USART1 ����ģʽΪ 115200 8-N-1���жϽ���*/
	USART1_Config ();   
	
  /*RC522ģ����������ĳ�ʼ������*/
	RC522_Init ();     
	
	printf ( "WF-RC522 Test\n" );
		
	PcdReset ();
  
  /*���ù�����ʽ*/
	M500PcdConfigISOType ( 'A' );
	
  while ( 1 )
  {
    /*IC�����*/
    IC_test ();                	
  }	    
}
/****************************END OF FILE**********************/

