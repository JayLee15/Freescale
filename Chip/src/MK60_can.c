/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_can.c
 * @brief      CAN����
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-09-02
 */

#include "common.h"
#include "MK60_port.h"
#include "MK60_can.h"


// CAN IDֵԽ�ͣ��������ȼ�Խ��
// CAN ������������


// ���ڷ��ͻ������ı��Ļ���������
#define CAN_CS_CODE_TX_INACTIVE     B8(1000)            //MB ���ڷǼ���״̬
#define CAN_CS_CODE_TX_ABORT        B8(1001)            //MB ������
#define CAN_CS_CODE_TX_DATA         B8(1100)            //MB Ϊһ����������֡��MB �� RTR λΪ 0��
#define CAN_CS_CODE_TX_REMOTE       B8(1100)            //MB Ϊһ������Զ������֡��MB �� RTR λΪ 1��
#define CAN_CS_CODE_TX_TANSWER      B8(1110)            //MB Ϊ��Զ������֡��һ�����ͻ�Ӧ֡

// ���ڽ��ջ������ı��Ļ���������
#define CAN_CS_CODE_RX_INACTIVE     B8(0000)            //MB ���ڷǼ���״̬
#define CAN_CS_CODE_RX_EMPTY        B8(0100)            //MB �����Ϊ��
#define CAN_CS_CODE_RX_FULL         B8(0010)            //MB Ϊ��
#define CAN_CS_CODE_RX_OVERRUN      B8(0110)            //д�뵽һ���������� ���� MB ������
#define CAN_CS_CODE_RX_RANSWER      B8(1010)            //һ����֡������Ϊȷ��һ��Զ������֡���ҷ���һ���ظ�֡��
#define CAN_CS_CODE_RX_BUSY         B8(0001)            //���ڸ��� MB �����ݡ�CPU ���������MB


CAN_MemMapPtr CANN[] = {CAN0_BASE_PTR, CAN1_BASE_PTR};

//�����̵�CANʱ��ԴΪ BUS ʱ��
CAN_band_cfg_t (*can_band_cfg)[CAN_BAUD_MAX];


//��������CANʱ��ԴΪ 50.00MHz
CAN_band_cfg_t can_band_cfg_50000K[CAN_BAUD_MAX] =
{
    //BAND, PRESDIV, PROP_SEG, PSEG1,   PSEG2, RJW
    {10,    624,    4,          0,      0,      0},         //������:87.50%
    {20,    124,    12,         2,      2,      2},         //������:85.00%
    {50,    124,    4,          0,      0,      0},         //������:87.50%
    {100,   24,     12,         2,      2,      2},         //������:85.00%
    {125,   24,     10,         1,      1,      1},         //������:87.50%
    {250,   24,     4,          0,      0,      0},         //������:87.50%
    {500,   4,      12,         2,      2,      2},         //������:85.00%
    {1000,  1,      9,          6,      6,      3},         //������:72.00%
};
//��������CANʱ��ԴΪ 60.00MHz
CAN_band_cfg_t can_band_cfg_60000K[CAN_BAUD_MAX] =
{
    //band, presdiv, prop_seg, pseg1, pseg2, rjw
    {10,    249,    16,         2,      2,      2},		//������:87.50%
    {20,    124,    16,         2,      2,      2},		//������:87.50%
    {50,    49,     16,         2,      2,      2},		//������:87.50%
    {100,   24,     16,         2,      2,      2},		//������:87.50%
    {125,   19,     16,         2,      2,      2},		//������:87.50%
    {250,   9,      16,         2,      2,      2},		//������:87.50%
    {500,   4,      16,         2,      2,      2},		//������:87.50%
    {1000,  2,      8,          4,      4,      3},		//������:75.00%
};

