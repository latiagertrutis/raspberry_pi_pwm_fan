/* **************************************************************** */
/* Filename: pwm_fan.h                                              */
/* Description:                                                     */
/* Author: Mateo Rodriguez Ripolles (teorodrip@posteo.net)          */
/* Maintainer:                                                      */
/* Created: Thu May 21 19:43:07 2020 (+0200)                        */
/* **************************************************************** */

#ifndef PWM_FAN_H
#define PWM_FAN_H

#define ABS(x) (x < 0 ? -x : x)

// PWM output on RPi Plug P1 pin 12 (which is GPIO pin 18)
// in alt fun 5.
// Note that this is the _only_ PWM pin available on the RPi IO headers
#define PIN RPI_GPIO_P1_12
// and it is controlled by PWM channel 0
#define PWM_CHANNEL 0
// This controls the max range of the PWM signal
#define PWM_RANGE 1024

/* Size of the buffer for reading temperature */
#define TEMPERATURE_BUFF 16

/* steps of temperature and speed of the fan */
#define STEPS 12
#define FAN_MIN 0
#define FAN_MAX PWM_RANGE
#define TEMP_STEP {48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70} //[°C]
#define SPEED_STEP {FAN_MIN, 205, 295, 385, 475, 565, 655, 745, 835, 925, 1000, FAN_MAX} // bcm2835_pwm_set_data;
#define TEMP_FILE "/sys/class/thermal/thermal_zone0/temp"

/* Fan speed will change only of the difference of temperature */
/* is higher than hysteresis */
#define HYST 1


#endif
