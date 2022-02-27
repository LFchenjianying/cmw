/**
 * @file cmw_rr.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 时间片轮转调度器(Round-Robin)
 * 
 * @version 0.1
 * @date 2021-07-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CMW_TIMESLICE_H__
#define __CMW_TIMESLICE_H__

#include "cmw.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined (CMW_USING_TIMESLICE)

typedef enum {
    RR_TASK_STOP,
    RR_TASK_RUN
} task_run_state_t;

typedef struct cmw_timeslice
{
    cmw_u32_t id;
    void (*task_cb)(void);
    task_run_state_t is_run;
    cmw_u32_t timer;
    cmw_u32_t len;
    cmw_list_t list;
} cmw_timeslice_t;

void cmw_timeslice_exec(void);
void cmw_timeslice_tick(void);
void cmw_timeslice_task_init(cmw_timeslice_t* obj, void (*task_hdl)(void), cmw_u32_t id, cmw_u32_t timeslice_len);
void cmw_timeslice_task_add(cmw_timeslice_t* obj);
void cmw_timeslice_task_delete(cmw_timeslice_t* obj);
cmw_u32_t cmw_timeslice_get_task_timeslice_len(cmw_timeslice_t* obj);
cmw_u32_t cmw_timeslice_get_task_num(void);
cmw_u8_t cmw_timeslice_task_is_exist(cmw_timeslice_t* obj);

#endif

#ifdef __cplusplus
}
#endif

#endif


