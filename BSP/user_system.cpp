#include "user_system.h"

float angular_rad_max = 6.2;
float angular_rad_cur = 1.0;

float cmd_liner_vel_x = 0;
float cmd_liner_vel_y = 0;
float cmd_angular_rad_z = 0;

float set_p = motor_driver::_p;
float set_i = motor_driver::_i;
float set_d = motor_driver::_d;

uint32_t loop_tick;
robot_base *xrobot;

#define user_delay_ms_start(t) *t = HAL_GetTick()
#define user_delay_ms_end(t,Delay) while((HAL_GetTick()-*t)<Delay){}
void serial2_ros_data();

void user_test(){

}

void user_system_init(){
	xrobot = new robot_base;
}

// double read_rpm(){
// 	int32_t tick_cur;
// 	static int32_t tick_last = 0;
// 	int32_t tick_inc;
// 	uint32_t cur_milli_time = 0;
// 	static uint32_t last_milli_time = 0;
// 	double delta_mins_time;
// 	double rpm;

// 	cur_milli_time = HAL_GetTick();

// 	tick_cur = __HAL_TIM_GET_COUNTER(&htim2);
// 	if(abs(tick_cur - tick_last) > 0xffff/2){
// 		if(tick_cur > tick_last){
// 			tick_inc = -0xffff - tick_last + tick_cur;
// 		}else{
// 			tick_inc = 0xffff -tick_last + tick_cur;
// 		}
// 	}else{
// 		tick_inc = tick_cur - tick_last;
// 	}
// 	tick_inc *= -1;
// 	delta_mins_time = (cur_milli_time - last_milli_time)/60000.0;
// 	rpm = tick_inc / 1560.0 / delta_mins_time;
// 	tick_last = tick_cur;
// 	last_milli_time = cur_milli_time;
// 	print_usart1("{%8.4f | %4d|%4d}\r\n",rpm,tick_inc,tick_cur);
// 	return rpm;
// }

void user_system_thread_0(){
	uint8_t rate = 10;
	print_usart1("user_system_thread_0() start...\r\n");
	xrobot->velocity_to_RPM(0.0,10.0);
	while(1){
		user_delay_ms_start(&loop_tick);
		HAL_IWDG_Refresh(&hiwdg1);
		print_usart1("run time %dms\r\n",HAL_GetTick() - loop_tick);
		user_delay_ms_end(&loop_tick,1000/rate);
	}
}

// void serial2_ros_data(){
// 	static float rpy_last[3] = {0};
// 	static serialData data;
// 	data.syn = _SERIAL_SYN_CODE_START;
// 	data.syn_CR = '\r';
// 	data.syn_LF = '\n';
// 	float *quar_rpy;

// 	while(HAL_UART_GetState(&huart4) == HAL_UART_STATE_BUSY_TX_RX){}
// 	data.type = VAL_POSE;
// 	data.dat.vel.liner[0] = xrobot->robot_linear_vel_x;
// 	data.dat.vel.liner[1] = 0;
// 	data.dat.vel.liner[2] = 0;
// 	HAL_UART_Transmit_DMA(&huart4,(uint8_t*)&data,sizeof(serialData));

// 	quar_rpy = Read_DMP();

// 	while(HAL_UART_GetState(&huart4) == HAL_UART_STATE_BUSY_TX_RX){}
// 	data.dat.vel.angular[0] = (quar_rpy[4]) - rpy_last[0])*10;
// 	data.dat.vel.angular[1] = (quar_rpy[5]) - rpy_last[1])*10;
// 	data.dat.vel.angular[2] = (quar_rpy[6]) - rpy_last[2])*10;

// 	rpy_last[0] = quar_rpy[4];
// 	rpy_last[1] = quar_rpy[5];
// 	rpy_last[2] = quar_rpy[6];

// 	HAL_UART_Transmit_DMA(&huart4,(uint8_t*)&data,sizeof(data));
// }

void user_delay_us(uint32_t us){

}