/**
 * @file cmw.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 * @version 0.1
 * @date 2021-06-10
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "cmw_def.h"
#include "cmw.h"
#ifdef CMW_USING_RTT
    #include <rtthread.h>
#endif
#include "stdlib.h"
#include <string.h>
/*************************** ring queue api start **********************************/

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
static cmw_bool_t rqueue_is_full ( cmw_ring_queue_t *queue )
{
    if ( ( queue->in_data.rear + 1 ) % queue->in_data.length == queue->in_data.front )
    {
        return TRUE;  // 队列已满
    }
    else
    { return FALSE; }
}
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
static cmw_bool_t rqueue_is_empty ( cmw_ring_queue_t *queue )
{
    if ( queue->in_data.front == queue->in_data.rear )
    {
        return TRUE;
    }
    else
    { return FALSE; }
}
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
    if ( queue == NULL || queue_data == NULL || queue_length == NULL || data_size == NULL ) { return FALSE; }

    queue->in_data.rear = 0;
    queue->in_data.front = 0;
    queue->in_data.length  = queue_length;
    queue->in_data.p_buffer    = queue_data;
    queue->in_data.data_size = data_size;
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

/*************************** 断言打印 **********************************/

/**
 * The CMW_ASSERT function.
 *
 * @param ex the assertion condition string
 * @param func the function name when assertion.
 * @param line the file line number when assertion.
 */
void cmw_assert_handler ( const char *ex_string, const char *func, cmw_u32_t line )
{
    #ifdef CMW_USING_RTT
    rt_kprintf ( "(%s) assertion failed at function:%s, line number:%d \n", ex_string, func, line );
    #else
    printf ( "(%s) assertion failed at function:%s, line number:%d \n", ex_string, func, line );
    #endif
}

/****************************** memory heap option****************************************/
/**
 * @brief Allocate a block of memory with a minimum of 'size' bytes.
 *
 * @param size
 * @return void*
 */
void *cmw_malloc ( cmw_size_t size )
{
    #ifdef CMW_USING_RTT
    return rt_malloc ( size );
    #else
    return malloc ( size );
    #endif
}

/**
 * This function will change the previously allocated memory block.
 *
 * @param rmem pointer to memory allocated by rt_malloc
 * @param newsize the required new size
 *
 * @return the changed memory block address
 */
void *cmw_realloc ( void *rmem, cmw_size_t newsize )
{
    #ifdef CMW_USING_RTT
    return rt_realloc ( rmem, newsize );
    #else
    return realloc ( rmem, newsize );
    #endif
}

/**
 * This function will release the previously allocated memory block by
 * rt_malloc. The released memory block is taken back to system heap.
 *
 * @param rmem the address of memory which will be released
 */
void cmw_free ( void *rmem )
{
    #ifdef CMW_USING_RTT
    rt_free ( rmem );
    #else
    free ( rmem );
    #endif
}

/******************************double list****************************************/
/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
void cmw_list_init ( cmw_list_t *l )
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
void cmw_list_insert_after ( cmw_list_t *l, cmw_list_t *n )
{
    l->next->prev = n;
    n->next = l->next;
    l->next = n;
    n->prev = l;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
void cmw_list_insert_before ( cmw_list_t *l, cmw_list_t *n )
{
    l->prev->next = n;
    n->prev = l->prev;
    l->prev = n;
    n->next = l;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
void cmw_list_remove ( cmw_list_t *n )
{
    n->next->prev = n->prev;
    n->prev->next = n->next;
    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
cmw_s32_t cmw_list_isempty ( const cmw_list_t *l )
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
cmw_u32_t cmw_list_len ( const cmw_list_t *l )
{
    cmw_u32_t len = 0;
    const cmw_list_t *p = l;

    while ( p->next != l )
    {
        p = p->next;
        len ++;
    }

    return len;
}

/******************************single list****************************************/

/**
 * @brief 单向链表初始化
 *
 * @param l
 */
void cmw_slist_init ( cmw_slist_t *l )
{
    l->next = CMW_NULL;
}

/**
 * @brief 单向链表节点添加
 *
 * @param l
 * @param n
 */
void cmw_slist_append ( cmw_slist_t *l, cmw_slist_t *n )
{
    struct cmw_slist_node *node;
    node = l;

    while ( node->next ) { node = node->next; }

    /* append the node to the tail */
    node->next = n;
    n->next = CMW_NULL;
}

/**
 * @brief 单向链表插入节点
 *
 * @param l
 * @param n
 */
void cmw_slist_insert ( cmw_slist_t *l, cmw_slist_t *n )
{
    n->next = l->next;
    l->next = n;
}

/**
 * @brief 获取单向链表长度
 *
 * @param l
 * @return cmw_u32_t
 */
cmw_u32_t cmw_slist_len ( const cmw_slist_t *l )
{
    cmw_u32_t len = 0;
    const cmw_slist_t *list = l->next;

    while ( list != CMW_NULL )
    {
        list = list->next;
        len ++;
    }

    return len;
}

/**
 * @brief 单向链表移除节点
 *
 * @param l
 * @param n
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_remove ( cmw_slist_t *l, cmw_slist_t *n )
{
    /* remove slist head */
    struct cmw_slist_node *node = l;

    while ( node->next && node->next != n ) { node = node->next; }

    /* remove node */
    if ( node->next != ( cmw_slist_t * ) 0 ) { node->next = node->next->next; }

    return l;
}

/**
 * @brief 获取第一个节点
 *
 * @param l
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_first ( cmw_slist_t *l )
{
    return l->next;
}

/**
 * @brief 单向链表获取最后一个节点
 *
 * @param l
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_tail ( cmw_slist_t *l )
{
    while ( l->next ) { l = l->next; }

    return l;
}

/**
 * @brief 单向链表获取下一个节点
 *
 * @param n
 * @return cmw_slist_t*
 */
cmw_slist_t *cmw_slist_next ( cmw_slist_t *n )
{
    return n->next;
}

/**
 * @brief 判断单向链表下一个节点是否存在
 *
 * @param l
 * @return int
 */
cmw_s32_t cmw_slist_isempty ( cmw_slist_t *l )
{
    return l->next == CMW_NULL;
}

