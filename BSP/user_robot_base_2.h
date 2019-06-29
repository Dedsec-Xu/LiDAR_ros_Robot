#ifndef __USER_ROBOT_BASE_2_H_
#define __USER_ROBOT_BASE_2_H_

#include "driver_motor.h"
#include "user_system.h"

class robot_base
{
    public:
        robot_base();
        ~robot_base();

        class motor_driver* motor_RL;
        class motor_driver* motor_RR;
        float robot_linear_vel_x;
        void velocity_to_RPM(float Liner_x, float angular_z);
        void run();
        void stop();

    private:
        float motor_prm_RL;
        float motor_prm_RR;
        
        static float wheel_circumference;
        static float wgeel_base_width;

        
        
};

#endif
