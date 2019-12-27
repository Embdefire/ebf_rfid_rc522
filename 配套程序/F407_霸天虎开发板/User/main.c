/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   RFID-RC522ģ��ʵ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "rc522_config.h"
#include "rc522_function.h"
#include <stdbool.h>
#include "./lcd/bsp_ili9806g_lcd.h"
#include "./font/fonts.h"	
#include "./systick/bsp_SysTick.h"


/**
  * @brief  ���Ժ���
  * @param  ��
  * @retval ��
  */
void IC_test ( void )
{
	char cStr [ 30 ];
  uint8_t ucArray_ID [ 4 ];   //�Ⱥ���IC�������ͺ�UID(IC�����к�)
	uint8_t ucStatusReturn;     //����״̬
  static uint8_t ucLineCount = 0; 
	
  while ( 1 )
  { 
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK ) //Ѱ��
      //��ʧ���ٴ�Ѱ��
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                 

		if ( ucStatusReturn == MI_OK  )
		{ 
      //����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X", 
                  ucArray_ID [ 0 ], ucArray_ID [ 1 ], 
                  ucArray_ID [ 2 ], ucArray_ID [ 3 ] );
								
				printf ( "%s\r\n",cStr ); 
				
				if ( ucLineCount == 0 )
          
      	LCD_SetTextColor(BLUE);
				ILI9806G_DispStringLine_EN( LINE(ucLineCount), cStr);
        			
				ucLineCount ++;
				
				if ( ucLineCount == 14 ) 
          ucLineCount = 0;								
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
	SysTick_Init();       //�δ�ʱ�ӳ�ʼ��
	
	Debug_USART_Config(); //USART1 ����ģʽΪ 115200 8-N-1���жϽ��� 
	
	RC522_Init ();        //RC522ģ����������ĳ�ʼ������
 	
	ILI9806G_Init ();     //LCD ��ʼ��
	ILI9806G_GramScan ( 6 );

	printf ( "WF-RC522 Test\n" );
	
	LCD_SetFont(&Font16x32);
	LCD_SetColors(RED,BLACK);

  ILI9806G_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH); //��������ʾȫ�� 
	ILI9806G_DispStringLine_EN(LINE(16) ,
             "Please put the IC card on WF-RC522 antenna area ..." );
	
	PcdReset ();
	M500PcdConfigISOType ( 'A' );//���ù�����ʽ
		
  while ( 1 )
  {
    IC_test ();//IC�����	
  }	
}


/*********************************************END OF FILE**********************/

