#include <rtthread.h>
#include "74165.h"
#include "74165_platform.h"

#define THREAD_STACK_SIZE 1024
#define THREAD_PRIORITY    20
#define THREAD_TICK        10

static IC74165_Handler_t handler;
static uint8_t data[IC74HC165_NODE_COUNT];

static void hc165_thread_entry(void *parameter)
{
#ifdef PKG_IC74HC165_GPIO_MODE
    IC74165_Platform_Init(&handler);
#elif defined(PKG_IC74HC165_SPI_MODE)
    IC74165_Platform_Init_SPI(&handler);
#endif

    if (IC74165_Init(&handler, IC74HC165_NODE_COUNT) != IC74165_OK)
    {
        rt_kprintf("74165 init failed\n");
        return;
    }

    while (1)
    {
        if (IC74165_ReadAll(&handler, data) == IC74165_OK)
        {
            rt_kprintf("[IC74165] Data:");
            for (int i = 0; i < IC74HC165_NODE_COUNT; i++)
            {
                rt_kprintf(" 0x%02X", data[i]);
            }
            rt_kprintf("\n");
        }
        else
        {
            rt_kprintf("74165 read failed\n");
        }

        rt_thread_mdelay(100);
    }
}

int hc165_demo(void)
{
    rt_thread_t thread = rt_thread_create("hc165",
                                          hc165_thread_entry,
                                          RT_NULL,
                                          THREAD_STACK_SIZE,
                                          THREAD_PRIORITY,
                                          THREAD_TICK);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
        return 0;
    }

    return -1;
}
MSH_CMD_EXPORT(hc165_demo, Start 74165 test thread);