/*!
 *  @brief      CAN ��ʼ��
 *  @param      CANn_e      CAN ģ���
 *  @param      CAN_BAUD_e  �����ʱ��
 *  @param      CAN_mode_e  CAN ģʽ( ���� �� �ػ� ģʽ)
 *  @since      v5.0
 *  Sample usage:       can_init(CAN1,CAN_BAUD_20K,CAN_LOOPBACK);   //��ʼ�� CAN1 �������� 20Kb/s �� �ػ�ģʽ
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
        ASSERT(0);          //��֧�� bus ʱ��Ϊ 50M �� 60M
    }

    switch(cann)
    {
    case CAN0:
        SIM_SCGC6 |= SIM_SCGC6_FLEXCAN0_MASK ;                  //���� CAN0 ʱ��
        port_init(CAN0_TX, ALT2);
        port_init(CAN0_RX, ALT2);

        break;
    case CAN1:
        SIM_SCGC3 |= SIM_SCGC3_FLEXCAN1_MASK;                   //���� CAN1 ʱ��
        port_init(CAN1_TX, ALT2);
        port_init(CAN1_RX, ALT2);
        break;

    }

    //ѡ��ʱ��Դ
    CAN_CTRL1_REG(canptr)   |= CAN_CTRL1_CLKSRC_MASK;           //ѡ��Ϊbus ʱ��

    CAN_MCR_REG(canptr) &= ~CAN_MCR_MDIS_MASK;                  //ʹ��CANģ��

    CAN_MCR_REG(canptr) |=  CAN_MCR_FRZ_MASK;                   //ʹ�ܶ���ģʽ

    while((CAN_MCR_REG(canptr) & CAN_MCR_LPMACK_MASK ));        //�ȴ���λ���͹���ģʽ�޷���λ�ɹ���

    CAN_MCR_REG(canptr) |= CAN_MCR_SOFTRST_MASK;                //�����λ

    while(!(CAN_MCR_REG(canptr) & CAN_MCR_FRZACK_MASK));        //�ȴ����붳��ģʽ

    //������仺����������0
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
                             //| CAN_CTRL2_EACEN_MASK            //�����������IDEƥ�䣬RTR��ƥ��
                             //| CAN_CTRL2_RRS_MASK              //���Զ�����Զ������֡
                             //| CAN_CTRL2_MRP_MASK              //�������ȴӽ���FIFO����ƥ��Ȼ������������ƥ��
                             | CAN_CTRL2_TASD(22)                //Tx Arbitration Start Delay
                            );

    //ʹ��һ��32λ������
    CAN_MCR_REG(canptr) = (CAN_MCR_REG(canptr) & (~CAN_MCR_IDAM_MASK)) | CAN_MCR_IDAM(0);

    //���ò�����
    can_setband(cann, band);

    //�ػ�ģʽ
    if(mode == CAN_LOOPBACK)
    {
        CAN_CTRL1_REG(canptr) |= CAN_CTRL1_LPB_MASK;//ʹ�ûػ�ģʽ
    }

    //Ĭ��ʹ�� ���� ����
    //CAN_MCR_REG(canptr) |= CAN_MCR_IRMQ_MASK;

    //��ʼ������Ĵ���
    i = NUMBER_OF_MB;
    while(i)
    {
        i--;
        CAN_RXIMR_REG(canptr, i) = 0x0;
    }

    CAN_RXMGMASK_REG(canptr) = 0x0;
    CAN_RX14MASK_REG(canptr) = 0x0;
    CAN_RX15MASK_REG(canptr) = 0x0;

    //ֻ���ڶ���ģʽ�²������� �������˳�����ģʽ
    CAN_MCR_REG(canptr) &= ~(CAN_MCR_HALT_MASK);

    //�ȴ�ģ���Ƴ�����ģʽ
    while( CAN_MCR_REG(canptr) & CAN_MCR_FRZACK_MASK);

    //�ȴ�ͬ��
    //�ȵ����ڶ���ģʽ������ģʽ����ֹͣģʽ
    while(CAN_MCR_REG(canptr) & CAN_MCR_NOTRDY_MASK);

}


/*!
 *  @brief      CAN ��������
 *  @param      CANn_e          CAN ģ���
 *  @param      mb_num_e        ���������
 *  @param      CAN_USR_ID_t    ID���
 *  @param      len             ���ݳ���
 *  @param      buff            ��������ַ
 *  @since      v5.0
 *  Sample usage:       can_tx(CAN1,CAN_TX_MB,can_tx_id,DATA_LEN, txbuff);  //CAN�������ݡ�������CAN_TX_MB������ID:tx_ID�����ݻ�����txbuff������ DATA_LEN
 */
