/**
 * @file cmw_key.h
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

#ifndef __CMW_BUTTON_H__
#define __CMW_BUTTON_H__

#include "cmw_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CMW_USING_BUTTON)

#define CMW_BTN_SCAN_FREQ_HZ 50 // How often flex_button_scan () is called
#define CMW_MS_TO_SCAN_CNT(ms) (ms / (1000 / CMW_BTN_SCAN_FREQ_HZ))

/* Multiple clicks interval, default 300ms */
#define MAX_MULTIPLE_CLICKS_INTERVAL (CMW_MS_TO_SCAN_CNT(300))

typedef enum
{
    CMW_BTN_PRESS_DOWN = 0,
    CMW_BTN_PRESS_CLICK,
    CMW_BTN_PRESS_DOUBLE_CLICK,
    CMW_BTN_PRESS_REPEAT_CLICK,
    CMW_BTN_PRESS_SHORT_START,
    CMW_BTN_PRESS_SHORT_UP,
    CMW_BTN_PRESS_LONG_START,
    CMW_BTN_PRESS_LONG_UP,
    CMW_BTN_PRESS_LONG_HOLD,
    CMW_BTN_PRESS_LONG_HOLD_UP,
    CMW_BTN_PRESS_MAX,
    CMW_BTN_PRESS_NONE,
} cmw_button_event_t;

typedef void (*button_response_callback)(void*);

typedef struct cmw_button
{
    struct cmw_button* next;

    cmw_u8_t  (*usr_button_read)(void *);
    button_response_callback  cb;

    cmw_u16_t scan_cnt;
    cmw_u16_t click_cnt;
    cmw_u16_t max_multiple_clicks_interval;  /* 连按间隔 */

    cmw_u16_t debounce_tick;              /* 防反跳 */
    cmw_u16_t short_press_start_tick;     /* 短按起始时刻 */
    cmw_u16_t long_press_start_tick;      /* 长按起始时刻 */
    cmw_u16_t long_hold_start_tick;       /* 长按保持起始时刻 */

    cmw_u8_t id;
    cmw_u8_t pressed_logic_level : 1;
    cmw_u8_t event               : 4;
    cmw_u8_t status              : 3;
} cmw_button_t;



cmw_s32_t flex_button_register(cmw_button_t *button);
cmw_button_event_t flex_button_event_read(cmw_button_t* button);
cmw_u8_t flex_button_scan(void);

#endif

#ifdef __cplusplus
}
#endif

#endif
