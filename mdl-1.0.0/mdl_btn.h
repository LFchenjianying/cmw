/**
 * @file mdl_key.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * 
 * @brief 通用按键驱动
 *          
 * @version 0.1
 * @date 2021-08-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MDL_BTN_H__
#define __MDL_BTN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mdl_def.h"

#define MDL_BTN_SCAN_FREQ_HZ 50 // How often flex_button_scan () is called
#define MDL_MS_TO_SCAN_CNT(ms) (ms / (1000 / MDL_BTN_SCAN_FREQ_HZ))

/* Multiple clicks interval, default 300ms */
#define MAX_MULTIPLE_CLICKS_INTERVAL (MDL_MS_TO_SCAN_CNT(300))

typedef enum
{
    MDL_BTN_PRESS_DOWN = 0,
    MDL_BTN_PRESS_CLICK,
    MDL_BTN_PRESS_DOUBLE_CLICK,
    MDL_BTN_PRESS_REPEAT_CLICK,
    MDL_BTN_PRESS_SHORT_START,
    MDL_BTN_PRESS_SHORT_UP,
    MDL_BTN_PRESS_LONG_START,
    MDL_BTN_PRESS_LONG_UP,
    MDL_BTN_PRESS_LONG_HOLD,
    MDL_BTN_PRESS_LONG_HOLD_UP,
    MDL_BTN_PRESS_MAX,
    MDL_BTN_PRESS_NONE,
} mdl_button_event_t;

typedef void (*button_response_callback)(void*);

typedef struct mdl_button
{
    struct mdl_button* next;

    mdl_u8_t  (*usr_button_read)(void *);
    button_response_callback  cb;

    mdl_u16_t scan_cnt;
    mdl_u16_t click_cnt;
    mdl_u16_t max_multiple_clicks_interval;  /* 连按间隔 */

    mdl_u16_t debounce_tick;              /* 防反跳 */
    mdl_u16_t short_press_start_tick;     /* 短按起始时刻 */
    mdl_u16_t long_press_start_tick;      /* 长按起始时刻 */
    mdl_u16_t long_hold_start_tick;       /* 长按保持起始时刻 */

    mdl_u8_t id;
    mdl_u8_t pressed_logic_level : 1;
    mdl_u8_t event               : 4;
    mdl_u8_t status              : 3;
} mdl_button_t;



mdl_s32_t flex_button_register(mdl_button_t *button);
mdl_button_event_t flex_button_event_read(mdl_button_t* button);
mdl_u8_t flex_button_scan(void);

#ifdef __cplusplus
}
#endif

#endif
