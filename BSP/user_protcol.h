#ifndef __USER_PROTCOL_H_
#define __USER_PROTCOL_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
enum _ptl_type{
	VAL_START = 0,
	VAL_POSE,
	VAL_IMU_RAW,
	VAL_VEL,
	VAL_PID,
	VAL_IMU,
	VAL_END
};

#define _SERIAL_SYN_CODE_START 0xFA
#pragma pack(1)
typedef struct _serial_data{
	uint8_t syn;
	uint8_t type;
	union{
		struct{
			float liner[3],angular[3];
		}vel;
		struct{
			bool rot_ok,acc_ok,mag_ok;
			double rot[3],acc[3],mag[3];
		}imu;
		float pid[3];
	}dat;
	uint8_t syn_CR;
	uint8_t syn_LF;
}serialData;
#pragma pack()

#endif
