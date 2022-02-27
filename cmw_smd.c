/**
 * @file cmw_smd.c
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 步进电机驱动
 *
 *          1. 实现任意三轴插补
 *          2. 实现独立轴的T形加减速
 *          3. 实现连读段前瞻平滑
 * @version 0.1
 * @date 2021-08-06
 *
 * @copyright Copyright (c) 2021
 *

 */
#define M_CMW_SSMD_EXTERN_DEF
#include "cmw.h"

#if defined(CMW_USING_SMD)

/**
 * @brief 步进器
 *          放在定时器中断服务函数中调用，用于计算下个脉冲的速度（频率）
 *          处理 脉冲 IO 刹车等功能
 *
 * @param p_ssmd
 * @return cmw_u32_t
 */
static cmw_u32_t ssmd_stepper ( ssmd_t *p_ssmd )
{
    struct ssmd_segment_st next_segment;

    if ( p_ssmd->run_state.pluse_io_state )
    {
        p_ssmd->config.pluse_io_high();
        p_ssmd->run_state.pluse_io_state = 0;
        return p_ssmd->run_state.speed;
    }
    else
    {
        p_ssmd->config.pluse_io_low();
        p_ssmd->run_state.pluse_io_state = 1;

        if ( p_ssmd->run_state.is_brake == TRUE ||
                p_ssmd->run_state.is_enable == FALSE )
        {
            p_ssmd->run_state.is_running = FALSE;
            p_ssmd->run_state.speed = p_ssmd->config.start_stop_speed;  // 立刻重置为起始速度
            // 清空缓冲垫
            p_ssmd->run_state.remain_distance = 0;
            p_ssmd->segment_queue.out_method.reset ( &p_ssmd->segment_queue );
            p_ssmd->plan_block_queue.out_method.reset ( &p_ssmd->plan_block_queue );
            return 0;
        }

        if ( p_ssmd->run_state.remain_distance )
        {
            if ( p_ssmd->run_state.current_dir )
            {
                p_ssmd->run_state.position++;
            }
            else
            {
                p_ssmd->run_state.position--;
            }

            p_ssmd->run_state.remain_distance -= 1;
        }

        // 剩余距离已经走完的时候取出下一个运动段
        if ( p_ssmd->run_state.remain_distance == 0 )
        {
            // 取出下一运动段
            if ( p_ssmd->segment_queue.out_method.dequeue ( &p_ssmd->segment_queue, &next_segment ) == TRUE )
            {
                if ( p_ssmd->run_state.current_dir == next_segment.dir )
                {
                    p_ssmd->run_state.speed += p_ssmd->run_state.acceleration;
                }

                p_ssmd->run_state.acceleration = next_segment.acceleration;
                p_ssmd->run_state.remain_distance = next_segment.distance;
                p_ssmd->run_state.current_dir = next_segment.dir;

                // 更换方向
                if ( p_ssmd->run_state.current_dir )
                {
                    p_ssmd->config.dir_forward();
                }
                else
                {
                    p_ssmd->config.dir_reverse();
                }

                return ( p_ssmd->run_state.speed );
            }
            else
            {
                p_ssmd->run_state.is_running = FALSE;
                return 0;
            }
        }
        else
        {
            p_ssmd->run_state.speed += p_ssmd->run_state.acceleration;
            return ( p_ssmd->run_state.speed );
        }
    }
}

/**
 * @brief 运动块分段处理
 *          将运动块分成多段（1-3）执行
 *
 * @param p_ssmd
 * @param p_temp_block
 * @return cmw_bool_t
 */
