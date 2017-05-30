/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：FIRE_TSL1401.c
 * 描述         ：线阵CCD函数库
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "common.h"
#include "math.h"
#include <stdarg.h>
#include "include.h"


static volatile tsl1401_status_e  tsl1401_flag = tsl1401_noint;//静态变量可能改变  CCD标志位 没初始化
static volatile uint32            tsl1401_time;//long int


static void tsl1401_restet();
static void tsl1401_delay(volatile uint32 time );
static void tsl1401_gather(void);

extern uint8 tsl1401[TSL1401_MAX*3][TSL1401_SIZE];

//配置CCD管脚 
//                          CCD1            CCD2     
/*ADCn_Ch_e   tsl1401_ch[] = {ADC1_DP3  ,  ADC0_SE9};   //CCD所用的 ADC通道  
PTXn_e      tsl1401_si[] = { PTE5 ,  PTB16};      //CCD所用的 SI管脚  
PTXn_e      tsl1401_clk[] = {  PTE4   ,  PTB17};      //CCD所用的 CLk管脚 */
//ADCn_Ch_e   tsl1401_ch[]  = { ADC0_DP1  ,  ADC1_DP1  ,  ADC0_DP3 };      //CCD所用的 ADC通道         
//PTXn_e      tsl1401_si[]  = {   PTE5   ,    PTE3   ,    PTB11  };      //CCD所用的 SI管脚
//PTXn_e      tsl1401_clk[] = {   PTE4   ,    PTE2   ,    PTB10  };      //CCD所用的 SI管脚

ADCn_Ch_e   tsl1401_ch[]  = { ADC0_DP0  ,  ADC1_DP1  ,  ADC0_DP3 };      //CCD所用的 ADC通道         
PTXn_e      tsl1401_si[]  = {   PTE5   ,    PTE3   ,    PTB11  };      //CCD所用的 SI管脚
PTXn_e      tsl1401_clk[] = {   PTE4   ,    PTE2   ,    PTB10  };      //CCD所用的 SI管脚

//CCD 采集回来的存储空间指针数组
uint8 *tsl1401_addr[TSL1401_MAX] = {0};           //CCD 采集图像的地址数组

//全部CCD的SI管脚输出数据      串行输入口，决定数据序列的起始 可由IO口控制
void tsl1401_si_out(uint8 data)
{
    uint8   i = TSL1401_MAX;

    ASSERT(data < 2);

    while(i--)
    {
        gpio_set(tsl1401_si[i], data);
    }
}

//全部CCD的CLK管脚输出数据
void tsl1401_clk_out(uint8 data)
{
    uint8   i = TSL1401_MAX;

    ASSERT(data < 2);

    while(i--)
    {
        gpio_set(tsl1401_clk[i], data);
    }
}

//tsl1401,线性CCD初始化，time为曝光时间，单位与 TSL1401_INT_TIME 的设置一致，初始化后必须调用tsl1401_set_addrs 来设置存储地址（或者先设置存储图像地址）
void tsl1401_init(uint32 time)
{
    uint8 i = TSL1401_MAX;

    while(i)
    {
        i--;
        adc_init(tsl1401_ch[i]); //初始化 CCD 所用的 ADC通道

        //初始化 CCD 的控制管脚 CLK 和 SI
        gpio_init (tsl1401_clk[i], GPO, 0);  //输出
        gpio_init (tsl1401_si[i] , GPO, 0);
    }

    tsl1401_time = time;
    TSL1401_INT_TIME(tsl1401_time);         //  设置中断时间  定义CCD 定时中断的中断时间设置函数 单位ms

    tsl1401_restet();                       //  丢弃第一帧的数据（乱的）
}

//设置采集图像的地址
//num 为 TSL1401_MAX 时，设置全部地址，否则设置指定的地址
void tsl1401_set_addrs(TSL1401_e num, uint8 *addr, ... )
{
    ASSERT(num <= TSL1401_MAX);

    va_list ap;                                 //创建栈指针ap
    uint8_t *value;
    uint8_t n = 0;

    if(num < TSL1401_MAX)
    {
        tsl1401_addr[num] = addr;
    }
    else if(num == TSL1401_MAX)
    {
        va_start(ap, addr);                         //获取可变参数列表的第一个参数的地址
        value = addr;
        while(num--)
        {
            tsl1401_addr[n++] = value;
            value = va_arg(ap, uint8_t * );             //获取可变参数的当前参数，返回指定类型并将指针指向下一参数
        }

        va_end(ap);                                 //清空va_list可变参数列表
    }
}




//tsl1401,线性CCD采集图像
void tsl1401_get_img(void)
{
    tsl1401_flag = tsl1401_start;           //启动采集
    //while(tsl1401_flag != tsl1401_finish);  //等待采集完成
}

//获取 tsl1401,线性CCD 曝光时间
uint32 tsl1401_get_time(void)
{
    return tsl1401_time;
}

//设置 tsl1401,线性CCD 曝光时间
void tsl1401_set_time(uint32 time)
{
    tsl1401_time = time;
    TSL1401_INT_TIME(tsl1401_time);         //  设置中断时间
}

//放入定时中断里，以便于定时复位曝光时间
void tsl1401_time_isr()
{
    //只判断是否开始采集和是否采集完成
    if(tsl1401_flag == tsl1401_start)
    {
        tsl1401_gather();
        tsl1401_flag = tsl1401_finish;
    }
    else
    {
        tsl1401_restet();                   // 复位
    }
}

void tsl1401_delay( uint32 time )
{
    volatile uint32 i = time;

    while(i--);
}


//采集图像
void tsl1401_gather(void)
{
#define TSL1401_TIME   10   //延时时间

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
            (tsl1401_addr[i])[k] = (uint8_t)adc_once(tsl1401_ch[i], ADC_8bit);   //采集某路模拟量的AD值  通道  精确度
            //*img++ =  adc_once(ADC1_AD8, ADC_8bit);
        }

        tsl1401_clk_out(1);        //CLK = 1
        tsl1401_delay(TSL1401_TIME);
        k++;
    }
#undef TSL1401_TIME
}

//用于调整曝光时间
void tsl1401_restet()
{
#define TSL1401_RETIME   1

    uint8 n = TSL1401_SIZE; //n为ADC采集次数

    tsl1401_clk_out(0);             //CLK = 0  //全部CCD的CLK管脚输出0 单片机获取AD值
    tsl1401_delay(TSL1401_RETIME); //延时
    tsl1401_si_out(1);              //SI  = 1 //全部CCD的SI管脚输出1  开始输出图像 
    tsl1401_delay(TSL1401_RETIME);
    tsl1401_clk_out(1);             //CLK = 1   AO开始输出数据
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
    tsl1401_set_addrs(TSL1401_MAX,(uint8 *)&tsl1401[0],(uint8 *)&tsl1401[1],(uint8 *)&tsl1401[2]);//设置采集图像的地址
    tsl1401_init(1);                         //初始化 线性CCD ，配置 中断时间为 time

}

