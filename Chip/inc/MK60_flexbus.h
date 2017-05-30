/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_flexbus.h
 * @brief      flexbus������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-09-02
 */

//���������������� flexbus_8080_init ������أ��޸ĺ������Ҫ�޸� flexbus_8080_init ����������
#define FB_BA       0x6000      //����ַ     = FB_BA * 0x10000 ���������� ��ͬ��Ƭѡ�ź�
                                //�ο� System memory map ��˵���� External Memory �ķ�ΧΪ 0x6000_0000 ~ 0x9FFF_FFFF
                                //�� FB_BA ��ȡֵ��ΧΪ 0x6000 ~ 0x9FFF

#define FB_BAM      0x0800      //����ַ��Ĥ = (FB_BA + FB_BAM)* 0x10000 ���������� ͬһ��CS���Ƶ������������Ŀ�ĵ�ַ

#define FB_8080_CMD     (*(volatile uint16 *)(FB_BA * 0x10000))
#define FB_8080_DATA    (*(volatile uint16 *)((FB_BA + FB_BAM )* 0x10000))



extern void flexbus_8080_init();


