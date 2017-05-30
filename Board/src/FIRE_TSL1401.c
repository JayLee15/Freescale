/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��FIRE_TSL1401.c
 * ����         ������CCD������
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "common.h"
#include "math.h"
#include <stdarg.h>
#include "include.h"


static volatile tsl1401_status_e  tsl1401_flag = tsl1401_noint;//��̬�������ܸı�  CCD��־λ û��ʼ��
static volatile uint32            tsl1401_time;//long int


static void tsl1401_restet();
static void tsl1401_delay(volatile uint32 time );
static void tsl1401_gather(void);

extern uint8 tsl1401[TSL1401_MAX*3][TSL1401_SIZE];

//����CCD�ܽ� 
//                          CCD1            CCD2     
/*ADCn_Ch_e   tsl1401_ch[] = {ADC1_DP3  ,  ADC0_SE9};   //CCD���õ� ADCͨ��  
PTXn_e      tsl1401_si[] = { PTE5 ,  PTB16};      //CCD���õ� SI�ܽ�  
PTXn_e      tsl1401_clk[] = {  PTE4   ,  PTB17};      //CCD���õ� CLk�ܽ� */
//ADCn_Ch_e   tsl1401_ch[]  = { ADC0_DP1  ,  ADC1_DP1  ,  ADC0_DP3 };      //CCD���õ� ADCͨ��         
//PTXn_e      tsl1401_si[]  = {   PTE5   ,    PTE3   ,    PTB11  };      //CCD���õ� SI�ܽ�
//PTXn_e      tsl1401_clk[] = {   PTE4   ,    PTE2   ,    PTB10  };      //CCD���õ� SI�ܽ�

ADCn_Ch_e   tsl1401_ch[]  = { ADC0_DP0  ,  ADC1_DP1  ,  ADC0_DP3 };      //CCD���õ� ADCͨ��         
PTXn_e      tsl1401_si[]  = {   PTE5   ,    PTE3   ,    PTB11  };      //CCD���õ� SI�ܽ�
PTXn_e      tsl1401_clk[] = {   PTE4   ,    PTE2   ,    PTB10  };      //CCD���õ� SI�ܽ�

//CCD �ɼ������Ĵ洢�ռ�ָ������
uint8 *tsl1401_addr[TSL1401_MAX] = {0};           //CCD �ɼ�ͼ��ĵ�ַ����

//ȫ��CCD��SI�ܽ��������      ��������ڣ������������е���ʼ ����IO�ڿ���
void tsl1401_si_out(uint8 data)
{
    uint8   i = TSL1401_MAX;

    ASSERT(data < 2);

    while(i--)
    {
        gpio_set(tsl1401_si[i], data);
    }
}

//ȫ��CCD��CLK�ܽ��������
void tsl1401_clk_out(uint8 data)
{
    uint8   i = TSL1401_MAX;

    ASSERT(data < 2);

    while(i--)
    {
        gpio_set(tsl1401_clk[i], data);
    }
}

//tsl1401,����CCD��ʼ����timeΪ�ع�ʱ�䣬��λ�� TSL1401_INT_TIME ������һ�£���ʼ����������tsl1401_set_addrs �����ô洢��ַ�����������ô洢ͼ���ַ��
void tsl1401_init(uint32 time)
{
    uint8 i = TSL1401_MAX;

    while(i)
    {
        i--;
        adc_init(tsl1401_ch[i]); //��ʼ�� CCD ���õ� ADCͨ��

        //��ʼ�� CCD �Ŀ��ƹܽ� CLK �� SI
        gpio_init (tsl1401_clk[i], GPO, 0);  //���
        gpio_init (tsl1401_si[i] , GPO, 0);
    }

    tsl1401_time = time;
    TSL1401_INT_TIME(tsl1401_time);         //  �����ж�ʱ��  ����CCD ��ʱ�жϵ��ж�ʱ�����ú��� ��λms

    tsl1401_restet();                       //  ������һ֡�����ݣ��ҵģ�
}

//���òɼ�ͼ��ĵ�ַ
//num Ϊ TSL1401_MAX ʱ������ȫ����ַ����������ָ���ĵ�ַ
void tsl1401_set_addrs(TSL1401_e num, uint8 *addr, ... )
{
    ASSERT(num <= TSL1401_MAX);

    va_list ap;                                 //����ջָ��ap
    uint8_t *value;
    uint8_t n = 0;

    if(num < TSL1401_MAX)
    {
        tsl1401_addr[num] = addr;
    }
    else if(num == TSL1401_MAX)
    {
        va_start(ap, addr);                         //��ȡ�ɱ�����б�ĵ�һ�������ĵ�ַ
        value = addr;
        while(num--)
        {
            tsl1401_addr[n++] = value;
            value = va_arg(ap, uint8_t * );             //��ȡ�ɱ�����ĵ�ǰ����������ָ�����Ͳ���ָ��ָ����һ����
        }

        va_end(ap);                                 //���va_list�ɱ�����б�
    }
}