static cmw_bool_t ssmd_segment ( ssmd_t *p_ssmd, struct ssmd_block_st *p_temp_block )
{
    cmw_bool_t ret = FALSE;
    struct ssmd_segment_st segment_1;
    struct ssmd_segment_st segment_2;
    struct ssmd_segment_st segment_3;
    cmw_u32_t acc_pluse_count = 0;  /* 加减速脉冲数量 */
    cmw_u32_t segment_pluse_count = 0;
    acc_pluse_count = ( ( p_temp_block->speed - p_ssmd->config.start_stop_speed ) / p_ssmd->config.acceleration );

    // 单段
    if ( p_temp_block->speed == p_ssmd->config.start_stop_speed ||
            p_ssmd->config.acceleration == 0 ||
            acc_pluse_count == 0 ||
            p_temp_block->distance <= acc_pluse_count )
    {
        segment_1.dir          = p_temp_block->dir;
        segment_1.acceleration = 0;
        segment_1.distance     = p_temp_block->distance;

        if ( p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_1 ) > 0 )
        {
            ret = TRUE;
        }
        else
        {
            ret = FALSE;
        }
    }
    else if ( p_temp_block->distance <= 2 * acc_pluse_count ) // 两段
    {
        segment_pluse_count =  p_temp_block->distance / 2;
        segment_1.dir = p_temp_block->dir;
        segment_2.dir = p_temp_block->dir;
        segment_1.acceleration = p_ssmd->config.acceleration;
        segment_2.acceleration = -p_ssmd->config.acceleration;
        segment_1.distance     = p_temp_block->distance - ( segment_pluse_count );
        segment_2.distance     = segment_pluse_count;

        if ( p_ssmd->segment_queue.out_method.get_remain_length ( &p_ssmd->segment_queue ) >= 2 )
        {
            p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_1 );
            p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_2 );
            ret = TRUE;
        }
        else
        {
            ret = FALSE;
        }
    }
    else   // 三段
    {
        //        if ( p_temp_block->distance <= ( 2 * acc_pluse_count + 3 ) )
        //        {
        //            acc_pluse_count -= 1;
        //        }
        segment_pluse_count =  p_temp_block->distance / 3;
        segment_1.dir = p_temp_block->dir;
        segment_2.dir = p_temp_block->dir;
        segment_3.dir = p_temp_block->dir;
        segment_1.acceleration = p_ssmd->config.acceleration;
        segment_2.acceleration = 0;
        segment_3.acceleration = -p_ssmd->config.acceleration;
        segment_1.distance     = acc_pluse_count ; // + 1
        segment_2.distance     = p_temp_block->distance - ( 2 * acc_pluse_count ); //  - 3
        segment_3.distance     = acc_pluse_count ; // + 2

        if ( p_ssmd->segment_queue.out_method.get_remain_length ( &p_ssmd->segment_queue ) >= 2 )
        {
            p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_1 );
            p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_2 );
            p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_3 );
            ret = TRUE;
        }
        else
        {
            ret = FALSE;
        }
    }

    return ret;
}

/**
 * @brief 单轴运动段规划：三段式加减速 结合下一个块的加减速 开始运动后要循环调用
 *
 * @param p_ssmd
 * @return cmw_bool_t
 */
