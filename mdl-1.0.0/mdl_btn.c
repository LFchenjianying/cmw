/**
 * @file mdl_key.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 通用按键驱动
 *          
 * @version 0.1
 * @date 2021-08-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "mdl_btn.h"

// 设置事件并执行回调
#define EVENT_SET_AND_EXEC_CB(btn, evt)                                        \
    do                                                                         \
    {                                                                          \
        btn->event = evt;                                                      \
        if(btn->cb)                                                            \
            btn->cb((mdl_button_t*)btn);                                       \
    } while(0)

/**
 * BTN_IS_PRESSED
 * 
 * 1: is pressed
 * 0: is not pressed
*/
#define BTN_IS_PRESSED(i) (g_btn_status_reg & (1 << i))

enum MDL_BTN_STAGE
{
    MDL_BTN_STAGE_DEFAULT = 0,
    MDL_BTN_STAGE_DOWN    = 1,
    MDL_BTN_STAGE_MULTIPLE_CLICK = 2
};

typedef uint32_t btn_type_t;

static mdl_button_t *btn_head = NULL;

/**
 * g_logic_level
 * 
 * The logic level of the button pressed, 
 * Each bit represents a button.
 * 
 * First registered button, the logic level of the button pressed is 
 * at the low bit of g_logic_level.
*/
btn_type_t g_logic_level = (btn_type_t)0;

/**
 * g_btn_status_reg
 * 
 * The status register of all button, each bit records the pressing state of a button.
 * 
 * First registered button, the pressing state of the button is 
 * at the low bit of g_btn_status_reg.
*/
btn_type_t g_btn_status_reg = (btn_type_t)0;

static mdl_u8_t button_cnt = 0;

/**
 * @brief Register a user button
 * 
 * @param button: button structure instance
 * @return Number of keys that have been registered, or -1 when error
*/
mdl_s32_t mdl_button_register(mdl_button_t *button)
{
    mdl_button_t *curr = btn_head;
    
    if (!button || (button_cnt > sizeof(btn_type_t) * 8))
    {
        return -1;
    }

    // 查找按键列表中是否已经存在该按键
    while (curr)
    {
        if(curr == button)
        {
            return -1;  /* already exist. */
        }
        curr = curr->next;
    }

    /**
     * First registered button is at the end of the 'linked list'.
     * btn_head points to the head of the 'linked list'.
    */
    button->next = btn_head;
    button->status = MDL_BTN_STAGE_DEFAULT;
    button->event = MDL_BTN_PRESS_NONE;
    button->scan_cnt = 0;
    button->click_cnt = 0;
    button->max_multiple_clicks_interval = MAX_MULTIPLE_CLICKS_INTERVAL;
    btn_head = button;

    /**
     * First registered button, the logic level of the button pressed is 
     * at the low bit of g_logic_level.
     * 第一个注册按钮时，按下的按钮的逻辑级别在g_logic_level的低位。
     * 越晚注册逻辑级别越高
    */
    g_logic_level |= (button->pressed_logic_level << button_cnt);
    button_cnt ++;

    return button_cnt;
}

/**
 * @brief Read all key values in one scan cycle
 * 
 * @param void
 * @return none
*/
static void mdl_button_read(void)
{
    mdl_u8_t i;
    mdl_button_t* target;

    /* The button that was registered first, the button value is in the low position of raw_data */
    btn_type_t raw_data = 0;

    for(target = btn_head, i = button_cnt - 1;
        (target != NULL) && (target->usr_button_read != NULL);
        target = target->next, i--)
    {
        raw_data = raw_data | ((target->usr_button_read)(target) << i);
    }

    g_btn_status_reg = (~raw_data) ^ g_logic_level;
}

