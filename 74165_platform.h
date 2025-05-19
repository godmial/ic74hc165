/**
 **********************************************************************************
 * @file   74165_platform.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  A sample Platform dependent layer for 74165 Driver
 **********************************************************************************
 *
 * Copyright (c) 2021 Hossein.M (MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************
 */

/* Define to prevent recursive inclusion ----------------------------------------*/
#ifndef _74165_PLATFORM_H_
#define _74165_PLATFORM_H_

/* Includes ---------------------------------------------------------------------*/

#include "74165.h"
#include <stdint.h>
#include <stdint.h>
#include <string.h>
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"

/* Functionality Options --------------------------------------------------------*/
/**
 * @brief  Specify IO Pins of STM32 connected to 74165
 */

#define IC74165_CLKINH_ENABLE 1

#define CP_PIN GET_PIN(A, 5) // CLK
#define PL_PIN GET_PIN(A, 7) // SH
#define Q7_PIN GET_PIN(A, 6)
#define CE_PIN GET_PIN(A, 8)

#define SPI_MAXIMUM_BUFFER_SIZE 64

void IC74165_Platform_Init(IC74165_Handler_t *Handler);

void IC74165_Platform_Init_SPI(IC74165_Handler_t *Handler);

/**
 ==================================================================================
                               ##### Functions #####
 ==================================================================================
 */

/**
 * @brief  Initialize platform dependent layer to communicate with 74165 using
 *         GPIO.
 * @param  Handler: Pointer to handler
 * @retval None
 */

#endif //! _74165_PLATFORM_H_