static cmw_bool_t ssmd_prepare_segment ( ssmd_t *p_ssmd )
{
    cmw_u32_t segment_pluse_count = 0;
    struct ssmd_block_st temp_block;
    cmw_bool_t ret = TRUE;
    struct ssmd_segment_st *p_last_segment = NULL;

    if ( p_ssmd == NULL ) { return FALSE; }

    if ( p_ssmd->run_state.is_enable == FALSE || p_ssmd->run_state.is_brake == TRUE )
    {
        p_ssmd->config.disable();
        return FALSE;
    }
    else
    {
        p_ssmd->config.enable();
    }

    // 获取运动控制块
    if ( p_ssmd->plan_block_queue.out_method.dequeue ( &p_ssmd->plan_block_queue, &temp_block ) == FALSE )
    {
        return FALSE;
    }

    // 剩余运动段数量必须大于三
    if ( p_ssmd->segment_queue.out_method.get_remain_length ( &p_ssmd->segment_queue ) < 3 )
    {
        return FALSE;
    }

    //如果该段速度为0 则直接跳过该段
    if ( temp_block.speed == 0 )
    {
        return FALSE;
    }

    // 电机起始速度和加速度都为0时直接跳过该段
    if ( p_ssmd->config.start_stop_speed == 0 && p_ssmd->config.acceleration == 0 )
    {
        return FALSE;
    }

    // 限制最低启动停止速度
    if ( temp_block.speed < p_ssmd->config.start_stop_speed )
    {
        temp_block.speed = p_ssmd->config.start_stop_speed;
    }

    // 如果当前速度小于起始速度,限制最低运动速度
    if ( p_ssmd->run_state.speed < p_ssmd->config.start_stop_speed )
    {
        p_ssmd->run_state.speed = p_ssmd->config.start_stop_speed;
    }

    // 如果起始速度为0，那么起始速度被赋值为加速度
    if ( p_ssmd->config.start_stop_speed == 0 )
    {
        p_ssmd->config.start_stop_speed = p_ssmd->config.acceleration;
    }

    // 前瞻处理
    if ( p_ssmd->segment_queue.out_method.get_rear_addr ( &p_ssmd->segment_queue, ( void ** ) &p_last_segment ) == TRUE )
    {
        if ( p_last_segment->acceleration == 0 ||
                ( p_last_segment->dir != temp_block.dir ) ||
                ( temp_block.speed == p_ssmd->config.start_stop_speed ) )
        {
            ret = ssmd_segment ( p_ssmd, &temp_block );
        }
        else // 此处只能是p_last_segment->acceleration < 0
        {
            struct ssmd_segment_st segment_1;
            struct ssmd_segment_st segment_2;
            struct ssmd_segment_st segment_3;
            cmw_u32_t last_block_speed = 0;
            cmw_u32_t acceleration_count = 0;  /* 加减速脉冲数量 */
            cmw_u32_t deceleration_count = 0; /* 减速脉冲数量 */
            last_block_speed = ( -p_last_segment->acceleration ) * p_last_segment->distance + p_ssmd->config.start_stop_speed;

            if ( last_block_speed < temp_block.speed ) // 加速
            {
                acceleration_count = ( ( temp_block.speed - last_block_speed ) / p_ssmd->config.acceleration );
                deceleration_count = ( ( temp_block.speed - p_ssmd->config.start_stop_speed ) / p_ssmd->config.acceleration );

                if ( temp_block.distance <= deceleration_count ) // 一段
                {
                    segment_1.distance = p_last_segment->distance;
                    segment_1.acceleration = p_last_segment->acceleration;
                    segment_1.dir  = p_last_segment->dir;
                    p_last_segment->distance = temp_block.distance;
                    p_last_segment->acceleration = 0;
                    p_last_segment->dir = temp_block.dir;

                    if ( p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_1 ) > 0 )
                    {
                        ret = TRUE;
                    }
                    else
                    {
                        ret = FALSE;
                    }
                }
                else if ( temp_block.distance <= ( deceleration_count + acceleration_count ) ) // 两段
                {
                    deceleration_count = temp_block.distance * ( ( ( float ) deceleration_count ) / ( acceleration_count + deceleration_count ) );
                    p_last_segment->acceleration = 0;   // 将上一段改为匀速
                    segment_1.dir  = temp_block.dir;
                    segment_2.dir  = temp_block.dir;
                    segment_1.acceleration = p_ssmd->config.acceleration;
                    segment_2.acceleration = -p_ssmd->config.acceleration;
                    segment_1.distance = temp_block.distance - deceleration_count;
                    segment_2.distance = deceleration_count;

                    if ( p_ssmd->segment_queue.out_method.get_remain_length ( &p_ssmd->segment_queue ) >= 2 )
                    {
                        p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_1 );
                        p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_2 );
                        ret = TRUE;
                    }
                    else
                    {
                        ret = FALSE;
                    }
                }
                else   // 三段
                {
                    p_last_segment->acceleration = 0;   // 将上一段改为匀速
                    segment_1.dir  = temp_block.dir;
                    segment_2.dir  = temp_block.dir;
                    segment_3.dir  = temp_block.dir;
                    segment_1.acceleration = p_ssmd->config.acceleration;
                    segment_2.acceleration = 0;
                    segment_3.acceleration = -p_ssmd->config.acceleration;
                    segment_1.distance = acceleration_count;
                    segment_2.distance = temp_block.distance - deceleration_count - acceleration_count;
                    segment_3.distance = deceleration_count;

                    if ( p_ssmd->segment_queue.out_method.get_remain_length ( &p_ssmd->segment_queue ) >= 2 )
                    {
                        p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_1 );
                        p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_2 );
                        p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_3 );
                        ret = TRUE;
                    }
                    else
                    {
                        ret = FALSE;
                    }
                }
            }
            else if ( last_block_speed > temp_block.speed ) // 减速
            {
                acceleration_count = ( ( last_block_speed - temp_block.speed ) / p_ssmd->config.acceleration );
                deceleration_count = ( ( temp_block.speed - p_ssmd->config.start_stop_speed ) / p_ssmd->config.acceleration );

                if ( temp_block.distance <= ( deceleration_count + acceleration_count ) ||
                        ( deceleration_count == 0 ) ) // 一段
                {
                    segment_1.distance = p_last_segment->distance;
                    segment_1.acceleration = p_last_segment->acceleration;
                    segment_1.dir  = p_last_segment->dir;
                    p_last_segment->distance = temp_block.distance;
                    p_last_segment->acceleration = 0;
                    p_last_segment->dir = temp_block.dir;

                    if ( p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_1 ) > 0 )
                    {
                        ret = TRUE;
                    }
                    else
                    {
                        ret = FALSE;
                    }
                }
                else   // 三段
                {
                    p_last_segment->acceleration = 0;   // 将上一段改为匀速
                    segment_1.dir  = temp_block.dir;
                    segment_2.dir  = temp_block.dir;
                    segment_3.dir  = temp_block.dir;
                    segment_1.acceleration = -p_ssmd->config.acceleration;
                    segment_2.acceleration = 0;
                    segment_3.acceleration = -p_ssmd->config.acceleration;
                    segment_1.distance = acceleration_count;
                    segment_2.distance = temp_block.distance - deceleration_count - acceleration_count;
                    segment_3.distance = deceleration_count;

                    if ( p_ssmd->segment_queue.out_method.get_remain_length ( &p_ssmd->segment_queue ) >= 2 )
                    {
                        p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_1 );
                        p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_2 );
                        p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_3 );
                        ret = TRUE;
                    }
                    else
                    {
                        ret = FALSE;
                    }
                }
            }
            else
            {
                segment_1.distance = p_last_segment->distance;
                segment_1.acceleration = p_last_segment->acceleration;
                segment_1.dir  = p_last_segment->dir;
                p_last_segment->distance = temp_block.distance;
                p_last_segment->acceleration = 0;
                p_last_segment->dir = temp_block.dir;

                if ( p_ssmd->segment_queue.out_method.enqueue ( &p_ssmd->segment_queue, &segment_1 ) > 0 )
                {
                    ret = TRUE;
                }
                else
                {
                    ret = FALSE;
                }
            }
        }
    }
    else
    {
        ret = ssmd_segment ( p_ssmd, &temp_block );
    }

    if ( ret == TRUE )
    {
        if ( p_ssmd->run_state.is_running == FALSE )
        {
            if ( p_ssmd->run_state.current_dir )
            {
                p_ssmd->config.dir_forward();
            }
            else
            {
                p_ssmd->config.dir_reverse();
            }

            // 根据起始速度计算第一个脉冲速度
            p_ssmd->run_state.speed = p_ssmd->config.start_stop_speed;
            p_ssmd->run_state.is_running = TRUE;
            p_ssmd->config.start();
        }
    }

    return ret;
}
/**
 * @brief 单轴运动块规划：加减速需要的脉冲数量 用户规划一次运动后调用
 *
 * @param p_ssmd
 * @param p_block
 * @return cmw_bool_t
 */
