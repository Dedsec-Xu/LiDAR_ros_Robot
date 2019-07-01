#include "imu_dmp.h"

#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)
#define PRINT_COMPASS   (0x08)
#define PRINT_EULER     (0x10)
#define PRINT_ROT_MAT   (0x20)
#define PRINT_HEADING   (0x40)
#define PRINT_PEDO      (0x80)
#define PRINT_LINEAR_ACCEL (0x100)
#define PRINT_GRAVITY_VECTOR (0x200)

volatile uint32_t hal_timestamp = 0;
#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)
#define COMPASS_ON      (0x04)

#define MOTION          (0)
#define NO_MOTION       (1)

/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (20)

#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)

#define PEDO_READ_MS    (1000)
#define TEMP_READ_MS    (500)
#define COMPASS_READ_MS (100)

#define q30 1073741824.0f
#define PI_2 (1.5707963267948966192313216936398f)

#define delay_ms HAL_Delay

int16_t gyro[3], accel[3], sensors;
float quat_rpy[7] = {0};
float *RPY = &quat_rpy[4];
float *q = quat_rpy;

struct rx_s {
    unsigned char header[3];
    unsigned char cmd;
};
struct hal_s {
    unsigned char lp_accel_mode;
    unsigned char sensors;
    unsigned char dmp_on;
    unsigned char wait_for_tap;
    volatile unsigned char new_gyro;
    unsigned char motion_int_mode;
    unsigned long no_dmp_hz;
    unsigned long next_pedo_ms;
    unsigned long next_temp_ms;
    unsigned long next_compass_ms;
    unsigned int report;
    unsigned short dmp_features;
    struct rx_s rx;
};



/* Platform-specific information. Kinda like a boardfile. */
struct platform_data_s {
    signed char orientation[9];
};

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from the
 * driver(s).
 * TODO: The following matrices refer to the configuration on internal test
 * boards at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */


static struct platform_data_s gyro_pdata = {
    .orientation = { 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1}
};

#if defined MPU9150 || defined MPU9250
static struct platform_data_s compass_pdata = {
    .orientation = { 0, 1, 0,
                     1, 0, 0,
                     0, 0, -1}
};
#define COMPASS_ENABLED 1
#elif defined AK8975_SECONDARY
static struct platform_data_s compass_pdata = {
    .orientation = {-1, 0, 0,
                     0, 1, 0,
                     0, 0,-1}
};
#define COMPASS_ENABLED 1
#elif defined AK8963_SECONDARY
static struct platform_data_s compass_pdata = {
    .orientation = {-1, 0, 0,
                     0,-1, 0,
                     0, 0, 1}
};
#define COMPASS_ENABLED 1
#endif



static  unsigned short inv_row_2_scale(const signed char *row);
static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
static void run_self_test(void);

void DMP_init(void)
{
    struct int_param_s int_param;
    if(mpu_init(&int_param)){
        print_usart1("mpu_set_sensor failed ......\r\n");
        while(1){}
    }
    if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))    //设置需要的传感器
        print_usart1("mpu_set_sensor complete ......\r\n");
    if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL)) //设置fifo
        print_usart1("mpu_configure_fifo complete ......\r\n");
    if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))              //设置采集样率
        print_usart1("mpu_set_sample_rate complete ......\r\n");
    if(!dmp_load_motion_driver_firmware())                //加载dmp固件
    print_usart1("dmp_load_motion_driver_firmware complete ......\r\n");
    if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
        print_usart1("dmp_set_orientation complete ......\r\n"); //设置陀螺仪方向
    if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL))
        print_usart1("dmp_enable_feature complete ......\r\n");
    if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))    //设置速率
        print_usart1("dmp_set_fifo_rate complete ......\r\n");
    run_self_test();                          //自检
    if(!mpu_set_dmp_state(1))                 //使能
        print_usart1("mpu_set_dmp_state complete ......\r\n");

}

float* Read_DMP(void)
{   
    unsigned long sensor_timestamp;
    unsigned char more;
    long quat[4];
    while(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more)!=0){} 
    if (sensors & INV_WXYZ_QUAT)
    {    
        q[0]=quat[0] / q30;
        q[1]=quat[1] / q30;
        q[2]=quat[2] / q30;
        q[3]=quat[3] / q30;
        RPY[0] = atan2(2*(q[2]*q[3] + q[0]*q[1]), 1-2*(q[1]*q[1]+q[2]*q[2]));
        RPY[1] = asin(2*(q[0]*q[2]-q[1]*q[3]));
        RPY[2] = atan2(2*(q[0]*q[3] + q[1]*q[2]), 1-2*(q[3]*q[3]]+q[2]*q[2]));
        
    }   
    return quat_rpy;

}

static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;

    return scalar;
}


static void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];
    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
	
        /* Push the calibrated data to the MPL library.
         *
         * MPL expects biases in hardware units << 16, but self test returns
		 * biases in g's << 16.
		 */
    	uint16_t accel_sens;
    	float gyro_sens;

        mpu_get_gyro_sens(&gyro_sens);
		gyro[0] = (long) (gyro[0] * gyro_sens);
		gyro[1] = (long) (gyro[1] * gyro_sens);
		gyro[2] = (long) (gyro[2] * gyro_sens);
        dmp_set_gyro_bias(gyro);


		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
        print_usart1("setting bias successfully!");
    }
}