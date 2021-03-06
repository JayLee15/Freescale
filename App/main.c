

#include "common.h"
#include "include.h"

#define B 0.14  //左右车轮间距
#define L 0.2   //前后车轮间距

#define pi 3.14159265358979324626

void Menu(void);


uint8 tsl1401[TSL1401_MAX*3][TSL1401_SIZE];                                                                         // 定义存储接收CCD图像的数组 3个CCD 每个三组数据   TSL1401_SIZE //定义CCD每次采集的次数
uint8 L_tsl1401[3][TSL1401_SIZE] = {0};
uint8 O_tsl1401[3][TSL1401_SIZE] = {0};

extern int16 L_count;
extern int16 R_count;
extern uint8 mid;
extern uint8 B_mid;
extern int16 Turn;
extern int16 Gyro_X;

int16 Gyr_hill;
int16 Encoder_max;
int Check_time     = 33;
int End_time       = 100;
char Check_flag    = 1;
char Check_barrier = 1;


extern char A_flag;
extern char B_flag;
extern char Bar_flag;

char S_flag;
int OLEDFlag = 0;
int SpeedFlag = 0;
extern char Gyroscope_flag;
extern uint8 O_cou,O_Ave,O_mid;
extern int32 OutputVaule_L;
extern int32 OutputVaule_R;
extern int16 Error_L;
extern int16 Error_R;
int16 MotorSet_L;
int16 MotorSet_R;
#define B 0.14  //左右车轮间距
#define L 0.2   //前后车轮间距

uint32 Time;
uint16 AD;
float Voltage;
float D_P = 0;
int i;
int32 Gyr_sum;
float Gyr_ave;

extern char Encoder_flag;
extern char CCD_Get_flag;
extern char UART_flag;
extern char Begin_flag;


float Steer_Para[6][2] = 
{ 
  0.15, 1.5,
  0.17 , 1.7 ,
  0.185, 1.85 ,
  0.19, 1.9 ,
  0.15 ,1.5 ,
  0.16 , 1.5
};

int16 Steer_Mid   = 698;//698;
int16 Steer_Left  = 784;
int16 Steer_Right = 612 ;
int16 Steer_Turn; 

int32 var[4];