/**
 * @brief Handle all key events in one scan cycle.
 *        Must be used after 'mdl_button_read' API
 * 
 * @param void
 * @return Activated button count
*/
static mdl_u8_t mdl_button_process(void)
{
    mdl_u8_t i;
    mdl_u8_t active_btn_cnt = 0;
    mdl_button_t* target;
    
    for (target = btn_head, i = button_cnt - 1; target != NULL; target = target->next, i--)
    {
        if (target->status > MDL_BTN_STAGE_DEFAULT)
        {
            target->scan_cnt ++;
            if (target->scan_cnt >= ((1 << (sizeof(target->scan_cnt) * 8)) - 1))
            {
                target->scan_cnt = target->long_hold_start_tick;
            }
        }

        switch (target->status)
        {
        case MDL_BTN_STAGE_DEFAULT: /* stage: default(button up) */
            if (BTN_IS_PRESSED(i)) /* is pressed */
            {
                target->scan_cnt = 0;
                target->click_cnt = 0;

                EVENT_SET_AND_EXEC_CB(target, MDL_BTN_PRESS_DOWN);

                /* swtich to button down stage */
                target->status = MDL_BTN_STAGE_DOWN;
            }
            else
            {
                target->event = MDL_BTN_PRESS_NONE;
            }
            break;

        case MDL_BTN_STAGE_DOWN: /* stage: button down */
            if (BTN_IS_PRESSED(i)) /* is pressed */
            {
                if (target->click_cnt > 0) /* multiple click */
                {
                    if (target->scan_cnt > target->max_multiple_clicks_interval)
                    {
                        EVENT_SET_AND_EXEC_CB(target, 
                            target->click_cnt < MDL_BTN_PRESS_REPEAT_CLICK ? 
                                target->click_cnt :
                                MDL_BTN_PRESS_REPEAT_CLICK);

                        /* swtich to button down stage */
                        target->status = MDL_BTN_STAGE_DOWN;
                        target->scan_cnt = 0;
                        target->click_cnt = 0;
                    }
                }
                else if (target->scan_cnt >= target->long_hold_start_tick)
                {
                    if (target->event != MDL_BTN_PRESS_LONG_HOLD)
                    {
                        EVENT_SET_AND_EXEC_CB(target, MDL_BTN_PRESS_LONG_HOLD);
                    }
                }
                else if (target->scan_cnt >= target->long_press_start_tick)
                {
                    if (target->event != MDL_BTN_PRESS_LONG_START)
                    {
                        EVENT_SET_AND_EXEC_CB(target, MDL_BTN_PRESS_LONG_START);
                    }
                }
                else if (target->scan_cnt >= target->short_press_start_tick)
                {
                    if (target->event != MDL_BTN_PRESS_SHORT_START)
                    {
                        EVENT_SET_AND_EXEC_CB(target, MDL_BTN_PRESS_SHORT_START);
                    }
                }
            }
            else /* button up */
            {
                if (target->scan_cnt >= target->long_hold_start_tick)
                {
                    EVENT_SET_AND_EXEC_CB(target, MDL_BTN_PRESS_LONG_HOLD_UP);
                    target->status = MDL_BTN_STAGE_DEFAULT;
                }
                else if (target->scan_cnt >= target->long_press_start_tick)
                {
                    EVENT_SET_AND_EXEC_CB(target, MDL_BTN_PRESS_LONG_UP);
                    target->status = MDL_BTN_STAGE_DEFAULT;
                }
                else if (target->scan_cnt >= target->short_press_start_tick)
                {
                    EVENT_SET_AND_EXEC_CB(target, MDL_BTN_PRESS_SHORT_UP);
                    target->status = MDL_BTN_STAGE_DEFAULT;
                }
                else
                {
                    /* swtich to multiple click stage */
                    target->status = MDL_BTN_STAGE_MULTIPLE_CLICK;
                    target->click_cnt ++;
                }
            }
            break;

        case MDL_BTN_STAGE_MULTIPLE_CLICK: /* stage: multiple click */
            if (BTN_IS_PRESSED(i)) /* is pressed */
            {
                /* swtich to button down stage */
                target->status = MDL_BTN_STAGE_DOWN;
                target->scan_cnt = 0;
            }
            else
            {
                if (target->scan_cnt > target->max_multiple_clicks_interval)
                {
                    EVENT_SET_AND_EXEC_CB(target, 
                        target->click_cnt < MDL_BTN_PRESS_REPEAT_CLICK ? 
                            target->click_cnt :
                            MDL_BTN_PRESS_REPEAT_CLICK);

                    /* swtich to default stage */
                    target->status = MDL_BTN_STAGE_DEFAULT;
                }
            }
            break;
        }
        
        if (target->status > MDL_BTN_STAGE_DEFAULT)
        {
            active_btn_cnt ++;
        }
    }
    
    return active_btn_cnt;
}

/**
 * mdl_button_event_read
 * 
 * @brief Get the button event of the specified button.
 * 
 * @param button: button structure instance
 * @return button event
*/
mdl_button_event_t mdl_button_event_read(mdl_button_t* button)
{
    return (mdl_button_event_t)(button->event);
}

/**
 * mdl_button_scan
 * 
 * @brief Start key scan.
 *        Need to be called cyclically within the specified period.
 *        Sample cycle: 5 - 20ms
 * 
 * @param void
 * @return Activated button count
*/
mdl_u8_t mdl_button_scan(void)
{
    mdl_button_read();
    return mdl_button_process();
}




