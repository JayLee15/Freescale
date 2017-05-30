/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_rtc.h
 * @brief      rtcģ�麯��
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-08-26
 */

#ifndef _MK60_RTC_H_
#define _MK60_RTC_H_

extern void     rtc_init(void);                         //��ʼ��

extern void     rtc_set_time(uint32 seconds);           //����ʱ��
extern uint32   rtc_get_time(void);                     //��ȡʱ��

extern uint8    rtc_set_alarm(uint32 alarm);            //��������
extern void     rtc_close_alarm(void);                  //�ر�����


#endif