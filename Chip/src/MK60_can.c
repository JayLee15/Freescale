/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       MK60_can.c
 * @brief      CAN函数
 * @author     野火科技
 * @version    v5.0
 * @date       2013-09-02
 */

#include "common.h"
#include "MK60_port.h"
#include "MK60_can.h"


// CAN ID值越低，报文优先级越高
// CAN 掩码用于屏蔽


// 用于发送缓冲区的报文缓冲区码字
#define CAN_CS_CODE_TX_INACTIVE     B8(1000)            //MB 处于非激活状态
#define CAN_CS_CODE_TX_ABORT        B8(1001)            //MB 被丢弃
#define CAN_CS_CODE_TX_DATA         B8(1100)            //MB 为一个发送数据帧（MB 的 RTR 位为 0）
#define CAN_CS_CODE_TX_REMOTE       B8(1100)            //MB 为一个发送远程请求帧（MB 的 RTR 位为 1）
#define CAN_CS_CODE_TX_TANSWER      B8(1110)            //MB 为是远程请求帧的一个发送回应帧

// 用于接收缓冲区的报文缓冲区码字
#define CAN_CS_CODE_RX_INACTIVE     B8(0000)            //MB 处于非激活状态
#define CAN_CS_CODE_RX_EMPTY        B8(0100)            //MB 激活并且为空
#define CAN_CS_CODE_RX_FULL         B8(0010)            //MB 为满
#define CAN_CS_CODE_RX_OVERRUN      B8(0110)            //写入到一个满缓冲区 导致 MB 被覆盖
#define CAN_CS_CODE_RX_RANSWER      B8(1010)            //一个新帧被配置为确认一个远程请求帧并且发送一个回复帧。
#define CAN_CS_CODE_RX_BUSY         B8(0001)            //正在更新 MB 的内容。CPU 不允许访问MB


CAN_MemMapPtr CANN[] = {CAN0_BASE_PTR, CAN1_BASE_PTR};

//本例程的CAN时钟源为 BUS 时钟
CAN_band_cfg_t (*can_band_cfg)[CAN_BAUD_MAX];


//仅适用于CAN时钟源为 50.00MHz
CAN_band_cfg_t can_band_cfg_50000K[CAN_BAUD_MAX] =
{
    //BAND, PRESDIV, PROP_SEG, PSEG1,   PSEG2, RJW
    {10,    624,    4,          0,      0,      0},         //采样点:87.50%
    {20,    124,    12,         2,      2,      2},         //采样点:85.00%
    {50,    124,    4,          0,      0,      0},         //采样点:87.50%
    {100,   24,     12,         2,      2,      2},         //采样点:85.00%
    {125,   24,     10,         1,      1,      1},         //采样点:87.50%
    {250,   24,     4,          0,      0,      0},         //采样点:87.50%
    {500,   4,      12,         2,      2,      2},         //采样点:85.00%
    {1000,  1,      9,          6,      6,      3},         //采样点:72.00%
};
//仅适用于CAN时钟源为 60.00MHz
CAN_band_cfg_t can_band_cfg_60000K[CAN_BAUD_MAX] =
{
    //band, presdiv, prop_seg, pseg1, pseg2, rjw
    {10,    249,    16,         2,      2,      2},		//采样点:87.50%
    {20,    124,    16,         2,      2,      2},		//采样点:87.50%
    {50,    49,     16,         2,      2,      2},		//采样点:87.50%
    {100,   24,     16,         2,      2,      2},		//采样点:87.50%
    {125,   19,     16,         2,      2,      2},		//采样点:87.50%
    {250,   9,      16,         2,      2,      2},		//采样点:87.50%
    {500,   4,      16,         2,      2,      2},		//采样点:87.50%
    {1000,  2,      8,          4,      4,      3},		//采样点:75.00%
};

/*!
 *  @brief      CAN 初始化
 *  @param      CANn_e      CAN 模块号
 *  @param      CAN_BAUD_e  波特率编号
 *  @param      CAN_mode_e  CAN 模式( 正常 、 回环 模式)
 *  @since      v5.0
 *  Sample usage:       can_init(CAN1,CAN_BAUD_20K,CAN_LOOPBACK);   //初始化 CAN1 ，波特率 20Kb/s ， 回环模式
 */
