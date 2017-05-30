/*
 * @file       Steering.c
 * @brief      舵机控制程序
 * @author     Jay Lee
 * @date       2016-01-20
 */

//黑线( 接地) , 红线( 电源线) 和白色(控制线) 
#include "common.h"
#include "include.h"
   
float Steering_P_0 = 0;
float Steering_D_0 = 0;
float Steering_P_1 = 0;
float Steering_D_1 = 0;
extern int SpeedFlag;
extern char Bar_flag;
extern char Startline_flag;
extern float Steer_Para[6][2];
extern int16 Steer_Mid;
extern int16 Steer_Left;
extern int16 Steer_Right;
extern uint8 Keep;
extern char Barrier;
char Stop_flag  = 1;
char Start_flag = 0;
char Back_flag  = 0;
char Left_flag  = 0;
char Right_flag = 0;

char P0[9]={0},D0[9]={0},P1[9]={0},D1[9]={0};
extern char Control_flag;
extern char PD_flag;

float Error=0,Error_Last=0,Error_LastLast=0;
int16 Turn=0,Turn_Last=0;
int16 Mid_Last=63,Mid_LastLast=63;

void Steering_init()
{
  FTM_PWM_init(Steering_FTM, Steering_CH,Steering_HZ,Steer_Mid);      
}

void Steering_Control(uint16 Steering_duty)
{
  FTM_PWM_Duty(Steering_FTM, Steering_CH,Steering_duty);      
}


void Steering(uint8 mid)            
{
    if(SpeedFlag==0)                
    {
      Steering_P_1 = Steer_Para[0][0];
      Steering_D_1 = Steer_Para[0][1];
    }
    else if(SpeedFlag==1)                
    {
      Steering_P_1 = Steer_Para[0][0];
      Steering_D_1 = Steer_Para[0][1];
    }
    else if(SpeedFlag==2)                
    {
      Steering_P_1 = Steer_Para[0][0];
      Steering_D_1 = Steer_Para[0][1];
    }
    else if(SpeedFlag==3)                
    {
      Steering_P_1 = Steer_Para[0][0];
      Steering_D_1 = Steer_Para[0][1];
    }
    else if(SpeedFlag==4)                
    {
      Steering_P_1 = Steer_Para[0][0];
      Steering_D_1 = Steer_Para[0][1];
    }
    else if(SpeedFlag==5)                
    {
      Steering_P_1 = Steer_Para[0][0];
      Steering_D_1 = Steer_Para[0][1];
    } 
  Error=63-mid;
  
//  if(fabsf(Error)<8)
//   Turn=Turn_Last+(int)(Steering_P_0*(Error-Error_Last)+Steering_D_0*(Error-2*Error_Last+Error_LastLast));
//  else 
//   Turn=Turn_Last+(int)(Steering_P_1*(Error-Error_Last)+Steering_D_1*(Error-2*Error_Last+Error_LastLast));
//    if(fabsf(Error)<22&&fabsf(Error_Last)<22)
//   Turn=(int16)(705+Steering_P_0*Error+Steering_D_0*(Error-Error_Last));
//  else 
//   Turn=(int16)(705+Steering_P_1*Error+Steering_D_1*(Error-Error_Last));
//  if((Error > 0)&&(Error < 18))
//    Turn=(int16)(Steer_Mid+Steering_P_0*Error*Error+Steering_D_0*Error*(Error-Error_Last));
//  else if((Error < 1)&&(Error > -18))
//    Turn=(int16)(Steer_Mid-Steering_P_0*Error*Error-Steering_D_0*Error*(Error-Error_Last)); 
//  else if(Error > 17)
//    Turn=(int16)(Steer_Mid+Steering_P_1*Error*Error+Steering_D_1*Error*(Error-Error_Last));
//  else if(Error < -17)
//    Turn=(int16)(Steer_Mid-Steering_P_1*Error*Error-Steering_D_1*Error*(Error-Error_Last)); 
  //if((Bar_flag == 1)&&(Turn_Last-Turn<8)&&(Turn_Last-Turn>-8)&&(Startline_flag == 0))
  if(Barrier == 1)
  {
    Error = 2.2*Error;
    //led(LED1,LED_ON);
  }
  if(Error > 0)
     Turn=(int16)(Steer_Mid+Steering_P_1*Error*Error+Steering_D_1*Error*(Error-Error_Last));
  else 
     Turn=(int16)(Steer_Mid-Steering_P_1*Error*Error-Steering_D_1*Error*(Error-Error_Last)); 

//  if(Startline_flag == 1)
//  {
//    Turn = Turn_Last;
//  }
  if((Turn_Last-Turn>80)||(Turn_Last-Turn<-80))
    Turn = (int16)((Turn_Last+Turn)/2);
  if(Turn>Steer_Left)
    Turn = Steer_Left;
  if(Turn<Steer_Right)
    Turn = Steer_Right;
  if(Left_flag == 1)
  {
    Turn = Steer_Left;
  }
  if(Right_flag == 1)
  { 
    Turn = Steer_Right;
  }
  
  Steering_Control(Turn);
  
  Turn_Last=Turn;
  Mid_LastLast=Mid_Last;  
  Mid_Last=mid;
  Error_LastLast=Error_Last;
  Error_Last=Error;
}



