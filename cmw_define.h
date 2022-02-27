/**
 * @file cmw_def.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 * @version 0.1
 * @date 2021-06-10
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __CMW_DEF_H__
#define __CMW_DEF_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#define CMW_USING_RTT

/**************************************************************************
*                               模块裁剪                                 *
**************************************************************************/
#define CMW_USING_LIST
#define CMW_USING_SLIST
#define CMW_USING_RQUEUE
#define CMW_USING_CTRL                      // Ctrl 组件
#define CMW_USING_SOFT_SPI                       // SPI 组件
#define CMW_USING_SOFT_IIC             
#define CMW_USING_SMD
#define CMW_USING_BUTTON
#define CMW_USING_TIMESLICE
#define CMW_USING_LAT

/**************************************************************************/


/**************************************************************************
*                               模块依赖                                 *
**************************************************************************/
#ifdef CMW_USING_LAT
#define CMW_USING_LIST
#define CMW_USING_SLIST
#endif



/**************************************************************************/

/**************************************************************************
*                              variable Typedef                           *
**************************************************************************/
typedef char                     cmw_s8_t;
typedef short int                cmw_s16_t;
typedef int                      cmw_s32_t;
typedef unsigned           char  cmw_u8_t;
typedef unsigned short     int   cmw_u16_t;
typedef unsigned           int   cmw_u32_t;
typedef char                    *cmw_ps8_t;
typedef short int               *cmw_ps16_t;
typedef int                     *cmw_ps32_t;
typedef unsigned           char *cmw_pu8_t;
typedef unsigned short     int  *cmw_pu16_t;
typedef unsigned           int  *cmw_pu32_t;
typedef float                    cmw_f32_t;
typedef double                   cmw_d64_t;
#define  cmw_inline            __inline

typedef int                             cmw_bool_t;      /**< boolean type */
typedef long                            cmw_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   cmw_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef cmw_base_t                      cmw_err_t;       /**< Type for error number */
typedef cmw_u32_t                       cmw_time_t;      /**< Type for time stamp */
typedef cmw_u32_t                       cmw_tick_t;      /**< Type for tick count */
typedef cmw_base_t                      cmw_flag_t;      /**< Type for flags */
typedef cmw_ubase_t                     cmw_size_t;      /**< Type for size number */
typedef cmw_ubase_t                     cmw_dev_t;       /**< Type for device */
typedef cmw_base_t                      cmw_off_t;       /**< Type for offset */


/** @addtogroup 输出类型定义(Exported_types)
  * @{
  */
#define CMW_NULL NULL

#ifndef TRUE
#define TRUE   (1)
#endif

#ifndef FALSE
#define FALSE  (0)
#endif

#define C_CMW_U32_MAXVAL    ((u32_t)0xffffffff)
#define C_CMW_U16_MAXVAL    ((u16_t)0xffff)
#define C_CMW_U8_MAXVAL     ((u8_t)0xff)

/*  error code definitions */
#define CMW_EOK                          0               /**< There is no error */
#define CMW_ERROR                        1               /**< A generic error happens */
#define CMW_ETIMEOUT                     2               /**< Timed out */
#define CMW_EFULL                        3               /**< The resource is full */
#define CMW_EEMPTY                       4               /**< The resource is empty */
#define CMW_ENOMEM                       5               /**< No memory */
#define CMW_ENOSYS                       6               /**< No system */
#define CMW_EBUSY                        7               /**< Busy */
#define CMW_EIO                          8               /**< IO error */
#define CMW_EINTR                        9               /**< Interrupted system call */
#define CMW_EINVAL                       10              /**< Invalid argument */

#ifdef __GNUC__
#  define CMW_NOINLINE __attribute__((noinline))
#  define CMW_SECTION(name) __attribute__ ((section (name)))
#  define CMW_UNUSED __attribute__((unused))
#else
#  define CMW_SECTION(name)
#  define CMW_NOINLINE
#  define CMW_UNUSED
#endif

/**
 * @brief uint32 bit define
 *
 */
typedef struct
{
    cmw_u32_t u32_bit_0: 1;
    cmw_u32_t u32_bit_1: 1;
    cmw_u32_t u32_bit_2: 1;
    cmw_u32_t u32_bit_3: 1;
    cmw_u32_t u32_bit_4: 1;
    cmw_u32_t u32_bit_5: 1;
    cmw_u32_t u32_bit_6: 1;
    cmw_u32_t u32_bit_7: 1;
    cmw_u32_t u32_bit_8: 1;
    cmw_u32_t u32_bit_9: 1;
    cmw_u32_t u32_bit_10: 1;
    cmw_u32_t u32_bit_11: 1;
    cmw_u32_t u32_bit_12: 1;
    cmw_u32_t u32_bit_13: 1;
    cmw_u32_t u32_bit_14: 1;
    cmw_u32_t u32_bit_15: 1;
    cmw_u32_t u32_bit_16: 1;
    cmw_u32_t u32_bit_17: 1;
    cmw_u32_t u32_bit_18: 1;
    cmw_u32_t u32_bit_19: 1;
    cmw_u32_t u32_bit_20: 1;
    cmw_u32_t u32_bit_21: 1;
    cmw_u32_t u32_bit_22: 1;
    cmw_u32_t u32_bit_23: 1;
    cmw_u32_t u32_bit_24: 1;
    cmw_u32_t u32_bit_25: 1;
    cmw_u32_t u32_bit_26: 1;
    cmw_u32_t u32_bit_27: 1;
    cmw_u32_t u32_bit_28: 1;
    cmw_u32_t u32_bit_29: 1;
    cmw_u32_t u32_bit_30: 1;
    cmw_u32_t u32_bit_31: 1;
} cmw_uint32_bit_t;


/**
 * @brief 通过结构体(tpye)中的成员(member)找到结构体指针(ptr)
 *
 */
#define cmw_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


/* 内存管理 */
void *cmw_malloc ( cmw_size_t size );
void *cmw_realloc ( void *rmem, cmw_size_t newsize );
void cmw_free ( void *rmem );



void cmw_assert_handler(const char *ex_string, const char *func, cmw_u32_t line);
#define CMW_ASSERT(EX)                                                        \
    if (!(EX))                                                                    \
    {                                                                             \
        cmw_assert_handler(#EX, __FUNCTION__, __LINE__);                          \
    }

#ifdef __cplusplus
}
#endif

#endif /* __CMW_DEF_H__ */




