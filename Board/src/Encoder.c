/*
 * @file       Encoder.c
 * @brief      ±àÂëÆ÷³ÌÐò
 * @author     Jay Lee
 * @date       2016-01-18
 */

#include "common.h"
#include "include.h"

int16 L_count;
int16 R_count;
extern char Encoder_flag;

void EncoderL_init()              //left encoder init
{
   FTM_QUAD_Init(EncoderL_Ftm); 
}

void EncoderR_init()             //right encoder init
{
    lptmr_pulse_init(EncoderR_LPT, 0xFFFF, LPT_Falling);   //ALT1 A19 ALT2 C5
    gpio_init (PTB16, GPI,0);
    //port_init_NoALT(EncoderR_LPT,PULLUP);       
}

int16 EncoderL_read()            //read the left encoder
{
    int16 countL; 
    countL = FTM_QUAD_get(EncoderL_Ftm);         
    FTM_QUAD_clean(EncoderL_Ftm);
    return countL;
}
int16 EncoderR_read()           //read the right encoder
{
    int16 countR;
    uint8 countR_sign;
    countR_sign = gpio_get(PTB16);
    countR =  lptmr_pulse_get();
    if(!countR_sign)
      countR = -countR;
    lptmr_pulse_clean();
    return countR;
}
void Speed_read()               //read the speed
{
   L_count = -EncoderR_read();
   R_count = -EncoderL_read();
  // printf("L:%d  R:%d\n",L_count,R_count);
   //Encoder_flag = 0;
}