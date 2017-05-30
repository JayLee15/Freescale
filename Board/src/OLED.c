


#include"common.h"
#include"include.h"
#include"OLED.h"


extern uint8 tsl1401[TSL1401_MAX*3][TSL1401_SIZE];                                                                       
extern uint8 L_tsl1401[3][TSL1401_SIZE];
extern uint8 O_tsl1401[3][TSL1401_SIZE];


unsigned char str[10];


//储存字库数据
const unsigned char F6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp 0
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *  10
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4  20
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >  30
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @  
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A  33
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G   
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H  40
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q   
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R  50
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [   
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55 60
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a  65
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f  70
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p  80
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z  90
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

/*************************************************************************
*  函数名称：delay_ms
*  功能说明：延时函数
*  参数说明：ms
*  函数返回：无
*  修改时间：
*  备    注：
*************************************************************************/

void delay_ms(uint8 ms)      
{   
   int ii,jj;
   if (ms<1) ms=1;          
   for(ii=0;ii<ms;ii++)     
     for(jj=0;jj<1335;jj++);
}
/*************************************************************************
*  函数名称：OLED_WrCmd(unsigned char cmd)
*  功能说明：模拟SPI总线写入命令
*  参数说明：cmd
*  函数返回：无
*  修改时间：
*  备    注：
*************************************************************************/
void OLED_WrCmd(unsigned char cmd)
{
  unsigned char i;
  OLED_DC=0;
  for(i=0;i<8;i++) //发送一个八位数据 
  {
    if((cmd << i) & 0x80)
       {
          OLED_SDA  = 1;
       }
    else  
          OLED_SDA  = 0;
    
    OLED_SCL = 0;
    OLED_SCL = 1;
  }
}




/*************************************************************************
*  函数名称：OLED_WrDat(unsigned char dat)
*  功能说明：模拟SPI总线写入数据
*  参数说明：dat
*  函数返回：无
*  修改时间：
*  备    注：
*************************************************************************/



void OLED_WrDat(unsigned char dat)	 
{
  unsigned char i;
  OLED_DC=1;  
  for(i=0;i<8;i++) //发送一个八位数据 
  {
    if((dat << i) & 0x80)
       {
          OLED_SDA  = 1;
       }
    else  
          OLED_SDA  = 0;
    
    OLED_SCL = 0;
    OLED_SCL = 1;
  }
}



/*************************************************************************
*  函数名称：OLED_Set_Pos（）
*  功能说明：设置OLED显示的位置
*  参数说明：x,y
*  函数返回：无
*  修改时间：
*  备    注：
*************************************************************************/

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
  OLED_WrCmd(0xb0+y);
  OLED_WrCmd(((x&0xf0)>>4)|0x10);
  OLED_WrCmd((x&0x0f)|0x01); 
} 


/*************************************************************************
*  函数名称：OLED_Fill
*  功能说明：初始化全屏
*  参数说明：bmp_dat
*  函数返回：无
*  修改时间：
*  备    注：
*************************************************************************/

void OLED_Fill(unsigned char bmp_dat) 
{
  unsigned char y,x;
  for(y=0;y<8;y++)
  {
    OLED_WrCmd(0xb0+y);
    OLED_WrCmd(0x01);
    OLED_WrCmd(0x10);
    for(x=0;x<128;x++)
        OLED_WrDat(bmp_dat);
  }
}

/*************************************************************************
*  函数名称：OLED_clear()
*  功能说明：初始化清屏 
*  参数说明：无
*  函数返回：无
*  修改时间：
*  备    注：
*************************************************************************/

void OLED_clear(void)
{
    unsigned char y,x;	
    for(y=0;y<8;y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10); 
        for(x=0;x<128;x++)
            OLED_WrDat(0);
    }			
}



/*************************************************************************
*  函数名称：OLED_init（）；
*  功能说明：OLED初始化
*  参数说明：无
*  函数返回：无
*  修改时间：
*  备    注：
*************************************************************************/

