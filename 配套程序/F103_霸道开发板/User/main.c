/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   RFID-RC522ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"	
#include "./lcd/bsp_ili9341_lcd.h"
#include "./flash/bsp_spi_flash.h"
#include "./SysTick/bsp_SysTick.h"
#include "rc522_config.h"
#include "rc522_function.h"
#include <stdbool.h>

void IC_CMT ( uint8_t * UID, uint8_t * KEY, uint8_t RW, uint8_t * Dat );
/**
  * @brief  IC���Ժ���
  * @param  ��
  * @retval ��
  */
uint8_t buffer[100];
uint8_t buff1[] = {0x50 ,0xBA, 0xA9, 0x89};
uint8_t buff2[]={0xFF ,0xFF, 0xFF, 0xFF, 0xFF, 0xFF};   // ��A��Կ
uint8_t buff3[]={0x55 ,0xBA, 0xA9, 0x89, 0x50 ,0xBA, 0xA9, 0x89, 0x50 ,0xBA, 0xA9, 0x89, 0x50 ,0xBA, 0xA9, 0x89};    // д�������
void IC_test ( void )
{
	char cStr [ 30 ];
  uint8_t ucArray_ID [ 4 ];    /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/                                                                                         
	uint8_t ucStatusReturn;      /*����״̬*/                                                                                          
  static uint8_t ucLineCount = 0; 
  int i;
  
  while ( 1 )
  { 
    /*******************��Ƭ������д����****************************/
    buff3[0]++;
    
    IC_CMT(buff1, buff2, 0, buff3);    // д��

    IC_CMT(buff1, buff2, 1, buffer);   // ����
    
    printf ("����������Ϊ��\n");
    
    for( i=0; i<16; i++)
    {
      printf ("0x%X\n", buffer[i]);    // ��ӡ����������
    }
    /*******************��Ƭ������д���� END****************************/  
    
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
          LCD_SetTextColor(RED); 	
				
				ILI9341_DispStringLine_EN(LINE(ucLineCount) , (char* )cStr );
       
				IC_CMT(buff1, buff2, 1, buffer);   // ��Ҫ�ڶ�һ���´�����д��ȥ����֪��Ϊʲô������Ӧ���������ܸı�ĳ��ģʽ�ɣ�
        
				ucLineCount ++;
				
				if ( ucLineCount == 17 ) ucLineCount = 0;	    
			}		
		}		
  }	
}
char PcdComMF522 ( uint8_t ucCommand, 
                    uint8_t * pInData,
                    uint8_t ucInLenByte, 
                    uint8_t * pOutData, 
                    uint32_t * pOutLenBit )	;
uint8_t buff[] = {0x06, 0x61, 0x1, 0x2, 0x3, 0x4 ,0x6, 0x1, 0x2, 0x3, 0x4};

uint32_t bufferlen;
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
  /*�δ�ʱ�ӳ�ʼ��*/
  SysTick_Init ();
  
	/*LCD ��ʼ��*/
	ILI9341_Init ();  
  
	/* USART config */
	USART_Config(); 
  
  /*RC522ģ����������ĳ�ʼ������*/
  RC522_Init ();     
	
  printf ( "WF-RC522 Test\n" );
	
  /*����0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�*/
	ILI9341_GramScan ( 6 ); 
  LCD_SetFont(&Font8x16);
	LCD_SetColors(BLACK,BLACK);
  
  /* ��������ʾȫ�� */
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
  
	/********��ʾ�ַ���ʾ��*******/
  LCD_SetTextColor(RED);  

  ILI9341_DispStringLine_EN(LINE(18),
       (char* )"Please put the IC card on WF-RC522 antenna area ..."); 
  
  LCD_SetTextColor(YELLOW);
  
  PcdReset ();
   
  /*���ù�����ʽ*/   
	M500PcdConfigISOType ( 'A' );
  
  while(1)
  { 
    /*IC�����	*/    
    IC_test ();     

  }

}


/* ------------------------------------------end of file---------------------------------------- */

