#include "common.h"
#include "include.h"
//�˴��������ص�ͷ�ļ�
#define  OPEN_SCOPER_DEC  0


//��ʱ�����������ʱ������ģ��IIC��������Ƶ�ʡ�
void tly_delay(uint16 k)		 
{
    uint8 i;	 

    while(--k)
    {
        for(i=0;i<10;i++); //��ʱʱ�䳤�̣�0--255��ѡ
    }
}

//��ȡL3G4200��1���ֽڣ�����IIC���� д�豸��ַ��д�Ĵ����ź�+д�豸��ַ�����豸�ź�
uint8 L3G4200_readbyte(uint8 address)
{
	uint8 ret;
	IIC_start();		//����
	send_byte(SlaveAddress);	//д���豸ID��0xD2������д���ź�
	send_byte(address);	//X��ַ
        
	IIC_start();		//���·��Ϳ�ʼ
	send_byte(SlaveAddress+1);	//д���豸ID��0xD2�����������ź�
	ret = read_byte();	//��ȡһ�ֽ�
	IIC_stop();

	return ret;
}



//д��L3G4200��1���ֽڣ�һ��IIC���� д�豸��ַ+�Ĵ�����ַ+��Ӧ����
void L3G4200_writebyte(uint8 address, uint8 thedata)
{
	IIC_start();		//����
	send_byte(SlaveAddress);	//д���豸ID����д���ź�
	send_byte(address);	//Device��Segment��ַ
	send_byte(thedata);	//���洢����
	IIC_stop();
}

/*
 * �������ܣ���L3G4200���ٶ����
 * �����w
 *       Status - ���ݼĴ���״̬
 *       Regs_Addr - ���ݼĴ�����ַ
 * ��������ֵ�����ٶ�ֵ��int16��
 */   
int16 L3G4200_GetResult(uint8 Status, uint8 Regs_Addr) 
{
  uint8  ret=0;
  uint16 cnt=0;
  int16  result,temp;
  
  if(Regs_Addr>L3G4200D_OUT_Z_H)
    return 0;                                        //��ʼ�涨��������Ч��ַ���Σ��ɱ������ַ���ͷ�ļ���
  
  // �ȴ�ת����ɲ�ȡ��ֵ 
  while(!(ret&Status)) 
  {
    ret = L3G4200_readbyte( L3G4200D_STATUS_REG);
    if(++cnt>2)
      break;
  }
  
  result= L3G4200_readbyte( Regs_Addr);
  temp  = L3G4200_readbyte( Regs_Addr-1);
  result=result<<8;
  result=result|temp;
  
  return result>>2;
}


//*****************************************************************

//��ʼ��L3G4200D��������Ҫ��ο�pdf�����޸�************************
//���üĴ������п���
//[�����õļĴ���]��L3G4200D_WHO_AM_I / L3G4200D_CTRL_REG1~5 / L3G4200D_REFERENCE / L3G4200D_FIFO_CTRL_REG / 
//                  L3G4200D_INT1_CFG / L3G4200D_INT1_TSH_XH~ZL / L3G4200D_INT1_DURATION
//���ϼĴ�����д��L3G4200D_WHO_AM_I���⣩������Ĵ���ֻ����
//��ʼ��L3G4200D��������Ҫ��ο�pdf�����޸�************************
void L3G4200_Init()
{
    //printf("\n\n\n***********����ϵ����1************\n");
   while(L3G4200_readbyte(L3G4200D_WHO_AM_I)!=0xD3);//IICͨ�š����ֻ��ڡ�����Ҫ��
    //{
    //DELAY_MS(100);
    //printf("L3G4200D����ʧ�ܣ���������");    
    //}
    //printf("\n\n\n***********����ϵ����2************\n");
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG1, 0xFF);   //REG1���ã� ODR��Output Data Rate-800[���] / ����ͨ�˲���ֹƵ�ʣ�Cut-Off-800 Hz / Normal or Sleep mode / XYZ enabled
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG2, 0x00);   //REG2���ã� Normal mode (reset reading HP_RESET_FILTER) / ��ͨ�˲�-56 Hz
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG3, 0x08);   //REG3���ã�Int 2 data ready Enabled / ������� / ���ࣨInt1�жϡ�Int1������Int1�жϴ��������á�ˮλ�ߡ������С��ն��У�Disabled
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG4, 0x30);   //REG4���ã�����+-2000dps ���ȵȡ�
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG5, 0x00);   //REG5���ã�Normal mode/FIFO disable/HPF disabled/Data in DataReg and FIFO are non-high-pass-filtered.
   tly_delay(10);                                 //REG5���ã�Non-high-pass-filtered data are used for interrupt generation.
}


//��ȡ3����ٶ�
int16 Gyro_X, Gyro_Y, Gyro_Z;
void L3G4200_XYZ()
{
    Gyro_X = L3G4200_GetResult(L3G4200D_STATUS_REG, L3G4200D_OUT_X_H);
    //Gyro_Y = L3G4200_GetResult(L3G4200D_STATUS_REG, L3G4200D_OUT_Y_H);
    //Gyro_Z = L3G4200_GetResult(L3G4200D_STATUS_REG, L3G4200D_OUT_Z_H);	
    
    //printf("\n L3G4200_XYZ:  X:%d Y:%d Z:%d ",Gyro_X
    //                                         ,Gyro_Y
    //                                         ,Gyro_Z);
    
   if(OPEN_SCOPER_DEC)
    {
    int16 DEC_var[3];
    DEC_var[0] = Gyro_X;
    DEC_var[1] = Gyro_Y;
    DEC_var[2] = Gyro_Z;
    
    //putchar(0x03);
    //putchar(0xfc);
    //uart_putbuff(UART0, (int8)ACC_var, 3);    //��������
   // putchar(0xfc);
    //putchar(0x03);   
    Sendware((uint8_t *)DEC_var, sizeof(DEC_var));
    //DELAY_MS(10); 
    }   
}

void L3G4200_Y()
{
  Gyro_Y = L3G4200_GetResult(L3G4200D_STATUS_REG, L3G4200D_OUT_Y_H);
}
