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

void Robo_Move(void);							//机器人运动控制
void Debug_Key(void);							//调试按键

#define pressed     0
#define unpressed   1

//速度控制函数宏定义，注释里有常用的几个变量可供选择
#define Liner_X  Kinematics.target_velocities.linear_x
#define Liner_Y  Kinematics.target_velocities.linear_y
#define Angular_Z Kinematics.target_velocities.angular_z

#define Angular_Yaw Kinematics.yaw.target_angular        
                                              /*（Kinematics.yaw.target_angular)   上位机传入参数
                                                 (pid_target_speed)       测试程序使用																																																																		
                                              */
#define Angular_Pitch  Kinematics.pitch.target_angular
                                              /*(Kinematics.pitch.target_angular)
																							
																							*/
                                      
																			//注意！角速度的英文是angular velocity，这里为了方便全部使用angular代表角速度，与原英文有别
#define Angle_Pitch gimbal_p.apid.target_angle
#define Angle_Yaw   gimbal_y.apid.target_angle
#endif


