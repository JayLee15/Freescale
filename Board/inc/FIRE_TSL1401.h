#ifndef _TSL1401_H_
#define _TSL1401_H_ 1



#define TSL1401_SIZE    128                         //定义CCD每次采集的次数

#define TSL1401_INT_TIME(time)  pit_init_ms(PIT0,time)     //定义CCD 定时中断的中断时间设置函数


typedef enum
{
    TSL1401_F,      //前
    TSL1401_B,      //后

    TSL1401_MAX,
} TSL1401_e;


typedef enum
{
    tsl1401_noint,      //没初始化
    tsl1401_start,      //开始采集
    //tsl1401_ing,        //采集中
    tsl1401_finish,     //采集结束
} tsl1401_status_e;


extern void tsl1401_init        (uint32 time);                              //tsl1401,线性CCD初始化，time为曝光时间，单位与 TSL1401_INT_TIME 的设置一致，初始化后必须调用tsl1401_set_addrs 来设置存储地址（或者先设置存储图像地址）
extern void tsl1401_set_addrs   (TSL1401_e num, uint8 *addr, ... );         //tsl1401,线性CCD设置采集数据的地址
extern void tsl1401_get_img     (void);                                     //tsl1401,线性CCD采集图像

extern void tsl1401_set_time    (uint32 time);                              //设置 tsl1401,线性CCD 曝光时间
extern uint32 tsl1401_get_time     (void);                                  //获取 tsl1401,线性CCD 曝光时间
extern void CCD_init(void);

extern void tsl1401_led_en      (TSL1401_e num);                            //开启补光灯
extern void tsl1401_led_dis     (TSL1401_e num);                            //关闭补光灯


extern void tsl1401_time_isr    ();                                         //定时曝光中断服务函数

#endif  // _TSL1401_H_