void can_init(CANn_e cann, CAN_BAUD_e band, CAN_mode_e mode)
{
    uint8 i;
    CAN_MemMapPtr canptr = CANN[cann];
    if(bus_clk_khz == 50000)
    {
         can_band_cfg = &can_band_cfg_50000K;
    }
    else if(bus_clk_khz == 60000)
    {
         can_band_cfg = &can_band_cfg_60000K;
    }
    else
    {
        ASSERT(0);          //仅支持 bus 时钟为 50M 或 60M
    }

    switch(cann)
    {
    case CAN0:
        SIM_SCGC6 |= SIM_SCGC6_FLEXCAN0_MASK ;                  //开启 CAN0 时钟
        port_init(CAN0_TX, ALT2);
        port_init(CAN0_RX, ALT2);

        break;
    case CAN1:
        SIM_SCGC3 |= SIM_SCGC3_FLEXCAN1_MASK;                   //开启 CAN1 时钟
        port_init(CAN1_TX, ALT2);
        port_init(CAN1_RX, ALT2);
        break;

    }

    //选择时钟源
    CAN_CTRL1_REG(canptr)   |= CAN_CTRL1_CLKSRC_MASK;           //选择为bus 时钟

    CAN_MCR_REG(canptr) &= ~CAN_MCR_MDIS_MASK;                  //使能CAN模块

    CAN_MCR_REG(canptr) |=  CAN_MCR_FRZ_MASK;                   //使能冻结模式

    while((CAN_MCR_REG(canptr) & CAN_MCR_LPMACK_MASK ));        //等待复位（低功耗模式无法复位成功）

    CAN_MCR_REG(canptr) |= CAN_MCR_SOFTRST_MASK;                //软件复位

    while(!(CAN_MCR_REG(canptr) & CAN_MCR_FRZACK_MASK));        //等待进入冻结模式

    //清空邮箱缓冲区内容清0
    i = 16;
    while(i)
    {
        i--;
        CAN_CS_REG(canptr, i) = 0;
        CAN_ID_REG(canptr, i) = 0;
        CAN_WORD0_REG(canptr, i) = 0;
        CAN_WORD1_REG(canptr, i) = 0;
    }

    CAN_CTRL2_REG(canptr) = (0
                             //| CAN_CTRL2_EACEN_MASK            //接收邮箱过滤IDE匹配，RTR不匹配
                             //| CAN_CTRL2_RRS_MASK              //不自动产生远程请求帧
                             //| CAN_CTRL2_MRP_MASK              //邮箱首先从接收FIFO队列匹配然后再在邮箱中匹配
                             | CAN_CTRL2_TASD(22)                //Tx Arbitration Start Delay
                            );

    //使用一个32位过滤器
    CAN_MCR_REG(canptr) = (CAN_MCR_REG(canptr) & (~CAN_MCR_IDAM_MASK)) | CAN_MCR_IDAM(0);

    //设置波特率
    can_setband(cann, band);

    //回环模式
    if(mode == CAN_LOOPBACK)
    {
        CAN_CTRL1_REG(canptr) |= CAN_CTRL1_LPB_MASK;//使用回环模式
    }

    //默认使用 独立 掩码
    //CAN_MCR_REG(canptr) |= CAN_MCR_IRMQ_MASK;

    //初始化掩码寄存器
    i = NUMBER_OF_MB;
    while(i)
    {
        i--;
        CAN_RXIMR_REG(canptr, i) = 0x0;
    }

    CAN_RXMGMASK_REG(canptr) = 0x0;
    CAN_RX14MASK_REG(canptr) = 0x0;
    CAN_RX15MASK_REG(canptr) = 0x0;

    //只有在冻结模式下才能配置 配置完退出冻结模式
    CAN_MCR_REG(canptr) &= ~(CAN_MCR_HALT_MASK);

    //等待模块推出冻结模式
    while( CAN_MCR_REG(canptr) & CAN_MCR_FRZACK_MASK);

    //等待同步
    //等到不在冻结模式，休眠模式或者停止模式
    while(CAN_MCR_REG(canptr) & CAN_MCR_NOTRDY_MASK);

}