void UART_Control()
{
   if(Control_flag!=0)
   {
     uart_rx_irq_dis(FIRE_PORT);
     switch(Control_flag)
      {
         case 1:
          Start_flag = 1;
          Back_flag  = 0;
          Stop_flag  = 0;
         break;
         case 2:
          Back_flag  = 1;
          Stop_flag  = 0;
          Start_flag  = 0;
         break;
         case 3:
          Left_flag  = 1;
         break;
         case 4:
          Right_flag = 1;
         break;
         case 5:
          Stop_flag  = 1;
          Start_flag = 0;
          Back_flag  = 0;
          Left_flag  = 0;
          Right_flag = 0;
         break;
      }
     Control_flag = 0;
     uart_rx_irq_en(FIRE_PORT);
   }
}
void PD_change()
{
   uint8 i=0;
   if(PD_flag!=0)
   {
     uart_rx_irq_dis(FIRE_PORT);
     switch(PD_flag == 1)
      {
         case 1:
          uart_getchar(FIRE_PORT,&P0[i]);
          while(P0[i++]!='O')
          uart_getchar(FIRE_PORT,&P0[i]);
          P0[i-1]='\0';
          i=0;
         break;
         
         case 2:
          uart_getchar(FIRE_PORT,&D0[i]);
          while(D0[i++]!='O')
          uart_getchar(FIRE_PORT,&D0[i]);
          D0[i-1]='\0';
          i=0;
         break;
         
         case 3:
          uart_getchar(FIRE_PORT,&P1[i]);
          while(P1[i++]!='O')
          uart_getchar(FIRE_PORT,&P1[i]);
          P1[i-1]='\0';
          i=0;
         break;
         
         case 4:
          uart_getchar(FIRE_PORT,&D1[i]);
          while(D1[i++]!='O')
          uart_getchar(FIRE_PORT,&D1[i]);
          D1[i-1]='\0';
          i=0;
         break;
      }
     
     if(PD_flag==1)
       Steering_P_0=100*(P0[0]-'0')+10*(P0[1]-'0')+(P0[2]-'0')+0.1*(float)(P0[4]-'0')+0.01*(float)(P0[5]-'0')+0.001*(float)(P0[6]-'0')+0.0001*(float)(P0[7]-'0');
     if(PD_flag==2)
       Steering_D_0=100*(D0[0]-'0')+10*(D0[1]-'0')+(D0[2]-'0')+0.1*(float)(D0[4]-'0')+0.01*(float)(D0[5]-'0')+0.001*(float)(D0[6]-'0')+0.0001*(float)(D0[7]-'0');
     if(PD_flag==3)
       Steering_P_1=100*(P1[0]-'0')+10*(P1[1]-'0')+(P1[2]-'0')+0.1*(float)(P1[4]-'0')+0.01*(float)(P1[5]-'0')+0.001*(float)(P1[6]-'0')+0.0001*(float)(P1[7]-'0');
     if(PD_flag==4)
       Steering_D_1=100*(D1[0]-'0')+10*(D1[1]-'0')+(D1[2]-'0')+0.1*(float)(D1[4]-'0')+0.01*(float)(D1[5]-'0')+0.001*(float)(D1[6]-'0')+0.0001*(float)(D1[7]-'0');
    
     PD_flag = 0;
     uart_rx_irq_en(FIRE_PORT);
   }
}  