# ic74hc165 软件包

该软件包提供了对 74HC165（串行输入并行输出移位寄存器）的驱动支持，可通过 GPIO 或 SPI 两种方式读取。

## 支持特性

- 支持 GPIO 软件模拟读取；
- 支持 SPI 硬件读取方式；
- 支持多级串联读取；
- 适配 RT-Thread 驱动模型。

## 使用说明

在 `RT-Thread Settings` 中勾选 `ic74hc165`，初始化代码示例：

```c
#include "74165.h"
#include "74165_platform.h"

static IC74165_Handler_t _165_handler;
static uint8_t data[1];

void test_165(void)
{
    IC74165_Platform_Init(&_165_handler);
    IC74165_Init(&_165_handler, 1);
    IC74165_ReadAll(&_165_handler, data);
}
```

## 许可证

MIT
