#ifndef _L3G4200D_H_
#define _L3G4200D_H_

#include "common.h"

#define	SlaveAddress   0xD2	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改

//L3G4200D寄存器
#define L3G4200D_WHO_AM_I 			        0x0F
#define L3G4200D_CTRL_REG1 				0x20
#define L3G4200D_CTRL_REG2 				0x21
#define L3G4200D_CTRL_REG3 				0x22
#define L3G4200D_CTRL_REG4 				0x23
#define L3G4200D_CTRL_REG5 				0x24
#define L3G4200D_REFERENCE 				0x25
#define L3G4200D_OUT_TEMP 		        		0x26
#define L3G4200D_STATUS_REG 			                0x27
#define L3G4200D_OUT_X_L 				        0x28
#define L3G4200D_OUT_X_H 				        0x29
#define L3G4200D_OUT_Y_L 				        0x2A
#define L3G4200D_OUT_Y_H 				        0x2B
#define L3G4200D_OUT_Z_L 				        0x2C
#define L3G4200D_OUT_Z_H 			        	0x2D
#define L3G4200D_FIFO_CTRL_REG 			        0x2E
#define L3G4200D_FIFO_SRC_REG 			                0x2F
#define L3G4200D_INT1_CFG 				0x30
#define L3G4200D_INT1_SRC 				        0x31
#define L3G4200D_INT1_TSH_XH 			        0x32
#define L3G4200D_INT1_TSH_XL 			        0x33
#define L3G4200D_INT1_TSH_YH 			        0x34
#define L3G4200D_INT1_TSH_YL 			        0x35
#define L3G4200D_INT1_TSH_ZH 			        0x36
#define L3G4200D_INT1_TSH_ZL 			        0x37
#define L3G4200D_INT1_DURATION 			        0x38


extern int16 Gyro_X, Gyro_Y, Gyro_Z;


uint8 L3G4200_readbyte(uint8 address);
void L3G4200_writebyte(uint8 address, uint8 thedata);
void L3G4200_Init();
int16 L3G4200_GetResult(uint8 Status, uint8 Regs_Addr);
void L3G4200_XYZ();
void L3G4200_Y();

extern int16 Gyro_X, Gyro_Y, Gyro_Z;


#endif