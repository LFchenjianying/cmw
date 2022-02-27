/**
 * @file mdl_rr.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 时间片轮转调度器(Round-Robin)
 *        使用步骤:
 *                  1. 使用硬件定时器或者其他定时方式定时调用 mdl_timeslice_tick函数
 *                  2. 将 mdl_timeslice_exec 放在一个线程或者主循环中调用
 *                  3. 初始化任务 mdl_timeslice_task_init
 *                  4. 添加任务  mdl_timeslice_task_add
 *
 * @version 0.1
 * @date 2021-07-16
 *
 * @copyright Copyright (c) 2021
 *
 * @note api list
 *          mdl_timeslice_exec                                  时间片调度函数
 *          mdl_timeslice_tick                                  时间片时基函数
 *          mdl_timeslice_task_init                             初始化任务(ID 和 时间片长度(延时时间))
 *          mdl_timeslice_task_add                              添加任务
 *          mdl_timeslice_task_delete                           删除任务
 *          mdl_timeslice_get_task_timeslice_len                获取时间片长度
 *          mdl_timeslice_get_task_num                          获取当前任务数量
 *          mdl_timeslice_task_is_exist                         判断指定任务是否存在
 */
#include "mdl_rr.h"

/**-
 * @brief Construct a new mdl list head object
 *
 */
static MDL_LIST_HEAD(timeslice_task_list);

/**
 * @brief 时间片轮转调度函数,需要轮训调用
 *
 */
void mdl_timeslice_exec(void)
{
    mdl_list_t *node;
    mdl_timeslice_t *task;
    mdl_list_for_each(node, &timeslice_task_list)
    {
        task = mdl_list_entry(node, mdl_timeslice_t, list);
        if (task->is_run == RR_TASK_RUN)
        {
            task->task_cb();
            task->is_run = RR_TASK_STOP;
        }
    }
}

/**
 * @brief 时间片基本时基,需要定时调用,作为时间片的基本时间单位
 *
 */
void mdl_timeslice_tick(void)
{
    mdl_list_t *node;
    mdl_timeslice_t *task;
    mdl_list_for_each(node, &timeslice_task_list)
    {
        task = mdl_list_entry(node, mdl_timeslice_t, list);
        if (task->timer != 0)
        {
            task->timer--;
            if (task->timer == 0)
            {
                task->is_run = RR_TASK_RUN;
                task->timer = task->len;
            }
        }
    }
}

/**
 * @brief 获取当前任务数量
 *
 * @return mdl_u32_t
 */
mdl_u32_t mdl_timeslice_get_task_num(void)
{
    return mdl_list_len(&timeslice_task_list);
}

/**
 * @brief 任务初始化
 *
 * @param obj
 * @param task_cb 任务控制块
 * @param id 任务ID
 * @param timeslice_len 分配的时间片长度(延时长度)
 */
void mdl_timeslice_task_init(mdl_timeslice_t *obj, void (*task_cb)(void), mdl_u32_t id, mdl_u32_t timeslice_len)
{
    obj->id = id;
    obj->is_run = RR_TASK_STOP;
    obj->task_cb = task_cb;
    obj->timer = timeslice_len;
    obj->len = timeslice_len;
}

/**
 * @brief 添加任务
 *
 * @param obj
 */
void mdl_timeslice_task_add(mdl_timeslice_t *obj)
{
    mdl_list_insert_before(&timeslice_task_list, &obj->list);
}

/**
 * @brief 判断任务是否存在于任务链中
 *
 * @param obj
 * @return mdl_u8_t
 */
mdl_u8_t mdl_timeslice_task_is_exist(mdl_timeslice_t *obj)
{
    mdl_u8_t isexist = 0;
    mdl_list_t *node;
    mdl_timeslice_t *task;
    mdl_list_for_each(node, &timeslice_task_list)
    {
        task = mdl_list_entry(node, mdl_timeslice_t, list);
        if (obj->id == task->id)
        { isexist = 1; }
    }
    return isexist;
}

/**
 * @brief 删除一个任务
 *
 * @param obj
 */
void mdl_timeslice_task_delete(mdl_timeslice_t *obj)
{
    if (mdl_timeslice_task_is_exist(obj))
    { mdl_list_remove(&obj->list); }
    else
    { return; }
}

/**
 * @brief 获取任务时间片长度(延时长度)
 *
 * @param obj
 * @return mdl_u32_t
 */
mdl_u32_t mdl_timeslice_get_task_timeslice_len(mdl_timeslice_t *obj)
{
    return obj->len;
}

