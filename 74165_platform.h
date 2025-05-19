#ifndef _74165_PLATFORM_H_
#define _74165_PLATFORM_H_

#include "74165.h"
#include <stdint.h>
#include <string.h>
#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"

/* CLKINH 可配置 */
#ifdef PKG_IC74HC165_USE_CLKINH
#define IC74165_CLKINH_ENABLE 1
#else
#define IC74165_CLKINH_ENABLE 0
#endif

/* SPI 总线与设备名称可配置 */
#define IC74165_SPI_BUS_NAME    PKG_IC74HC165_SPI_BUS_NAME
#define IC74165_SPI_DEV_NAME    PKG_IC74HC165_SPI_DEV_NAME

/* 默认引脚定义 */
#define CP_PIN GET_PIN(A, 5)
#define PL_PIN GET_PIN(A, 7)
#define Q7_PIN GET_PIN(A, 6)
#define CE_PIN GET_PIN(A, 8)

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
