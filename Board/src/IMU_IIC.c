#include "common.h"
#include "include.h"
//�˴��������ص�ͷ�ļ�



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
 
//SA0����ӵ�

//�ڲ����ݶ���
uint8 IIC_ad_main; //�����ӵ�ַ	    
uint8 IIC_ad_sub;  //�����ӵ�ַ	   
uint8 *IIC_buf;    //����|�������ݻ�����	    
uint8 IIC_num;     //����|�������ݸ���	     

#define ack 1      //��Ӧ��
#define no_ack 0   //��Ӧ��	 

//************************************************
//ѭ���ղ������������ʱ
//************************************************
void nops()		 
{
  uint8 k=11;  //FXAS:3	 
    while(--k);
}

//************************************************
//IIC��ʼ���������ſ�©��������øߵ�ƽ
//************************************************
void IMU_IIC_Init()
{
    port_init_NoALT(IMU_SCL,ODO); //PORT��ʼ����OutDrain Output
    port_init_NoALT(IMU_SDA,ODO); //PORT��ʼ����OutDrain Output  
  
    gpio_init  (IMU_SCL, GPO, 1); //��ʼ��SCL������ߵ�ƽ
    gpio_init  (IMU_SDA, GPO, 1); //��ʼ��SDA������ߵ�ƽ
}

//************************************************
//IIC����ʼλ����CLKΪ�ߵ�ƽʱ��SDA�����½���
//************************************************
void IIC_start(void)
{
	//SCL0;	
	//nops();
	SDA1;        //SCL�͵�ƽʱ������SDA�������ı䡣ǰ�������ȷ��SDA���ڸߵ�ƽ״̬
	//nops();
	SCL1;        //SCl���ߣ�����״̬���
	nops();	
	//nops();
	SDA0;        //SDA�����½��أ�������������
	nops();	
	//nops();
	SCL0;        //SCL����
}

//************************************************
//IIC��ֹͣλ����CLKΪ�ߵ�ƽʱ��SDA����������
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
	SDA1;         //SDA���������أ�����ֹͣ����
	//nops();	
	//nops();
	//SCL0;
}

//************************************************
//��Ӧ��(����ack:SDA=0��no_ack:SDA=1)
//************************************************
void IIC_ack_main(uint8 ack_main)
{
	SCL0;	
	nops();
	if(ack_main)
	{
	  SDA0; //ack��Ӧ��
	}
	else 
	{
	  SDA1; //no_ack����Ӧ��
	}
	//nops();	
	nops();
	SCL1;
	//nops();	
	nops();
	SCL0;
}

//*************************************************
//�ֽڷ��ͳ��� ����λ�ȷ��͡���Bing-hong Wu
//����c(������Data or Access)���������մ�Ӧ��
//�������Ǵ�Ӧ��λ��  �������޸�Ϊ���ǡ�
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
		  SDA1; //�жϷ���λ���� 1
		}
		else 
		{
		  SDA0; //�жϷ���λ���� 0
		}
		nops();
		SCL1;
		nops();	
		//nops();
		SCL0;
	}
	nops();
	SDA1;  //������8bit���ͷ�����[SDA����]׼������Ӧ��λ
	nops();
	SCL1;  //[SCL����]
	//nops(); 	
	nops();//sda�����ݼ��Ǵ�Ӧ��λ              
	SCL0;  //�����Ǵ�Ӧ��λ|��Ҫ���ƺ�ʱ��
}

//**************************************************
//�ֽڽ��ճ���
//�����������������ݣ��˳���Ӧ���|��Ӧ����|IIC_ack_main()ʹ��
//return: uchar��1�ֽ�
//**************************************************
uint8 read_byte(void)
{
    uint8 i;
    uint8 c;
	c=0;
	SCL0;
	nops();
	SDA1; //��������Ϊ���뷽ʽ
	nops();
	DIR0;
	for(i=0;i<8;i++)
	{
		nops();
		SCL0;    //��ʱ����Ϊ�ͣ�׼����������λ
		nops();
		SCL1;    //��ʱ����Ϊ�ߣ�ʹ��������������Ч
		nops();
		c<<=1;
		if(SDAIN)
		  c+=1;  //ѭ��������λ�������յ����ݴ�c
	}
	SCL0;	
	nops();
	DIR1;
	return c;
}

//***************************************************
//�����ӵ�ַ�������͵��ֽ����ݣ�ST+Device Access+Data+SP
//***************************************************
void send_to_byte(uint8 ad_main,uint8 c)
{
	IIC_start();
	send_byte(ad_main); //����������ַ
	send_byte(c);       //��������c
	IIC_stop();
}

//***************************************************
//�����ӵ�ַ�������Ͷ��ֽ����ݣ�ST+Device Access+Access+Data+SP
//***************************************************
void send_to_nbyte(uint8 ad_main,uint8 ad_sub,uint8 *buf,uint8 num)
{
	uint8 i;
	IIC_start();
	send_byte(ad_main); //����������ַ
	send_byte(ad_sub); //���������ӵ�ַ
	for(i=0;i<num;i++)
	{
		send_byte(*buf); //��������*buf
		buf++;
	}
	IIC_stop();
}

//***************************************************
//�����ӵ�ַ���������ֽ����ݣ� ST+Device Access+Read+SP
//function:������ַ���������ݴ��ڽ��ջ�������ǰ�ֽ�
void read_from_byte(uint8 ad_main,uint8 *buf)
{
	IIC_start();
	send_byte(ad_main); //����������ַ
	*buf=read_byte();
	IIC_ack_main(no_ack); //����Ӧ��<no_ack=0>
	IIC_stop();
}

//***************************************************
//�����ӵ�ַ����������ֽ����ݣ�ST+Device Access+Access+Read+SP
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
		IIC_ack_main(ack); //��Ӧ��<ack=1>
		buf++;
	}
	*buf=read_byte();
	buf++; //����ָ����������壬Ŀ���ǲ�����bufָ����һ��ַ
	IIC_ack_main(no_ack); //����Ӧ��<no_ack=0>
	IIC_stop();
}