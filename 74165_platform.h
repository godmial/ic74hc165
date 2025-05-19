/**
 **********************************************************************************
 * @file   74165_platform.h
 * @author Hossein.M
 * @brief  A sample Platform dependent layer for 74165 Driver
 **********************************************************************************
 */

 #ifndef _74165_PLATFORM_H_
 #define _74165_PLATFORM_H_
 
 #include "74165.h"
 #include <stdint.h>
 #include <string.h>
 #include "rtthread.h"
 #include "rtdevice.h"
 #include "board.h"
 
 /* Functionality Options --------------------------------------------------------*/
 
 #define IC74165_CLKINH_ENABLE 1
 
 /**
  * @brief  固定的 IO 引脚定义
  */
 #define CP_PIN GET_PIN(A, 5)   // CLK
 #define PL_PIN GET_PIN(A, 7)   // SH/LD
 #define Q7_PIN GET_PIN(A, 6)   // Q7
 #define CE_PIN GET_PIN(A, 8)   // CLK_INH
 
 #define SPI_MAXIMUM_BUFFER_SIZE 64
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 void IC74165_Platform_Init(IC74165_Handler_t *Handler);
 void IC74165_Platform_Init_SPI(IC74165_Handler_t *Handler);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif //! _74165_PLATFORM_H_
 