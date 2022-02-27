#define M_MDL_MOTOR_EXTERN_DEF
#include "mdl.h"

#if (M_INCLUDE_MDL_MOTOR)




// 电机启动参数初始化 加速步长 启停速度 加速度 加速脉冲上限
void f_MdlStepMotorConfig(MdlStepMotorCtrlStDef *pstStepMotorCtl, \
	                    Uint32 u32StartSpeed, \
						Uint32 u32Acc, \
						Uint32 u32AccStep
                          )
{
	pstStepMotorCtl->u32SetStartSpeed = u32StartSpeed;
	pstStepMotorCtl->u32SetAcc  = u32Acc;
	pstStepMotorCtl->u32SetAccStep  = u32AccStep;
}

// 电机启动函数 设置启动速度 计算计数脉冲数量 起始加速度
Uint32 f_MdlStepMotorStart(MdlStepMotorCtrlStDef *pstStepMotorCtl, \
					Uint32 u32SetSpeed,\
					Uint32 u32Acc)
{
	pstStepMotorCtl->u32SetAcc  = u32Acc;
	if (pstStepMotorCtl->u32SetAcc != 0)
	{
		pstStepMotorCtl->u32CurSpeed = pstStepMotorCtl->u32SetStartSpeed;
		pstStepMotorCtl->uint32_acc_pulse_num = (((u32SetSpeed - pstStepMotorCtl->u32SetStartSpeed)/pstStepMotorCtl->u32SetAcc) * \
								pstStepMotorCtl->u32SetAccStep);
		return 0;
	}
	return 1;
}

// 计算电机实时速度 返回当前速度  当前已发送脉冲数量	预设脉冲数量	预设速度 （必须大于启停速度）
Uint32 f_MdlStepMotorSpeedCalc(MdlStepMotorCtrlStDef *pstStepMotorCtl, \
					 Uint32 uint32_cur_pulse_num,\
					 Uint32 uint32_set_pulse_num,\
					 Uint32 u32SetSpeed
)
{
	// 当启停速度大于设定速度的时候  
	if (u32SetSpeed < pstStepMotorCtl->u32SetStartSpeed)
	{
		pstStepMotorCtl->u32CurSpeed = pstStepMotorCtl->u32SetStartSpeed;
		return (pstStepMotorCtl->u32CurSpeed);
	}
	
	// 当加速度为0的时候
	if (pstStepMotorCtl->u32SetAcc == 0)
	{
		pstStepMotorCtl->u32CurSpeed = u32SetSpeed;
		return (pstStepMotorCtl->u32CurSpeed);
	}
	
	if (uint32_set_pulse_num > (pstStepMotorCtl->uint32_acc_pulse_num * 2))   // 脉冲数量大于加减速脉冲数量
	{
		if (uint32_cur_pulse_num < pstStepMotorCtl->uint32_acc_pulse_num)   // 加速过程
		{
			
			if (
			    (uint32_cur_pulse_num != 0) &&
			    (uint32_cur_pulse_num % pstStepMotorCtl->u32SetAccStep == 0)
			)
			{
				pstStepMotorCtl->u32CurSpeed += pstStepMotorCtl->u32SetAcc;
				if (pstStepMotorCtl->u32CurSpeed > u32SetSpeed)
				{
					pstStepMotorCtl->u32CurSpeed = u32SetSpeed;
				}
			}
			
		}
		else  if ((uint32_set_pulse_num - uint32_cur_pulse_num ) < pstStepMotorCtl->uint32_acc_pulse_num) 
		{ // 减速过程
			if (
			    (uint32_cur_pulse_num % pstStepMotorCtl->u32SetAccStep == 0)
			)
			{
				pstStepMotorCtl->u32CurSpeed -= pstStepMotorCtl->u32SetAcc;
				if (pstStepMotorCtl->u32CurSpeed < pstStepMotorCtl->u32SetStartSpeed)
				{
					pstStepMotorCtl->u32CurSpeed = pstStepMotorCtl->u32SetStartSpeed;
				}
			}
		} 
		else    // 匀速过程
		{
			pstStepMotorCtl->u32CurSpeed = u32SetSpeed;
		}
	}
	else
	{
		if (uint32_cur_pulse_num < (uint32_set_pulse_num/3))   // 加速过程
		{
			
			if (
			    (uint32_cur_pulse_num != 0) &&
			    (uint32_cur_pulse_num % pstStepMotorCtl->u32SetAccStep == 0)
			)
			{
				pstStepMotorCtl->u32CurSpeed += pstStepMotorCtl->u32SetAcc;
				if (pstStepMotorCtl->u32CurSpeed > u32SetSpeed)
				{
					pstStepMotorCtl->u32CurSpeed = u32SetSpeed;
				}
			}
			
		}else  if (uint32_cur_pulse_num > (uint32_set_pulse_num*2)/3)
		{ // 减速过程
			if (
			    (uint32_cur_pulse_num % pstStepMotorCtl->u32SetAccStep == 0)
			)
			{
				pstStepMotorCtl->u32CurSpeed -= pstStepMotorCtl->u32SetAcc;
				if (pstStepMotorCtl->u32CurSpeed < pstStepMotorCtl->u32SetStartSpeed)
				{
					pstStepMotorCtl->u32CurSpeed = pstStepMotorCtl->u32SetStartSpeed;
				}
			}
		} 

	}
  return (pstStepMotorCtl->u32CurSpeed);
}						 




#endif  /* M_INCLUDE_MDL_MOTOR */





