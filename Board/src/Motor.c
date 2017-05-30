/* * @file       Motor.c
 * @brief      电机驱动程序
 * @author     Jay Lee
 * @date       2016-01-17
 */

#include "common.h"
#include "include.h"


float Kp = 65.7;//11.05;    //40 2127//电机P比例参数
float Ki = 0.5;//0.05;//4.7;//6.0;//3.5;   //0.6                   //电机I积分参数
float Kd = 8.5;//1.8;//5.5;//3.2;//3.3;    //0.5                  //电机D微分参数
//float Kp = 29.7;//11.05;    //40 2127//电机P比例参数
//float Ki = 1.8;//6.0;//3.5;   //0.6                   //电机I积分参数
//float Kd = 3.1;//3.2;//3.3;    //0.5                  //电机D微分参数
float index;     //变积分系数

extern int32 var[4];
extern char Check_time_flag;
extern char PID_flag;
extern char Stop_flag;
extern char Start_flag;
extern char Back_flag;
extern char Left_flag;
extern char Right_flag;
extern char Lock_flag;
extern char Begin_flag;
extern char End_flag; 
extern char Startline_flag;
extern char Startline_flag_last;
char strp[9]={0};
char stri[9]={0};
char strd[9]={0};

float P_L;
float I_L;
float D_L;
float P_R;
float I_R;
float D_R;


float Error_L;
float LastError_L;
float LastLastError_L;
float Integral_L;
int16 OutputVaule_L;
int16 LastVaule_L;
extern int16 MotorSet_L;

float Error_R;
float LastError_R;
float LastLastError_R;
float Integral_R;
int16 OutputVaule_R;
int16 LastVaule_R;
extern int16 MotorSet_R;

extern int16 L_count;
extern int16 R_count;

void  Left_Forward(uint16 duty)                                                                                     //left  motor forward
{
    FTM_PWM_Duty(L_Ftm, MotorL_CH1,  0);
    FTM_PWM_Duty(L_Ftm, MotorL_CH0,  duty);
}

void Left_Backward(uint16 duty)                                                                                     //left  motor backward
{
    FTM_PWM_Duty(L_Ftm, MotorL_CH1,  duty);
    FTM_PWM_Duty(L_Ftm, MotorL_CH0,  0);
}

void Right_Forward(uint16 duty)                                                                                     //right motor forward
{
  FTM_PWM_Duty(R_Ftm, MotorR_CH1,  0);
  FTM_PWM_Duty(R_Ftm, MotorR_CH0,  duty); 
}
void Right_Backward(uint16 duty)                                                                                    //right motor backward
{
  FTM_PWM_Duty(R_Ftm, MotorR_CH1,  duty);
  FTM_PWM_Duty(R_Ftm, MotorR_CH0,  0); 
}

void Left_Speed_Control(int16 duty)                                                                                 //left  motor control
{
  if(duty>=0)
  {
   Left_Forward(duty);
  }
  else
  {
  duty = -duty;
  Left_Backward(duty);
  }

}

void Right_Speed_Control(int16 duty)                                                                                //right motor control
{
  if(duty>=0)
  {
    Right_Forward(duty);
  }
  else
  {
   duty = -duty;
   Right_Backward(duty);
  }
}

void Motor_init()                                                                                                   // motor init
{
  FTM_PWM_init(L_Ftm, MotorL_CH0,L_Freq,30); 
  FTM_PWM_init(L_Ftm, MotorL_CH1,L_Freq,30);
  FTM_PWM_init(R_Ftm, MotorR_CH0,R_Freq,30); 
  FTM_PWM_init(R_Ftm, MotorR_CH1,R_Freq,30);  
}