void  main(void)
{  
//    key_init(KEY_D);
//    while(key_check(KEY_D) ==  KEY_DOWN);
    DisableInterrupts;
//   pit_time_start(PIT1);    
//  Time = pit_time_get_us(PIT1);
//  pit_time_close  (PIT1);
//  printf("Time:%d us\n",Time); 
//    MotorSet_L = 90;
//    MotorSet_R = 90;
//    int L_duty = -400;
//    int R_duty = -400;
    float Tan[178]={0.3500,0.3438,0.3378,0.3318,0.3260,0.3202,0.3146,0.3090,0.3035,0.2981,0.2928,0.2876,0.2824,0.2773,0.2723,0.2673,0.2624,0.2576,0.2529,0.2482,0.2435,0.2390,0.2344,0.2300,0.2255,0.2212,0.2168,0.2126,0.2083,0.2041,0.2000,0.1959,0.1918,0.1878,0.1838,0.1799,0.1760,0.1721,0.1683,0.1645,0.1607,0.1569,0.1532,0.1495,0.1459,0.1423,0.1386,0.1351,0.1315,0.1280,0.1245,0.1210,0.1175,0.1141,0.1106,0.1072,0.1038,0.1005,0.0971,0.0938,0.0905,0.0872,0.0839,0.0806,0.0773,0.0741,0.0708,0.0676,0.0644,0.0612,0.0580,0.0548,0.0516,0.0485,0.0453,0.0421,0.0390,0.0358,0.0327,0.0296,0.0265,0.0233,0.0202,0.0171,0.0140,0.0109,0.0078,0.0047,0.0016,-0.0016,-0.0047,-0.0078,-0.0109,-0.0140,-0.0171,-0.0202,-0.0233,-0.0265,-0.0296,-0.0327,-0.0358,-0.0390,-0.0421,-0.0453,-0.0485,-0.0516,-0.0548,-0.0580,-0.0612,-0.0644,-0.0676,-0.0708,-0.0741,-0.0773,-0.0806,-0.0839,-0.0872,-0.0905,-0.0938,-0.0971,-0.1005,-0.1038,-0.1072,-0.1106,-0.1141,-0.1175,-0.1210,-0.1245,-0.1280,-0.1315,-0.1351,-0.1386,-0.1423,-0.1459,-0.1495,-0.1532,-0.1569,-0.1607,-0.1645,-0.1683,-0.1721,-0.1760,-0.1799,-0.1838,-0.1878,-0.1918,-0.1959,-0.2000,-0.2041,-0.2083,-0.2126,-0.2168,-0.2212,-0.2255,-0.2300,-0.2344,-0.2390,-0.2435,-0.2482,-0.2529,-0.2576,-0.2624,-0.2673,-0.2723,-0.2773,-0.2824,-0.2876,-0.2928,-0.2981,-0.3035,-0.3090,-0.3146,-0.3202,-0.3260,-0.3318,-0.3378,-0.3438,-0.3500};   
    
    
    uart_init (FIRE_PORT, FIRE_BAUD);                                                                               //初始化串口UART3
    led_init(LED0);
    led_init(LED1);
    led_init(LED2);
    led_init(LED3);
    led_init(LED4);
    led_init(LED5);
    
    key_init(KEY_U);
    key_init(KEY_D);
    key_init(KEY_L);
    key_init(KEY_R);
    port_init(PTD7, ALT1 | IRQ_FALLING | PULLUP );
    port_init(PTC6, ALT1 | IRQ_FALLING | PULLUP );
    
//    set_vector_handler(PORTD_VECTORn ,GRAYA_IRQHandler);
//    enable_irq (PORTD_IRQn);
//    set_vector_handler(PORTC_VECTORn ,GRAYB_IRQHandler);
//    enable_irq (PORTC_IRQn);
//    
    
    adc_init(ADC0_SE18);                                                                                            //ADC初始化
    
//    led(LED0,LED_ON);
//    led(LED1,LED_OFF);
//    led(LED2,LED_OFF);
    IMU_IIC_Init();
    delay_ms(100);
    L3G4200_Init();
    delay_ms(100);
    MMA8451_Init();
    delay_ms(100);
    
    EncoderL_init();
    EncoderR_init();
    Steering_init(); 
    D_KEY_init();
    Motor_init();
    OLED_init();
    Menu();                                                                                                                //初始化 线性CCD
    CCD_init();
   
    set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);                                                               //把 PIT0_IRQHandler 函数添加到中断向量表
    enable_irq(PIT0_IRQn);
    
    set_vector_handler(UART3_RX_TX_VECTORn,UART3_IRQHandler);                                                       // 蓝牙串口中断，设置中断服务函数到中断向量表里  
    uart_rx_irq_en(UART3); 
  // Steering_Control(610);//635 835
  
    EnableInterrupts;

                                                                                                                    //配置 中断函数 和 使能中断

      
    while(1)
    {

        
//     Left_Speed_Control(L_duty);
//     Right_Speed_Control(R_duty);
//       
      if (Gyroscope_flag == 1)
      {
        Gyr_sum = 0;
        for(i=0;i<4;i++)
        {
          L3G4200_XYZ();
          Gyr_sum =+ Gyro_X; 
        }
        Gyr_ave = (int32)(Gyr_sum/5); 
        //printf("Gyr:%d \n",Gyr_ave);
        Gyroscope_flag = 0;
      }
     
      if(Encoder_flag == 1)
      {
//      led(LED0,LED_ON);
//      led(LED1,LED_ON);
//      led(LED3,LED_ON);
//    
//        led(LED1,LED_ON);
//        pit_time_start(PIT1);
//        L3G4200_XYZ();                                                                                  //138us   7次取平均值
//     Time = pit_time_get_us(PIT1);     
//     pit_time_close  (PIT1);
//      printf("Time:%d us\n",Time);

        Steer_Turn = Turn - Steer_Mid;
        if(SpeedFlag==0) 
        {       
          D_P = 1.67;
          Encoder_max = 95;
          Gyr_hill = 22;
          if(57 < mid && mid < 69 && 54 < B_mid && B_mid < 72)                                                      //3us
          {
            MotorSet_L = 87;
            MotorSet_R = 87; 
          }
          else if((50> B_mid || B_mid > 76) && 48 < mid && mid < 79)
          { 
            MotorSet_L = 85;
            MotorSet_R = 85;
          }
          else
          {
            MotorSet_L = 83;
            MotorSet_R = 83;
          }           
        }
        else if(SpeedFlag==1) 
        {       
          D_P = 1.7;
          Encoder_max = 100;
          Gyr_hill = 22;
          if(57 < mid && mid < 69 && 54 < B_mid && B_mid < 72)                                                      //3us
          {
            MotorSet_L = 92;
            MotorSet_R = 92; 
          }
          else if((50> B_mid || B_mid > 76) && 48 < mid && mid < 79)
          { 
            MotorSet_L = 90;
            MotorSet_R = 90;
          }
          else
          {
            MotorSet_L = 88;
            MotorSet_R = 88;
          }           
        }
        else if(SpeedFlag==2) 
        {       
          D_P = 1.8;
          Encoder_max = 105;
          Gyr_hill = 22;
          if(57 < mid && mid < 69 && 54 < B_mid && B_mid < 72)                                                      //3us
          {
            MotorSet_L = 97;
            MotorSet_R = 97; 
          }
          else if((50> B_mid || B_mid > 76) && 48 < mid && mid < 79)
          { 
            MotorSet_L = 95;
            MotorSet_R = 95;
          }
          else
          {
            MotorSet_L = 93;
            MotorSet_R = 93;
          }           
        }
        else if(SpeedFlag==3) 
        {       
          D_P = 1.9;
          Encoder_max = 110;
          Gyr_hill = 22;
          if(57 < mid && mid < 69 && 54 < B_mid && B_mid < 72)                                                      //3us
          {
            MotorSet_L = 102;
            MotorSet_R = 102; 
          }
          else if((50> B_mid || B_mid > 76) && 48 < mid && mid < 79)
          { 
            MotorSet_L = 100;
            MotorSet_R = 100;
          }
          else
          {
            MotorSet_L = 98;
            MotorSet_R = 98;
          }           
        }
        else if(SpeedFlag==4) 
        {       
          D_P = 1.4;
          Encoder_max = 85;
          Gyr_hill = 22;
          if(57 < mid && mid < 69 && 54 < B_mid && B_mid < 72)                                                      //3us
          {
            MotorSet_L = 77;
            MotorSet_R = 77; 
          }
          else if((50> B_mid || B_mid > 76) && 48 < mid && mid < 79)
          { 
            MotorSet_L = 75;
            MotorSet_R = 75;
          }
          else
          {
            MotorSet_L = 73;
            MotorSet_R = 73;
          }           
        }
        else if(SpeedFlag==5) 
        {       
          D_P = 1.5;
          Encoder_max = 90;
          Gyr_hill = 22;
          if(57 < mid && mid < 69 && 54 < B_mid && B_mid < 72)                                                      //3us
          {
            MotorSet_L = 82;
            MotorSet_R = 82; 
          }
          else if((50> B_mid || B_mid > 76) && 48 < mid && mid < 79)
          { 
            MotorSet_L = 80;
            MotorSet_R = 80;
          }
          else
          {
            MotorSet_L = 78;
            MotorSet_R = 78;
          }           
        }
             
        if((Gyr_ave > Gyr_hill))
        {
           MotorSet_L = 58;
           MotorSet_R = 58;
        }

        
       if(((Steer_Right-1) < Turn && Turn < (Steer_Mid-10))||(Turn > (Steer_Mid + 10) && Turn < (Steer_Left+1)))
       {
         MotorSet_L = (int16)(MotorSet_L*(1+D_P*Tan[Turn-Steer_Right]));//100 0.74
         MotorSet_R = (int16)(MotorSet_R*(1-D_P*Tan[Turn-Steer_Right]));
       }


        Speed_read();
        if (L_count > Encoder_max)
          L_count = Encoder_max;
        if (R_count > Encoder_max)
          R_count = Encoder_max;
        
        
        
        Motor();                                                                                                    //17us
        var[0] = L_count;
        var[1] = R_count;
        
        //if((D_KEY_get(PTA14))&&(!D_KEY_get(PTA15))&&(!D_KEY_get(PTA16))&&(!D_KEY_get(PTA17)))
        //  Sendware((uint8_t *)var, sizeof(var));
        Encoder_flag = 0;
     }
        
      
     if(CCD_Get_flag == 1)                        
     {
//      led(LED0,LED_OFF);
//      led(LED1,LED_OFF);
//      led(LED2,LED_OFF);
//      led(LED3,LED_OFF);
//    
//      led(LED1,LED_ON);

       tsl1401_get_img();                     //6921us
   //         pit_time_start(PIT1);
       tsl1401_time_isr();                                                                                          //放入定时中断里，以便于定时复位曝光时间
  //    Time = pit_time_get_us(PIT1);     
  //   pit_time_close  (PIT1);
   //   printf("Time:%d us\n",Time); 

        CCD_A();                                                                                                    //34us
        CCD_B();                                                                                                    //43us
        if((A_flag==1)&&(B_flag==0))
        {
          mid = B_mid;
          led(LED0,LED_ON);
        }
        else
        {
        led(LED0,LED_OFF);
        }
        if(A_flag==1)
        {
        led(LED1,LED_ON);
        }
        else
        {
          led(LED1,LED_OFF);
        }
        if(B_flag==1)
        {
        led(LED2,LED_ON);
        }
        else
        {
          led(LED2,LED_OFF);
        }
        if(mid==63)
        {
         led(LED3,LED_ON);
        }
        else
        {
          led(LED3,LED_OFF);
        }
        
        
//        if(S_flag == 1)
//        {
//          Steering(B_mid);                                                                                        //10us
//          led(LED02,LED_ON);
//        }
//        else
//        {
//          Steering(mid);
//          led(LED2,LED_OFF);
//        }
      
         Steering(mid);
      //Steering_Control(Steer_Mid);// 806 716  626 

        CCD_Get_flag = 0;
//        led(LED1,LED_OFF);  
//        if(!D_KEY_get(PTA16))
//          uart_putchar (FIRE_PORT,4);
        //if((D_KEY_get(PTA14))&&(!D_KEY_get(PTA15))&&(D_KEY_get(PTA16))&&(D_KEY_get(PTA17)))
        OLED_CCD();   
        if((!D_KEY_get(PTA14))&&(D_KEY_get(PTA15))&&(!D_KEY_get(PTA16))&&(!D_KEY_get(PTA17))) 
        Send_CCD((uint8 *)&tsl1401[0], 1*TSL1401_SIZE, (uint8 *)&tsl1401[1], 1*TSL1401_SIZE);
        if((!D_KEY_get(PTA14))&&(!D_KEY_get(PTA15))&&(!D_KEY_get(PTA16))&&(!D_KEY_get(PTA17)))
        {
          DisplayInt(0,0,O_cou);
          DisplayInt(30,0,O_Ave);
          DisplayInt(0,5,O_mid);
        }
           
      } 
      
//      if( gpio_get(PTC7) ==  1)
//      {
//        AD = adc_once (ADC0_SE18, ADC_8bit);
//        OLED_P6x8Str(0,0,"AD:");
//        DisplayInt(18,0,AD);
//        Voltage = AD*0.0364;
//        OLED_P6x8Str(0,3,"Voltage:");
//        OLED_P6x8Float(50,3,Voltage);
//        OLED_P6x8Str(70,3,"V");    
//      }
//      
      
           
      if(UART_flag == 1)
      {
        UART_Control();
        PID_Change();
        PD_change();
        UART_flag = 0;
      }
      
      
      
  } 

}
void Menu(void)
{
   OLED_P6x8Str(10,0,"   2016 NXP DHU    ");
   OLED_P6x8Str(0,2,"Potoelectricity Zero");
root:while(1)
    {
        if(key_check(KEY_L)==KEY_UP)
        {
            OLEDFlag++; 
        }
        else if(key_check(KEY_D)==KEY_UP)
        {
            OLEDFlag--;
        }
        else if(key_check(KEY_U)==KEY_UP)
        {
            break;
        }
        if(OLEDFlag>5)
        {
            OLEDFlag=0;
        }
        if(OLEDFlag<0)
        {
            OLEDFlag=5;
        }
        OLED_P6x8Str(22,6,"                 ");
        switch(OLEDFlag)
        {
          case 0: OLED_P6x8Str(22,5,"0.Choose Speed      ");
            break;
            
          case 1: OLED_P6x8Str(22,5,"1.Steer Correct     ");
            break;
            
          case 2: OLED_P6x8Str(22,5,"2.Check Time        ");
            break; 
            
          case 3: OLED_P6x8Str(22,5,"3.Startline ON      ");
            break;
            
          case 4: OLED_P6x8Str(22,5,"4.Barrier ON        ");
            break; 
            
          case 5: OLED_P6x8Str(22,5,"5.End time          ");
            break; 
            
            
        }
        DELAY_MS(150); 
    }
    switch(OLEDFlag)
    {
      case 0:
        DELAY_MS(300); 
        while(1)
        {
            if(key_check(KEY_L)==KEY_UP)
           {
              SpeedFlag++; 
           }
           else if(key_check(KEY_D)==KEY_UP)
           {
              SpeedFlag--;
           }
           else if(key_check(KEY_R)==KEY_UP)
           {
              goto root;
           } 
           else if(key_check(KEY_U)==KEY_UP)
           {
              break;
           } 
           if(SpeedFlag>5)
           {
             SpeedFlag=0;
           }
           if(SpeedFlag<0)
           {
              SpeedFlag=5;
            }
            switch(SpeedFlag)
           {
               case 0: OLED_P6x8Str(22,5," 1.Speed 1      ");
                break;
            
               case 1: OLED_P6x8Str(22,5," 2.Speed 2      ");
                break;
            
               case 2: OLED_P6x8Str(22,5," 3.Speed 3      ");
                break;  
                
               case 3: OLED_P6x8Str(22,5," 4.Speed 4      ");
                break; 
                
               case 4: OLED_P6x8Str(22,5," 5.Speed 5      ");
                break; 
                
               case 5: OLED_P6x8Str(25,5," 6.Speed 6      ");
                break;  
            }
           DELAY_MS(150); 
        }
        break;
      case 1:
        DELAY_MS(300); 
         while(1)
        {
           if(key_check(KEY_L)==KEY_UP)
           {
              Steer_Mid++; 
           }
           else if(key_check(KEY_D)==KEY_UP)
           {
              Steer_Mid--;
           }
           else if(key_check(KEY_R)==KEY_UP)
           {
              goto root;
           } 
           else if(key_check(KEY_U)==KEY_UP)
           {
              break;
           } 
           Steer_Left  = Steer_Mid+86;
           Steer_Right = Steer_Mid-86;
           Steering_Control(Steer_Mid);
           OLED_P6x8Str(25,5," Steer_Mid:      ");
           DisplayInt(35,6,Steer_Mid);
           DELAY_MS(125); 
        }
        break;
      case 2:
        DELAY_MS(300); 
         while(1)
        {
           if(key_check(KEY_L)==KEY_UP)
           {
              Check_time++; 
           }
           else if(key_check(KEY_D)==KEY_UP)
           {
              Check_time--;
           }
           else if(key_check(KEY_R)==KEY_UP)
           {
              goto root;
           } 
           else if(key_check(KEY_U)==KEY_UP)
           {
              break;
           } 
           OLED_P6x8Str(25,5," Check_time:      ");
           DisplayInt(35,6,Check_time);
           DELAY_MS(150); 
        }
        break;
      case 3:
        DELAY_MS(300); 
         while(1)
        {
           if(key_check(KEY_L)==KEY_UP)
           {
              Check_flag = 1; 
           }
           else if(key_check(KEY_D)==KEY_UP)
           {
              Check_flag = 0;
           }
           else if(key_check(KEY_R)==KEY_UP)
           {
              goto root;
           } 
           else if(key_check(KEY_U)==KEY_UP)
           {
              break;
           } 
           if(Check_flag == 1)
           {
              OLED_P6x8Str(25,5," Startline ON      ");
           }
           else
           {
              OLED_P6x8Str(25,5," Startline OFF     ");
           }
           DELAY_MS(125); 
        }
        break;
      case 4:
         DELAY_MS(300); 
         while(1)
        {
           if(key_check(KEY_L)==KEY_UP)
           {
              Check_barrier = 1; 
           }
           else if(key_check(KEY_D)==KEY_UP)
           {
              Check_barrier = 0;
           }
           else if(key_check(KEY_R)==KEY_UP)
           {
              goto root;
           } 
           else if(key_check(KEY_U)==KEY_UP)
           {
              break;
           } 
           if(Check_barrier == 1)
           {
              OLED_P6x8Str(25,5," Barrier ON      ");
           }
           else
           {
              OLED_P6x8Str(25,5," Barrier OFF     ");
           }
           DELAY_MS(150); 
        }
        break;
      case 5:
        DELAY_MS(300); 
         while(1)
        {
           if(key_check(KEY_L)==KEY_UP)
           {
              End_time++; 
           }
           else if(key_check(KEY_D)==KEY_UP)
           {
              End_time--;
           }
           else if(key_check(KEY_R)==KEY_UP)
           {
              goto root;
           } 
           else if(key_check(KEY_U)==KEY_UP)
           {
              break;
           } 
           OLED_P6x8Str(25,5," End_time:      ");
           DisplayInt(35,6,End_time);
           DELAY_MS(150); 
        }
        break;
      
    }

    OLED_P6x8Str(22,5,"  Go!Go!Go!      ");
    OLED_P6x8Str(22,6," To Be NO.1!     ");
    DELAY_MS(250);
    OLED_clear();
}