static cmw_bool_t ssmd_plan_block ( ssmd_t *p_ssmd, struct ssmd_block_st *p_block )
{
    if ( p_ssmd == NULL ) { return FALSE; }

    if ( p_ssmd->plan_block_queue.out_method.get_remain_length ( &p_ssmd->plan_block_queue ) > 0 )
    {
        p_ssmd->plan_block_queue.out_method.enqueue ( &p_ssmd->plan_block_queue, p_block );
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * @brief 刹车使能或者失能
 *
 * @param p_ssmd
 * @param is_brake
 */
static void ssmd_brake ( ssmd_t *p_ssmd, cmw_bool_t is_brake )
{
    p_ssmd->run_state.is_brake = is_brake;
}

/**
 * @brief 使能或者失能电机
 *
 * @param p_ssmd
 * @param is_enabled
 */
static void ssmd_enable ( ssmd_t *p_ssmd, cmw_bool_t is_enable )
{
    p_ssmd->run_state.is_enable = is_enable;
}

/**
 * @brief 单轴初始化
 *
 * @param p_ssmd
 * @param config
 * @return cmw_bool_t
 */
cmw_bool_t cmw_ssmd_init ( ssmd_t *p_ssmd, struct ssmd_config_st config )
{
    if ( p_ssmd == NULL )
    {
        return FALSE;
    }

    /* 初始化运动段队列，运动规划队列 */
    cmw_ring_queue_create ( &p_ssmd->plan_block_queue, ( cmw_rqueue_data_t * ) p_ssmd->block_buffer, SSMD_PLAN_BLOCK_LENGTH, sizeof ( struct ssmd_block_st ) );
    cmw_ring_queue_create ( &p_ssmd->segment_queue, ( cmw_rqueue_data_t * ) p_ssmd->segment_buffer, SSMD_SEGMENT_LENGTH, sizeof ( struct ssmd_segment_st ) );

    if ( config.pluse_io_high == NULL ||
            config.pluse_io_low  == NULL ||
            config.dir_forward   == NULL ||
            config.dir_reverse   == NULL  ||
            config.enable        == NULL ||
            config.disable       == NULL )
    {
        return FALSE;
    }

    p_ssmd->run_state.is_enable = TRUE;
    p_ssmd->run_state.is_brake = FALSE;
    /* 电机基本属性配置 */
    p_ssmd->config.start_stop_speed = config.start_stop_speed;
    p_ssmd->config.acceleration     = config.acceleration;
    p_ssmd->config.max_speed        = config.max_speed;
    p_ssmd->config.pluse_io_high    = config.pluse_io_high;
    p_ssmd->config.pluse_io_low     = config.pluse_io_low;
    p_ssmd->config.dir_forward      = config.dir_forward;
    p_ssmd->config.dir_reverse      = config.dir_reverse;
    p_ssmd->config.enable           = config.enable;
    p_ssmd->config.disable          = config.disable;
    p_ssmd->config.start            = config.start;
    p_ssmd->config.stop             = config.stop;
    /* 提供给外部调用的方法 */
    p_ssmd->method.plan_block = ssmd_plan_block;
    p_ssmd->method.prepare_segment = ssmd_prepare_segment;
    p_ssmd->method.stepper = ssmd_stepper;
    p_ssmd->method.brake = ssmd_brake;
    p_ssmd->method.enable = ssmd_enable;
    return TRUE;
}

#endif
//
