#include "common.h"
#include "include.h"
//此处请添加相关的头文件



//#define IMU_SDA PTA12//SDA IO
//#define IMU_SCL PTA13//SCL IO

#define IMU_SDA PTE1//SDA IO
#define IMU_SCL PTE6//SCL IO

//#define SDAIN   PTXn_T(IMU_SDA,IN)
//#define SDA1    PTXn_T(IMU_SDA,OUT) = 1;
//#define SCL1    PTXn_T(IMU_SCL,OUT) = 1;
//#define DIR1    PTXn_T(IMU_SDA,DDR) = 1;
//#define SDA0    PTXn_T(IMU_SDA,OUT) = 0;
//#define SCL0    PTXn_T(IMU_SCL,OUT) = 0;
//#define DIR0    PTXn_T(IMU_SDA,DDR) = 0;

#define SDAIN   PTE1_IN
#define SDA1    PTE1_OUT = 1;
#define SCL1    PTE6_OUT = 1;
#define DIR1    PTE1_DDR = 1; //DDR=Define direction = 1: Output
#define SDA0    PTE1_OUT = 0;
#define SCL0    PTE6_OUT = 0;
#define DIR0    PTE1_DDR = 0; //DDR=Define direction = 0: Input
 
//SA0必须接地

//内部数据定义
uint8 IIC_ad_main; //器件从地址	    
uint8 IIC_ad_sub;  //器件子地址	   
uint8 *IIC_buf;    //发送|接收数据缓冲区	    
uint8 IIC_num;     //发送|接收数据个数	     

#define ack 1      //主应答
#define no_ack 0   //从应答	 

//************************************************
//循环空操作——软件延时
//************************************************
void nops()		 
{
  uint8 k=11;  //FXAS:3	 
    while(--k);
}

//************************************************
//IIC初始化——引脚开漏输出，并置高电平
//************************************************
void IMU_IIC_Init()
{
    port_init_NoALT(IMU_SCL,ODO); //PORT初始化：OutDrain Output
    port_init_NoALT(IMU_SDA,ODO); //PORT初始化：OutDrain Output  
  
    gpio_init  (IMU_SCL, GPO, 1); //初始化SCL：输出高电平
    gpio_init  (IMU_SDA, GPO, 1); //初始化SDA：输出高电平
}

//************************************************
//IIC送起始位，当CLK为高电平时，SDA出现下降沿
//************************************************
void IIC_start(void)
{
	//SCL0;	
	//nops();
	SDA1;        //SCL低电平时，允许SDA数据做改变。前两条语句确定SDA处于高电平状态
	//nops();
	SCL1;        //SCl拉高，进行状态检测
	nops();	
	//nops();
	SDA0;        //SDA出现下降沿，满足启动条件
	nops();	
	//nops();
	SCL0;        //SCL拉低
}

//************************************************
//IIC送停止位，当CLK为高电平时，SDA出现上升沿
//************************************************
void IIC_stop(void)
{
	SCL0;
	//nops();
	SDA0;         //Ensure the LOW Level in SDA
	nops();
	SCL1;
	//nops();	
	//nops();
	SDA1;         //SDA出现上升沿，满足停止条件
	//nops();	
	//nops();
	//SCL0;
}

//************************************************
//主应答(包含ack:SDA=0和no_ack:SDA=1)
//************************************************
void IIC_ack_main(uint8 ack_main)
{
	SCL0;	
	nops();
	if(ack_main)
	{
	  SDA0; //ack主应答
	}
	else 
	{
	  SDA1; //no_ack无需应答
	}
	//nops();	
	nops();
	SCL1;
	//nops();	
	nops();
	SCL0;
}

//*************************************************
//字节发送程序 ：高位先发送——Bing-hong Wu
//发送c(可以是Data or Access)，送完后接收从应答
//【不考虑从应答位】  【可以修改为考虑】
//*************************************************
void send_byte(uint8 c)
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		SCL0;	
		nops();
		if((c<<i) & 0x80)
		{
		  SDA1; //判断发送位：发 1
		}
		else 
		{
		  SDA0; //判断发送位：发 0
		}
		nops();
		SCL1;
		nops();	
		//nops();
		SCL0;
	}
	nops();
	SDA1;  //发送完8bit，释放总线[SDA拉高]准备接收应答位
	nops();
	SCL1;  //[SCL拉高]
	//nops(); 	
	nops();//sda上数据即是从应答位              
	SCL0;  //不考虑从应答位|但要控制好时序
}

//**************************************************
//字节接收程序
//接收器件传来的数据，此程序应配合|主应答函数|IIC_ack_main()使用
//return: uchar型1字节
//**************************************************
uint8 read_byte(void)
{
    uint8 i;
    uint8 c;
	c=0;
	SCL0;
	nops();
	SDA1; //置数据线为输入方式
	nops();
	DIR0;
	for(i=0;i<8;i++)
	{
		nops();
		SCL0;    //置时钟线为低，准备接收数据位
		nops();
		SCL1;    //置时钟线为高，使数据线上数据有效
		nops();
		c<<=1;
		if(SDAIN)
		  c+=1;  //循环读数据位，将接收的数据存c
	}
	SCL0;	
	nops();
	DIR1;
	return c;
}

//***************************************************
//向无子地址器件发送单字节数据：ST+Device Access+Data+SP
//***************************************************
void send_to_byte(uint8 ad_main,uint8 c)
{
	IIC_start();
	send_byte(ad_main); //发送器件地址
	send_byte(c);       //发送数据c
	IIC_stop();
}

//***************************************************
//向有子地址器件发送多字节数据：ST+Device Access+Access+Data+SP
//***************************************************
void send_to_nbyte(uint8 ad_main,uint8 ad_sub,uint8 *buf,uint8 num)
{
	uint8 i;
	IIC_start();
	send_byte(ad_main); //发送器件地址
	send_byte(ad_sub); //发送器件子地址
	for(i=0;i<num;i++)
	{
		send_byte(*buf); //发送数据*buf
		buf++;
	}
	IIC_stop();
}

//***************************************************
//从无子地址器件读单字节数据： ST+Device Access+Read+SP
//function:器件地址，所读数据存在接收缓冲区当前字节
void read_from_byte(uint8 ad_main,uint8 *buf)
{
	IIC_start();
	send_byte(ad_main); //发送器件地址
	*buf=read_byte();
	IIC_ack_main(no_ack); //无需应答<no_ack=0>
	IIC_stop();
}

//***************************************************
//从有子地址器件读多个字节数据：ST+Device Access+Access+Read+SP
//***************************************************
void read_from_nbyte(uint8 ad_main,uint8 ad_sub,uint8 *buf,uint8 num)
{
	uint8 i;
	IIC_start();
	send_byte(ad_main);
	send_byte(ad_sub);
	for(i=0;i<num-1;i++)
	{
		*buf=read_byte();
		IIC_ack_main(ack); //主应答<ack=1>
		buf++;
	}
	*buf=read_byte();
	buf++; //本次指针调整无意义，目的是操作后buf指向下一地址
	IIC_ack_main(no_ack); //无需应答<no_ack=0>
	IIC_stop();
}