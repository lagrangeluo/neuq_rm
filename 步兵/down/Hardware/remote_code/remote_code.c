/**
  ******************************************************************************
  * @file    Project/HARDWARE/remote_code.c 
  * @author  Siyuan Qiao & Junyu Luo
  * @version V1.0.0
  * @date    1.2021
  * @brief   
  ******************************************************************************
  * @attention 遥控器控制程序
  ******************************************************************************
      ..................NEUQ_SUDO..................
*/

#include "remote_code.h"
#include "DJi_remote.h"
#include "FS_remote.h"
#include "motor.h"
#include "kinematic.h"
#include "fric.h"
#include "stm32f4xx_tim.h"
#include "gimbal.h"
#include "imuReader.h"              
#include <math.h>
#include "mode.h"
#include "delay.h"
u8 Control_Mode = control_mode;

//内部全局变量，方便调试
float x_speed=0,y_speed=0,r_speed=0,trigger_speed=0,theta=0;
float cx_speed=0,cy_speed=0;
int flag = 0;
//内部函数声明
float caculate_linear_speed(int width,int mid,int min,int max);
float caculate_rotational_speed(int width,int mid,int min,int max);
float caculate_gimbal_pitch_angle(int width,int mid,int min,int max);
float caculate_gimbal_yaw_angle(int width,int mid,int min,int max);

/**
  * @brief  遥控代码，将遥控器输出对应到机器人具体动作上，放在定时器里不断地刷
  */
void Remote_Control()    //这个函数里就不断地判断每个通道的值，如果满足条件就做相应动作
{	
	if(chassis_CH_width>remote_min_value && chassis_CH_width<remote_max_value)		//如果满足遥控条件
	{
		//标志位改为遥控模式
		Control_Mode &= remote_control;											//修改Control_Mode第二位为0
	}
	else
	{
		//标志位改为自动模式
		Control_Mode |= auto_control;												//修改Control_Mode第二位为1		
	}
	
	if((Control_Mode & auto_control) != auto_control)			//如果控制模式不等于自动控制，即遥控控制
	{
		
			if(Remote_control_mode == chassis_CH_width)
			{  
				 x_speed=caculate_linear_speed(y_CH_width,y_initial_value,y_min_value,y_max_value);
				 y_speed=caculate_linear_speed(x_CH_width,x_initial_value,x_min_value,x_max_value);
			   r_speed=caculate_rotational_speed(r_CH_width,r_initial_value,r_min_value,r_max_value);  
				
				switch(trigger_CH_width)
				{
					case  1:
					
			   	fric1_on(2200);//1500
				  fric2_on(2200);
         static int count_1=1;	
					count_1++;
					if(count_1>100)
					{trigger_speed = 150;
					    count_1=1;           }
					if(motor5.actual_speed<20&&motor5.actual_speed>-20)    						//堵转
					{ 
						static int count_=1;
					  count_++;
						trigger_speed =pow(-1,count_)*100;
						if(count_>100)
							count_=1;
					}
					break;
					
					case 2:
					 trigger_speed = 150;
				   fric1_on(1000);
				   fric2_on(1000);
					if(motor5.actual_speed<20&&motor5.actual_speed>-20)    						//堵转
					{ 
						static int count_=1;
					  count_++;
						trigger_speed =pow(-1,count_)*50;
						if(count_>100)
							count_=1;
					}
					break;
					
					case 3:
						trigger_speed = 0;
				    fric1_on(400);//1000
				    fric2_on(400);
					break;
					
					default:
	      	break;
				}						
			                      }
			if(Remote_control_mode == gimbal_CH_width)
			{
				switch(gimbal_modes)
				{
				case(gimbal_pwm_mode):  				//pwm模式下控制云台转角

			  	pwm_pulse_p=caculate_gimbal_pitch_angle(i_CH_width,i_initial_value,i_min_value,i_max_value);
				  pwm_pulse_y=caculate_gimbal_yaw_angle(x_CH_width,x_initial_value,x_min_value,x_max_value);
				
				break;
				case(gimbal_can_mode):           //can模式下控制云台转角
					Kinematics.pitch.target_angle = caculate_gimbal_pitch_angle(i_CH_width,i_initial_value,i_min_value,i_max_value);
					Kinematics.yaw.target_angle = caculate_gimbal_yaw_angle(x_CH_width,x_initial_value,x_min_value,x_max_value);
        break;
				default:break;
        }
				switch (trigger_CH_width)
				{
					case 1:
				  fric1_on(1500);
				  fric2_on(1500);
          static int count_1=1;	
					count_1++;
					if(count_1>100)
					{trigger_speed = 100;
					    count_1=1;           }
					if(motor5.actual_speed<20&&motor5.actual_speed>-20)    						//堵转
					{ 
						static int count_=1;
					  count_++;
						trigger_speed =pow(-1,count_)*50;
						if(count_>100)
							count_=1;
					}
					break;
					case 2:
					trigger_speed = -50;
				  fric1_on(1000);
				  fric2_on(1000);
					if(motor5.actual_speed<20&&motor5.actual_speed>-20)    						//反转
					{ 
						static int count_=1;
					  count_++;
						trigger_speed =pow(-1,count_)*50;
						if(count_>100)
							count_=1;
					}
					break;
					case 3:
					trigger_speed = 0;
				  fric1_on(1000);
				  fric2_on(1000);
					break;
					
					default:
	      	break;
				                  }											
				       		}
			
			
		
		if(Remote_control_mode == dance_CH_width)	//小陀螺模式
		{
		     x_speed=caculate_linear_speed(y_CH_width,y_initial_value,y_min_value,y_max_value);
				 y_speed=caculate_linear_speed(x_CH_width,x_initial_value,x_min_value,x_max_value);
			   r_speed=caculate_rotational_speed(r_CH_width,r_initial_value,r_min_value,r_max_value); 
		theta = Kinematics.actual_velocities.angular_z * 0.004f + theta; 
	  theta = yawRead();
		cx_speed = x_speed*cos(theta) + y_speed*sin(theta);
		cy_speed = y_speed*cos(theta) - x_speed*sin(theta);
    x_speed=cx_speed;
		y_speed=cy_speed;
		gimbal_speed_control(yaw_angularRead(),Kinematics.pitch.target_angular);
		set_gimbal_current();
	
		}
		if((Control_Mode&DJi_Remote_Control) == DJi_Remote_Control)
		{
			y_speed = y_speed;
			r_speed = -r_speed; //取反，使逆时针旋转为正向
		}
		else if((Control_Mode&FS_Remote_Control) == FS_Remote_Control)		//因为FS_Remote_Control = 0，因此判断时必须放在else if里
		{
			y_speed = -y_speed;
		}
		Kinematics.target_velocities.linear_x=x_speed;//放在robomove中执行.
		Kinematics.target_velocities.linear_y=y_speed;
		Kinematics.target_velocities.angular_z=r_speed;
		chassic_speed_control(x_speed,y_speed,r_speed);
		trigger_control(trigger_speed);	
	}
	
}


