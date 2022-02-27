/**
 * @file mdl_rr.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 时间片轮转调度器(Round-Robin)
 * 
 * @version 0.1
 * @date 2021-07-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MDL_RR_H__
#define __MDL_RR_H__

#include "mdl_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RR_TASK_STOP,
    RR_TASK_RUN
} task_run_state_t;

typedef struct mdl_timeslice
{
    mdl_u32_t id;
    void (*task_cb)(void);
    task_run_state_t is_run;
    mdl_u32_t timer;
    mdl_u32_t len;
    mdl_list_t list;
} mdl_timeslice_t;

void mdl_timeslice_exec(void);
void mdl_timeslice_tick(void);
void mdl_timeslice_task_init(mdl_timeslice_t* obj, void (*task_hdl)(void), mdl_u32_t id, mdl_u32_t timeslice_len);
void mdl_timeslice_task_add(mdl_timeslice_t* obj);
void mdl_timeslice_task_delete(mdl_timeslice_t* obj);
mdl_u32_t mdl_timeslice_get_task_timeslice_len(mdl_timeslice_t* obj);
mdl_u32_t mdl_timeslice_get_task_num(void);
mdl_u8_t mdl_timeslice_task_is_exist(mdl_timeslice_t* obj);



#ifdef __cplusplus
}
#endif

#endif


