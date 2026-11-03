# Simulink 模型说明

本目录存放《手把手教你学电控》系列配套的 Simulink 模型，按搭建顺序排列，从触发逻辑验证一直到完整的 FOC 闭环。每个模型都在前一个的基础上增加一部分，方便对照学习。

## 模型一览

| 文件 | 内容 |
|---|---|
| `Trigger_Test.slx` | 触发逻辑验证，自增计数测试 |
| `Trigger_Algorithm.slx` | 触发框架加六个算法模块 |
| `Trigger_Carrier.slx` | 加入等腰三角载波与顶点触发 |
| `Trigger_CarrierPWM 1.slx` | 载波加 PWM 比较，输出六路互补信号 |
| `Trigger_FOC_Com.slx` | 完整 FOC 闭环，含一拍延迟和死区设置 |

## 各模型说明

### Trigger_Test.slx

触发逻辑的最小验证模型。Pulse Generator 产生触发脉冲，Trigger 子系统在每次触发时执行 MATLAB Function 里的自增代码，Scope 观察累加结果。仿真后看到阶梯上升的波形，说明触发生效，计算周期也正确。

### Trigger_Algorithm.slx

在触发框架上加入六个核心算法模块：Clarke 变换、Park 变换、d 轴电流调节器、q 轴电流调节器、逆 Park 变换、SVPWM。输入三相电流、电角度和转速，输出三相占空比。这个模型只搭了算法骨架，还没有接入被控对象。

### Trigger_Carrier.slx

加入载波生成器。MATLAB Function 产生等腰三角载波，同时在载波顶点输出一个脉冲作为触发信号，替代前面独立的 Pulse Generator，让触发和载波同步。模型里已经配好电机、三相逆变器、直流电源和采样模块。

### Trigger_CarrierPWM 1.slx

在载波基础上加入 PWM 生成模块。载波与三相占空比比较，输出六路互补的开关信号，对应三相桥的上下管，送进逆变器门极。到这里，从采样、控制、调制到功率级已经串成一条完整链路。

### Trigger_FOC_Com.slx

完整的 FOC 闭环模型，也是整套教程的最终版本。包含单采样单更新载波、PWM 生成、电流环、速度环、Clarke/Park/逆 Park、SVPWM、一拍延迟（Unit Delay）、死区时间设置，以及电机、逆变器和采样。Step 模块给出转速或负载扰动，可以验证系统的跟随性能和抗扰性能。死区设置子系统用 Transport Delay 和逻辑运算模拟上下管的死区时间。

## 使用说明

- 需要 MATLAB/Simulink，建议 R2022b 及以上
- 模型里的电机参数按 MATLAB 预设填写，换成自己的电机时修改对应块参数
- 死区时间设置子系统的延迟时间按实际驱动板填写
- 仿真步长、powergui 的离散时间要和开关频率对应，否则载波和触发的时间关系会失准

## 补充说明

后续会按系列文章继续补充模型，新增的会追加到上表。
