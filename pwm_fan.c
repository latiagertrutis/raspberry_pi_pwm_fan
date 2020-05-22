/* **************************************************************** */
/* Filename: pwm.c                                                  */
/* Description:                                                     */
/* Author: Mateo Rodriguez Ripolles (teorodrip@posteo.net)          */
/* Maintainer:                                                      */
/* Created: Thu May 21 19:18:19 2020 (+0200)                        */
/* **************************************************************** */

#include <bcm2835.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include "pwm_fan.h"

static volatile sig_atomic_t stop;

static void stop_pwm_fan(int signum)
{
	stop = 1;
}

static void init_pwm()
{
	// Set the output pin to Alt Fun 5, to allow PWM channel 0 to be output there
	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_ALT5);
	// Clock divider is set to 16.
	// With a divider of 16 and a RANGE of 1024, in MARKSPACE mode,
	// the pulse repetition frequency will be
	// 1.2MHz/1024 = 1171.875Hz, suitable for driving a DC motor with PWM
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
	bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
	bcm2835_pwm_set_range(PWM_CHANNEL, PWM_RANGE);
}

static int read_temperature(char *temp_file)
{
	int fd;
	ssize_t rd;
	char tmp_buff[TEMPERATURE_BUFF];

	/* open temperature file */
	fd = open(temp_file, O_RDONLY);
	if (fd < 1)
		return -1;

	/* read from temperature file */
	rd = read(fd, tmp_buff, TEMPERATURE_BUFF);
	if (rd < 0 || rd >= TEMPERATURE_BUFF)
		return -1;
	if (close(fd) < 0)
		return -1;

	tmp_buff[rd] = 0;
	return atoi(tmp_buff);
}

static uint32_t calc_interpolation(uint32_t speed_i,
																	 uint32_t speed_i_1,
																	 int temp_i,
																	 int temp_i_1,
																	 int temp)
{
	double diff_1, diff_2, diff_3;

	diff_1 = speed_i_1 - speed_i;
	diff_2 = temp_i_1 - temp_i;
	diff_3 = temp - temp_i;
	return ((uint32_t)((diff_1 / diff_2) * diff_3)) + speed_i;
}

static uint32_t calc_fan_speed(int cpu_temp)
{
	int temp_step[STEPS] = TEMP_STEP;
	uint32_t speed_step[STEPS] = SPEED_STEP;

	/* Check if the speed is in the extreme, else */
	/*lineary interpole the speed */
	if (cpu_temp < temp_step[0])
		return speed_step[0];
	if (cpu_temp > temp_step[STEPS - 1])
		return speed_step[STEPS - 1];
	for (int i = 0; i < STEPS - 1; ++i)
		{
			if (cpu_temp >= temp_step[i] && cpu_temp < temp_step[i + 1])
				return calc_interpolation(speed_step[i],
																	speed_step[i + 1],
																	temp_step[i],
																	temp_step[i + 1],
																	cpu_temp);
		}
	return speed_step[STEPS - 1];
}

int main(int argc, char **argv)
{
	int cpu_temp, cpu_temp_old;
	uint32_t fan_speed, fan_speed_old;

	stop = 0;
	cpu_temp = cpu_temp_old = 0;
	fan_speed = fan_speed_old = FAN_MAX;
	if (!bcm2835_init())
		return 1;

	signal(SIGINT, stop_pwm_fan);

	init_pwm();

	while (!stop)
		{
			bcm2835_delay(WAIT_TIME);

			/* Get the temperature */
			cpu_temp = read_temperature(TEMP_FILE);
			if (cpu_temp < 0)
				{
					printf("Error reading temperature from:\n%s\n", TEMP_FILE);
					continue;
				}
			/* Check if temperature reaches the umbral */
			if (ABS(cpu_temp - cpu_temp_old) < HYST)
				continue;

			fan_speed = calc_fan_speed(cpu_temp);

			#ifdef TGT_DBG
			printf("Temperature: %d\nSpeed: %d\n\n", cpu_temp, fan_speed);
			#endif

			if (fan_speed != fan_speed_old)
				{
					bcm2835_pwm_set_data(PWM_CHANNEL, fan_speed);
					fan_speed_old = fan_speed;
				}
			cpu_temp_old = cpu_temp;
		}
	printf("pwm_fan stopped.\n");
	bcm2835_close();
	return 0;
}