void can_tx(CANn_e cann, mb_num_e nMB, CAN_USR_ID_t id, uint8 len, void *buff)
{
    uint32 word;
    CAN_MemMapPtr canptr = CANN[cann];


    ASSERT(len <= 8 );       //���ԣ�һ�η�����󳤶�Ϊ 8�ֽ�



    //�����Ĳ���Ϊ���͹���
    CAN_CS_REG(canptr, nMB)   = ( 0
                                  | CAN_CS_CODE(CAN_CS_CODE_TX_INACTIVE)  //������д�������
                                  | (id.IDE << CAN_CS_IDE_SHIFT)             //������дIDEλ
                                  | (id.RTR << CAN_CS_RTR_SHIFT)             //������дRTRλ
                                  | CAN_CS_DLC(len)                   //������д���ݳ���
                                );

    //������дID
    if(id.IDE)
    {
        //��չ֡
        CAN_ID_REG(canptr, nMB)   = ( 0
                                      | CAN_ID_PRIO(1)
                                      | CAN_ID_EXT(id.ID)
                                    );
    }
    else
    {
        //��׼֡
        CAN_ID_REG(canptr, nMB)   = ( 0
                                      | CAN_ID_PRIO(1)
                                      | CAN_ID_STD(id.ID)
                                    );
    }

    //������д����
    word = *(uint32 *)buff;
    CAN_WORD0_REG(canptr, nMB) = SWAP32(word);

    word = *((uint32 *)buff + 1);
    CAN_WORD1_REG(canptr, nMB) = SWAP32(word);

    //��ʼ����
    CAN_CS_REG(canptr, nMB)   =   ( 0
                                    | CAN_CS_CODE(CAN_CS_CODE_TX_DATA)              //д������룬MB Ϊһ����������֡��MB �� RTR λΪ 0��
                                    //| CAN_CS_RTR_MASK
                                    | CAN_CS_DLC(len)                               //������д���ݳ���
                                  );

    //��ʱ�ȴ�������ɣ����ʹ���ж�����ʱ�ȴ�����ɾ����
    while(!(CAN_IFLAG1_REG(canptr) & (1 << nMB)));


    //�屨�Ļ������жϱ�־
    CAN_IFLAG1_REG(canptr)  = (1 << nMB);

}

/*!
 *  @brief      ʹ�� CAN ���ջ�����
 *  @param      CANn_e          CAN ģ���
 *  @param      mb_num_e        ���������
 *  @param      CAN_USR_ID_t    ID���
 *  @since      v5.0
 *  Sample usage:      can_rxbuff_enble(CAN1,CAN_RX_MB,can_my_id);                 //ʹ�ܽ��ջ�����
 */
void can_rxbuff_enble(CANn_e cann, mb_num_e nMB, CAN_USR_ID_t id)
{
    CAN_MemMapPtr canptr = CANN[cann];

    //��MB����Ϊ�Ǽ���״̬
    CAN_CS_REG(canptr, nMB) = CAN_CS_CODE(CAN_CS_CODE_RX_INACTIVE);


    if(id.IDE )         //��չ֡
    {
        CAN_ID_REG(canptr, nMB) =   ( 0
                                      | CAN_ID_EXT( id.ID)
                                    );
        CAN_CS_REG(canptr, nMB) = CAN_CS_IDE_MASK;

        CAN_CS_REG(canptr, nMB) = (0
                                   | CAN_CS_IDE_MASK
                                   | (id.RTR << CAN_CS_IDE_SHIFT)
                                   | CAN_CS_CODE(CAN_CS_CODE_RX_EMPTY)//������ջ�����
                                  );
    }
    else
    {
        //��׼֡
        CAN_ID_REG(canptr, nMB) =   ( 0
                                      | CAN_ID_STD( id.ID)
                                    );

        CAN_CS_REG(canptr, nMB) = (0
                                   | (id.RTR << CAN_CS_IDE_SHIFT)
                                   | CAN_CS_CODE(CAN_CS_CODE_RX_EMPTY)//������ջ�����
                                  );

    }
}

/*!
 *  @brief      CAN ������������
 *  @param      CANn_e          CAN ģ���
 *  @param      mb_num_e        ���������
 *  @param      mask            ����
 *  @since      v5.0
 *  Sample usage:      can_rx_mask(CAN1,CAN_RX_MB,0x00FF);                 //CAN1 ��CAN_RX_MB ������ ��������Ϊ 0x00FF
 */

