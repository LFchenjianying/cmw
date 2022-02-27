/**
 * @file cmw_ctrl.c
 *
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 * @version 0.1
 * @date 2021-06-10
 *
 * @copyright Copyright (c) 2021
 *
 */




#define  M_CMW_CTRL_EXTERN_DEF
#include "cmw_ctrl.h"


/*
                                        �ٲ���
                                        ����������������������������                                        һ��ִ�л���                                 ����ִ�нṹ                                 ����ִ�нṹ
                                        |           |                                               ����������������������������                               ____________
                                        |   ��     |                                             | ִ          |                               |          |
                                        |   ��     |    ��������0��ʾ�޶�����   |  ��        |      ��������0��ʾ�޶�����      |          |   ��������0��ʾ�޶�����
                                        |   ��     | ========================= |   ��          |     =========================     |          |  =========================
            ------------                |   ��     |    ���������0��ʾ�޲����� |  ��       |  ���������0��ʾ�޲�����       |          |   ���������0��ʾ�޲�����
            |  �������     |=========  |  ��      | =========================  |               |      =========================  |            |   =========================
            ------------                |   ��     |                                           |                  |                                 |          |
                                        |           |                                         ����������������������������                                ����������������������������
                                        |           |
                                        |           |
                                        ����������������������������
*/


/**
 * @brief ʹ�ÿ������
 *
 */
#if defined(CMW_USING_CTRL)


/**
 * @brief ð������,�������е����ݴ�С�����������
 *
 * @param pu32_array
 * @param u32_array_length
 */
static void cmw_ctrl_bubble_sort ( cmw_u32_t *pu32_array, cmw_u32_t u32_array_length )
{
    cmw_u32_t u32_cnt1 = 0, u32_cnt2 = 0;
    cmw_u32_t cmw_u32_temp_data = 0;

    for ( u32_cnt1 = 0; u32_cnt1 < ( u32_array_length - 1 ); u32_cnt1++ )
    {
        for ( u32_cnt2 = 0; u32_cnt2 < ( u32_array_length - u32_cnt1 - 1 ); u32_cnt2++ )
        {
            if ( pu32_array[u32_cnt2] > pu32_array[u32_cnt2 + 1] )
            {
                cmw_u32_temp_data          = pu32_array[u32_cnt2];
                pu32_array[u32_cnt2]   = pu32_array[u32_cnt2 + 1];
                pu32_array[u32_cnt2 + 1] = cmw_u32_temp_data;
            }
        } /* for(u32_cnt2 = 0; u32_cnt2 < (u32_array_length - u32_cnt1 - 1); u32_cnt2++) */
    }
}


/**
 * @brief ��ָ���Ŀ��ƶ�����ӿ�����������ֵԽ�����ȼ�Խ��
 *
 * @param pst_ctrl_obj
 * @param u32_priority
 * @param f_ctrl_condition
 *
 */
static void ctrl_add_condition ( cmw_ctrl_obj_st *pst_ctrl_obj, cmw_u32_t u32_priority, cmw_u32_t ( *f_ctrl_condition ) ( void *p_input_par ) )
{
    if ( u32_priority >= C_CMW_CTRL_LEVEL_MAX ) { return; }

    if ( pst_ctrl_obj->out_data.u32_conditiion_num < C_CMW_CTRL_LEVEL_MAX )
    {
        pst_ctrl_obj->in_method.f_ctrl_condition[u32_priority] = f_ctrl_condition;
        pst_ctrl_obj->out_data.u32_condition_table[pst_ctrl_obj->out_data.u32_conditiion_num++] = u32_priority;
    }

    /* ��ǰ��Num����Ԫ�������� */
    cmw_ctrl_bubble_sort ( pst_ctrl_obj->out_data.u32_condition_table, pst_ctrl_obj->out_data.u32_conditiion_num );
}



/**
 * @brief �޸��������ָ��
 *
 * @param pst_ctrl_obj
 * @param p_input_par
 */
static void ctrl_remodify_Input_par ( cmw_ctrl_obj_st *pst_ctrl_obj, void *p_input_par )
{
    pst_ctrl_obj->out_data.p_input_par = p_input_par;
}

