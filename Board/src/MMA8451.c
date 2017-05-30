#include "common.h"
#include "include.h"

//�˴��������ص�ͷ�ļ�
//�˴��������ص�ͷ�ļ�
#define  OPEN_SCOPER_ACC  0
void mma_delay(uint16 k)		 //��ʱ����
{
    uint8 i;	 //�ں����У�����������������Ȼ����ܵ���
    while(--k)
    {
       for(i=0;i<1;i++); //��ʱʱ�䳤�̣�0--255��ѡ
    }
}


//��ȡMMA8451��1���ֽ�
uint8 MMA8451_readbyte(uint8 address)
{
	uint8 ret;
	IIC_start();		//����
	send_byte(MMA845x_IIC_ADDRESS);	//д���豸ID��д�ź�
	send_byte(address);	//X��ַ
	IIC_start();		//���·��Ϳ�ʼ
	send_byte(MMA845x_IIC_ADDRESS+1);	//д���豸ID������
	ret = read_byte();	//��ȡһ�ֽ�
	IIC_stop();

	return ret;
}



//д��MMA8451��1���ֽ�
void MMA8451_writebyte(uint8 address, uint8 thedata)
{
	IIC_start();		//����
	send_byte(MMA845x_IIC_ADDRESS);	//д���豸ID��д�ź�
	send_byte(address);	//X��ַ
	send_byte(thedata);	//д���豸ID������
	IIC_stop();
}


/*
 * �������ܣ���MAA8451���ٶ����
 * �����w
 *       Status - ���ݼĴ���״̬
 *       Regs_Addr - ���ݼĴ�����ַ
 * ��������ֵ�����ٶ�ֵ��int16��
 */   
int16 MMA8451_GetResult(uint8 Status, uint8 Regs_Addr) 
{
  uint8  ret=0;
  uint16 cnt=0;
  int16  result,temp;
  int    n=0;
  if(Regs_Addr>MMA8451_REG_OUTZ_LSB)
    return 0;
  
  // �ȴ�ת����ɲ�ȡ��ֵ 
  while(!(ret&Status)) 
  {
    ret = MMA8451_readbyte( MMA8451_REG_STATUS);
    if(++cnt>2)
      break;
  }
  
  result= MMA8451_readbyte( Regs_Addr);
  temp  = MMA8451_readbyte( Regs_Addr+1);
  result=result<<8;
  result=result|temp;

  return result>>2;
}


//��ȡ3����ٶ�            ʾ������ʾxyz������
int16 ACC_X, ACC_Y, ACC_Z;
void MMA8451_XYZ()
{
    //ACC_X = MMA8451_GetResult(MMA8451_STATUS_X_READY, MMA8451_REG_OUTX_MSB);
    //ACC_Y = MMA8451_GetResult(MMA8451_STATUS_Y_READY, MMA8451_REG_OUTY_MSB);
    ACC_Z = MMA8451_GetResult(MMA8451_STATUS_X_READY, MMA8451_REG_OUTZ_MSB);	   
    //printf("\n MMA8451_XYZ:  X:%d Y:%d Z:%d ",ACC_X
    //                                         ,ACC_Y
    //                                         ,ACC_Z);
        //ACC_X=1;
        //ACC_Y=2;
        //ACC_Z=3;
 /*   uint8 ACC_var[6] = {1,0,2,0,3,0};
    putchar(0x03);
    putchar(0xfc);
    uart_putbuff(UART0, ACC_var, 6);    //��������
    putchar(0xfc);
    putchar(0x03);
  */  
//    if(OPEN_SCOPER_ACC)
//    {
//    int16 ACC_var[3];
//    ACC_var[0] = ACC_X;
//    ACC_var[1] = ACC_Y;
//    ACC_var[2] = ACC_Z;
//    //putchar(0x03);
//    //putchar(0xfc);
//    //uart_putbuff(UART0, (int8)ACC_var, 3);    //��������
//   // putchar(0xfc);
//    //putchar(0x03);     
//    vcan_sendware(ACC_var, sizeof(ACC_var));
//    //DELAY_MS(10); 
//    }
    
}


//MMA8451��ʼ��
void MMA8451_Init()
{	
    while(MMA8451_readbyte(MMA8451_REG_WHOAMI)!=0x1a);
	mma_delay(10);
	MMA8451_writebyte(MMA8451_REG_SYSMOD,0x00);    //Ĭ��ģʽStandby Mode
	mma_delay(10);
	MMA8451_writebyte(MMA8451_REG_CTRL_REG2,0x02); //High Resolution
        mma_delay(10);
        MMA8451_writebyte(0x0F,0x10);
        mma_delay(10);
	MMA8451_writebyte(MMA8451_REG_CTRL_REG1,0x01); //����ģʽ,800HZ
}



  		
  
