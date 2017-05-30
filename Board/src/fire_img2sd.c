#include "common.h"
#include "fire_img2sd.h"
#include "ff.h"
#include "FIRE_camera.h"       //����ͷ��ͷ�ļ�

static FATFS firefs;    //�ļ�ϵͳ
static FIL   firesrc;   //�ļ�


void img_sd_init(void)
{
    int     fireres;
    char    myfilename[20];
    uint16  imgsize[] = {
                            CAMERA_H,
                            CAMERA_W,
                            //CAMERA_COLOR,
                         };      //�ߡ�������ͷ��ɫ����
    uint32  mybw;
    uint32  Imag_num = 0;

    f_mount(0, &firefs);

    do
    {
        Imag_num ++;
        sprintf(myfilename, "0:/fire%d.sd", Imag_num);
        fireres = f_open( &firesrc , myfilename, FA_CREATE_NEW | FA_WRITE);

        if(firefs.fs_type == 0)
        {
            firesrc.fs = 0;
            return;
        }

    }while(fireres == FR_EXIST);        //����ļ����ڣ�������������1

    if ( fireres == FR_OK )
    {
        fireres = f_write(&firesrc, imgsize, sizeof(imgsize),&mybw);  //��д��ߺͿ�������λ������
    }
    else
    {
        f_close(&firesrc);
        firesrc.fs = 0;
    }
}




void img_sd_save(uint8 * imgaddr,uint32 size)
{
#define F_SYNC_TIME   30      //���� n �κ� �� ͬ��

    int   fireres;
    uint32 mybw;
    static uint8 time = 0;
    //uint32 size = CAMERA_SIZE;

    if(firesrc.fs != 0)
    {
        time ++;

        fireres = f_write(&firesrc, imgaddr, size ,&mybw);

        if(fireres != FR_OK)
        {
            f_close(&firesrc);
            firesrc.fs = 0;
        }

        if(time > F_SYNC_TIME)
        {
            time = 0 ;
            f_sync(&firesrc);
        }
    }
}

void img_sd_exit(void)
{
    f_close(&firesrc);
    firesrc.fs = 0;
}