/**
 * @brief ��ѵ�������������ݽ������Ӧ����
 *
 * @param pst_ctrl_obj
 */
static void ctrl_loop_exec ( cmw_ctrl_obj_st *pst_ctrl_obj )
{
    cmw_u32_t u32Cnt = 0;
    cmw_u32_t u32RetOp = 0;

    if ( pst_ctrl_obj->out_data.u32_conditiion_num > C_CMW_CTRL_LEVEL_MAX ) { return; }

    for ( u32Cnt = 0; u32Cnt < pst_ctrl_obj->out_data.u32_conditiion_num; u32Cnt++ )
    {
        if ( pst_ctrl_obj->in_method.f_ctrl_condition[pst_ctrl_obj->out_data.u32_condition_table[u32Cnt]] )
        {
            u32RetOp = pst_ctrl_obj->in_method.f_ctrl_condition[pst_ctrl_obj->out_data.u32_condition_table[u32Cnt]] ( pst_ctrl_obj->out_data.p_input_par );

            if ( u32RetOp )         /* ��ѯ���в�������������ѭ�� */
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    /* ��һ����������ִ�и��ݲ�����ִ����ز��� */
    if ( u32RetOp )
    {
        if ( pst_ctrl_obj->in_method.f_actuator_1 )
        {
            u32RetOp = pst_ctrl_obj->in_method.f_actuator_1 ( u32RetOp, pst_ctrl_obj->out_data.p_input_par );
        }
    }

    if ( u32RetOp )
    {
        if ( pst_ctrl_obj->in_method.f_actuator_2 )
        {
            u32RetOp = pst_ctrl_obj->in_method.f_actuator_2 ( u32RetOp, pst_ctrl_obj->out_data.p_input_par );
        }
    }

    if ( u32RetOp )
    {
        if ( pst_ctrl_obj->in_method.f_actuator_3 )
        {
            u32RetOp = pst_ctrl_obj->in_method.f_actuator_3 ( u32RetOp, pst_ctrl_obj->out_data.p_input_par );
        }
    }
}

/**
 * @brief ���ƶ����ʼ��
 *
 * @param pst_ctrl_obj
 * @param p_input_par
 * @param f_actuator_1
 * @param f_actuator_2
 * @param f_actuator_3
 */
void cmw_ctrl_obj_config ( cmw_ctrl_obj_st *pst_ctrl_obj, void *p_input_par,
                           cmw_u32_t ( *f_actuator_1 ) ( cmw_u32_t u32_input_op, void *p_input_par ),
                           cmw_u32_t ( *f_actuator_2 ) ( cmw_u32_t u32_input_op, void *p_input_par ),
                           cmw_u32_t ( *f_actuator_3 ) ( cmw_u32_t u32_input_op, void *p_input_par ) )
{
    cmw_u32_t u32Cnt = 0;
    /* ����ж� */
    CMW_ASSERT ( pst_ctrl_obj );
    CMW_ASSERT ( f_actuator_1 );
    CMW_ASSERT ( f_actuator_2 );
    CMW_ASSERT ( f_actuator_3 );
    pst_ctrl_obj->out_data.p_input_par = p_input_par;
    pst_ctrl_obj->in_method.f_actuator_1 = f_actuator_1;
    pst_ctrl_obj->in_method.f_actuator_2 = f_actuator_2;
    pst_ctrl_obj->in_method.f_actuator_3 = f_actuator_3;

    for ( u32Cnt = 0; u32Cnt > C_CMW_CTRL_LEVEL_MAX; u32Cnt++ )
    {
        pst_ctrl_obj->in_method.f_ctrl_condition[u32Cnt] = 0;
    }

    pst_ctrl_obj->out_data.u32_conditiion_num = 0;
    pst_ctrl_obj->out_method.f_add_condition  = ctrl_add_condition;
    pst_ctrl_obj->out_method.f_loop          = ctrl_loop_exec;
    pst_ctrl_obj->out_method.f_remodify_input_par = ctrl_remodify_Input_par;
}



#endif // #if (CMW_USING_CTRL)


