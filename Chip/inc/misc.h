/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       misc.h
 * @brief      野火K60 平台其他需要用到的函数的实现
 * @author     野火科技
 * @version    v5.0
 * @date       2013-06-26
 */

#ifndef __MISC_H__
#define __MISC_H__

#include "common.h"


void write_vtor (int);                                              //设置中断向量表的地址
void set_vector_handler(VECTORn_t , void pfunc_handler(void));      //设置中断函数到中断向量表里


//兼容旧代码
#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         //使能IRQ
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        //禁止IRQ
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  //设置优先级


#define EnableInterrupts                asm(" CPSIE i");            //使能全部中断
#define DisableInterrupts               asm(" CPSID i");            //禁止全部中断

void fire_cpy( uint8 *dst, uint8 *src, uint32 count);

#endif  /* __MISC_H__ */

