/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       MK60_rtc.h
 * @brief      rtc模块函数
 * @author     野火科技
 * @version    v5.0
 * @date       2013-08-26
 */

#ifndef _MK60_RTC_H_
#define _MK60_RTC_H_

extern void     rtc_init(void);                         //初始化

extern void     rtc_set_time(uint32 seconds);           //设置时间
extern uint32   rtc_get_time(void);                     //获取时间

extern uint8    rtc_set_alarm(uint32 alarm);            //设置闹钟
extern void     rtc_close_alarm(void);                  //关闭闹钟


#endif