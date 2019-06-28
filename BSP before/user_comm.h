#ifndef __USER_COMM_H_
#define __USER_COMM_H_

#include "user_system.h"
#ifdef __cplusplus
	extern "C"{
#endif
		
void user_comm_init(void);
void print_usart1(const char *format, ...);

#ifdef  __cplusplus
}
#endif

#endif
