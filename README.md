
# ic74hc165 软件包

`ic74hc165` 是一个用于驱动并行输入串行输出芯片 **74HC165** 的 RT-Thread 软件包，支持 **GPIO** 和 **SPI** 两种通信模式，适用于按键阵列、编码器阵列等输入扩展场景。

该软件包基于 Hossein.M 开源驱动适配，并通过 Kconfig 支持灵活配置。

---

## 📁 软件包结构

```
ic74hc165/
├── 74165.c / 74165.h               # 逻辑核心实现
├── 74165_platform.c / .h          # 平台适配层，支持 GPIO/SPI 模式
├── sample/
│   └── sample.c                   # 示例代码，线程方式读取输入
├── Kconfig                        # 软件包配置入口
├── SConscript                     # 构建脚本
└── README.md
```

---

## ⚙️ Kconfig 配置项说明

在 RT-Thread Env 或 RT-Thread Studio 中执行 `menuconfig`，启用：

```
RT-Thread Settings >
  Software Packages >
    Peripheral Drivers >
      [*] ic74hc165: 74HC165 GPIO/SPI 驱动
```

配置项说明：

| 配置项 | 说明 |
|--------|------|
| 通信模式选择（GPIO / SPI） | GPIO/SPI 二选一 |
| SPI 总线名称 | 默认 spi1，可自定义 |
| SPI 设备名称 | 默认 spi10，可自定义 |
| 启用示例线程 | 是否启用 sample/sample.c |
| 启用 CLK_INH 控制 | 是否使用 CE 管脚控制 |
| 芯片节点数量 | 支持多片级联 |

---

## 🚀 示例运行（控制台命令）

```sh
hc165_demo
```

运行后输出：

```
[IC74165] Data: 0x3F 0x00
```

---

## 🔌 接线说明（示例）

| 信号 | 功能 | GPIO 示例 |
|------|------|------------|
| CP   | 串行时钟输入 | PA5 |
| PL   | 并行加载 | PA7 |
| Q7   | 串行数据输出 | PA6 |
| CE   | 禁止时钟（可选） | PA8 |



---

## ⚠️ 使用 SPI 模式时的注意事项

若使用 SPI 模式读取 74HC165，请务必完成以下准备工作：

1. **打开 RT-Thread SPI 驱动支持**：
   - 在 `menuconfig` 中启用 `Device Drivers > Enable SPI Bus Device`

2. **取消 board.h 中 SPI 总线设备的注释**：
   - 确保 `board.h` 中定义了 `BSP_USING_SPI1` 或其他 SPIx 相关宏

3. **CubeMX 配置 SPI 为全双工模式**：
   - 确保 SPI 工作在主模式（Master）+ 全双工（Full Duplex）配置
   - MISO（接 Q7），SCK（接 CP），CS 可使用软件控制（PL）

未正确配置将导致 SPI 设备找不到或读取数据错误。

---

## 📄 License

本软件包遵循 MIT 协议，原始作者为 [Hossein.M](https://github.com/Hossein-M98)。

---

## 📬 联系方式

- GitHub: [godmial](https://github.com/godmial)
- Gitee：[godmial](https://gitee.com/godmial)
- 邮箱: 2633967641@qq.com
