/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   RFID-R522ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 F429 ������
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
#include "./lcd/bsp_lcd.h"
#include "./systick/bsp_SysTick.h"
#include "./font/fonts.h"
#include "./sdram/bsp_sdram.h"

/**
  * @brief  ���Ժ���
  * @param  ��-
  * @retval ��
  */
void IC_test ( void )
{
	char cStr [ 30 ];
  u8 ucArray_ID [ 4 ]; /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/                                                                                            
	u8 ucStatusReturn;   /*����״̬ */                                                                                           
  static u8 ucLineCount = 0; 
		
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
				
				if ( ucLineCount == 0 )
         
				  LCD_SetTextColor(LCD_COLOR_RED);
				  LCD_DisplayStringLine(LINE(ucLineCount), (uint8_t* )cStr);
        
				  ucLineCount ++;
				
				if ( ucLineCount == 12 ) ucLineCount = 0;							
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
  SysTick_Init();   

  /*USART1 ����ģʽΪ 115200 8-N-1���жϽ���*/
  Debug_USART_Config();   

  /*RC522ģ����������ĳ�ʼ������*/
  RC522_Init ();     

  /*��ʼ��Һ����*/
  LCD_Init();
  LCD_LayerInit();

  printf ( "WF-RC522 Test\n" );

  LTDC_Cmd(ENABLE);
	
	/*�ѱ�����ˢ��ɫ*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
  
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
  LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BLACK);
  
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/		
  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  
  LCD_SetTextColor(LCD_COLOR_BLUE);

  LCD_DisplayStringLine(LINE(14),
               (uint8_t*)"Please put the IC card on WF-RC522 antenna area ..." );

  PcdReset ();
  M500PcdConfigISOType ( 'A' );/*���ù�����ʽ*/

  while ( 1 )
  {
    IC_test ();/*IC�����*/	
  }	
}



/*********************************************END OF FILE**********************/

