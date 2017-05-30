/*
 * @file       Steering.h
 * @brief      舵机控制程序 头文件
 * @author     Jay Lee
 * @date       2016-01-20
 */
#ifndef _Steering_H
#define _Steering_H



#define Steering_FTM   FTM2
#define Steering_CH    FTM_CH0
#define Steering_HZ    50


extern void Steering_init(); 
extern void Steering_Control(uint16 Steering_duty);
extern void Steering(uint8 mid); 
extern void PD_change();
extern void UART_Control();

#endif