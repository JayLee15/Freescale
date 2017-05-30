#include "common.h"
#include "include.h"


                                                                                                                    //所测得赛道所占约为50个点

uint8 B_mid = 64,B_width = 0;
uint8 B_last = 64;
uint8 B_width_last = 0;  
extern uint8 tsl1401[TSL1401_MAX*3][TSL1401_SIZE];
extern uint8 L_tsl1401[3][TSL1401_SIZE];


char B_flag;
char B_Bar_flag = 0;
char B_Bar_flag_last = 0;
char B_Straight_flag = 0;
char B_Straight_flag_last = 0;
char B_Barrier = 0;
char B_Barrier_last = 0;
int8 B_R_x=120,B_L_x=8,B_R_x_last=120,B_L_x_last=8;
uint8 B_Keep = 0;
int B_mid_Change = 0;


void CCD_B(void)
{
  uint16 B_Ave=0,B_Sum=0,B_Max=0,B_Min=255,B_ave=0,B_sum=0;
  uint8 B_L_flag=0,B_R_flag=0,i;
  uint8 B_R_x,B_L_x,B_cou=0;
  uint8 B_x,j,PixelB_1,PixelB_2;
  int8 B_D_L,B_D_R;
  int8 B_diff;
  B_flag = 0;
  for(i=0;i<128;i++)                                                                                                //find the min and max
  {

      B_Sum+=tsl1401[1][i];
    if((i<28)||(i>99))  
      B_sum+=tsl1401[1][i];                                                                                         //the sum of the middle image
    if(tsl1401[1][i]<B_Min)             
      B_Min = tsl1401[1][i];
    if(tsl1401[1][i]>B_Max)
      B_Max =tsl1401[1][i];
  }
  
  B_Ave = (B_Max+B_Min)*60/100;                                                                                     //set the threshold
  B_ave = (B_sum/56);                                                                                               //the average of the middle part

  if(B_Ave < tsl1401[1][63])
  {
    for(i=63;i>7;i--)
     if(B_Ave > tsl1401[1][i])
     {
       B_L_x = i;
       B_L_flag = 1;
       break;
     }                                                                                                              //find the left boundary
    for(i=64;i<120;i++)
     if(B_Ave>tsl1401[1][i])
     {
       B_R_x = i;
       B_R_flag = 1;
       break;
     }                                                                                                              //find the right boundary
    if((B_L_flag==1)&(B_R_flag==1))
    {
      B_mid = (B_L_x+B_R_x)/2;                                                                                      //both of boundarys are not miss
    }
    else if((B_L_flag==1)&(B_R_flag==0))                                                                            //the right boundary is miss
    {
      B_R_x=120;
      B_mid = (B_L_x + B_R_x) / 2;
    }
    else if((B_L_flag==0)&(B_R_flag==1))
    {
      B_L_x = 8;
      B_mid = (B_L_x + B_R_x) / 2;
    }
    else 
      B_mid = B_last;
  }
  else
  {
    if(B_last<64)
    {
      for(i=63;i>7;i--)
        if(B_Ave<tsl1401[1][i])
        {
          B_R_x = i;
          B_R_flag = 1;
          for(B_x=(int)i/2;B_x>7;B_x--)
          {
            if(B_Ave>tsl1401[1][i])
            {
              B_L_x = B_x;
              B_L_flag = 1;
              break;
            }
          }
          break;
        }
    }
    else
    {
      for(i=64;i<120;i++)
        if(B_Ave<tsl1401[1][i])
        {
          B_L_x=i;
          B_L_flag=1;
          for(B_x=(int)(i+127)/2;B_x<120;B_x++)
          {
            if(B_Ave>tsl1401[1][i])
            {
              B_R_x = B_x;
              B_R_flag = 1;
              break;
            }
          }
          break;
        }
    }
    if((B_L_flag==1)&(B_R_flag==0)) 
    {
      B_mid=(B_L_x+B_R_x)/2; 
    }
    else if((B_L_flag==1)&(B_R_flag==0)) 
    {
      B_R_x=120;
      B_mid = (B_L_x + B_R_x) / 2;
    }
    else if((B_L_flag==0)&(B_R_flag==1)) 
    {
      B_L_x = 8;
      B_mid = (B_L_x + B_R_x) / 2;
    }
    else
       B_mid = B_last;
  }
  
  B_cou = 0;                                                                                                        //the dispersion of pixel
  B_Ave = B_Sum/128;                                                                                                //the average all pixel 

 /* for(i=8;i<120;i++)
  {
    if(abs(tsl1401[0][i]-Ave)<15)
      cou++;
  }
  if((cou>75)&&(CONDI))
    B_mid=B_last;
*/
    B_width = B_R_x-B_L_x; 
  
  for(j=0;j<128;j++)
  {
    if((j>7)&&(j<120))
    {
    B_diff = tsl1401[1][j]-B_Ave;
    if((B_diff<18)&&(B_diff>-18))
    {
      B_cou++;
    }
    }
    L_tsl1401[1][j] = tsl1401[1][j];
  }
  if((B_Ave>50)&&(B_width>45))
  {
    B_flag = 1;
  }
  

  B_D_L = abs(B_L_x-B_L_x_last);
  B_D_R = abs(B_R_x-B_R_x_last);
  B_mid_Change = abs(B_mid-B_last);
  
  if((B_D_L<4)&&(B_D_R<4)&&(B_mid_Change<4))
  {
    B_Keep = B_Keep+1;
    if(B_Keep>20)
    {
      B_Keep = 20;
    }
  }
  else
  {
    B_Keep = 0;
  }
  
  
//  if((((B_D_L<5)&&(B_D_R>6))||((B_D_R<5)&&(B_D_L>6)))&&(B_Ave<100)&&(B_R_x<110)&&(B_L_x>18))
//  {
//    B_Bar_flag = 1;  
//  }
//  if((B_mid>59)&&(B_mid<67))
//  {
//    B_Bar_flag = 0;
//  }
//  if((B_Bar_flag == 1)&&(B_Keep>1))
//  {
//    B_Barrier = 1;
//    //OLED_P6x8Str(0,3,"B_Barrier");
//    led(LED4,LED_ON);
//  }
//  else 
//  {
//     B_Barrier = 0;
//     led(LED4,LED_OFF);
//  }
  if(B_mid == 63)
  {
    led(LED1,LED_ON);
  }
  else
  {
    led(LED1,LED_OFF);
  }
  tsl1401[1][B_mid] = 0; 
  tsl1401[1][B_L_x] = 0;
  tsl1401[1][B_R_x] = 0;
  B_last = B_mid;
  B_R_x_last = B_R_x;
  B_L_x_last = B_L_x;
  B_width_last = B_width;
  B_Barrier_last = B_Barrier;
}