#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_rtc.h"      //RTC
#include  "MK60_adc.h"      //ADC
#include  "MK60_dac.h"      //DAC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_can.h"      //CAN
#include  "MK60_sdhc.h"     //SDHC
#include  "MK60_usb.h"      //usb

#include  "FIRE_LED.H"          //LED
#include  "FIRE_KEY.H"          //KEY
#include  "FIRE_MMA7455.h"      //三轴加速度MMA7455
#include  "FIRE_NRF24L0.h"      //无线模块NRF24L01+
#include  "FIRE_RTC_count.h"    //RTC 时间转换
#include  "FIRE_camera.h"       //摄像头总头文件
#include  "FIRE_LCD.h"          //液晶总头文件
#include  "ff.h"                //FatFs
#include  "FIRE_TSL1401.h"      //线性CCD
#include  "FIRE_key_event.h"    //按键消息处理
#include  "FIRE_NRF24L0_MSG.h"  //无线模块消息处理
#include  "OLED.h"
#include  "FIRE_BMP.h"          //BMP
#include  "fire_img2sd.h"       //存储图像到sd卡一个文件
#include  "fire_sd_app.h"       //SD卡应用（显示sd看上图片固件）

#include "MK60_it.h"
#include "Steering.h" 
#include "Motor.h"
#include "Encoder.h"
#include "L3G4200D.h"
#include "IMU_IIC.h"
#include "MMA8451.h"
#include  "CCDA.h"
#include  "CCDB.h"

#endif  //__INCLUDE_H__
