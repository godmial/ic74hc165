/**
 **********************************************************************************
 * @file   74165_platform.c
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

#include "74165_platform.h"
#include <rtthread.h>
#include <rtdevice.h>

/* 通用 GPIO 配置接口 */
static void IC74165_SetGPIO_OUT(rt_base_t GPIO_Pin)
{
    rt_pin_mode(GPIO_Pin, PIN_MODE_OUTPUT);
}

static void IC74165_SetGPIO_IN(rt_base_t GPIO_Pin)
{
    rt_pin_mode(GPIO_Pin, PIN_MODE_INPUT);
}

/* CLKINH 写入函数（GPIO + SPI 通用） */
#if (IC74165_CLKINH_ENABLE)
static void IC74165_ClkInhWrite(uint8_t Level)
{
    rt_pin_write(CE_PIN, Level);
}
#endif

#ifdef PKG_IC74HC165_MODE_GPIO
/* ====================== GPIO 模式实现 ========================== */

static void IC74165_PlatformInit(void)
{
#if (IC74165_CLKINH_ENABLE)
    IC74165_SetGPIO_OUT(CE_PIN);
#endif
    IC74165_SetGPIO_OUT(CP_PIN);
    IC74165_SetGPIO_OUT(PL_PIN);
    IC74165_SetGPIO_IN(Q7_PIN);
}

static void IC74165_PlatformDeInit(void) {}

static void IC74165_ClkWrite(uint8_t Level)
{
    rt_pin_write(CP_PIN, Level);
}

static void IC74165_ShLdWrite(uint8_t Level)
{
    rt_pin_write(PL_PIN, Level);
}

static uint8_t IC74165_QhRead(void)
{
    return rt_pin_read(Q7_PIN);
}

static void IC74165_DelayUs(uint8_t Delay)
{
    rt_hw_us_delay(Delay);
}

void IC74165_Platform_Init(IC74165_Handler_t *Handler)
{
    IC74165_PLATFORM_SET_COMMUNICATION(Handler, IC74165_COMMUNICATION_GPIO);
    IC74165_PLATFORM_LINK_INIT(Handler, IC74165_PlatformInit);
    IC74165_PLATFORM_LINK_DEINIT(Handler, IC74165_PlatformDeInit);
#if (IC74165_CLKINH_ENABLE)
    IC74165_PLATFORM_LINK_CLKINHWRITE(Handler, IC74165_ClkInhWrite);
#endif
    IC74165_PLATFORM_LINK_GPIO_CLKWRITE(Handler, IC74165_ClkWrite);
    IC74165_PLATFORM_LINK_GPIO_SHLDWRITE(Handler, IC74165_ShLdWrite);
    IC74165_PLATFORM_LINK_GPIO_QHREAD(Handler, IC74165_QhRead);
    IC74165_PLATFORM_LINK_GPIO_DELAYUS(Handler, IC74165_DelayUs);
}
#endif /* PKG_IC74HC165_MODE_GPIO */

#ifdef PKG_IC74HC165_MODE_SPI
/* ====================== SPI 模式实现 ========================== */

static struct rt_spi_device *spi_dev = RT_NULL;

/* 挂载 SPI 设备 */
static int rt_hw_spi_74165_init(void)
{
    rt_hw_spi_device_attach(IC74165_SPI_BUS_NAME, IC74165_SPI_DEV_NAME, GPIOB, GPIO_PIN_10);
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_74165_init);

/* SPI 初始化 */
static void IC74165_PlatformInit_SPI(void)
{
    spi_dev = (struct rt_spi_device *)rt_device_find(IC74165_SPI_DEV_NAME);
    if (spi_dev == RT_NULL)
    {
        rt_kprintf("Error: SPI device not found.\n");
        return;
    }

    struct rt_spi_configuration cfg = {
        .mode = RT_SPI_MASTER | RT_SPI_MODE_1 | RT_SPI_MSB,
        .max_hz = 3000000,
        .data_width = 8,
    };
    rt_spi_configure(spi_dev, &cfg);

#if (IC74165_CLKINH_ENABLE)
    IC74165_SetGPIO_OUT(CE_PIN);
#endif
}

/* SPI 反初始化 */
static void IC74165_PlatformDeInit_SPI(void)
{
    spi_dev = RT_NULL;
}

/* SPI 传输接口 */
static void IC74165_SPI_SendReceive(uint8_t *SendData, uint8_t *ReceiveData, uint8_t Len)
{
    uint8_t TxBuff[SPI_MAXIMUM_BUFFER_SIZE] = {0xFF};
    struct rt_spi_message msg = {
        .send_buf = SendData ? SendData : TxBuff,
        .recv_buf = ReceiveData,
        .length = Len};

    if (rt_spi_transfer_message(spi_dev, &msg) != RT_EOK)
    {
        rt_kprintf("spi transfer failed!\n");
    }
}

/* 注册 SPI 接口 */
void IC74165_Platform_Init_SPI(IC74165_Handler_t *Handler)
{
    IC74165_PLATFORM_SET_COMMUNICATION(Handler, IC74165_COMMUNICATION_SPI);
    IC74165_PLATFORM_LINK_INIT(Handler, IC74165_PlatformInit_SPI);
    IC74165_PLATFORM_LINK_DEINIT(Handler, IC74165_PlatformDeInit_SPI);
#if (IC74165_CLKINH_ENABLE)
    IC74165_PLATFORM_LINK_CLKINHWRITE(Handler, IC74165_ClkInhWrite);
#endif
    IC74165_PLATFORM_LINK_SPI_SENDRECEIVE(Handler, IC74165_SPI_SendReceive);
}
#endif /* PKG_IC74HC165_MODE_SPI */
