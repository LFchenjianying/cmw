/**
 * @file mdl.h
 * @author {chenjianying} ({LFchenjianying@outlook.com})
 * @brief 
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __MDL_H__
#define __MDL_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "mdl_def.h"


#ifdef MDL_USING_CTRL
	#include "mdl_ctrl.h"
#endif

#ifdef MDL_USING_SPI
	#include "mdl_spi.h"
#endif

#ifdef MDL_USING_LAT
	#include "mdl_lat.h"
#endif

#ifdef MDL_USING_TIMESLICE
	#include "mdl_rr.h"
#endif

#ifdef MDL_USING_SMD
	#include "mdl_smd.h"
#endif

#ifdef MDL_USING_BTN
	#include "mdl_btn.h"
#endif

#ifdef __cplusplus
}
#endif


#endif /* __MDL_H__ */

