/**
 * @file cmw_rqueue.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 *          1. 非覆盖环形队列
 * @version 0.1
 * @date 2022-02-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CMW_QUEUE_H__
#define __CMW_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cmw_define.h"

#ifdef CMW_USING_RQUEUE

typedef void *cmw_rqueue_data_t;

/**
 * @brief 队列对象结构体
 *
 */
struct cmw_ring_queue
{
    struct queue_in_data
    {
        cmw_u32_t rear; /* position of queue rear */
        cmw_u32_t front; /* position of queue front */
        cmw_u32_t length; /* maximun length of queue */
        void **p_buffer;
        cmw_u32_t data_size; /* position of queue data size */
    } in_data;

    struct queue_out_method
    {
        cmw_bool_t ( *reset ) ( struct cmw_ring_queue *p_queue );
        cmw_bool_t ( *enqueue ) ( struct cmw_ring_queue *p_queue, cmw_rqueue_data_t queue_data );
        cmw_bool_t ( *dequeue ) ( struct cmw_ring_queue *p_queue, cmw_rqueue_data_t queue_data );
        cmw_bool_t ( *get_front ) ( struct cmw_ring_queue *p_queue, cmw_rqueue_data_t queue_data );
        cmw_bool_t ( *get_rear ) ( struct cmw_ring_queue *p_queue, cmw_rqueue_data_t queue_data );
        cmw_bool_t ( *get_front_addr ) ( struct cmw_ring_queue *p_queue, cmw_rqueue_data_t *queue_data );
        cmw_bool_t ( *get_rear_addr ) ( struct cmw_ring_queue *p_queue, cmw_rqueue_data_t *queue_data );
        cmw_u32_t ( *get_remain_length ) ( struct cmw_ring_queue *queue );
    } out_method;
};

typedef struct cmw_ring_queue  cmw_ring_queue_t;

/**
 * @brief 创建队列
 *
 * @param queue
 * @param queue_data
 * @param queue_length
 * @param data_size
 * @return cmw_bool_t
 */
cmw_bool_t cmw_ring_queue_create ( cmw_ring_queue_t *queue, cmw_rqueue_data_t *queue_data, uint32_t queue_length, uint32_t data_size );

#endif



#ifdef __cplusplus
}
#endif


#endif /* __CMW_H__ */


