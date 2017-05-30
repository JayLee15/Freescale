/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      野火K60 平台中断复位函数
 * @author     野火科技
 * @version    v5.0
 * @date       2013-06-26
 */

   
//    pit_time_start(PIT1);
//      Time = pit_time_get_us(PIT1);     
//      pit_time_close  (PIT1);
//      printf("Time:%d us\n",Time); 
#include    "MK60_it.h"
#include    "include.h"


extern  uint32 Time;
extern int Check_time;
extern int End_time;

char Encoder_flag = 0;
char CCD_Get_flag = 0;
char UART_flag    = 0;
char PID_flag     = 0;
char Control_flag = 0;
char PD_flag      = 0;
char Lock_flag    = 0;
char Begin_flag   = 0;
char End_flag     = 0;
char Check_time_flag   = 0;
char Gyroscope_flag = 0;
extern char Stop_flag;

uint16 IRQ_time = 0;
uint16 Set_time = 0;

void PIT0_IRQHandler()                                                                                              
{ 
    IRQ_time++;
   if(IRQ_time % 4 == 0)
   {
     Gyroscope_flag = 1;
   }
   if(IRQ_time % Encoder_time == 0)
   {
     Encoder_flag=1;
   }
   if(IRQ_time % CCD_time == 0)
   {
     CCD_Get_flag = 1;
   }
    if(IRQ_time % 1000 == 0)
    {
      Set_time = Set_time+1;
    }
    if (Set_time == 3)
    {
      Begin_flag = 1;
    }
    if (Set_time == 6)
    {
      Lock_flag  = 1;
    }
    if(Set_time == Check_time)
    {
      Check_time_flag = 1;
    }
    if(Set_time == End_time)
    {
      End_flag = 1;
      Set_time = 0;
    }
    if(IRQ_time % 3000 == 0)
    {
      IRQ_time = 0; 
    }
      
    
    PIT_Flag_Clear(PIT0);
    PIT_Flag_Clear(PIT0);  
}
//0x31 前进
//0x32 后退
//0x33 左转
//0x34 右转
//0x35 停止


void UART3_IRQHandler()
{
  DisableInterrupts;
 // led_turn(LED0);
  char ch;
  uart_getchar (FIRE_PORT,&ch);
  UART_flag = 1;
  switch(ch)
  {
  case '1':
    Control_flag = 1;
    led(LED0,LED_ON);
    break; 
  case '2':
    Control_flag = 2;
    break;
  case '3':
    Control_flag = 3;
    break;
  case '4':
    Control_flag = 4;
    break;
  case '5':
    Control_flag = 5;
    break;
    
  case 'P':
    PID_flag = 1;
    led(LED0,LED_ON);
    break;
  case 'I':
    PID_flag = 2;
    break;
  case 'D':
    PID_flag = 3;
    break;
    
  case 'S':
    PD_flag = 1;
    break; 
  case 'T':
    PD_flag = 2;
    break;
  case 'R':
    PD_flag = 3;
    break; 
  case 'U':
    PD_flag = 4;
    break;
    
  default:break;
  }
  
 EnableInterrupts;
}

void GRAYA_IRQHandler(void)
{
  End_flag = 1;
  PORT_FUNC(D,7,GRAYA_IRQHandler);
}
void GRAYB_IRQHandler(void)
{
  End_flag = 1;
  PORT_FUNC(C,6,GRAYB_IRQHandler);
}