/*!
 *  @brief      CAN 发送数据
 *  @param      CANn_e          CAN 模块号
 *  @param      mb_num_e        缓冲区编号
 *  @param      CAN_USR_ID_t    ID编号
 *  @param      len             数据长度
 *  @param      buff            缓冲区地址
 *  @since      v5.0
 *  Sample usage:       can_tx(CAN1,CAN_TX_MB,can_tx_id,DATA_LEN, txbuff);  //CAN发送数据。缓冲区CAN_TX_MB，报文ID:tx_ID，数据缓冲区txbuff，长度 DATA_LEN
 */
void can_tx(CANn_e cann, mb_num_e nMB, CAN_USR_ID_t id, uint8 len, void *buff)
{
    uint32 word;
    CAN_MemMapPtr canptr = CANN[cann];


    ASSERT(len <= 8 );       //断言，一次发送最大长度为 8字节



    //以下四步骤为发送过程
    CAN_CS_REG(canptr, nMB)   = ( 0
                                  | CAN_CS_CODE(CAN_CS_CODE_TX_INACTIVE)  //缓冲区写激活代码
                                  | (id.IDE << CAN_CS_IDE_SHIFT)             //缓冲区写IDE位
                                  | (id.RTR << CAN_CS_RTR_SHIFT)             //缓冲区写RTR位
                                  | CAN_CS_DLC(len)                   //缓冲区写数据长度
                                );

    //缓冲区写ID
    if(id.IDE)
    {
        //拓展帧
        CAN_ID_REG(canptr, nMB)   = ( 0
                                      | CAN_ID_PRIO(1)
                                      | CAN_ID_EXT(id.ID)
                                    );
    }
    else
    {
        //标准帧
        CAN_ID_REG(canptr, nMB)   = ( 0
                                      | CAN_ID_PRIO(1)
                                      | CAN_ID_STD(id.ID)
                                    );
    }

    //缓冲区写内容
    word = *(uint32 *)buff;
    CAN_WORD0_REG(canptr, nMB) = SWAP32(word);

    word = *((uint32 *)buff + 1);
    CAN_WORD1_REG(canptr, nMB) = SWAP32(word);

    //开始发送
    CAN_CS_REG(canptr, nMB)   =   ( 0
                                    | CAN_CS_CODE(CAN_CS_CODE_TX_DATA)              //写激活代码，MB 为一个发送数据帧（MB 的 RTR 位为 0）
                                    //| CAN_CS_RTR_MASK
                                    | CAN_CS_DLC(len)                               //缓冲区写数据长度
                                  );

    //限时等待发送完成（如果使用中断则限时等待语句可删除）
    while(!(CAN_IFLAG1_REG(canptr) & (1 << nMB)));


    //清报文缓冲区中断标志
    CAN_IFLAG1_REG(canptr)  = (1 << nMB);

}

/*!
 *  @brief      使能 CAN 接收缓冲区
 *  @param      CANn_e          CAN 模块号
 *  @param      mb_num_e        缓冲区编号
 *  @param      CAN_USR_ID_t    ID编号
 *  @since      v5.0
 *  Sample usage:      can_rxbuff_enble(CAN1,CAN_RX_MB,can_my_id);                 //使能接收缓冲区
 */
void can_rxbuff_enble(CANn_e cann, mb_num_e nMB, CAN_USR_ID_t id)
{
    CAN_MemMapPtr canptr = CANN[cann];

    //将MB配置为非激活状态
    CAN_CS_REG(canptr, nMB) = CAN_CS_CODE(CAN_CS_CODE_RX_INACTIVE);


    if(id.IDE )         //扩展帧
    {
        CAN_ID_REG(canptr, nMB) =   ( 0
                                      | CAN_ID_EXT( id.ID)
                                    );
        CAN_CS_REG(canptr, nMB) = CAN_CS_IDE_MASK;

        CAN_CS_REG(canptr, nMB) = (0
                                   | CAN_CS_IDE_MASK
                                   | (id.RTR << CAN_CS_IDE_SHIFT)
                                   | CAN_CS_CODE(CAN_CS_CODE_RX_EMPTY)//激活接收缓冲区
                                  );
    }
    else
    {
        //标准帧
        CAN_ID_REG(canptr, nMB) =   ( 0
                                      | CAN_ID_STD( id.ID)
                                    );

        CAN_CS_REG(canptr, nMB) = (0
                                   | (id.RTR << CAN_CS_IDE_SHIFT)
                                   | CAN_CS_CODE(CAN_CS_CODE_RX_EMPTY)//激活接收缓冲区
                                  );

    }
}

