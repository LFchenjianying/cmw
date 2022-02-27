#ifndef M_MDL_MOTOR_H
#define M_MDL_MOTOR_H

#ifdef M_MDL_MOTOR_EXTERN_DEF
	#define M_MDL_MOTOR_EXTERN
#else 
	#define M_MDL_MOTOR_EXTERN extern
#endif /* M_MDL_MOTORR_EXTERN_DEF */
#include "hmc_type.h"


typedef struct {
  Uint32       u32CurSpeed;       // ʵʱ�ٶ�

	
// ����ʱ����Ĳ���	
  Uint32       uint32_acc_pulse_num;    // �Ӽ���������Ҫ������

// Ԥ�����	
  Uint32       u32SetAcc;        // ���ٶ�
  Uint32       u32SetStartSpeed;       // ��ʼ�ٶ�
  Uint32       u32SetAccStep;   // �������岽��
} MdlStepMotorCtrlStDef;


// �������������ʼ�� ���ٲ��� ��ͣ�ٶ� ���ٶ� ������������
void f_MdlStepMotorConfig(MdlStepMotorCtrlStDef *pstStepMotorCtl, \
	                    Uint32 u32StartSpeed, \
			      Uint32 u32Acc, \
			      Uint32 u32AccStep
				     );
			      
// ����������� ���������ٶ� ��������������� ��ʼ���ٶ�
Uint32 f_MdlStepMotorStart(MdlStepMotorCtrlStDef *pstStepMotorCtl, \
					Uint32 u32SetSpeed,\
					Uint32 u32Acc

 );
					
// ������ʵʱ�ٶ� ���ص�ǰ�ٶ�  ��ǰ�ѷ�����������	Ԥ����������	Ԥ���ٶ� �����������ͣ�ٶȣ�
Uint32 f_MdlStepMotorSpeedCalc(MdlStepMotorCtrlStDef *cc, \
					 Uint32 uint32_cur_pulse_num,\
					 Uint32 uint32_set_pulse_num,\
					 Uint32 u32SetSpeed
);		
	
	
#endif /* M_MDL_MOTOR_H */	