void OLED_init(void)
{
    gpio_init(PTB20,GPO,0);
    gpio_init(PTB21,GPO,0);
    gpio_init(PTB22,GPO,0);
    gpio_init(PTB23,GPO,0);
    gpio_init(PTC0,GPO,0);
    OLED_SCL=1;
    OLED_RST=0;
    delay_ms(50);
    OLED_RST=1;       //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕   
    OLED_WrCmd(0xae);
    OLED_WrCmd(0x00);
    OLED_WrCmd(0x10);
    OLED_WrCmd(0x40);
    OLED_WrCmd(0x81);
    OLED_WrCmd(0xcf); 
    OLED_WrCmd(0xa1);
    OLED_WrCmd(0xc8);
    OLED_WrCmd(0xa6);
    OLED_WrCmd(0xa8);
    OLED_WrCmd(0x3f);
    OLED_WrCmd(0xd3);
    OLED_WrCmd(0x00);
    OLED_WrCmd(0xd5);
    OLED_WrCmd(0x80);
    OLED_WrCmd(0xd9);
    OLED_WrCmd(0xf1);
    OLED_WrCmd(0xda);
    OLED_WrCmd(0x12);
    OLED_WrCmd(0xdb);
    OLED_WrCmd(0x40);
    OLED_WrCmd(0x20);
    OLED_WrCmd(0x02);
    OLED_WrCmd(0x8d);
    OLED_WrCmd(0x14);
    OLED_WrCmd(0xa4);
    OLED_WrCmd(0xa6);
    OLED_WrCmd(0xaf);
    OLED_Fill(0x00);  
    OLED_Set_Pos(0,0);
}


/*************************************************************************
*  函数名称：OLED_P6x8Str（）
*  功能说明：显示6*8的字符数据
*  参数说明：x,y,ch[]
*  函数返回：无
*  修改时间：
*  备    注：
*************************************************************************/

void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;      
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>126){x=0;y++;}
        OLED_Set_Pos(x,y);    
    for(i=0;i<6;i++) 
    {
      OLED_WrDat(F6x8[c][i]);
      delay_ms(60);
    }
    x+=6;
    j++;
  }
}


/*************************************************************************
*  函数名称：OLED_P14*16（）
*  功能说明：显示14*16的汉字
*  参数说明：x,y,ch[]
*  函数返回：无
*  修改时间：
*  备    注：
************************************************************************/

void OLED_P14x16(uint8 x,uint8 y,unsigned char a[])
{
  uint8 adder=0,wm=0;
  OLED_Set_Pos(x , y);
  for(wm = 0;wm < 14;wm++)
 {
   OLED_WrDat(a[adder]);
   adder += 1;
  }
   OLED_Set_Pos(x,y+1);
  for(wm = 0;wm < 14;wm++)
  {
    OLED_WrDat(a[adder]);
    adder += 1;
  }
}

/**=============================================================
*函数名： void LCD_PutPixel(u8 x,u8 y)
*功能描述：绘制一个点（x,y）
*参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
*返回：无
===========================================================**/
void OLED_PutPixel(uint8 x,uint8 y)
{
	uint8 data1;  //data1当前点的数据

         OLED_Set_Pos(x,y);
	data1 = 0x01<<(y%8);
	OLED_WrCmd(0xb0+(y>>3));
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x00);
	OLED_WrDat(data1);
}
 


