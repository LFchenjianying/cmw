/**
 * @file mdl_ctrl.c
 * 
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */




#define  M_MDL_CTRL_EXTERN_DEF
#include "mdl_ctrl.h"


/*
							            仲裁器
								        ——————————————							              一级执行机构								 二级执行结构          						三级执行结构
								        |			|						                     	——————————————                               ____________
								        |	多  	|							                  |	执		     |								 |			|
								        |	级  	|    操作数（0表示无动作）   |  行	     |		操作数（0表示无动作）	  	 |			|   操作数（0表示无动作）
								        |	条  	| ========================= |	机		     |     =========================	 |			|  =========================
			------------		        |	件		|	 输入参数（0表示无参数） |  构	     | 	输入参数（0表示无参数）		 |			|   输入参数（0表示无参数）
			|  输入参数     |=========  |  仲		| =========================  |			     |      =========================  |			|   =========================
			------------			    |	裁		|						                    |			       |								 |			|
							            |			|						                  ——————————————								——————————————
							            |			|
							            |			|
						                ——————————————
*/


/**
 * @brief 使用控制组件
 * 
 */
#if defined(MDL_USING_CTRL)


/**
 * @brief 冒泡排序,将数组中的数据从小到大进行排列
 * 
 * @param pu32_array 
 * @param u32_array_length 
 */
static void mdl_ctrl_bubble_sort(mdl_u32_t *pu32_array, mdl_u32_t u32_array_length)
{
	mdl_u32_t u32_cnt1 = 0, u32_cnt2 = 0;
	mdl_u32_t mdl_u32_temp_data = 0;
	
	for(u32_cnt1 = 0; u32_cnt1 < (u32_array_length - 1); u32_cnt1++)
	{
		for(u32_cnt2 = 0; u32_cnt2 < (u32_array_length - u32_cnt1 - 1); u32_cnt2++)
		{
			if(pu32_array[u32_cnt2] > pu32_array[u32_cnt2+1])
			{
				mdl_u32_temp_data          = pu32_array[u32_cnt2];
				pu32_array[u32_cnt2]   = pu32_array[u32_cnt2+1];
				pu32_array[u32_cnt2+1] = mdl_u32_temp_data;
			}
		} /* for(u32_cnt2 = 0; u32_cnt2 < (u32_array_length - u32_cnt1 - 1); u32_cnt2++) */
	}
}


/**
 * @brief 在指定的控制对象添加控制条件，数值越大优先级越低
 * 
 * @param pst_ctrl_obj 
 * @param u32_priority 
 * @param f_ctrl_condition
 *  
 */
static void ctrl_add_condition(mdl_ctrl_obj_st *pst_ctrl_obj,mdl_u32_t u32_priority, mdl_u32_t (*f_ctrl_condition)(void *p_input_par))
{
	if (u32_priority >= C_MDL_CTRL_LEVEL_MAX) return;

	if (pst_ctrl_obj->out_data.u32_conditiion_num < C_MDL_CTRL_LEVEL_MAX)
	{
		pst_ctrl_obj->in_method.f_ctrl_condition[u32_priority] = f_ctrl_condition;
		
		pst_ctrl_obj->out_data.u32_condition_table[pst_ctrl_obj->out_data.u32_conditiion_num++] = u32_priority;
	}
	/* 对前面Num个单元进行排序 */
	mdl_ctrl_bubble_sort(pst_ctrl_obj->out_data.u32_condition_table, pst_ctrl_obj->out_data.u32_conditiion_num);
}



/**
 * @brief 修改输入参数指针
 * 
 * @param pst_ctrl_obj 
 * @param p_input_par 
 */
static void ctrl_remodify_Input_par(mdl_ctrl_obj_st *pst_ctrl_obj, void *p_input_par)
{
	pst_ctrl_obj->out_data.p_input_par = p_input_par;
}

