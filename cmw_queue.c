/**
 * @file cmw_rqueue.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2022-02-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "cmw.h"

/*************************** ring queue api start **********************************/
#ifdef CMW_USING_RQUEUE
/**
 * @brief reset the queue
 *
 * @param queue
 */
static cmw_bool_t rqueue_reset ( cmw_ring_queue_t *queue )
{
    if ( queue == NULL ) { return FALSE; }

    queue->in_data.front = 0;
    queue->in_data.rear = 0;
    return TRUE;
}
/**
 * @brief 判断队列是否已满
 *
 * @param queue
 * @return cmw_bool_t
 */
static cmw_bool_t rqueue_is_full ( cmw_ring_queue_t *queue )
{
    if ( ( queue->in_data.rear + 1 ) % queue->in_data.length == queue->in_data.front )
    {
        return TRUE;  // 队列已满
    }
    else
    { return FALSE; }
}

/**
 * @brief 入队
 *
 * @param queue
 * @param queue_data
 * @return cmw_bool_t
 */
static cmw_bool_t rqueue_enqueue ( cmw_ring_queue_t *queue, void *queue_data )
{
    if ( rqueue_is_full ( queue ) != TRUE )  //队列未满
    {
        int p_dec = ( int ) queue->in_data.p_buffer + queue->in_data.rear * queue->in_data.data_size ;
        memcpy ( ( void * ) p_dec, queue_data, queue->in_data.data_size );
        queue->in_data.rear = ( ( queue->in_data.rear + 1 ) % queue->in_data.length ) ; //尾指针偏移
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}
/**
 * @brief 判读队列是否为空
 *
 * @param queue
 * @return cmw_bool_t
 */
static cmw_bool_t rqueue_is_empty ( cmw_ring_queue_t *queue )
{
    if ( queue->in_data.front == queue->in_data.rear )
    {
        return TRUE;
    }
    else
    { return FALSE; }
}

/**
 * @brief 出队
 *
 * @param queue
 * @param queue_data
 * @return cmw_bool_t
 */
static cmw_bool_t rqueue_dequeue ( cmw_ring_queue_t *queue, cmw_rqueue_data_t queue_data )
{
    if ( rqueue_is_empty ( queue ) != TRUE )  //队列未空
    {
        int p_dec = ( int ) queue->in_data.p_buffer + queue->in_data.front * queue->in_data.data_size ;
        memcpy ( queue_data, ( void * ) p_dec, queue->in_data.data_size );
        //memcpy ( queue_data, queue->in_data.p_buffer + queue->in_data.front * queue->in_data.data_size, queue->in_data.data_size );
        queue->in_data.front = ( queue->in_data.front + 1 ) % queue->in_data.length ;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}


/**
 * @brief 获取当前队列中的队列出口数据
 *
 * @param queue
 * @param queue_data
 * @return cmw_bool_t
 */
static cmw_bool_t rqueue_get_front ( cmw_ring_queue_t *queue, cmw_rqueue_data_t queue_data )
{
    if ( queue == NULL || queue_data == NULL ) { return FALSE; }

    if ( rqueue_is_empty ( queue ) != TRUE )  //队列未空
    {
        int p_dec = ( int ) queue->in_data.p_buffer + queue->in_data.front * queue->in_data.data_size ;
        memcpy ( queue_data, ( void * ) p_dec, queue->in_data.data_size );
        //memcpy ( queue_data, queue->in_data.p_buffer + queue->in_data.front * queue->in_data.data_size, queue->in_data.data_size );
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief 获取当前队列中的队列入口数据(尾部)
 *
 * @param queue
 * @param queue_data
 * @return cmw_bool_t
 */
static cmw_bool_t rqueue_get_rear ( cmw_ring_queue_t *queue, cmw_rqueue_data_t queue_data )
{
    if ( queue == NULL || queue_data == NULL ) { return FALSE; }

    if ( rqueue_is_empty ( queue ) != TRUE )  //队列未空
    {
        int p_dec = ( int ) queue->in_data.p_buffer + queue->in_data.rear * queue->in_data.data_size ;
        memcpy ( queue_data, ( void * ) p_dec, queue->in_data.data_size );
        //memcpy ( queue_data, queue->in_data.p_buffer + queue->in_data.rear * queue->in_data.data_size, queue->in_data.data_size );
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief 获取当前队列中的队列出口数据地址
 *
 * @param queue
 * @param queue_data
 * @return cmw_bool_t
 */
static cmw_bool_t rqueue_get_front_addr ( cmw_ring_queue_t *queue, cmw_rqueue_data_t *queue_data )
{
    if ( queue == NULL || queue_data == NULL ) { return FALSE; }

    if ( rqueue_is_empty ( queue ) != TRUE )  //队列未空
    {
        int p_dec = 0 ;

        if ( queue->in_data.rear )
        {
            p_dec = ( int ) queue->in_data.p_buffer + ( queue->in_data.rear - 1 ) * queue->in_data.data_size ;
        }
        else
        {
            p_dec = ( int ) queue->in_data.p_buffer + ( queue->in_data.length - 1 ) * queue->in_data.data_size ;
        }

        *queue_data = ( cmw_rqueue_data_t ) p_dec;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief 获取当前队列中的队列入口数据地址(尾部)
 *
 * @param queue
 * @param queue_data
 * @return cmw_bool_t
 */
static cmw_bool_t rqueue_get_rear_addr ( cmw_ring_queue_t *queue, cmw_rqueue_data_t *queue_data )
{
    if ( queue == NULL || queue_data == NULL ) { return FALSE; }

    if ( rqueue_is_empty ( queue ) != TRUE )  //队列未空
    {
        int p_dec = 0 ;

        if ( queue->in_data.rear )
        {
            p_dec = ( int ) queue->in_data.p_buffer + ( queue->in_data.rear - 1 ) * queue->in_data.data_size ;
        }
        else
        {
            p_dec = ( int ) queue->in_data.p_buffer + ( queue->in_data.length - 1 ) * queue->in_data.data_size ;
        }

        *queue_data = ( cmw_rqueue_data_t ) p_dec;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}


/**
 * @brief 获取剩余缓冲区长度
 *
 * @param queue
 * @return cmw_u32_t
 */
static cmw_u32_t rqueue_remain_length ( cmw_ring_queue_t *queue )
{
    if ( queue->in_data.front > queue->in_data.rear )
    {
        return ( queue->in_data.front - queue->in_data.rear );
    }
    else if ( queue->in_data.front < queue->in_data.rear )
    {
        return ( queue->in_data.length - queue->in_data.rear + queue->in_data.front );
    }
    else
    {
        return queue->in_data.length;
    }
}
/**
 * @brief 创建队列
 *
 * @param queue
 * @param queue_data
 * @param u32_max_length
 * @return cmw_bool_t
 */
cmw_bool_t cmw_ring_queue_create ( cmw_ring_queue_t *queue, cmw_rqueue_data_t *queue_data, uint32_t queue_length, uint32_t data_size )
{
    if ( queue == NULL || queue_data == NULL || queue_length == 0 || data_size == 0 ) { return FALSE; }

    queue->in_data.rear = 0;
    queue->in_data.front = 0;
    queue->in_data.length  = queue_length;
    queue->in_data.p_buffer    = queue_data;
    queue->in_data.data_size  = data_size;
    queue->out_method.reset    = rqueue_reset;
    queue->out_method.enqueue  = rqueue_enqueue;
    queue->out_method.dequeue   = rqueue_dequeue;
    queue->out_method.get_rear   = rqueue_get_rear;
    queue->out_method.get_front = rqueue_get_front;
    queue->out_method.get_rear_addr   = rqueue_get_rear_addr;
    queue->out_method.get_front_addr = rqueue_get_front_addr;
    queue->out_method.get_remain_length = rqueue_remain_length;
    return TRUE;
}
#endif

