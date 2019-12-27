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
  * ʵ��ƽ̨:Ұ�� F103-MINI STM32 ������ 
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
#include "bsp_lcd.h"
#include "./font/fonts.h"


/**
  * @brief  ���Ժ���
  * @param  ��
  * @retval ��
  */
void IC_test ( void )
{
	char cStr [ 30 ];
  uint8_t ucArray_ID [ 4 ];   /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/                                                                                          
	uint8_t ucStatusReturn;     /*����״̬ */                                                                                        
  static uint8_t ucLineCount = 0; 
	
  while ( 1 )
  { 
    /*Ѱ��*/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )  
       /*��ʧ���ٴ�Ѱ��*/
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                

		if ( ucStatusReturn == MI_OK  )
		{
      /*����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X",
                  ucArray_ID [ 0 ],
                  ucArray_ID [ 1 ],
                  ucArray_ID [ 2 ],
                  ucArray_ID [ 3 ] );
								
				printf ( "%s\r\n",cStr ); 			
				
				ILI9341_DispStringLine_EN(LINE(ucLineCount), cStr );
				
				ucLineCount ++;
				
				if ( ucLineCount == 10 ) ucLineCount = 0;
							
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
	
  /*USART1 ����ģʽΪ 115200 8-N-1���жϽ��� */
	USART1_Config ();  
	
  /*RC522ģ����������ĳ�ʼ������*/
	RC522_Init ();     
	
  /*LCD ��ʼ��*/
	LCD_Init ();        
	
	ILI9341_GramScan ( 6 );
	printf ( "WF-RC522 Test\n" );
	
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);

  /* ��������ʾȫ�� */
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	
	ILI9341_DispStringLine_EN(LINE(13), 
         "Please put the IC card on WF-RC522 antenna area ..." );
	
	LCD_SetTextColor(BLUE);
	
	PcdReset ();
  
  /*���ù�����ʽ*/
	M500PcdConfigISOType ( 'A' );
	
	
  while ( 1 )
  {
    /*IC�����	*/
    IC_test ();
  }	
}



/****************************END OF FILE**********************/

