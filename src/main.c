/****************************************************************************
 *
 *   Copyright (C) 2013 PX4 Development Team. All rights reserved.
 *   Author: Pavel Kirienko <pavel.kirienko@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <ch.h>
#include <hal.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <config/config.h>
#include <sys.h>
#include <led.h>
#include <console.h>
#include <watchdog.h>
#include <motor/motor.h>
#include <motor/realtime/adc.h>

void application_halt_hook(void)
{
	motor_emergency();
	led_set_rgb(1, 0, 0);
}

static int init(void)
{
	int res = 0;

	led_init();
	led_set_rgb(0.05, 0.05, 0.05);

	/*
	 * Config
	 */
	res = config_init();
	if (res)
		return res;

	/*
	 * Safety
	 */
	watchdog_init();

	/*
	 * Motor control
	 */
	res = motor_init();
	if (res)
		return res;

	usleep(10000);
	res = motor_test_hardware();
	if (res)
		return res;
	lowsyslog("Power stage OK\n");

	if (motor_test_motor())
		lowsyslog("Motor is not connected or damaged\n");
	else
		lowsyslog("Motor OK\n");

	return 0;
}

__attribute__((noreturn))
static void die(int status)
{
	usleep(100000);
	lowsyslog("Init failed (%i)\n", status);
	// Really there is nothing left to do; just sit there and beep sadly:
	while (1) {
		motor_beep(100, 400);
		led_set_rgb(1, 0, 0);
		sleep(3);
	}
}

static void do_startup_beep(void)
{
	motor_beep(1000, 100);
	usleep(100 * 1000);
	motor_beep(1000, 100);
}

static void print_banner(void)
{
	lowsyslog("\n\n\n");
	lowsyslog("\x1b\x5b\x48");      // Home sweet home
	lowsyslog("\x1b\x5b\x32\x4a");  // Clear
	lowsyslog("PX4ESC\n");
}

int main(void)
{
	halInit();
	chSysInit();
	sdStart(&STDOUT_SD, NULL);

	usleep(300000);
	print_banner();

	const int init_status = init();

	console_init();

	if (init_status)
		die(init_status);

	do_startup_beep();

	motor_confirm_initialization();

	chThdSetPriority(LOWPRIO);

#if !NDEBUG
	// Debugging code, will be removed later
	extern uint64_t motor_timer_hnsec(void);
	uint64_t prev_ts = motor_timer_hnsec();
#endif

	while (1) {
#if !NDEBUG
		// Timer debugging
		const uint64_t ts = motor_timer_hnsec();
		assert(ts > prev_ts);
		prev_ts = ts;
#endif

		chThdYield();

		sleep(1);
		lowsyslog("%f\n", motor_adc_convert_temperature(motor_adc_get_last_sample().temperature));
	}

	return 0;
}
