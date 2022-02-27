/**
 * @file mdl_ctrl.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief
 * @version 0.1
 * @date 2021-06-10
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef __MDL_CTRL_H__
#define __MDL_CTRL_H__

#ifdef M_MDL_CTRL_EXTERN_DEF
    #define M_MDL_CTRL_EXTERN
#else
    #define M_MDL_CTRL_EXTERN extern
#endif /* M_MDL_CTRL_EXTERN_DEF */

/**************************************************************************
*                               Include Files                            *
**************************************************************************/


#include "mdl_def.h"

#if defined(MDL_USING_CTRL)
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 最大控制等级
 *
 */
#define C_MDL_CTRL_LEVEL_MAX 10

/**
 * @brief 控制对象定义
 *
 */
typedef struct mdl_ctrl_obj
{

    struct ctrl_out_data
    {
        void *p_input_par;                               /* 输入参数指针 */
        mdl_u32_t u32_conditiion_num;
        mdl_u32_t u32_condition_table[C_MDL_CTRL_LEVEL_MAX];    /* 条件列表 */
    } out_data;
    struct ctrl_in_method
    {
        mdl_u32_t (*f_actuator_1)(mdl_u32_t u32InOp, void *pInPar);            /* 一级执行结构,返回Op，Op==0标识无动作 */
        mdl_u32_t (*f_actuator_2)(mdl_u32_t u32InOp, void *pInPar);            /* 二级执行结构,返回Op，Op==0标识无动作 */
        mdl_u32_t (*f_actuator_3)(mdl_u32_t u32InOp, void *pInPar);            /* 三级执行结构,返回Op，Op==0标识无动作 */
        mdl_u32_t (*f_ctrl_condition[C_MDL_CTRL_LEVEL_MAX])(void *p_input_par);   /* 函数指针数组，返回Op，Op==0标识无动作 */
    } in_method;
    struct ctrl_out_method
    {
        void (*f_loop)(struct mdl_ctrl_obj *pstCtrlObj);
        void (*f_remodify_input_par)(struct mdl_ctrl_obj *pstCtrlObj, void *pInPar);
        void (*f_add_condition)(struct mdl_ctrl_obj *pstCtrlObj, mdl_u32_t u32Priority, mdl_u32_t (*f_ctrl_condition)(void *p_input_par));

    } out_method;
} mdl_ctrl_obj_st;





/**
 * @brief 控制对象初始化
 *
 * @param pstCtrlObj
 * @param pInPar
 * @param f_actuator_1
 * @param f_actuator_2
 * @param f_actuator_3
 */
void mdl_ctrl_obj_config(mdl_ctrl_obj_st *pstCtrlObj, void *pInPar,
                         mdl_u32_t (*f_actuator_1)(mdl_u32_t u32InOp, void *pInPar),
                         mdl_u32_t (*f_actuator_2)(mdl_u32_t u32InOp, void *pInPar),
                         mdl_u32_t (*f_actuator_3)(mdl_u32_t u32InOp, void *pInPar));



#ifdef __cplusplus
}
#endif

#endif /* MDL_USING_CTRL */




#endif  /* __MDL_CTRL_H__ */


