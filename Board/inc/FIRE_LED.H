 /*!
  *     COPYRIGHT NOTICE
  *     Copyright (c) 2013,野火科技
  *     All rights reserved. 
  *     技术讨论：野火初学论坛 http://www.chuxue123.com
  *
  *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
  *     修改内容时必须保留野火科技的版权声明。
  *
  * @file       FIRE_LED.h
  * @brief      led驱动头文件
  * @author     野火科技
  * @version    v5.0
  * @date       2013-07-9
  */

#ifndef __FIRE_LED_H__
#define __FIRE_LED_H__


/*! 枚举LED编号 */
typedef enum
{
  LED0,
  LED1,
  LED2,
  LED3,
  LED4,
  LED5,
  LED_MAX,
}LED_e;

/*! 枚举LED亮灭状态 */
typedef enum LED_status
{
  LED_ON  =   0,            //灯亮(对应低电平)
  LED_OFF =   1             //灯暗(对应高电平)
}LED_status; 


extern void    led_init(LED_e);            //初始化LED端口
extern void    led(LED_e,LED_status);      //设置LED灯亮灭
extern void    led_turn(LED_e);            //设置LED灯亮灭反转

#endif  //__FIRE_LED_H__