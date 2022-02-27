#ifndef M_MDL_MOTOR_H
#define M_MDL_MOTOR_H

#ifdef M_MDL_MOTOR_EXTERN_DEF
	#define M_MDL_MOTOR_EXTERN
#else 
	#define M_MDL_MOTOR_EXTERN extern
#endif /* M_MDL_MOTORR_EXTERN_DEF */
#include "hmc_type.h"


typedef struct {
  Uint32       u32CurSpeed;       // 实时速度

	
// 启动时计算的参数	
  Uint32       uint32_acc_pulse_num;    // 加减速脉冲需要的数量

// 预设参数	
  Uint32       u32SetAcc;        // 加速度
  Uint32       u32SetStartSpeed;       // 起始速度
  Uint32       u32SetAccStep;   // 加速脉冲步长
} MdlStepMotorCtrlStDef;


// 电机启动参数初始化 加速步长 启停速度 加速度 加速脉冲上限
void f_MdlStepMotorConfig(MdlStepMotorCtrlStDef *pstStepMotorCtl, \
	                    Uint32 u32StartSpeed, \
			      Uint32 u32Acc, \
			      Uint32 u32AccStep
				     );
			      
// 电机启动函数 设置启动速度 计算计数脉冲数量 起始加速度
Uint32 f_MdlStepMotorStart(MdlStepMotorCtrlStDef *pstStepMotorCtl, \
					Uint32 u32SetSpeed,\
					Uint32 u32Acc

 );
					
// 计算电机实时速度 返回当前速度  当前已发送脉冲数量	预设脉冲数量	预设速度 （必须大于启停速度）
Uint32 f_MdlStepMotorSpeedCalc(MdlStepMotorCtrlStDef *cc, \
					 Uint32 uint32_cur_pulse_num,\
					 Uint32 uint32_set_pulse_num,\
					 Uint32 u32SetSpeed
);		
	
	
#endif /* M_MDL_MOTOR_H */	

