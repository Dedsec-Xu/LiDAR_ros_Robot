#define q30 1073741824.0f
#define PI_2 (1.5707963267948966192313216936398f)
#define Kp 1.0f
#define Ki 0.001f
#define halfT 0.001f
#define t
#include "Read_dmp.h"


float quat_rpy1[7] = {0};
float *RPY1 = &quat_rpy1[4];
float *q_new = quat_rpy1;
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
float exInt = 0, eyInt = 0, ezInt = 0;
ScaledData_Def myAccelScaled2, myGyroScaled2;

float * READ_DMP2(void)
{
    MPU6050_Get_Gyro_Scale(&myGyroScaled2);
    //print_usart1("Gyro: x:%.3f, y:%.3f, z:%.3f\r\n", myGyroScaled.x, myGyroScaled.y, myGyroScaled.z);
    MPU6050_Get_Accel_Scale(&myAccelScaled2);
    //print_usart1("Accel: x:%.3f, y:%.3f, z:%.3f\r\n", myAccelScaled.x, myAccelScaled.y, myAccelScaled.z);
    long quat[4];
    float gx,gy,gz,ax,ay,az;
    float norm;

    gx = myGyroScaled2.x;
    gy = myGyroScaled2.y;
    gz = myGyroScaled2.z;
    ax = myAccelScaled2.x/1000.0f;
    ay = myAccelScaled2.y/1000.0f;
    az = myAccelScaled2.z/1000.0f;

    

    float vx, vy, vz;
    float ex, ey, ez;
    norm = sqrt(ax*ax + ay*ay + az*az);       
    ax = ax / norm; 
    ay = ay / norm; 
    az = az / norm;  
    //下面把四元数换算成方向余弦中的第三行的三个元素。刚好vx、vy、vz 。其实就是上一次的欧拉角（四元数）的机体坐标参考系换算出来的重力的单位向量。
    //estimated direction of gravity 
    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3); 
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3; 
    //axyz是机体坐标参照系上，加速度计测出来的重力向量，也就是实际测出来的重力向量。
    //axyz是测量得到的重力向量，vxyz是陀螺积分后的姿态来推算出的重力向量，它们都是机体坐标参照系上的重力向量。
    //那它们之间的误差向量，就是陀螺积分后的姿态和加计测出来的姿态之间的误差。
    //向量间的误差，可以用向量叉积（也叫向量外积、叉乘）来表示，exyz就是两个重力向量的叉积。
   // 这个叉积向量仍旧是位于机体坐标系上的，而陀螺积分误差也是在机体坐标系，而且叉积的大小与陀螺积分误差成正比，正好拿来纠正陀螺。（你可以自己拿东西想象一下）由于陀螺是对机体直接积分，所以对陀螺的纠正量会直接体现在对机体坐标系的纠正。
    //integral error scaled integral gain 
    exInt = exInt + ex*Ki;
    eyInt = eyInt + ey*Ki; 
    ezInt = ezInt + ez*Ki; 
    //用叉积误差来做PI修正陀螺零偏
    //integral error scaled integral gain 
    exInt = exInt + ex*Ki; 
    eyInt = eyInt + ey*Ki; 
    ezInt = ezInt + ez*Ki;  // adjusted gyroscope measurements 
    gx = gx + Kp*ex + exInt; 
    gy = gy + Kp*ey + eyInt; 
    gz = gz + Kp*ez + ezInt; 
    //四元数微分方程，其中T为测量周期，为陀螺仪角速度，以下都是已知量，这里使用了求解四元数微分方程：
    //integrate quaternion rate and normalise 
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT; 
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT; 
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT; 
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;
    norm = 1.0f/sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;
    q_new[0] = q0 / q30;
    q_new[1] = q1 / q30;
    q_new[2] = q2 / q30;
    q_new[3] = q3 / q30;
    RPY1[0] = atan2(2 * (q2 * q3 + q0 * q1), 1 - 2 * (q1 * q1 + q2 * q2));
    RPY1[1] = asin(2 * (q0 * q2 - q1 * q3));
    RPY1[2] = atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q3 * q3 + q2 * q2));
    return quat_rpy1;
}