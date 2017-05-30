/*
 * @file       Motor.h
 * @brief      电机驱动程序 头文件
 * @author     Jay Lee
 * @date       2016-01-17
 */
#ifndef _Motor_H
#define _Motor_H




#define L_Ftm FTM0         //left  motor FTM
#define R_Ftm FTM0         //left  motor FTM
#define MotorL_CH0 FTM_CH2 //left  motor FTM gallery 
#define MotorL_CH1 FTM_CH3
#define MotorR_CH0 FTM_CH0 //right motor FTM gallery
#define MotorR_CH1 FTM_CH1
#define L_Freq 10*1000     //left  motor pwm frequency
#define R_Freq 10*1000     //right motor pwm frequency






extern void Left_Forward(uint16 duty);       //left  motor forward
extern void Left_Backward(uint16 duty);      //left  motor backward
extern void Right_Forward(uint16 duty);      //right motor forward
extern void Right_Backward(uint16 duty);     //left  motor backward
extern void Left_Speed_Control(int16 duty);  //left  motor control
extern void Right_Speed_Control(int16 duty); //right motor control
extern void Motor_init();                    //motor init
extern void Motor();
extern void Stop();
extern void PID_Change();



#endif