void can_rx_mask(CANn_e cann, mb_num_e nMB, uint32 mask)
{
    uint8           bFreezeMode;
    CAN_MemMapPtr   canptr = CANN[cann];

    //���붳��ģʽ
    if(!(CAN_MCR_REG(canptr) & CAN_MCR_HALT_MASK))
    {
        CAN_MCR_REG(canptr)  |= (CAN_MCR_HALT_MASK);

        // �ȴ����� ���붳��ģʽ
        while(!(CAN_MCR_REG(canptr) & CAN_MCR_FRZACK_MASK));

        bFreezeMode = 0;
    }
    else
    {
        bFreezeMode = 1;
    }

    // ����Ƿ�ʹ�ö�������
    if(CAN_MCR_REG(canptr)  & CAN_MCR_IRMQ_MASK)
    {
        // ��������
        CAN_RXIMR_REG(canptr, nMB) = mask;
    }
    else
    {
        // 14/15 ��ʹ�ö�����
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
            // ʣ���֧��ȫ������
            CAN_RXMGMASK_REG(canptr)  = mask;
        }
    }
    // �ָ�CAN ����ģʽ
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
 *  @brief      CAN ��������
 *  @param      CANn_e          CAN ģ���
 *  @param      mb_num_e        ���������
 *  @param      CAN_USR_ID_t    ID���
 *  @param      len             ���ݳ���
 *  @param      buff            ��������ַ
 *  @since      v5.0
 *  Sample usage:       can_rx(CAN1,CAN_RX_MB,&can_rx_id,&can_rx_len,can_rx_data);  //CAN �� CAN_RX_MB �������� �����յ��� ID ������ can_rx_id ����ȱ����� can_rx_len�����ݱ����� can_rx_data
 */
void can_rx(CANn_e cann, mb_num_e nMB, CAN_USR_ID_t *id, uint8 *len, void   *buff)
{
    uint8  length;
    uint32 word;

    CAN_MemMapPtr canptr = CANN[cann];

    *(uint32 *)id = 0;

    if((CAN_CS_REG(canptr, nMB)  & CAN_CS_CODE_MASK) != CAN_CS_CODE(CAN_CS_CODE_RX_FULL) ) //������û�н��յ����ݣ����ش���
    {
        *len = 0;
        return;
    }

    length = ( CAN_CS_REG(canptr, nMB) & CAN_CS_DLC_MASK ) >> CAN_CS_DLC_SHIFT;

    if(length < 1) //���յ������ݳ���С��1�����ش���
    {
        *len = 0;
        return;
    }

    //�ж��Ǳ�׼֡������չ֡
    if(! ( CAN_CS_REG(canptr, nMB) & CAN_CS_IDE_MASK ) )
    {
        id->ID = ( CAN_ID_REG(canptr, nMB) & CAN_ID_STD_MASK ) >> CAN_ID_STD_SHIFT; // ��ñ�׼ID
    }
    else
    {
        id->ID = ( CAN_ID_REG(canptr, nMB) & CAN_ID_EXT_MASK ) >> CAN_ID_EXT_SHIFT; // ��ȡ ��չID
        id->IDE = 1 ; //�����չ��ID
    }

    if(CAN_CS_REG(canptr, nMB) & CAN_CS_RTR_MASK)
    {
        id->RTR = 1; //���ΪԶ��֡����
    }

    word = CAN_WORD0_REG(canptr, nMB);
    *((uint32 *)buff) = SWAP32(word);

    word = CAN_WORD1_REG(canptr, nMB);
    *((uint32 *)buff + 1) = SWAP32(word);


    *len = length;

    CAN_TIMER_REG(canptr);      //���� MB

}

/*!
 *  @brief      ʹ�� CAN ���������պͷ����ж�
 *  @param      CANn_e          CAN ģ���
 *  @param      mb_num_e        ���������
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
 *  @brief      ��ֹ CAN ���������պͷ����ж�
 *  @param      CANn_e          CAN ģ���
 *  @param      mb_num_e        ���������
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
 *  @brief      �� CAN �������жϱ�־
 *  @param      CANn_e          CAN ģ���
 *  @param      mb_num_e        ���������
 *  @since      v5.0
 *  Sample usage:       can_clear_flag(CAN1,MB_NUM_5);
 */
void can_clear_flag(CANn_e cann, mb_num_e nMB)
{
    CAN_MemMapPtr canptr = CANN[cann];

    CAN_IFLAG1_REG(canptr) = (1 << nMB);
}


/*!
 *  @brief      ��� CAN �������жϱ�־
 *  @param      CANn_e          CAN ģ���
 *  @param      mb_num_e        ���������
 *  @since      v5.0
 *  Sample usage:       can_get_flag(CAN1,MB_NUM_5);
 */