/*!
 *  @brief      CAN 接收掩码配置
 *  @param      CANn_e          CAN 模块号
 *  @param      mb_num_e        缓冲区编号
 *  @param      mask            掩码
 *  @since      v5.0
 *  Sample usage:      can_rx_mask(CAN1,CAN_RX_MB,0x00FF);                 //CAN1 的CAN_RX_MB 缓冲区 配置掩码为 0x00FF
 */

void can_rx_mask(CANn_e cann, mb_num_e nMB, uint32 mask)
{
    uint8           bFreezeMode;
    CAN_MemMapPtr   canptr = CANN[cann];

    //进入冻结模式
    if(!(CAN_MCR_REG(canptr) & CAN_MCR_HALT_MASK))
    {
        CAN_MCR_REG(canptr)  |= (CAN_MCR_HALT_MASK);

        // 等待进入 进入冻结模式
        while(!(CAN_MCR_REG(canptr) & CAN_MCR_FRZACK_MASK));

        bFreezeMode = 0;
    }
    else
    {
        bFreezeMode = 1;
    }

    // 检测是否使用独立掩码
    if(CAN_MCR_REG(canptr)  & CAN_MCR_IRMQ_MASK)
    {
        // 独立掩码
        CAN_RXIMR_REG(canptr, nMB) = mask;
    }
    else
    {
        // 14/15 是使用独立的
        if(nMB == MB_NUM_14)
        {
            CAN_RX14MASK_REG(canptr)  = mask;
        }
        else if (nMB == MB_NUM_15)
        {
            CAN_RX15MASK_REG(canptr)  = mask;
        }
        else
        {
            // 剩余的支持全局掩码
            CAN_RXMGMASK_REG(canptr)  = mask;
        }
    }
    // 恢复CAN 操作模式
    if(!bFreezeMode)
    {
        // De-assert Freeze Mode
        CAN_MCR_REG(canptr)  &= ~ (CAN_MCR_HALT_MASK);

        // Wait till exit of freeze mode
        while( CAN_MCR_REG(canptr)  & CAN_MCR_FRZACK_MASK);

        // Wait till ready
        while( CAN_MCR_REG(canptr) & CAN_MCR_NOTRDY_MASK);
    }
}

/*!
 *  @brief      CAN 接收数据
 *  @param      CANn_e          CAN 模块号
 *  @param      mb_num_e        缓冲区编号
 *  @param      CAN_USR_ID_t    ID编号
 *  @param      len             数据长度
 *  @param      buff            缓冲区地址
 *  @since      v5.0
 *  Sample usage:       can_rx(CAN1,CAN_RX_MB,&can_rx_id,&can_rx_len,can_rx_data);  //CAN 从 CAN_RX_MB 接收数据 ，接收到的 ID 保存在 can_rx_id 里，长度保存在 can_rx_len，数据保存在 can_rx_data
 */