/**=============================================================
**函数名： int_to_str(int a)
**功能描述：将一个整形数据转换成字符串显示；
**参数：整形a
**返回：字符串首地址
=============================================================**/
void DisplayInt(uint8 x,uint8 y,int a)
{
   int i=0,j,m=1,n=1;
   
   
   while(m!=0)
   {
   n=n*10;
   m=a/n;
   i++;
   }
  ;
   for(j=0;j<i;j++)
   {
    n=n/10;
    str[j]=(char)(a/n+48);
    a=a%n;
   }
   str[j]='\0';
   OLED_P6x8Str(x,y,str);
   
}
/**=============================================================
**函数名： OLED_CAMERA(uint8 *imgaddr,uint32 camera_w,uint32 camera_h)
**功能描述：将图像显示到OLED上
**参数：
**返回：无
=============================================================**/
void OLED_CAMERA(uint8 *imgaddr,uint32 camera_w,uint32 camera_h)
{
    uint8 data,tmp;
    uint32 i,j,maj;
    uint8 n;
    //OLED_clear();
    OLED_Set_Pos(0,0);
    for(j=0;j<camera_h/8+1;j++)
    {
        OLED_Set_Pos(24,j);
        for(i=0;i<camera_w;i++)
        {
            data=0x00;
            for(n=0;n<8;n++)
            {
                maj=(j*8+n)*10+i/8;
                if(maj < camera_w*camera_h/8 )
                {    tmp=(imgaddr[maj] & ( 0x80 >> (i%8)))<<(i%8);}
                else
                {   tmp=(0x00 & ( 0x80 >> (i%8)))<<(i%8) ;}
                data=data | ((tmp >> 7) << n);
            }
            OLED_WrDat(data);
        }
    }
}

void OLED_P6x8Float(uint8 x,uint8 y, float flo)
{
    uint8 c=0,i=0,n;
    uint32 m=1;
    uint8 flag=0;
    uint16 ch=(uint16)flo;
     float fh;
    fh=flo-ch;
    //消除干扰
    OLED_Set_Pos(x,y);
    for(i=0;i<24;i++)
       OLED_WrDat(0x00);
    
    for(uint32 j=5;j>0;j--)  //整数部分
    {
        for(n=1,m=1;n<j;n++)
            m=m*10;
        c =(uint8)(ch/m)+48-32;
	if(flag==0&&c==16&&j!=1)
        {;
        }
        else
        {
	OLED_Set_Pos(x+flag*8,y);
	for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
        ch=ch%m;
        flag++;
        }
    }
    if(flag>=2) return;
    OLED_Set_Pos(x+flag*8,y);
    OLED_WrDat(0x00);
    OLED_WrDat(0x80);
    OLED_WrDat(0x00);
    x=x+3;
    //小数部分
 
        fh=fh*10;
        c=(uint8)fh+48-32;
        OLED_Set_Pos(x+flag*8,y);
	for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
    
}
/**=============================================================
**函数名： OLED_CCD()
**功能描述：将CCD图像显示到OLED上
**参数：
**返回：无
=============================================================**/
void OLED_CCD()                                                                                                     /*OLED 显示CCD图像*/
{
  uint8 i,j,PixelB_1,PixelB_2,Pixel_1,Pixel_2; 
  for(i=0;i<128;i++)                                                                                                //find the min and max
  {
     if((!D_KEY_get(PTA14))&&(!D_KEY_get(PTA15))&&(D_KEY_get(PTA16))&&(!D_KEY_get(PTA17)))
     {
      Pixel_1 = O_tsl1401[0][i];
      OLED_Set_Pos(i,Pixel_1);
      OLED_WrCmd(0xb0+(Pixel_1>>3));
      OLED_WrCmd(((i&0xf0)>>4)|0x10);
      OLED_WrCmd((i&0x0f)|0x00);
      OLED_WrDat(0);
      Pixel_2 = (uint8)(tsl1401[0][i]/5);
      O_tsl1401[0][i] = Pixel_2; 
      OLED_PutPixel(i,Pixel_2);    
     }
     if((!D_KEY_get(PTA14))&&(!D_KEY_get(PTA15))&&(!D_KEY_get(PTA16))&&(D_KEY_get(PTA17)))
     {
      PixelB_1 = O_tsl1401[1][i];
      OLED_Set_Pos(i,PixelB_1);
      OLED_WrCmd(0xb0+(PixelB_1>>3));
      OLED_WrCmd(((i&0xf0)>>4)|0x10);
      OLED_WrCmd((i&0x0f)|0x00);
      OLED_WrDat(0);
      PixelB_2 = (uint8)(tsl1401[1][i]/5);
      O_tsl1401[1][i] = PixelB_2; 
      OLED_PutPixel(i,PixelB_2);
      }
  }
}
