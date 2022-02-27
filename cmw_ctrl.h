/**
 * @file cmw_ctrl.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 * @version 0.1
 * @date 2021-06-10
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __CMW_CTRL_H__
#define __CMW_CTRL_H__

#ifdef M_CMW_CTRL_EXTERN_DEF
    #define M_CMW_CTRL_EXTERN
#else
    #define M_CMW_CTRL_EXTERN extern
#endif /* M_CMW_CTRL_EXTERN_DEF */

/**************************************************************************
*                               Include Files                            *
**************************************************************************/


#include "cmw_define.h"

#if defined(CMW_USING_CTRL)
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 最大控制等级
 *
 */
#define C_CMW_CTRL_LEVEL_MAX 10

/**
 * @brief 控制对象定义
 *
 */
typedef struct cmw_ctrl_obj
{

    struct ctrl_out_data
    {
        void *p_input_par;                               /* 输入参数指针 */
        cmw_u32_t u32_conditiion_num;
        cmw_u32_t u32_condition_table[C_CMW_CTRL_LEVEL_MAX];    /* 条件列表 */
    } out_data;
    struct ctrl_in_method
    {
        cmw_u32_t (*f_actuator_1)(cmw_u32_t u32InOp, void *pInPar);            /* 一级执行结构,返回Op，Op==0标识无动作 */
        cmw_u32_t (*f_actuator_2)(cmw_u32_t u32InOp, void *pInPar);            /* 二级执行结构,返回Op，Op==0标识无动作 */
        cmw_u32_t (*f_actuator_3)(cmw_u32_t u32InOp, void *pInPar);            /* 三级执行结构,返回Op，Op==0标识无动作 */
        cmw_u32_t (*f_ctrl_condition[C_CMW_CTRL_LEVEL_MAX])(void *p_input_par);   /* 函数指针数组，返回Op，Op==0标识无动作 */
    } in_method;
    struct ctrl_out_method
    {
        void (*f_loop)(struct cmw_ctrl_obj *pstCtrlObj);
        void (*f_remodify_input_par)(struct cmw_ctrl_obj *pstCtrlObj, void *pInPar);
        void (*f_add_condition)(struct cmw_ctrl_obj *pstCtrlObj, cmw_u32_t u32Priority, cmw_u32_t (*f_ctrl_condition)(void *p_input_par));

    } out_method;
} cmw_ctrl_obj_st;





/**
 * @brief 控制对象初始化
 *
 * @param pstCtrlObj
 * @param pInPar
 * @param f_actuator_1
 * @param f_actuator_2
 * @param f_actuator_3
 */
void cmw_ctrl_obj_config(cmw_ctrl_obj_st *pstCtrlObj, void *pInPar,
                         cmw_u32_t (*f_actuator_1)(cmw_u32_t u32InOp, void *pInPar),
                         cmw_u32_t (*f_actuator_2)(cmw_u32_t u32InOp, void *pInPar),
                         cmw_u32_t (*f_actuator_3)(cmw_u32_t u32InOp, void *pInPar));



#ifdef __cplusplus
}
#endif

#endif /* CMW_USING_CTRL */




#endif  /* __CMW_CTRL_H__ */


