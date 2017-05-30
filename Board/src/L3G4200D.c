#include "common.h"
#include "include.h"
//此处请添加相关的头文件
#define  OPEN_SCOPER_DEC  0


//延时函数：软件延时，控制模拟IIC的数据流频率。
void tly_delay(uint16 k)		 
{
    uint8 i;	 

    while(--k)
    {
        for(i=0;i<10;i++); //延时时间长短，0--255可选
    }
}

//读取L3G4200的1个字节：两次IIC交互 写设备地址及写寄存器信号+写设备地址及读设备信号
uint8 L3G4200_readbyte(uint8 address)
{
	uint8 ret;
	IIC_start();		//启动
	send_byte(SlaveAddress);	//写入设备ID：0xD2，及【写】信号
	send_byte(address);	//X地址
        
	IIC_start();		//重新发送开始
	send_byte(SlaveAddress+1);	//写入设备ID：0xD2，及【读】信号
	ret = read_byte();	//读取一字节
	IIC_stop();

	return ret;
}



//写入L3G4200的1个字节：一次IIC交互 写设备地址+寄存器地址+相应数据
void L3G4200_writebyte(uint8 address, uint8 thedata)
{
	IIC_start();		//启动
	send_byte(SlaveAddress);	//写入设备ID及【写】信号
	send_byte(address);	//Device中Segment地址
	send_byte(thedata);	//所存储数据
	IIC_stop();
}

/*
 * 函数功能：读L3G4200角速度输出
 * 参数
 *       Status - 数据寄存器状态
 *       Regs_Addr - 数据寄存器地址
 * 函数返回值：加速度值（int16）
 */   
int16 L3G4200_GetResult(uint8 Status, uint8 Regs_Addr) 
{
  uint8  ret=0;
  uint16 cnt=0;
  int16  result,temp;
  
  if(Regs_Addr>L3G4200D_OUT_Z_H)
    return 0;                                        //初始规定：其余无效地址屏蔽，可变更。地址详见头文件。
  
  // 等待转换完成并取出值 
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

//初始化L3G4200D，根据需要请参考pdf进行修改************************
//配置寄存器进行控制
//[可配置的寄存器]：L3G4200D_WHO_AM_I / L3G4200D_CTRL_REG1~5 / L3G4200D_REFERENCE / L3G4200D_FIFO_CTRL_REG / 
//                  L3G4200D_INT1_CFG / L3G4200D_INT1_TSH_XH~ZL / L3G4200D_INT1_DURATION
//以上寄存器可写（L3G4200D_WHO_AM_I除外），其余寄存器只读。
//初始化L3G4200D，根据需要请参考pdf进行修改************************
void L3G4200_Init()
{
    //printf("\n\n\n***********程序断点测试1************\n");
   while(L3G4200_readbyte(L3G4200D_WHO_AM_I)!=0xD3);//IIC通信【握手环节】，必要。
    //{
    //DELAY_MS(100);
    //printf("L3G4200D连接失败，继续重试");    
    //}
    //printf("\n\n\n***********程序断点测试2************\n");
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG1, 0xFF);   //REG1配置： ODR：Output Data Rate-800[最高] / （低通滤波截止频率）Cut-Off-800 Hz / Normal or Sleep mode / XYZ enabled
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG2, 0x00);   //REG2配置： Normal mode (reset reading HP_RESET_FILTER) / 高通滤波-56 Hz
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG3, 0x08);   //REG3配置：Int 2 data ready Enabled / 推挽输出 / 其余（Int1中断、Int1启动、Int1中断触发沿配置、水位线、满队列、空队列）Disabled
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG4, 0x30);   //REG4配置：量程+-2000dps 【等等】
   tly_delay(10);
   L3G4200_writebyte(L3G4200D_CTRL_REG5, 0x00);   //REG5配置：Normal mode/FIFO disable/HPF disabled/Data in DataReg and FIFO are non-high-pass-filtered.
   tly_delay(10);                                 //REG5配置：Non-high-pass-filtered data are used for interrupt generation.
}


//读取3轴角速度
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
    //uart_putbuff(UART0, (int8)ACC_var, 3);    //发送数据
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
