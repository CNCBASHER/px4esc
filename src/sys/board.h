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

#pragma once

/// Assert is needed for STM32 SPL
#include <assert.h>
#define assert_param(x) assert(x)

#define STM32_HSECLK            24000000

#define STM32F10X_MD

/*
 * GPIO
 */
// Misc
#define GPIO_PORT_SERIAL_RX     GPIOA
#define GPIO_PIN_SERIAL_RX      10

// LED
#define GPIO_PORT_LED_ERROR     GPIOB
#define GPIO_PIN_LED_ERROR      4

#define GPIO_PORT_LED_STATUS    GPIOB
#define GPIO_PIN_LED_STATUS     3

// Testpoints
#define GPIO_PORT_TEST_A        GPIOB
#define GPIO_PIN_TEST_A         15
// TODO: ADC testpoint is temporarily allocated on the I2C SCL pin
#define GPIO_PORT_TEST_ADC      GPIOB
#define GPIO_PIN_TEST_ADC       10
// TODO: Motor timer testpoint is temporarily allocated on the I2C SDA pin
#define GPIO_PORT_TEST_MTIM     GPIOB
#define GPIO_PIN_TEST_MTIM      11
// TODO: Zero cross monitor testpoint is temporarily allocated on the PPM input pin
#define GPIO_PORT_TEST_MZC      GPIOA
#define GPIO_PIN_TEST_MZC       8

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

#define VAL_GPIOACRL            0xb8000008      // 7..0
// TODO: remove testpad output from PPM pin:
#define VAL_GPIOACRH            0x888b88b3      // 15..8
#define VAL_GPIOAODR            0x00000000

#define VAL_GPIOBCRL            0xbb8222bb
// TODO: remove testpad outputs from I2C pins:
#define VAL_GPIOBCRH            0x3888338b
#define VAL_GPIOBODR            0x00000000

#define VAL_GPIOCCRL            0x88888888
#define VAL_GPIOCCRH            0x88888888
#define VAL_GPIOCODR            0x00000000

#define VAL_GPIODCRL            0x88888888
#define VAL_GPIODCRH            0x88888888
#define VAL_GPIODODR            0x00000000

#define VAL_GPIOECRL            0x88888888
#define VAL_GPIOECRH            0x88888888
#define VAL_GPIOEODR            0x00000000

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
    void boardInit(void);
    void debugPortDisable(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */
