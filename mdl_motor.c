#define M_MDL_MOTOR_EXTERN_DEF
#include "mdl.h"

#if (M_INCLUDE_MDL_MOTOR)




// �������������ʼ�� ���ٲ��� ��ͣ�ٶ� ���ٶ� ������������
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

// ����������� ���������ٶ� ��������������� ��ʼ���ٶ�
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

// ������ʵʱ�ٶ� ���ص�ǰ�ٶ�  ��ǰ�ѷ�����������	Ԥ����������	Ԥ���ٶ� �����������ͣ�ٶȣ�
Uint32 f_MdlStepMotorSpeedCalc(MdlStepMotorCtrlStDef *pstStepMotorCtl, \
					 Uint32 uint32_cur_pulse_num,\
					 Uint32 uint32_set_pulse_num,\
					 Uint32 u32SetSpeed
)
{
	// ����ͣ�ٶȴ����趨�ٶȵ�ʱ��  
	if (u32SetSpeed < pstStepMotorCtl->u32SetStartSpeed)
	{
		pstStepMotorCtl->u32CurSpeed = pstStepMotorCtl->u32SetStartSpeed;
		return (pstStepMotorCtl->u32CurSpeed);
	}
	
	// �����ٶ�Ϊ0��ʱ��
	if (pstStepMotorCtl->u32SetAcc == 0)
	{
		pstStepMotorCtl->u32CurSpeed = u32SetSpeed;
		return (pstStepMotorCtl->u32CurSpeed);
	}
	
	if (uint32_set_pulse_num > (pstStepMotorCtl->uint32_acc_pulse_num * 2))   // �����������ڼӼ�����������
	{
		if (uint32_cur_pulse_num < pstStepMotorCtl->uint32_acc_pulse_num)   // ���ٹ���
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
		{ // ���ٹ���
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
		else    // ���ٹ���
		{
			pstStepMotorCtl->u32CurSpeed = u32SetSpeed;
		}
	}
	else
	{
		if (uint32_cur_pulse_num < (uint32_set_pulse_num/3))   // ���ٹ���
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
		{ // ���ٹ���
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





