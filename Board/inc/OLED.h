
#ifndef OLED_H_
#define OLED_H_


#include    "common.h"
#include    "include.h"




#define OLED_SCL PTB22_OUT
#define OLED_SDA PTB21_OUT
#define OLED_RST PTB20_OUT
#define OLED_DC  PTB23_OUT
#define OLED_CS  PTC0_OUT





void delay_ms(uint8 ms);
void OLED_WrCmd(uint8 cmd);
void OLED_WrDat(uint8 dat);
void OLED_Set_Pos(uint8 x, uint8 y);
void OLED_Fill(uint8 bmp_dat) ;
void OLED_clear(void);
void OLED_init(void);
void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_P14x16(uint8 x,uint8 y,uint8 a[]);
void OLED_PutPixel(uint8 x,uint8 y);
void DisplayInt(uint8 x,uint8 y,int a);
void OLED_CAMERA(uint8 *imgaddr,uint32 camera_w,uint32 camera_h);
void OLED_P6x8Float(uint8 x,uint8 y, float flo);
void OLED_CCD();


extern unsigned char str[10];






#endif 