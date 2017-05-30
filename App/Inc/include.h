#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
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
#include  "FIRE_MMA7455.h"      //������ٶ�MMA7455
#include  "FIRE_NRF24L0.h"      //����ģ��NRF24L01+
#include  "FIRE_RTC_count.h"    //RTC ʱ��ת��
#include  "FIRE_camera.h"       //����ͷ��ͷ�ļ�
#include  "FIRE_LCD.h"          //Һ����ͷ�ļ�
#include  "ff.h"                //FatFs
#include  "FIRE_TSL1401.h"      //����CCD
#include  "FIRE_key_event.h"    //������Ϣ����
#include  "FIRE_NRF24L0_MSG.h"  //����ģ����Ϣ����
#include  "OLED.h"
#include  "FIRE_BMP.h"          //BMP
#include  "fire_img2sd.h"       //�洢ͼ��sd��һ���ļ�
#include  "fire_sd_app.h"       //SD��Ӧ�ã���ʾsd����ͼƬ�̼���

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
