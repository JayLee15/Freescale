
#include "FIRE_key_event.h"
#include "FIRE_UI_VAR.h"
//#include "NRF24L0.h"
//#include "NRF24L0_MSG.h"


void key_event_init()
{
    //初始化 全部 按键
    key_init(KEY_MAX);

    //初始化临时变量
    var_init();

    //同步全部数据并显示
    var_syn(VAR_MAX);       //同步全部 ,必须先同步再显示全部，因为有可能同步失败。
    var_display(VAR_MAX);   //显示全部
}

void deal_key_event()
{
    KEY_MSG_t keymsg;

    while(get_key_msg(& keymsg))     //获得按键就进行处理
    {
        if(keymsg.status == KEY_DOWN)
        {
            switch(keymsg.key)
            {
            case KEY_U:
                var_value(VAR_ADD);
                break;

            case KEY_D:
                var_value(VAR_SUB);
                break;

            case KEY_L:
                var_select(VAR_PREV);
                break;

            case KEY_R:
                var_select(VAR_NEXT);
                break;

            case KEY_B:
                var_ok();
                break;

            case KEY_A:
                val_cancel();
                break;

            case KEY_START:
                save_var(CAR_CTRL,CAR_START);
                var_syn(CAR_CTRL);
                var_display(CAR_CTRL);
                break;
            case KEY_STOP:
                save_var(CAR_CTRL,CAR_STOP);
                var_syn(CAR_CTRL);
                var_display(CAR_CTRL);
                break;

            default:
                break;
            }
        }
        else if(keymsg.status == KEY_HOLD)
        {
            switch(keymsg.key)
            {
            case KEY_U:
                var_value(VAR_ADD_HOLD);
                break;

            case KEY_D:
                var_value(VAR_SUB_HOLD);
                break;

            case KEY_L:
                var_select(VAR_PREV_HOLD);
                break;

            case KEY_R:
                var_select(VAR_NEXT_HOLD);
                break;

            case KEY_B:                //长按 OK 键 同步全部数据并显示
                var_syn(VAR_MAX);       //同步全部 ,必须先同步再显示全部，因为有可能同步失败。
                var_display(VAR_MAX);   //显示全部
                break;

            case KEY_START:
                save_var(CAR_CTRL,CAR_START);
                while(var_syn(CAR_CTRL)==0);
                var_display(CAR_CTRL);
                break;

            case KEY_STOP:
                save_var(CAR_CTRL,CAR_STOP);
                while(var_syn(CAR_CTRL)==0);
                var_display(CAR_CTRL);

            default:            //不需要处理 cancel
                break;
            }
        }
        else
        {
            //KEY_UP ,不进行处理
        }
    }
}





