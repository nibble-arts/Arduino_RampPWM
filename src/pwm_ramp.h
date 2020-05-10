/*
 * LOCO-CAN PWM_RAMP class header file
 * 
 * @author: Thomas H Winkler
 * @copyright: 2020
 * @lizence: GG0
 * 
 * using can library: https://github.com/sandeepmistry/arduino-CAN
 */

#ifndef PWM_RAMP_H
#define PWM_RAMP_H

#include "simpletimeout.h"

// =====================================
//
// PWM_RAMP frequencies on Port 9,10 (Atmega 328)
//
// =====================================
#define PWM_RAMP_500HZ 0
#define PWM_RAMP_1KHZ 0
#define PWM_RAMP_31KHZ 1

#define	PWM_RAMP_MAX_VAL 255

class PWM_RAMP {

	public:
		PWM_RAMP(void);
		void begin(uint8_t port, uint16_t update_rate, bool pwm);

		void pwmWrite(uint16_t pwm); // set target pwm with default ramp
		void pwmWrite(uint16_t pwm,uint8_t ramp); // set target pwm and ramp

		uint16_t pwmRead(void);
		uint16_t pwmReadTarget(void);

		void set_ramp(uint16_t ramp); // set ramp time


		bool stopped(void);

		uint16_t update(void);


	private:
		uint16_t ramp();
		void write_pwm(uint16_t pwm);

		uint8_t _port;
		uint16_t _pwm;

		// target pwm and current ramp_time (0-max in ms)
		uint16_t _target_pwm;
		uint16_t _ramp_time;

		// current pwm
		uint16_t _current_pwm;

		// timeout value > RAMP_DEFAULT
		uint16_t _update_rate;
		SIMPLETIMEOUT _timer;
};



#endif