void Motor()
{
//  static float PreError_L[20]={0};
//  static float PreError_R[20]={0};
//  int8 i;
  if (End_flag == 1)
  {
    Stop_flag = 1;
  }
  if((Begin_flag == 1)&&(End_flag == 0))
  {
    Stop_flag = 0;
  }


  
  if(Back_flag == 1)
  {
    OutputVaule_L = -200;
    OutputVaule_R = -200; 
  }
  


  if(Startline_flag == 1)
  {
    MotorSet_L = 0;
    MotorSet_R = 0;  
  }
  
  
  
  
  LastLastError_L = LastError_L;
  LastLastError_R = LastError_R;

  LastError_L = Error_L;
  LastError_R = Error_R;

  LastVaule_L = OutputVaule_L;
  LastVaule_R = OutputVaule_R;

  Error_L = MotorSet_L-L_count;
  Error_R = MotorSet_R-R_count;
  
//   Integral_L = 0;
//   for(i=0;i<19;i++)
//   {
//     PreError_L[i]=PreError_L[i+1]; 
//     Integral_L+=PreError_L[i];
//   }
//  PreError_L[19]=Error_L;
//  Integral_L+=PreError_L[19];
//  
//   Integral_R = 0;
//   for(i=0;i<19;i++)
//   {
//     PreError_R[i]=PreError_R[i+1]; 
//     Integral_R+=PreError_R[i];
//   }
//  PreError_R[19]=Error_R;
//  Integral_R+=PreError_R[19];
  
  
  

  
//  if(abs(Error_L)>=25) 
//    index=0;
//  else 
//  {
//    index=1;
//    Integral_L += Error_L;
//  }
//  
//  if(abs(Error_R)>=25)
//    index=0;
//  //else if(abs(Error_R)>10) index=(30-abs(Error_L))/20;
//  else 
//  {
//    index=1;
//    Integral_R += Error_R;
//  }
//  P_L = (int16)(Kp*(Error_L-LastError_L));
//  I_L = (int16)(Ki*Error_L);
//  D_L = (int16)(Kd*(LastLastError_L-2*LastError_L+Error_L));
//  P_R = (int16)(Kp*(Error_R-LastError_R));
//  I_R = (int16)(Ki*Error_R);
//  D_R = (int16)(Kd*(LastLastError_R-2*LastError_R+Error_R));
  
  Integral_L += Error_L;
  Integral_R += Error_R;
  if(Integral_L>30000)                                                                                             //限制输出
  {
    Integral_L = 30000;
  }
  if(Integral_L<-30000)                                                                                             //限制输出
  {
   Integral_L = -30000;
  }
  if(Integral_R>30000)                                                                                             //限制输出
  {
    Integral_R = 30000;
  }
  if(Integral_R<-30000)                                                                                             //限制输出
  {
   Integral_R = -30000;
  }

  if((L_count - MotorSet_L > 2)&&(R_count - MotorSet_R > 2))
  {
    Integral_L = 0;
    Integral_R = 0;
  }
  
  if((Startline_flag == 1))
  {
    Integral_L = 0;
    Integral_R = 0;
  }
 
  P_L = Kp*Error_L;
  I_L = Ki*Integral_L;
  D_L = Kd*(Error_L-LastError_L);
  P_R = Kp*Error_R;
  I_R = Ki*Integral_R;
  D_R = Kd*(Error_R-LastError_R);
  

  
  
  OutputVaule_L=(int16)(P_L+I_L+D_L);
  OutputVaule_R=(int16)(P_R+I_R+D_R);
  
 var[2] = Integral_L;
 var[3] = Integral_R;
//  var[4] = LastError_L;
//  var[5] = LastError_R;
  
//  OutputVaule_L=(int16)(LastVaule_L+
//                     (float)(Kp*(Error_L-LastError_L))+
//                     (float)(Ki*Error_L)+
//                      //  index*Ki*Error_L+
//                     (float)(Kd*(LastLastError_L-2*LastError_L+Error_L)));
//  
//  OutputVaule_R=(int16)(LastVaule_R+
//                      (float)(Kp*(Error_R-LastError_R))+
//                      (float)(Ki*Error_R)+
//                       // index*Ki*Error_R+
//                      (float)(Kd*(LastLastError_R-2*LastError_R+Error_R)));
  
  

  if(OutputVaule_L>970)                                                                                             //限制输出
  {
    OutputVaule_L=970;
  }
  if(OutputVaule_L<-970)
  {
    OutputVaule_L=-970;
  }
  if(OutputVaule_R>970)                                                                                             //限制输出
  {
    OutputVaule_R=970;
  }
  if(OutputVaule_R<-970)
  {
    OutputVaule_R=-970;
  }
  if(((L_count < 3)||(R_count <3))&&(Lock_flag == 1))
  {
      Stop();
  }
  if(Stop_flag == 1) 
  {
    Stop();
  }
  Left_Speed_Control(OutputVaule_L);
  Right_Speed_Control(OutputVaule_R);

}

void Stop()
{
  OutputVaule_L = 0;
  OutputVaule_R = 0;  
}

void PID_Change()
{
  uint8 i=0;
   if(PID_flag!=0)
   {
     uart_rx_irq_dis(FIRE_PORT);
     switch(PID_flag == 1)
      {
         case 1:
          uart_getchar(FIRE_PORT,&strp[i]);
          while(strp[i++]!='O')
          uart_getchar(FIRE_PORT,&strp[i]);
          strp[i-1]='\0';
          i=0;
         break;
         
         case 2:
          uart_getchar(FIRE_PORT,&stri[i]);
          while(stri[i++]!='O')
          uart_getchar(FIRE_PORT,&stri[i]);
          stri[i-1]='\0';
          i=0;
         break;
         
         case 3:
          uart_getchar(FIRE_PORT,&strd[i]);
          while(strd[i++]!='O')
          uart_getchar(FIRE_PORT,&strd[i]);
          strd[i-1]='\0';
          i=0;
         break;
      }
     if(PID_flag==1)
       Kp=100*(strp[0]-'0')+10*(strp[1]-'0')+(strp[2]-'0')+0.1*(float)(strp[4]-'0')+0.01*(float)(strp[5]-'0')+0.001*(float)(strp[6]-'0')+0.0001*(float)(strp[7]-'0');
     if(PID_flag==2)
       Ki=100*(stri[0]-'0')+10*(stri[1]-'0')+(stri[2]-'0')+0.1*(float)(stri[4]-'0')+0.01*(float)(stri[5]-'0')+0.001*(float)(stri[6]-'0')+0.0001*(float)(stri[7]-'0');
     if(PID_flag==3)
       Kd=100*(strd[0]-'0')+10*(strd[1]-'0')+(strd[2]-'0')+0.1*(float)(strd[4]-'0')+0.01*(float)(strd[5]-'0')+0.001*(float)(strd[6]-'0')+0.0001*(float)(strd[7]-'0');
     PID_flag = 0;
     uart_rx_irq_en(FIRE_PORT);
   }
}