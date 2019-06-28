#ifndef __DRIVER_MOTOR_H_
#define __DRIVER_MOTOR_H_

#include "driver_motor.h"
#include "user_system.h"


class motor_driver{
    public:
        motor_driver(TIM_HandleTypeDef& encoder_htim,
                    int8_t encoder_direction,
                    TIM_HandleTypeDef& pwm_htim,
                    uint32_t pwm_channel,
                    uint16_t pwm_max,
                    GPIO_TypeDef* gpiox_in,
                    uint16_t pin_in1,
                    uint16_t pin_in2);
        ~motor_driver(){}

        static float _p;
        static float _i;
        static float _d;
        int16_t tick_inc;

        double err_total;
        double err_previous;
        float rpm_cur;
        static float _motor_max_rpm;

        void set_rpm(float rpm);
        static void set_pid(float p,float i,float define);

        int16_t get_tick_inc();
        double read_rpm();
        void spin(int16_t pwm);

    private:
        TIM_HandleTypeDef& _pwm_htim;
        uint32_t _pwm_channel;
        uint16_t _pwm_max;
        GPIO_TypeDef* _gpiox_in;
        uint16_t _pin_in1;
        uint16_t _pin_in2;

        TIM_HandleTypeDef& _encoder_htim;
        int32_t tick_last;
        int32_t tick_cur;
        int32_t tick_total;
        int8_t _encoder_direction;

        uint32_t last_milli_time;
        static float _ticks_per_rev;
};

#endif