uint32 can_get_flag(CANn_e cann, mb_num_e nMB)
{
    CAN_MemMapPtr canptr = CANN[cann];

    return (CAN_IFLAG1_REG(canptr) & (1 << nMB));
}


/*!
 *  @brief      ���� CAN �Ĳ�����
 *  @param      CANn_e          CAN ģ���
 *  @param      CAN_BAUD_e      �����ʱ��
 *  @since      v5.0
 *  Sample usage:       can_get_flag(CAN1,MB_NUM_5);
 */

void can_setband(CANn_e cann, CAN_BAUD_e band)
{

    //�����ʼ��㹫ʽ Bit Rate = bus / ((PRESDIV+1) * (4 + PROP_SEG + PSEG1 + PSEG2 ))

    /*
    �����ʹ�ʽ�Ƶ�����:

    ʱ��ݶtime quanta��fTQ = fcanclk / (PRESDIV+1)  ����С��ʱ�䵥λ       ******* ��ʽ1

    λʱ���� 4 ��ʱ�����ɣ�
        ͬ����Sync_Seg = 1��
        ����ʱ���  Prop_Seg = (PROP_SEG + 1)��
        ��λ�����1 Phase_Seg1 =(PSEG1+1)
        ��λ�����2 Phase_Seg2 =  (PSEG2 + 1)


                        ͬ����       ����ʱ���      ��λ�����1    ��λ�����2
     Bit Rate = fTQ / (   1   +   ((PROP_SEG + 1)  + (PSEG1+1))   + (PSEG2 + 1)   ) = fTQ / (4 + PROP_SEG + PSEG1 + PSEG2)  ******* ��ʽ2

     �ɹ�ʽ1�͹�ʽ2�����Ƶ��������ʼ��㹫ʽ��
            Bit Rate = bus / ((PRESDIV+1) * (4 + PROP_SEG + PSEG1 + PSEG2 ))

     ����ʱ��� �Ǹ���ʵ�ʵ�·���ӳ��������ģ��� �������������ӳ� + ���յ�·���ӳ� + ������·���ӳ� ֮�͵�2��������ʱ�����ȷ����������õķ����ͱȽ���

     ���ڲ������λ�ã��Ѳ���������Ϊ CiA �Ƽ���ֵ��
        75%     when ������ > 800K
        80%     when ������ > 500K
        87.5%   when ������ <= 500K


    CAN ���߲��������÷���
    1�����ȼ���CANʱ�ӺͲ����ʵı�ֵ�����ݱ�ֵ�趨Ԥ��Ƶֵ��λʱ�� ��Χ�� 8-25 ��ʱ��ݶ�֮�䣻
        �� λʱ�� nTQ = bus / ((PRESDIV+1) /Bit Rate ��ȡֵ��Χ��  8-25 ֮�䣬�Ӷ�ȷ�� PRESDIV ��ֵ(ֵ����Ψһ�ģ��������)��
        ʱ��ݶ� �� λʱ�� ����������

    2��λʱ�� = 1 + ((PROP_SEG + 1)  + (PSEG1+1))   + (PSEG2 + 1)�� ��PROP_SEG�� PSEG1 ��PSEG2 ���������йأ�
    PSEG1 = PSEG2 �� PSEG1 + 1  = PSEG2
    ��֪�����ߵ� �ӳ�ʱ�䣬������� PROP_SEG (������TQ��������)���̶���� PSEG1 �� PSEG2��
    �������������Ϊ CiA �Ƽ���ֵ���Ӷ���� PSEG2 ��ֵ��PSEG1 = PSEG2 ���̶��õ� PROP_SEG��

    3��RJW +1 <= PSEG1 +1 �� RJW+1 <= 4 ,RJW ȡ ������������������ ���ֵ

    ��ʵ���� bus Ϊ 50MHz ������ CAN ������Ϊ 10k:
    ��:nTQ = 50M/10K/((PRESDIV+1) �� 8~25֮�䣬�� PRESDIV 199~624 ֮�䡣ȡ nTQ =20 TQ���� PRESDIV =249
    ������ �Ƽ� �� 87.5% ���ң����� PSEG2 + 1 = (1- 87.5% )* 20 ,PSEG2 = 1.5 ��ȡ�� �� PSEG2 = 2 ��PSEG1 = PSEG2 -1 = 1 �� �̶� PROP_SEG = 13
    PSEG1 �� 3 ����Сֵ�� PSEG1 = 1 ������ RJW = 1


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









