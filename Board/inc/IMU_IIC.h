#ifndef _IMU_IIC_H_
#define _IMU_IIC_H_

#include "common.h"

void IIC_start(void);
void IIC_stop(void);
void IIC_ack_main(uint8 ack_main);
void send_byte(uint8 c);
uint8 read_byte(void);
void send_to_byte(uint8 ad_main,uint8 c);
void send_to_nbyte(uint8 ad_main,uint8 ad_sub,uint8 *buf,uint8 num);
void read_from_byte(uint8 ad_main,uint8 *buf);
void read_from_nbyte(uint8 ad_main,uint8 ad_sub,uint8 *buf,uint8 num);
void IMU_IIC_Init();



#endif