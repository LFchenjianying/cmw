/**
 * @file cmw.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __CMW_H__
#define __CMW_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "cmw_define.h"


#ifdef CMW_USING_LAT
	#include "cmw_lat.h"
#endif

#ifdef CMW_USING_TIMESLICE
	#include "cmw_timeslice.h"
#endif


#ifdef CMW_USING_RQUEUE
	#include "cmw_queue.h"
#endif

#ifdef CMW_USING_SMD          // 因为SMD使用了RQUEUE所以 必须先包含cmw_queue.h这个头文件
	#include "cmw_smd.h"
#endif

#ifdef CMW_USING_BUTTON
	#include "cmw_button.h"
#endif

#ifdef CMW_USING_CTRL
	#include "cmw_ctrl.h"
#endif

#ifdef CMW_USING_SOFT_SPI
	#include "cmw_soft_spi.h"
#endif

#ifdef CMW_USING_SOFT_IIC
	#include "cmw_soft_iic.h"
#endif



#if defined(CMW_USING_LIST) || defined(CMW_USING_SLIST)
	#include "cmw_list.h"
#endif




#ifdef __cplusplus
}
#endif


#endif /* __CMW_H__ */

