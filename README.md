# open-motor-control

沉沙motor一下的电控开源库 —— 电机控制算法、仿真模型、C 语言驱动与实战资料，开源共享，共同使用。

> 作者主页（知乎）：[沉沙motor一下](https://www.zhihu.com/people/gun-ne-ni)
> 持续更新电控知识、调试经验与工程踩坑记录，欢迎关注交流。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

---

## 这是什么

这里整理我这些年做电机控制（电控）积累下来的东西：能跑的代码、能复现的仿真、能直接抄的参数和调参思路。目标很简单——让刚入行的朋友少走我走过的弯路，让有经验的朋友能直接拿去改。

内容都在持续补充中，欢迎 Issue 提需求、PR 提改进。

## 目录结构规划

```
open-motor-control/
├── src/               # C 语言电机控制核心库
│   ├── foc/           # FOC 磁场定向控制（Clarke/Park、SVPWM）
│   ├── pid/           # 电流环 / 速度环 / 位置环 PID
│   ├── observer/      # 龙伯格观测器、滑模观测器、磁链观测
│   ├── encoder/       # 增量式 / 绝对式编码器、霍尔换相
│   └── hal/           # 硬件抽象层（STM32 / GD32 / TI C2000）
├── simulation/        # 仿真模型
│   ├── matlab/        # MATLAB/Simulink 模型（PMSM、BLDC、无感）
│   └── python/        # Python 快速验证脚本与波形分析
├── examples/          # 可直接编译运行的最小示例
├── docs/              # 原理推导、调参笔记、调试手册
└── README.md
```

## 计划覆盖的内容

### 1. C 语言开源电机库

- **FOC 核心**：Clarke 变换、Park 变换、七段式 SVPWM 生成，全部定点/浮点双版本
- **电流环**：d/q 轴解耦 PI，抗积分饱和，参数自整定参考值
- **速度/位置环**：串级 PID，带前馈补偿
- **无感控制**：滑模观测器（SMO）、龙伯格观测器、高频注入起步
- **编码器接口**：增量式 ABZ、SSI、霍尔三相互补换相

代码风格：纯 C99，无动态内存分配，中断安全，方便移植到 MCU。

### 2. 仿真

- **Simulink 模型**：PMSM 矢量控制完整链路，含电机本体、逆变器、采样、控制器
- **参数辨识**：Rs / Ld / Lq / 磁链 ψf 的离线与在线辨识脚本
- **Python 验证**：算法先在 PC 上跑通波形，再上板，减少烧板次数

### 3. 实战资料

- 调参顺序与经验值（电流环 → 速度环 → 位置环）
- 常见故障排查：电流震荡、启动失败、低速抖动、过流保护误触发
- 示波器抓波形与数据分析方法
- 硬件设计注意事项：采样电阻、电流传感器、母线电容、栅极驱动

## 快速上手

后续示例会保证「一个命令编译、一个命令烧录」。当前先提供算法骨架，可直接复制进你的工程：

```c
#include "foc.h"

foc_ctx_t foc;
foc_init(&foc, 0.0001f);          /* 控制周期 100us */

void TIM_ISR(void) {
    float ia = adc_read_ia();
    float ib = adc_read_ib();
    float ic = -(ia + ib);

    foc_update(&foc, ia, ib, ic, encoder_get_angle());
    pwm_set_duty(foc.duty_a, foc.duty_b, foc.duty_c);
}
```

## 参与贡献

- 发现 bug、想要某个功能：直接开 Issue，说清现象和你的硬件平台
- 想提交代码：请保持 C99、无动态内存、注释用中文或英文均可
- 分享你的调试经验同样有价值，欢迎写进 `docs/`

## 交流

知乎主页：[https://www.zhihu.com/people/gun-ne-ni](https://www.zhihu.com/people/gun-ne-ni)

电控这条路不好走，但一起走会好很多。

## 许可证

[MIT](LICENSE) —— 随便用，商用也行，保留版权声明即可。
