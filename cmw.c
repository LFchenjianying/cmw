/**
 * @file cmw.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 *        本文件用于实现基本数据结构、数据转换等基本功能
 *          当前实现的模块如下：
 *              1. 队列
 *              2. 双链表
 *              3. 单链表
 * @version 0.1
 * @date 2021-06-10
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "cmw.h"
#ifdef CMW_USING_RTT
    #include <rtthread.h>
#endif

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
