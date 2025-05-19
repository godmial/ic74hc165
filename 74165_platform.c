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

/* Includes ---------------------------------------------------------------------*/
#include "74165_platform.h"
#include <rtthread.h>
#include <rtdevice.h>

/**
 ==================================================================================
                           ##### Private Functions #####
 ==================================================================================
 */

void IC74165_SetGPIO_OUT(rt_base_t GPIO_Pin)
{
    rt_pin_mode(GPIO_Pin, PIN_MODE_OUTPUT);
}

void IC74165_SetGPIO_IN(rt_base_t GPIO_Pin)
{
    rt_pin_mode(GPIO_Pin, PIN_MODE_INPUT);
}

static void IC74165_PlatformInit(void)
{
#if (IC74165_CLKINH_ENABLE)
    IC74165_SetGPIO_OUT(CE_PIN);
#endif
    IC74165_SetGPIO_OUT(CP_PIN);
    IC74165_SetGPIO_OUT(PL_PIN);
    IC74165_SetGPIO_IN(Q7_PIN);
}

static void IC74165_PlatformDeInit(void)
{
}

#if (IC74165_CLKINH_ENABLE)
static void IC74165_ClkInhWrite(uint8_t Level)
{
    rt_pin_write(CE_PIN, Level);
}
#endif

static uint8_t IC74165_QhRead(void)
{
    return rt_pin_read(Q7_PIN);
}

static void IC74165_ClkWrite(uint8_t Level)
{
    rt_pin_write(CP_PIN, Level);
}

static void IC74165_ShLdWrite(uint8_t Level)
{
    rt_pin_write(PL_PIN, Level);
}

static void IC74165_DelayUs(uint8_t Delay)
{
    rt_hw_us_delay(Delay);
}

struct rt_spi_device *spi_dev;
static void IC74165_PlatformInit_SPI(void)
{

    spi_dev = (struct rt_spi_device *)rt_device_find("spi10");
    if (spi_dev == RT_NULL)
    {
        rt_kprintf("Error: SPI device not found.\n");
        return -1;
    }

    //  配置 spi_dev
    struct rt_spi_configuration _74165_spi_config;
    _74165_spi_config.mode = RT_SPI_MASTER | RT_SPI_MODE_1 | RT_SPI_MSB;
    _74165_spi_config.max_hz = 3000000;
    _74165_spi_config.data_width = 8;
    rt_spi_configure((struct rt_spi_device *)spi_dev, &_74165_spi_config);

#if (IC74165_CLKINH_ENABLE)
    IC74165_SetGPIO_OUT(CE_PIN);
#endif

    return 0;
}

static int rt_hw_spi_74165_init(void)
{
    rt_hw_spi_device_attach("spi1", "spi10", GPIOB, GPIO_PIN_10);
    // IC74165_PlatformInit_SPI();

    return RT_EOK;
}
/* 导出到自动初始化 */
INIT_COMPONENT_EXPORT(rt_hw_spi_74165_init);

static void IC74165_PlatformDeInit_SPI(void)
{
    // #if (IC74165_CLKINH_ENABLE)
    //     rt_pin_mode(IC74165_CLKINH_GPIO, PIN_MODE_INPUT);
    // #endif

    //     rt_spi_detach_device(spi_device_handle);
    //     spi_device_handle = RT_NULL;
}

static void IC74165_SPI_SendReceive(uint8_t *SendData, uint8_t *ReceiveData, uint8_t Len)
{

    struct rt_spi_message spi_message;
    uint8_t TxBuff[SPI_MAXIMUM_BUFFER_SIZE] = {0xFF};

    while (Len > SPI_MAXIMUM_BUFFER_SIZE)
    {
        spi_message.send_buf = SendData ? SendData : TxBuff;
        spi_message.recv_buf = ReceiveData;
        spi_message.length = SPI_MAXIMUM_BUFFER_SIZE;

        if (rt_spi_transfer(spi_dev, spi_message.send_buf, spi_message.recv_buf, spi_message.length) != RT_EOK)
            return;

        Len -= SPI_MAXIMUM_BUFFER_SIZE;
        if (ReceiveData)
            ReceiveData += SPI_MAXIMUM_BUFFER_SIZE;
        if (SendData)
            SendData += SPI_MAXIMUM_BUFFER_SIZE;
    }

    if (Len)
    {
        spi_message.send_buf = SendData ? SendData : TxBuff;
        spi_message.recv_buf = ReceiveData;
        spi_message.length = Len;

        if (rt_spi_transfer(spi_dev, spi_message.send_buf, spi_message.recv_buf, spi_message.length) != RT_EOK)
            return;
    }

    struct rt_spi_message msg;
    msg.send_buf = SendData;
    msg.recv_buf = ReceiveData;
    msg.length = Len;

    if (rt_spi_transfer_message(spi_dev, &msg) != Len)
    {
        rt_kprintf("spi transfer failed!\n");
    }
}

/**
 ==================================================================================
                            ##### Public Functions #####
 ==================================================================================
 */

/**
 * @brief  Initialize platform dependent layer to communicate with 74165 using
 *         GPIO.
 * @param  Handler: Pointer to handler
 * @retval None
 */
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

/**
 * @brief  Initialize platform dependent layer to communicate with 74165 using
 *         SPI.
 * @param  Handler: Pointer to handler
 * @retval None
 */
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
