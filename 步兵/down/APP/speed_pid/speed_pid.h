#ifndef _SPEED_PID_H
#define _SPEED_PID_H





void VPID_Init_All(void);			//���ת��PID������ʼ��
void vpid_PI_realize(float kp,float ki);				//���ת��PIDʵ��
void tvpid_PI_realize(float kp,float ki);      //����
void set_chassis_motor_speed(int moto1_speed,int moto2_speed,int moto3_speed,int moto4_speed);			//����Ŀ���ٶ�
void set_trigger_motor_speed(int moto5_speed);	
void set_gimbal1_motor_speed(int gimbal1_speed);
int abs(int input);				//�����ֵ����void apid_GIMBAL_realize(float kpa,float kia,float kpv,float kiv);
void apid_GIMBAL_PI_realize(float kpa,float kia,float kda);
void set_GIMBAL_angle(int gimbal1_angle,int gimbal2_angle);
int pid_auto(void);
int pid_pc(void);
#endif
