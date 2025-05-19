#include "74165.h"
#include "74165_platform.h"

static IC74165_Handler_t handler;
static uint8_t data[1];

int ic74165_sample(void)
{
    IC74165_Platform_Init(&handler);
    if (IC74165_Init(&handler, 1) != IC74165_OK)
    {
        rt_kprintf("Init failed\n");
        return -1;
    }

    if (IC74165_ReadAll(&handler, data) == IC74165_OK)
    {
        rt_kprintf("74165 read: 0x%02X\n", data[0]);
    }
    else
    {
        rt_kprintf("74165 read failed\n");
    }

    return 0;
}
MSH_CMD_EXPORT(ic74165_sample, 74165 sample test);
