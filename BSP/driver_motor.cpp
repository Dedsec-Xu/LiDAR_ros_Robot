#include "driver_motor.h"

#define Millisecond() HAL_GetTick()
#define constrain(value,low,high) ((value)<(low)?(low):((value)>(high)?(high):(value)))

float motor_driver::_p = 0.2;
float motor_driver::_i = 0.5;
float motor_driver::_d = 0.3;

float motor_driver::_ticks_per_rev = 1560;
float motor_driver::_motor_max_rpm = 300;

motor_driver::motor_driver(TIM_HandleTypeDef& encoder_htim,
                            int8_t encoder_direction,
                            TIM_HandleTypeDef& pwm_htim,
                            uint32_t pwm_channel,
                            uint16_t pwm_max,
                            GPIO_TypeDef* gpiox_in,
                            uint16_t pin_in1,
                            uint16_t pin_in2)
                            :_pwm_htim(pwm_htim),
                            _encoder_htim(encoder_htim)
{
    err_total = 0;
    err_previous = 0;
    _pwm_channel = pwm_channel;
    _pwm_max = pwm_max;
    _gpiox_in = gpiox_in;
    _pin_in1 = pin_in1;
    _pin_in2 = pin_in2;

    __HAL_TIM_SET_COMPARE(&_pwm_htim,_pwm_channel,0);
    HAL_TIM_PWM_Start(&_pwm_htim,_pwm_channel);

    HAL_GPIO_WritePin(_gpiox_in,_pin_in1,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_gpiox_in,_pin_in2,GPIO_PIN_RESET);

    _encoder_direction = encoder_direction;
    tick_total = 0;
    tick_last = 0;
    HAL_TIM_Encoder_Start(&_encoder_htim,TIM_CHANNEL_ALL);
}

void motor_driver::set_rpm(float rpm){
    int16_t _pwm;
    double err;

    rpm_cur = read_rpm();

    err = constrain(rpm,-_motor_max_rpm,_motor_max_rpm) - rpm_cur;
    err_total += err;

    err_total = constrain(err_total,-_pwm_max/_i,_pwm_max/_i);
    _pwm = _p*err + _i*err_total + _d*(err-err_previous);

    err_previous = err;

    _pwm = constrain(_pwm,-_pwm_max,_pwm_max);

    if(abs(_pwm)<40){
        _pwm = 0;
    }
    spin(_pwm);
}


void motor_driver::spin(int16_t pwm){
    if((rpm_cur>0 && pwm<0) || (rpm_cur<0 && pwm>0)){
        __HAL_TIM_SET_COMPARE(&_pwm_htim,_pwm_channel,0);
    }
    if(pwm>0){
        HAL_GPIO_WritePin(_gpiox_in,_pin_in2,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_gpiox_in,_pin_in1,GPIO_PIN_SET);
    }
    if(pwm<0){
        HAL_GPIO_WritePin(_gpiox_in,_pin_in1,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_gpiox_in,_pin_in2,GPIO_PIN_SET);
        pwm=-pwm;
    }
    __HAL_TIM_SET_COMPARE(&_pwm_htim,_pwm_channel,pwm);
}

double motor_driver::read_rpm(){
	uint32_t cur_milli_time = 0;
	double delta_mins_time;
    tick_cur = get_tick_inc();
    cur_milli_time = Millisecond();
	
	delta_mins_time = (cur_milli_time - last_milli_time)/60000.0;
	rpm_cur = (tick_cur / _ticks_per_rev) / delta_mins_time;
	last_milli_time = cur_milli_time;
	return rpm_cur;
}

int16_t motor_driver::get_tick_inc()
{
    tick_cur = __HAL_TIM_GET_COUNTER(&_encoder_htim);
    if(abs(tick_cur - tick_last) > 0xffff/2){
		if(tick_cur > tick_last){
			tick_inc = -0xffff - tick_last + tick_cur;
		}else{
			tick_inc = 0xffff -tick_last + tick_cur;
		}
	}else{
		tick_inc = tick_cur - tick_last;
	}
    tick_last = tick_cur;
    tick_inc *= _encoder_direction;
    tick_total += tick_inc;
    return tick_inc;
}

void motor_driver::set_pid(float p, float i, float d)
{
    _p = p;
    _i = i;
    _d = d;
}
