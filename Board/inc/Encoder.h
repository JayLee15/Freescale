/*
 * @file       Encoder.h
 * @brief      ±àÂëÆ÷ Í·ÎÄ¼þ
 * @author     Jay Lee
 * @date       2016-01-18
 */



#ifndef _Encoder_H
#define _Encoder_H


#define EncoderL_Ftm FTM1
#define EncoderR_LPT LPT0_ALT1

extern void EncoderL_init(void);   //left encoder init
extern int16 EncoderL_read(void);  //read the left encoder
extern void EncoderR_init(void);   //right encoder init
extern int16 EncoderR_read(void);  //read the right encoder
extern void Speed_read();               //read the speed

#endif