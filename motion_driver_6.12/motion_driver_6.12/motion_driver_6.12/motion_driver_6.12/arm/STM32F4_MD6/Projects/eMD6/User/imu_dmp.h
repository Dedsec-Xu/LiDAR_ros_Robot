#ifndef __IMU_DMP_H
#define __IMU_DMP_H

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "user_system.h"

extern int16_t gyro[3],accel[3],sensors;
extern float quat_rpy[7];

void DMP_init(void);
float* Read_DMP(void);

#endif