void can_rx(CANn_e cann, mb_num_e nMB, CAN_USR_ID_t *id, uint8 *len, void   *buff)
{
    uint8  length;
    uint32 word;

    CAN_MemMapPtr canptr = CANN[cann];

    *(uint32 *)id = 0;

    if((CAN_CS_REG(canptr, nMB)  & CAN_CS_CODE_MASK) != CAN_CS_CODE(CAN_CS_CODE_RX_FULL) ) //缓冲区没有接收到数据，返回错误
    {
        *len = 0;
        return;
    }

    length = ( CAN_CS_REG(canptr, nMB) & CAN_CS_DLC_MASK ) >> CAN_CS_DLC_SHIFT;

    if(length < 1) //接收到的数据长度小于1，返回错误
    {
        *len = 0;
        return;
    }

    //判断是标准帧还是扩展帧
    if(! ( CAN_CS_REG(canptr, nMB) & CAN_CS_IDE_MASK ) )
    {
        id->ID = ( CAN_ID_REG(canptr, nMB) & CAN_ID_STD_MASK ) >> CAN_ID_STD_SHIFT; // 获得标准ID
    }
    else
    {
        id->ID = ( CAN_ID_REG(canptr, nMB) & CAN_ID_EXT_MASK ) >> CAN_ID_EXT_SHIFT; // 获取 扩展ID
        id->IDE = 1 ; //标记扩展的ID
    }

    if(CAN_CS_REG(canptr, nMB) & CAN_CS_RTR_MASK)
    {
        id->RTR = 1; //标记为远程帧类型
    }

    word = CAN_WORD0_REG(canptr, nMB);
    *((uint32 *)buff) = SWAP32(word);

    word = CAN_WORD1_REG(canptr, nMB);
    *((uint32 *)buff + 1) = SWAP32(word);


    *len = length;

    CAN_TIMER_REG(canptr);      //解锁 MB

}

/*!
 *  @brief      使能 CAN 缓冲区接收和发送中断
 *  @param      CANn_e          CAN 模块号
 *  @param      mb_num_e        缓冲区编号
 *  @since      v5.0
 *  Sample usage:       can_irq_en(CAN1,MB_NUM_5);
 */
void can_irq_en(CANn_e cann, mb_num_e nMB)
{
    CAN_MemMapPtr canptr = CANN[cann];


    CAN_IMASK1_REG(canptr) = (1 << nMB);

    if(cann == CAN0)
    {
        enable_irq(CAN0_ORed_MB_IRQn);
    }
    else if(cann == CAN1)
    {
        enable_irq(CAN1_ORed_MB_IRQn);
    }
}

/*!
 *  @brief      禁止 CAN 缓冲区接收和发送中断
 *  @param      CANn_e          CAN 模块号
 *  @param      mb_num_e        缓冲区编号
 *  @since      v5.0
 *  Sample usage:       can_irq_dis(CAN1,MB_NUM_5);
 */
void can_irq_dis(CANn_e cann, mb_num_e nMB)
{
    CAN_MemMapPtr canptr = CANN[cann];

    CAN_IMASK1_REG(canptr) &= ~ (1 << nMB);

    if(cann == CAN0)
    {
        disable_irq(CAN0_ORed_MB_IRQn);
    }
    else if(cann == CAN1)
    {
        disable_irq(CAN1_ORed_MB_IRQn);
    }

}

/*!
 *  @brief      清 CAN 缓冲区中断标志
 *  @param      CANn_e          CAN 模块号
 *  @param      mb_num_e        缓冲区编号
 *  @since      v5.0
 *  Sample usage:       can_clear_flag(CAN1,MB_NUM_5);
 */
void can_clear_flag(CANn_e cann, mb_num_e nMB)
{
    CAN_MemMapPtr canptr = CANN[cann];

    CAN_IFLAG1_REG(canptr) = (1 << nMB);
}


/*!
 *  @brief      获得 CAN 缓冲区中断标志
 *  @param      CANn_e          CAN 模块号
 *  @param      mb_num_e        缓冲区编号
 *  @since      v5.0
 *  Sample usage:       can_get_flag(CAN1,MB_NUM_5);
 */
uint32 can_get_flag(CANn_e cann, mb_num_e nMB)
{
    CAN_MemMapPtr canptr = CANN[cann];

    return (CAN_IFLAG1_REG(canptr) & (1 << nMB));
}


/*!
 *  @brief      设置 CAN 的波特率
 *  @param      CANn_e          CAN 模块号
 *  @param      CAN_BAUD_e      波特率编号
 *  @since      v5.0
 *  Sample usage:       can_get_flag(CAN1,MB_NUM_5);
 */

