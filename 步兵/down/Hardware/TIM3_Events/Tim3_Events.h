#ifndef _TIM3_EVENTS_H
#define _TIM3_EVENTS_H

#include <string.h>
#include <stdio.h>
#include "key.h"
#include "led.h"
#include "bsp_debug_usart.h"
#include "bsp_uart7.h"
#include "fric.h"
#include "motor.h"
#include "kinematic.h"
#include "remote_code.h"
#include "angle_pid.h"
#include "speed_pid.h"
#include "stm32f4xx_tim.h"
#include "gimbal.h"
#include "json.h"
#include "angle_pid.h"

void Robo_Move(void);							//�������˶�����
void Debug_Key(void);							//���԰���

#define pressed     0
#define unpressed   1

//�ٶȿ��ƺ����궨�壬ע�����г��õļ��������ɹ�ѡ��
#define Liner_X  Kinematics.target_velocities.linear_x
#define Liner_Y  Kinematics.target_velocities.linear_y
#define Angular_Z Kinematics.target_velocities.angular_z

#define Angular_Yaw Kinematics.yaw.target_angular        
                                              /*��Kinematics.yaw.target_angular)   ��λ���������
                                                 (pid_target_speed)       ���Գ���ʹ��																																																																		
                                              */
#define Angular_Pitch  Kinematics.pitch.target_angular
                                              /*(Kinematics.pitch.target_angular)
																							
																							*/
                                      
																			//ע�⣡���ٶȵ�Ӣ����angular velocity������Ϊ�˷���ȫ��ʹ��angular������ٶȣ���ԭӢ���б�
#define Angle_Pitch gimbal_p.apid.target_angle
#define Angle_Yaw   gimbal_y.apid.target_angle
#endif


