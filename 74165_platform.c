/**
 **********************************************************************************
 * @file   74165_platform.c
 * @brief  Platform-dependent implementation for 74165 Driver
 **********************************************************************************
 */

 #include "74165_platform.h"
 #include <rtthread.h>
 #include <rtdevice.h>
 
 static void IC74165_SetGPIO_OUT(rt_base_t GPIO_Pin)
 {
     rt_pin_mode(GPIO_Pin, PIN_MODE_OUTPUT);
 }
 
 static void IC74165_SetGPIO_IN(rt_base_t GPIO_Pin)
 {
     rt_pin_mode(GPIO_Pin, PIN_MODE_INPUT);
 }
 
 #ifdef PKG_IC74HC165_GPIO_MODE
 
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
 
 #if (IC74165_CLKINH_ENABLE)
 static void IC74165_ClkInhWrite(uint8_t Level)
 {
     rt_pin_write(CE_PIN, Level);
 }
 #endif
 
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
 #endif /* PKG_IC74HC165_GPIO_MODE */
 
 #ifdef PKG_IC74HC165_SPI_MODE
 
 static struct rt_spi_device *spi_dev = RT_NULL;
 
 static void IC74165_PlatformInit_SPI(void)
 {
     spi_dev = (struct rt_spi_device *)rt_device_find("spi10");
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
 
 static void IC74165_PlatformDeInit_SPI(void)
 {
     spi_dev = RT_NULL;
 }
 
 static void IC74165_SPI_SendReceive(uint8_t *SendData, uint8_t *ReceiveData, uint8_t Len)
 {
     uint8_t TxBuff[SPI_MAXIMUM_BUFFER_SIZE] = {0xFF};
     struct rt_spi_message msg = {
         .send_buf = SendData ? SendData : TxBuff,
         .recv_buf = ReceiveData,
         .length = Len
     };
 
     if (rt_spi_transfer_message(spi_dev, &msg) != Len)
     {
         rt_kprintf("spi transfer failed!\n");
     }
 }
 
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
 #endif /* PKG_IC74HC165_SPI_MODE */
 