void can_setband(CANn_e cann, CAN_BAUD_e band)
{

    //波特率计算公式 Bit Rate = bus / ((PRESDIV+1) * (4 + PROP_SEG + PSEG1 + PSEG2 ))

    /*
    波特率公式推导过程:

    时间份额（time quanta）fTQ = fcanclk / (PRESDIV+1)  是最小的时间单位       ******* 公式1

    位时间由 4 个时间段组成：
        同步段Sync_Seg = 1、
        传播时间段  Prop_Seg = (PROP_SEG + 1)、
        相位缓冲段1 Phase_Seg1 =(PSEG1+1)
        相位缓冲段2 Phase_Seg2 =  (PSEG2 + 1)


                        同步段       传播时间段      相位缓冲段1    相位缓冲段2
     Bit Rate = fTQ / (   1   +   ((PROP_SEG + 1)  + (PSEG1+1))   + (PSEG2 + 1)   ) = fTQ / (4 + PROP_SEG + PSEG1 + PSEG2)  ******* 公式2

     由公式1和公式2即可推导出波特率计算公式：
            Bit Rate = bus / ((PRESDIV+1) * (4 + PROP_SEG + PSEG1 + PSEG2 ))

     传播时间段 是根据实际电路的延迟来决定的，即 总线驱动器的延迟 + 接收电路的延迟 + 总线线路的延迟 之和的2倍。但这时间很难确定，因而配置的方法就比较灵活。

     关于采样点的位置，把采样点设置为 CiA 推荐的值：
        75%     when 波特率 > 800K
        80%     when 波特率 > 500K
        87.5%   when 波特率 <= 500K


    CAN 总线波特率配置方法
    1、首先计算CAN时钟和波特率的比值，根据比值设定预分频值，位时间 范围在 8-25 个时间份额之间；
        即 位时间 nTQ = bus / ((PRESDIV+1) /Bit Rate 的取值范围在  8-25 之间，从而确定 PRESDIV 的值(值不是唯一的，灵活配置)。
        时间份额 和 位时间 必须是整数

    2、位时间 = 1 + ((PROP_SEG + 1)  + (PSEG1+1))   + (PSEG2 + 1)， 与PROP_SEG、 PSEG1 和PSEG2 三个参数有关；
    PSEG1 = PSEG2 或 PSEG1 + 1  = PSEG2
    若知道总线的 延迟时间，即可算得 PROP_SEG (调高至TQ的整数倍)，继而求得 PSEG1 和 PSEG2。
    否则采样点设置为 CiA 推荐的值，从而求得 PSEG2 的值。PSEG1 = PSEG2 ，继而得到 PROP_SEG。

    3、RJW +1 <= PSEG1 +1 且 RJW+1 <= 4 ,RJW 取 满足以上两个条件的 最大值

    【实例】 bus 为 50MHz 下设置 CAN 波特率为 10k:
    答:nTQ = 50M/10K/((PRESDIV+1) 在 8~25之间，即 PRESDIV 199~624 之间。取 nTQ =20 TQ，即 PRESDIV =249
    采样点 推荐 在 87.5% 左右，所以 PSEG2 + 1 = (1- 87.5% )* 20 ,PSEG2 = 1.5 ，取整 得 PSEG2 = 2 ，PSEG1 = PSEG2 -1 = 1 ， 继而 PROP_SEG = 13
    PSEG1 和 3 中最小值是 PSEG1 = 1 ，所以 RJW = 1


    */

    CAN_MemMapPtr canptr = CANN[cann];
    CAN_band_cfg_t *pcan_ban_cfg = &(*can_band_cfg)[band];


    CAN_CTRL1_REG(canptr) = (0
                             | CAN_CTRL1_PROPSEG(pcan_ban_cfg->prop_seg)
                             | CAN_CTRL1_RJW(pcan_ban_cfg->rjw)
                             | CAN_CTRL1_PSEG1(pcan_ban_cfg->pseg1)
                             | CAN_CTRL1_PSEG2(pcan_ban_cfg->pseg2)
                             | CAN_CTRL1_PRESDIV(pcan_ban_cfg->presdiv)
                            );
}









