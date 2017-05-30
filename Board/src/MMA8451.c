#include "common.h"
#include "include.h"

//此处请添加相关的头文件
//此处请添加相关的头文件
#define  OPEN_SCOPER_ACC  0
void mma_delay(uint16 k)		 //延时函数
{
    uint8 i;	 //在函数中，必须先声明变量，然后才能调用
    while(--k)
    {
       for(i=0;i<1;i++); //延时时间长短，0--255可选
    }
}


//读取MMA8451的1个字节
uint8 MMA8451_readbyte(uint8 address)
{
	uint8 ret;
	IIC_start();		//启动
	send_byte(MMA845x_IIC_ADDRESS);	//写入设备ID及写信号
	send_byte(address);	//X地址
	IIC_start();		//重新发送开始
	send_byte(MMA845x_IIC_ADDRESS+1);	//写入设备ID及读信
	ret = read_byte();	//读取一字节
	IIC_stop();

	return ret;
}



//写入MMA8451的1个字节
void MMA8451_writebyte(uint8 address, uint8 thedata)
{
	IIC_start();		//启动
	send_byte(MMA845x_IIC_ADDRESS);	//写入设备ID及写信号
	send_byte(address);	//X地址
	send_byte(thedata);	//写入设备ID及读信
	IIC_stop();
}


/*
 * 函数功能：读MAA8451加速度输出
 * 参数
 *       Status - 数据寄存器状态
 *       Regs_Addr - 数据寄存器地址
 * 函数返回值：加速度值（int16）
 */   
int16 MMA8451_GetResult(uint8 Status, uint8 Regs_Addr) 
{
  uint8  ret=0;
  uint16 cnt=0;
  int16  result,temp;
  int    n=0;
  if(Regs_Addr>MMA8451_REG_OUTZ_LSB)
    return 0;
  
  // 等待转换完成并取出值 
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


//读取3轴加速度            示波器显示xyz轴数据
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
    uart_putbuff(UART0, ACC_var, 6);    //发送数据
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
//    //uart_putbuff(UART0, (int8)ACC_var, 3);    //发送数据
//   // putchar(0xfc);
//    //putchar(0x03);     
//    vcan_sendware(ACC_var, sizeof(ACC_var));
//    //DELAY_MS(10); 
//    }
    
}


//MMA8451初始化
void MMA8451_Init()
{	
    while(MMA8451_readbyte(MMA8451_REG_WHOAMI)!=0x1a);
	mma_delay(10);
	MMA8451_writebyte(MMA8451_REG_SYSMOD,0x00);    //默认模式Standby Mode
	mma_delay(10);
	MMA8451_writebyte(MMA8451_REG_CTRL_REG2,0x02); //High Resolution
        mma_delay(10);
        MMA8451_writebyte(0x0F,0x10);
        mma_delay(10);
	MMA8451_writebyte(MMA8451_REG_CTRL_REG1,0x01); //主动模式,800HZ
}



  		
  
