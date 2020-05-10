/*
 * LOCO-CAN PWM_RAMP class
 * 
 * @author: Thomas H Winkler
 * @copyright: 2020
 * @lizence: GG0
 * 
 * using can library: https://github.com/sandeepmistry/arduino-CAN
 */


#include <Arduino.h>
#include "pwm_ramp.h"
#include "simpletimeout.h"


PWM_RAMP::PWM_RAMP(void) {
}


void PWM_RAMP::begin(uint8_t port, uint16_t update_rate, bool frequency) {
	
	// set port
	_port = port;
	_update_rate = update_rate;

	pinMode(port, OUTPUT);

	// reset values
	_current_pwm = 0;
	_target_pwm = 0;

	// start timer
	_timer.begin(_update_rate);

	write_pwm(0);
	set_ramp(0);

	// start pwm
	#ifdef DEBUG
		Serial.print("PWM_RAMP frequency set to ");
	#endif

	// Set PWM_RAMP frequency to 31 kHz for pin 9 and 10
	if (frequency) {

		#ifdef DEBUG
			Serial.print("31 kHz");
		#endif

		TCCR1B = TCCR1B & 0b11111000 | 1;
	}	

	#ifdef DEBUG
		else {
			Serial.print("500 Hz");
		}

		Serial.print(" at port ");
		Serial.println(port);
	#endif

}


// update using PWM_RAMP_UPDATE_RATE
uint16_t PWM_RAMP::update() {

	// ramp rate
	if (_timer.update()) {
		_current_pwm = ramp();
		write_pwm(_current_pwm);
	}

	return _current_pwm;
}


// pwm is stopped
// target and current are 0
bool PWM_RAMP::stopped(void) {
	return (_target_pwm || _current_pwm) == 0;
}


// set target pwm using default ramp
void PWM_RAMP::pwmWrite(uint16_t pwm) {

	
	// restrict pwm to max val setting
	if (pwm > PWM_RAMP_MAX_VAL) {
		pwm = PWM_RAMP_MAX_VAL;
	}

	_target_pwm = pwm;
}


// set ramp time
void PWM_RAMP::set_ramp(uint16_t ramp_time) {

	// ramp_time must be PWM_RAMP_UPDATE_RATE as minimum
	if (ramp_time < _update_rate) {
		ramp_time = _update_rate;
	}

	#ifdef DEBUG
		Serial.print("set ramp to ");
		Serial.println(ramp_time, DEC);
	#endif

	_ramp_time = ramp_time;
}


// calculate ramp value
uint16_t PWM_RAMP::ramp() {

	uint16_t delta;

	// check for != 0
	// set immediately
	if (_ramp_time == 0) {

	}

	// calculate ramp
	else {

		// steps = time for ramp (ms) / time for update repeat (ms)
		delta = (PWM_RAMP_MAX_VAL * _update_rate) / _ramp_time;

		// set delta to minimum 1
		if (delta == 0) {
			delta = 1;
		}

		// increase pwm
		if (_target_pwm > _current_pwm) {

			// restrict to max
			if ((_current_pwm + delta) >= _target_pwm) {

				_current_pwm = _target_pwm;
			}

			else {
				_current_pwm += delta;
			}
		}

		// decrease pwm
		if (_target_pwm < _current_pwm) {

			// restrict to 0
			if (_current_pwm < delta) {
				_current_pwm = 0;
			}

			else {
				_current_pwm -= delta;
			}
		}
	}


	return _current_pwm;
}


// get current pwm value
uint16_t PWM_RAMP::pwmRead(void) {
	return _current_pwm;
}


// get target pwm value
uint16_t PWM_RAMP::pwmReadTarget(void) {
	return _target_pwm;
}


// write pwm to port
void PWM_RAMP::write_pwm(uint16_t pwm) {

	// write pwm
	analogWrite(_port, pwm);
}