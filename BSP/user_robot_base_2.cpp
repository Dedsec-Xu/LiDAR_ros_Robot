#include "user_robot_base_2.h"

float robot_base::wheel_circumference = 0.068 * PI;// need wheel zhouchang measurement
float robot_base::wgeel_base_width = 0.20;// need wheel kuandu measurement

robot_base::robot_base()
{
    motor_RL = new motor_driver(htim2, 1, htim1, TIM_CHANNEL_2, 999, U1_1_GPIO_Port, U1_1_Pin, U1_2_Pin);
    motor_RR = new motor_driver(htim3, -1, htim1, TIM_CHANNEL_1, 999, U2_1_GPIO_Port, U2_1_Pin, U2_2_Pin);
    robot_linear_vel_x = 0;
}

robot_base::~robot_base()
{
    delete motor_RL;
    delete motor_RR;
}
void robot_base::velocity_to_RPM(float liner_vel_x, float angular_rad_z)
{
    static float linear_vel_x_min;
    static float angular_rad_z_min;
    static float tangential_vel;
    float scale = 1;

    linear_vel_x_min = liner_vel_x *60;
    angular_rad_z_min = angular_rad_z * 60;

    tangential_vel = angular_rad_z_min * wgeel_base_width/2;

    while(1)
    {
        motor_prm_RL = (linear_vel_x_min - tangential_vel)/wheel_circumference;
        motor_prm_RR = (linear_vel_x_min + tangential_vel)/wheel_circumference;//if zuoyou revert change +-
        if(abs(motor_prm_RL)<= motor_driver::_motor_max_rpm
        && abs(motor_prm_RR)<=motor_driver::_motor_max_rpm)
        {
            break;
        }
        scale -= 0.0333334;
        linear_vel_x_min *= scale;
        tangential_vel *= scale;

    }
    run();

}

void robot_base::run()
{
    motor_RL->set_rpm(motor_prm_RL);
    motor_RR->set_rpm(motor_prm_RR);
    double average_rpm = (motor_RL->rpm_cur + motor_RR->rpm_cur)/2;
    double average_rps = average_rpm / 60;

    robot_linear_vel_x = average_rps * wheel_circumference;

}

void robot_base::stop()
{
    motor_RL->set_rpm(0);
    motor_RR->set_rpm(0);
    
}
