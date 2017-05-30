/*
 * @file       CCDA.c
 * @brief      CCD算法程序
 * @author     Jay Lee
 * @date       2016-01-17
 */
#include "common.h"
#include "include.h"


                                                                                                                    //所测得赛道所占约为50个点

uint8 mid = 63,width = 0;
uint8 last = 63;
uint8 width_last = 0;   
extern uint8 B_mid;
extern uint8 tsl1401[TSL1401_MAX*3][TSL1401_SIZE];
extern uint8 L_tsl1401[3][TSL1401_SIZE];
extern char B_flag;
extern int16 Turn;
extern int16 Turn_Last;
extern int16 Steer_Mid;
extern float Gyr_ave;
extern char B_flag;
extern char Check_time_flag;
char A_flag   = 0;
char Bar_flag = 0;
char Bar_flag_last = 0;
char Straight_flag = 0;
char Straight_flag_last = 0;
char Startline_flag = 0;
char Startline_flag_last = 0;
char Barrier = 0;
char Barrier_last = 0;
char BT_flag = 0;
char Line_flag = 0;
extern char B_Barrier;
extern char B_Barrier_last;
uint8 O_cou,O_Ave,O_mid;
int8 R_x=108,L_x=20,diff,R_x_last=108,L_x_last=20;
uint8 Keep = 0;
int L_Change = 0;
int R_Change = 0;
int mid_Change = 0;
int Re = 20;
void CCD_A(void)
{
  uint16 Ave=0,Ave_L=0,Ave_R=0,Sum=0,ave=0,sum=0,cou=0;
  uint16 Max=0,Min=255,Max_L=0,Max_R=0,Min_L=255,Min_R=255,Max_diff=0;
  uint8 L_flag=0,R_flag=0,i,j;
  uint8 x,m;
  int8 D_L,D_R;
  A_flag = 0;
//  uint8 tsl1401_diff[TSL1401_SIZE];
  
  
//  for(i=0;i<127;i++)  
//   {
//     if(tsl1401[0][i]>tsl1401[0][i+1])
//      tsl1401_diff[i]=tsl1401[0][i]-tsl1401[0][i+1];
//     else
//      tsl1401_diff[i]=tsl1401[0][i+1]-tsl1401[0][i];
//   }
    
  
  for(i=0;i<128;i++)                                                                                                //find the min and max
  {

      Sum+=tsl1401[0][i];
    

    
//    if((i>0)&&(i<127))
//    {
//      m = (tsl1401[0][i-1]+tsl1401[0][i+1])/2;
//      if((tsl1401[0][i]-m)>20)
//        tsl1401[0][i] = m;
//    }
    
    
//    if((i<20)||(i>108)) 
//      sum+=tsl1401[0][i];                                                                                           //the sum of the middle image
  
//    if(i<64)
//    {
//     if(tsl1401[0][i]<Min_L)            
//       Min_L = tsl1401[0][i];
//     if(tsl1401[0][i]>Max_L)
//       Max_L = tsl1401[0][i];
//    }
//    else
//    {
//      if(tsl1401[0][i]<Min_R)            
//        Min_R = tsl1401[0][i];
//      if(tsl1401[0][i]>Max_R)
//        Max_R = tsl1401[0][i];
//    }
//    
//    if(Min_L<Min_R)
//      Min = Min_L;
//    else
//      Min = Min_R;
//
//    if(Max_L<Max_R)
//      Max = Max_R;
//    else
//      Max = Max_L;
    if(tsl1401[0][i]<Min)             
      Min = tsl1401[0][i];
    if(tsl1401[0][i]>Max)
      Max =tsl1401[0][i];
//    if(tsl1401_diff[i]>Max_diff)
//      Max_diff =tsl1401_diff[i];
    
    

  }
  
  Ave = (Max+Min)*56/100;                                                                                             //set the threshold
//  Ave_L = (Max_L+Min_L)*60/100;
//  Ave_R = (Max_R+Min_R)*60/100;
  //ave = (sum/56);                                                                                                   //the average of the middle part
  
  if(Ave<tsl1401[0][last])
  {
    for(i=last;i>20;i--)
     if(Ave>tsl1401[0][i])
     {
       L_x=i;
       L_flag=1;
       break;
     }                                                                                                              //find the left boundary
    for(i=last;i<108;i++)
     if(Ave>tsl1401[0][i])
     {
       R_x=i;
       R_flag=1;
       break;
     }                                                                                                              //find the right boundary
    if((L_flag==1)&(R_flag==1))
    {
      mid = (L_x+R_x)/2;                                                                                              //both of boundarys are not miss
    }
    else if((L_flag==1)&(R_flag==0))                                                                                //the right boundary is miss
    {
      R_x = 108;
      mid = (L_x + R_x) / 2;
    }
    else if((L_flag==0)&(R_flag==1))
    {
      L_x = 20;
      mid = (L_x + R_x) / 2;
    }
    else 
    {
      mid = last;
      R_x = R_x_last ;
      L_x = L_x_last ;
    }
  }
  else
  {
    if(last < 64)
    {
      for(i=last;i>20;i--)
        if(Ave<tsl1401[0][i])
         {
           R_x = i;
           R_flag = 1;
           for(x=i;x>20;x--)
             if(Ave>tsl1401[0][x])
             { 
               L_x = x;
               L_flag = 1;
               break;
             }
           break;
         }

 

    }
    else
    {
     for(i=last;i<108;i++)
       if(Ave<tsl1401[0][i])
        {
          L_x = i;
          L_flag = 1;
          for(x=i;x<108;x++)
            if(Ave>tsl1401[0][x])
             {
               R_x = x;
               R_flag = 1;
               break;
              }
          break;
        }
     

    }
    if((L_flag==1)&(R_flag==1)) 
    {
      mid = (L_x+R_x)/2; 
    }
    else if((L_flag==1)&(R_flag==0)) 
    {
      R_x = 108;
      mid = (L_x+R_x)/2;
    }
    else if((L_flag==0)&(R_flag==1)) 
    {
      L_x = 20;
      mid = (L_x+R_x)/2;
    }
    else 
    {
      mid = last;
      R_x = R_x_last ;
      L_x = L_x_last ; 
    }
  }
  

  
  if((R_x-L_x)<0)
   {
     mid = last;
     R_x = R_x_last ;
     L_x = L_x_last ;
   }
  
  
//  if(Max_diff<20)
//  {
//     mid = last;
//  }
  
  cou = 0;                                                                                                          //the dispersion of pixel
  Ave = Sum/128;                                                                                                     //the average all pixel 
  for(j=0;j<128;j++)
  {
    if((j>7)&&(j<120))
    {
    diff = tsl1401[0][j]-Ave;
    if((diff<18)&&(diff>-18))
    {
      cou++;
    }
    }
    L_tsl1401[0][j] = tsl1401[0][j];
  }
  if((Ave>60)&&(width>50))
  {
    A_flag = 1;
  }
  if((cou>70)&&(Ave<70))
  {
   mid = last;
   R_x = R_x_last;
   L_x = L_x_last;
  }
//  if(cou>70)
//  {
//   mid = last;
//   R_x = R_x_last;
//   L_x = L_x_last;
//  }
  
  D_L = abs(L_x-L_x_last);
  D_R = abs(R_x-R_x_last);
  width = R_x-L_x;
  L_Change = abs(L_x-L_x_last);
  R_Change = abs(R_x-R_x_last);
  mid_Change = abs(mid-last);
  
    if((L_Change<5)&&(R_Change<5)&&(mid_Change<5))
  {
    Keep = Keep+1;
    if(Keep>20)
    {
      Keep = 20;
    }
  }
  else
  {
    Keep = 0;
  }
  
  
//  if((((D_L<5)&&(D_R>6))||((D_R<5)&&(D_L>6)))&&(Ave<100)&&(R_x<90)&&(L_x>38))
//  {
//    Bar_flag = 1;  
//  }
//  if((mid>57)&&(mid<69))
//  {
//    Bar_flag = 0;
//  }
//  if((Bar_flag == 1))
//  {
//    Barrier = 1;
//    //OLED_P6x8Str(0,5,"Barrier");
//    led(LED5,LED_ON);
//  }
//  else 
//  {
//     Barrier = 0;
//     led(LED5,LED_OFF);
//  }

  
  if((57 < mid) && (mid < 69)&&(57 < last) && (last < 69) && (Keep>2) )
  {
    Straight_flag = 1;
  }
  else
  {
    Straight_flag = 0;
  }
  if((Straight_flag_last==1)&&(((D_L<5)&&(mid_Change>3)&&(width<35)&&(D_R>4))||((D_R<5)&&(mid_Change>3)&&(width<35)&&(D_L>4)))&&(R_x<90)&&(L_x>38)&&(width_last<50)&&(width_last>42))
  {
    BT_flag = 1;
    Re = 0;
  }
  if((mid>57)&&(mid<69))
  {
    Barrier = 0;
  } 
  if((BT_flag == 1)&&(Re == 2))
  {
    if((width<40)&&(width>25))
    {
      Barrier = 1;
    }
    else if((width<50)&&(width>40))
    {
      Line_flag = 1;
    }
  }

  if((Straight_flag_last==1)&&((Line_flag == 1)||((D_L>3)&&(D_R>3)&&(mid_Change<5)&&(width<35)&&(width_last<50)&&(width_last>42)))&&(Check_time_flag == 1)&&(Turn - Steer_Mid<30)&&(Turn-Steer_Mid>-30))
  {
    Startline_flag = 1;  
    mid = last;
    OLED_P6x8Str(0,0,"Stop line");
    Line_flag = 0;
  }
  
//  if( Startline_flag == 1)
//  {
//    mid = 63;
//  }
  O_cou = width;
  O_Ave = Ave;
  O_mid = mid;
//  mid = (L_x+R_x)/2;
  tsl1401[0][mid] = 0; 
  tsl1401[0][L_x] = 0;
  tsl1401[0][R_x] = 0;
  Straight_flag_last = Straight_flag;
  last=mid;
  R_x_last = R_x;
  L_x_last = L_x;
  width_last = width;
  Startline_flag_last = Startline_flag;
  Barrier_last = Barrier;
  Re = Re+1;
  if(Re > 20)
    Re = 20;
  //uart_putchar (FIRE_PORT,Ave);

}