//tsl1401,����CCD�ɼ�ͼ��
void tsl1401_get_img(void)
{
    tsl1401_flag = tsl1401_start;           //�����ɼ�
    //while(tsl1401_flag != tsl1401_finish);  //�ȴ��ɼ����
}

//��ȡ tsl1401,����CCD �ع�ʱ��
uint32 tsl1401_get_time(void)
{
    return tsl1401_time;
}

//���� tsl1401,����CCD �ع�ʱ��
void tsl1401_set_time(uint32 time)
{
    tsl1401_time = time;
    TSL1401_INT_TIME(tsl1401_time);         //  �����ж�ʱ��
}

//���붨ʱ�ж���Ա��ڶ�ʱ��λ�ع�ʱ��
void tsl1401_time_isr()
{
    //ֻ�ж��Ƿ�ʼ�ɼ����Ƿ�ɼ����
    if(tsl1401_flag == tsl1401_start)
    {
        tsl1401_gather();
        tsl1401_flag = tsl1401_finish;
    }
    else
    {
        tsl1401_restet();                   // ��λ
    }
}

void tsl1401_delay( uint32 time )
{
    volatile uint32 i = time;

    while(i--);
}


//�ɼ�ͼ��
void tsl1401_gather(void)
{
#define TSL1401_TIME   10   //��ʱʱ��

    uint8_t n = TSL1401_SIZE, k = 0;
    uint8_t i;

    tsl1401_clk_out(0);             //CLK = 0
    tsl1401_delay(TSL1401_TIME);
    tsl1401_si_out(1);              //SI  = 1
    tsl1401_delay(TSL1401_TIME);
    tsl1401_clk_out(1);             //CLK = 1
    tsl1401_delay(TSL1401_TIME);
    tsl1401_si_out(0);              //SI  = 0
    tsl1401_delay(TSL1401_TIME);

    while(n--)
    {

        tsl1401_clk_out(0);        //CLK = 0

        i = TSL1401_MAX;
        while(i--)
        {
            (tsl1401_addr[i])[k] = (uint8_t)adc_once(tsl1401_ch[i], ADC_8bit);   //�ɼ�ĳ·ģ������ADֵ  ͨ��  ��ȷ��
            //*img++ =  adc_once(ADC1_AD8, ADC_8bit);
        }

        tsl1401_clk_out(1);        //CLK = 1
        tsl1401_delay(TSL1401_TIME);
        k++;
    }
#undef TSL1401_TIME
}

//���ڵ����ع�ʱ��
void tsl1401_restet()
{
#define TSL1401_RETIME   1

    uint8 n = TSL1401_SIZE; //nΪADC�ɼ�����

    tsl1401_clk_out(0);             //CLK = 0  //ȫ��CCD��CLK�ܽ����0 ��Ƭ����ȡADֵ
    tsl1401_delay(TSL1401_RETIME); //��ʱ
    tsl1401_si_out(1);              //SI  = 1 //ȫ��CCD��SI�ܽ����1  ��ʼ���ͼ�� 
    tsl1401_delay(TSL1401_RETIME);
    tsl1401_clk_out(1);             //CLK = 1   AO��ʼ�������
    tsl1401_delay(TSL1401_RETIME);
    tsl1401_si_out(0);              //SI  = 0 
    tsl1401_delay(TSL1401_RETIME);

    while(n--)
    {
        tsl1401_clk_out(0);             //CLK = 0
        tsl1401_delay(TSL1401_RETIME);
        tsl1401_clk_out(1);             //CLK = 1
        tsl1401_delay(TSL1401_RETIME);
    }
#undef TSL1401_RETIME
}

void CCD_init()
{
    tsl1401_set_addrs(TSL1401_MAX,(uint8 *)&tsl1401[0],(uint8 *)&tsl1401[1],(uint8 *)&tsl1401[2]);//���òɼ�ͼ��ĵ�ַ
    tsl1401_init(1);                         //��ʼ�� ����CCD ������ �ж�ʱ��Ϊ time

}