/**
 * @brief 轮训控制条件并根据结果做对应操作
 * 
 * @param pst_ctrl_obj 
 */
static void ctrl_loop_exec(mdl_ctrl_obj_st *pst_ctrl_obj)
{
	mdl_u32_t u32Cnt = 0;
	mdl_u32_t u32RetOp = 0;

	if (pst_ctrl_obj->out_data.u32_conditiion_num > C_MDL_CTRL_LEVEL_MAX) return;

	for (u32Cnt = 0; u32Cnt < pst_ctrl_obj->out_data.u32_conditiion_num; u32Cnt++)
	{
		if (pst_ctrl_obj->in_method.f_ctrl_condition[pst_ctrl_obj->out_data.u32_condition_table[u32Cnt]])
		{
			u32RetOp = pst_ctrl_obj->in_method.f_ctrl_condition[pst_ctrl_obj->out_data.u32_condition_table[u32Cnt]](pst_ctrl_obj->out_data.p_input_par);
			if (u32RetOp) 			/* 查询到有操作数立刻跳出循环 */
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	
	/* 从一级到三级逐级执行根据操作数执行相关操作 */
	if (u32RetOp)
	{
		if ( pst_ctrl_obj->in_method.f_actuator_1)
		{
			u32RetOp = pst_ctrl_obj->in_method.f_actuator_1(u32RetOp, pst_ctrl_obj->out_data.p_input_par);
		}
	}
	if (u32RetOp)
	{
		if (pst_ctrl_obj->in_method.f_actuator_2)
		{
			u32RetOp = pst_ctrl_obj->in_method.f_actuator_2(u32RetOp, pst_ctrl_obj->out_data.p_input_par);
		}
	}
	if (u32RetOp)
	{
		if (pst_ctrl_obj->in_method.f_actuator_3)
		{
			u32RetOp = pst_ctrl_obj->in_method.f_actuator_3(u32RetOp, pst_ctrl_obj->out_data.p_input_par);
		}
	}
}

/**
 * @brief 控制对象初始化
 * 
 * @param pst_ctrl_obj 
 * @param p_input_par 
 * @param f_actuator_1 
 * @param f_actuator_2 
 * @param f_actuator_3 
 */
void mdl_ctrl_obj_config(mdl_ctrl_obj_st *pst_ctrl_obj, void *p_input_par,
	mdl_u32_t (*f_actuator_1)(mdl_u32_t u32_input_op, void *p_input_par),
	mdl_u32_t (*f_actuator_2)(mdl_u32_t u32_input_op, void *p_input_par),
	mdl_u32_t (*f_actuator_3)(mdl_u32_t u32_input_op, void *p_input_par))
{
	mdl_u32_t u32Cnt = 0;
	/* 入参判断 */
	MDL_ASSERT(pst_ctrl_obj);
	MDL_ASSERT(f_actuator_1);
	MDL_ASSERT(f_actuator_2);
	MDL_ASSERT(f_actuator_3);

	pst_ctrl_obj->out_data.p_input_par = p_input_par;
	pst_ctrl_obj->in_method.f_actuator_1 = f_actuator_1;
	pst_ctrl_obj->in_method.f_actuator_2 = f_actuator_2;
	pst_ctrl_obj->in_method.f_actuator_3 = f_actuator_3;
	for (u32Cnt = 0; u32Cnt > C_MDL_CTRL_LEVEL_MAX; u32Cnt++)
	{
		pst_ctrl_obj->in_method.f_ctrl_condition[u32Cnt] = 0;
	}
	pst_ctrl_obj->out_data.u32_conditiion_num = 0;


	pst_ctrl_obj->out_method.f_add_condition  = ctrl_add_condition;
	pst_ctrl_obj->out_method.f_loop          = ctrl_loop_exec;
	pst_ctrl_obj->out_method.f_remodify_input_par = ctrl_remodify_Input_par;
}



#endif // #if (MDL_USING_CTRL)