// 函数: caculate_speed()
// 描述: 将遥控器摇杆输出映射到机器人三轴速度上
// 参数：width：通道值 
//			 mid：通道中间值 
//			 min：通道输出最小值
//       max：通道输出最大值
// 输出：对应的速度值
//内部函数，用户无需调用
static float caculate_linear_speed(int width,int mid,int min,int max)
{
  float speed=0;
  if(width>=(mid+2))		//中间消除波动
    speed=(1.0*(width-(mid+2))/(max-(mid+2))*max_base_linear_speed);
  else if(width<=(mid-2))
    speed=(1.0*(width-(mid-2))/((mid-2)-min)*max_base_linear_speed);
  else
    speed=0;
  return speed;                
}

static float caculate_rotational_speed(int width,int mid,int min,int max)
{
  float speed=0;
  if(width>=(mid+2))		//中间消除波动
    speed=(1.0*(width-(mid+2))/(max-(mid+2))*max_base_rotational_speed);
  else if(width<=(mid-2))
    speed=(1.0*(width-(mid-2))/((mid-2)-min)*max_base_rotational_speed);
  else
    speed=0;
  return speed;                
}

static float caculate_gimbal_pitch_angle(int width,int mid,int min,int max)
{
	switch(gimbal_modes)
	{
		case(gimbal_pwm_mode):
			
	   float pwm_pulse=BASIC_PITCH_ANGLE_PWM;
		 if(width>=(mid+2))
		  pwm_pulse=(BASIC_PITCH_ANGLE_PWM - 1.0*(width-(mid+2))/(max-(mid+2))*210);
	   else if(width<=(mid-2))
	    pwm_pulse=(BASIC_PITCH_ANGLE_PWM + 1.0*((mid-2)-width)/((mid-2)-min)*105);
	   else
		  pwm_pulse=BASIC_PITCH_ANGLE_PWM;
	   return pwm_pulse;
	
		 break;
		 
		case(gimbal_can_mode):
		 float can_angle=BASIC_PITCH_ANGLE_CAN;
		 if(width>=(mid+2))
		  can_angle=(BASIC_PITCH_ANGLE_CAN - 1.0*(width-(mid+2))/(max-(mid+2))*2047);
	   else if(width<=(mid-2))
	    can_angle=(BASIC_PITCH_ANGLE_CAN + 1.0*((mid-2)-width)/((mid-2)-min)*1023);
	   else
		  can_angle=BASIC_PITCH_ANGLE_CAN;
	   return can_angle;
		 
		 break;
		 default:break;
   }
}

static float caculate_gimbal_yaw_angle(int width,int mid,int min,int max)
{
	switch(gimbal_modes)
	{
		case(gimbal_pwm_mode):
	   float pwm_pulse=BASIC_YAW_ANGLE_PWM;
		 if(width>=(mid+2))
	 	  pwm_pulse=(BASIC_YAW_ANGLE_PWM - 1.0*(width-(mid+2))/(max-(mid+2))*420);
	   else if(width<=(mid-2))
	    pwm_pulse=(BASIC_YAW_ANGLE_PWM + 1.0*((mid-2)-width)/((mid-2)-min)*420);
	   else
		  pwm_pulse=BASIC_YAW_ANGLE_PWM;
	   return pwm_pulse;

		 break;
		 
		 case(gimbal_can_mode):
		 float can_angle=BASIC_YAW_ANGLE_CAN;
		 if(width>=(mid+2))
		  can_angle=(BASIC_YAW_ANGLE_CAN - 1.0*(width-(mid+2))/(max-(mid+2))*4095);
	   else if(width<=(mid-2))
	    can_angle=(BASIC_YAW_ANGLE_CAN + 1.0*((mid-2)-width)/((mid-2)-min)*4095);
	   else
		  can_angle=BASIC_YAW_ANGLE_CAN;
	   return can_angle;
		 
		 break;
		 default:break;
   }
}



