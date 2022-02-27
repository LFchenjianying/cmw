/**
 * @file cmw_smd.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 * @version 0.1
 * @date 2021-08-06
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __CMW_SMD_H__
#define __CMW_SMD_H__

#ifdef M_CMW_SSMD_EXTERN_DEF
    #define M_CMW_SSMD_EXTERN
#else
    #define M_CMW_SSMD_EXTERN extern
#endif /* M_BSP_SSMD_EXTERN_DEF */

#include "cmw.h"

#if defined(CMW_USING_SMD)

#ifdef __cplusplus
extern "C" {
#endif

// 单轴步进电机驱动 T 型加减速
typedef struct ssmd_st
{
#define SSMD_PLAN_BLOCK_LENGTH (10)
#define SSMD_SEGMENT_LENGTH (3*SSMD_PLAN_BLOCK_LENGTH)
    struct ssmd_config_st
    {
        cmw_u32_t start_stop_speed; /* 最大启动速度 */
        cmw_u32_t acceleration;         /* 最大加速度 */
        cmw_u32_t max_speed;  /* max of speed */
        void ( *pluse_io_high ) ( void );
        void ( *pluse_io_low ) ( void );
        void ( *dir_forward ) ( void );
        void ( *dir_reverse ) ( void );
        void ( *enable ) ( void );
        void ( *disable ) ( void );
        void ( *start ) ( void );
        void ( *stop ) ( void );
    } config;
    struct
    {
        cmw_u32_t speed;     /* 实时速度 */
        cmw_s32_t acceleration;  /* 实时加速度 */
        cmw_s32_t position; /* 实时脉冲位置 */
        cmw_u32_t remain_distance; /* 剩余需要走的距离 */
        cmw_u8_t  pluse_io_state;
        cmw_u8_t  current_dir; /* 当前方向:反向为0 正向为非零 */
        cmw_u8_t  is_running;
        cmw_u8_t  is_enable; /* 是否是能 */
        cmw_u8_t  is_brake;   /* 是否刹车 */
    } run_state;
    struct ssmd_segment_st
    {
        cmw_s32_t acceleration;
        cmw_u32_t distance; /* 距离 */
        cmw_u32_t dir;  /* 方向:反向为0 正向为非零 */
    } segment_buffer[SSMD_SEGMENT_LENGTH]; /* 三段式 T 型加减速*/
    struct ssmd_block_st
    {
        cmw_u32_t dir;  /* 方向:反向为0 正向为非零 */
        cmw_u32_t distance; /* 距离 */
        cmw_u32_t speed;  /* speed */
    } block_buffer[SSMD_PLAN_BLOCK_LENGTH];   /* 运动规划块*/
    struct ssmd_method
    {
        cmw_bool_t ( *prepare_segment ) ( struct ssmd_st *p_ssmd );
        cmw_bool_t ( *plan_block ) ( struct ssmd_st *p_ssmd, struct ssmd_block_st *p_block );
        cmw_u32_t ( *stepper ) ( struct ssmd_st *p_ssmd );
        void ( *brake ) ( struct ssmd_st *p_ssmd, cmw_bool_t is_brake );
        void ( *enable ) ( struct ssmd_st *p_ssmd, cmw_bool_t is_enabled );
    } method;  /* method name */
    cmw_ring_queue_t plan_block_queue; /* queue of plan */
    cmw_ring_queue_t segment_queue;    /* queue of segment */
} ssmd_t;


cmw_bool_t cmw_ssmd_init ( ssmd_t *p_ssmd, struct ssmd_config_st config );

#ifdef __cplusplus
}
#endif

#endif /* __CMW_SMD_H__ */

#endif /* __CMW_SMD_H */

