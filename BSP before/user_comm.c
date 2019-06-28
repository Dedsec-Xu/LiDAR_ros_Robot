#include "user_comm.h"

uint8_t rxbuf1[128],rxbuf2[128];
uint8_t rx1_len,rx2_len;
serialData serial_rx_data;

void user_comm_init(void){
	serial_rx_data.syn = serial_rx_data.syn_CR = serial_rx_data.syn_LF = serial_rx_data.type = 0;
	do{
	}while(HAL_OK != HAL_UART_Receive_IT(&huart4,(uint8_t*)&serial_rx_data,1));
	print_usart1("user_comm_init() OK\r\n");
}

void print_usart1(const char *format, ...){
#ifdef DEBUG
	static char buf[128];
	va_list ap;
	va_start(ap,format);
	while(HAL_UART_GetState(&huart3) == HAL_UART_STATE_BUSY_TX){}
	if(vsprintf(buf,format,ap)>0){
		HAL_UART_Transmit_DMA(&huart3,(uint8_t *)buf,strlen(buf));
	}
#endif
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart4){
		if(serial_rx_data.syn != _SERIAL_SYN_CODE_START){
			do{
			}while(HAL_OK != HAL_UART_Receive_IT(&huart4,(uint8_t*)&serial_rx_data,1));
			return;
		}
		if(serial_rx_data.syn == _SERIAL_SYN_CODE_START && serial_rx_data.syn_CR != '\r' && serial_rx_data.syn_LF != '\n'){
			do{
			}while(HAL_OK != HAL_UART_Receive_IT(&huart4,(uint8_t*)&serial_rx_data + 1,sizeof(serialData) - 1));
			return;
		}
		if(serial_rx_data.syn != _SERIAL_SYN_CODE_START || serial_rx_data.syn_CR != '\r' || serial_rx_data.syn_LF != '\n'){
			print_usart1("serial data error\r\n");
			serial_rx_data.syn = serial_rx_data.syn_CR = serial_rx_data.syn_LF = serial_rx_data.type = 0;
			do{
			}while(HAL_OK != HAL_UART_Receive_IT(&huart4,(uint8_t*)&serial_rx_data,1));
			return;
		}
		if(serial_rx_data.type == VAL_VEL){
			cmd_liner_vel_x = serial_rx_data.dat.vel.liner[0];
			cmd_liner_vel_y = serial_rx_data.dat.vel.liner[1];
			cmd_angular_rad_z = serial_rx_data.dat.vel.angular[2];
			cmd_flag = 0;
		}

		if(serial_rx_data.type == VAL_PID){
			set_p = serial_rx_data.dat.pid[0];
			set_i = serial_rx_data.dat.pid[1];
			set_d = serial_rx_data.dat.pid[2];
		}
		serial_rx_data.syn = serial_rx_data.syn_CR = serial_rx_data.syn_LF = serial_rx_data.type = 0;
		do{
		}while(HAL_OK != HAL_UART_Receive_IT(&huart4,(uint8_t*)&serial_rx_data,1));
	}
}