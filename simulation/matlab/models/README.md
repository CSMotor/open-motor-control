# Simulink 模型说明

本目录存放《手把手教你学电控》系列配套的 Simulink 模型，按搭建顺序排列，从触发逻辑验证一直到完整的 FOC 闭环。每个模型都在前一个的基础上增加一部分，方便对照学习。

## 模型一览

| 文件 | 内容 |
|---|---|
| `Trigger_Test.slx` | 触发逻辑验证，自增计数测试 |
| `Trigger_Algorithm.slx` | 触发框架加六个算法模块 |
| `Trigger_Carrier.slx` | 加入等腰三角载波与顶点触发 |
| `Trigger_CarrierPWM 1.slx` | 载波加 PWM 比较，输出六路互补信号 |
| `Trigger_FOC_Com.slx` | 完整 FOC 闭环，含一拍延迟 |
| `Trigger_FOC_Deadtime.slx` | 在 Com 基础上加入三路死区 |
| `Trigger_FOC_Parameter.slx` | 参数化版本，传感器独立成模块 |
| `Trigger_FOC_NFO.slx` | 参数化模型加非线性磁链观测器，仅观测未闭环 |

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

完整的 FOC 闭环模型。包含单采样单更新载波、PWM 生成、电流环、速度环、Clarke/Park/逆 Park、SVPWM、一拍延迟（Unit Delay），以及电机、逆变器和采样。Step 模块给出转速或负载扰动，可以验证系统的跟随性能和抗扰性能。

### Trigger_FOC_Deadtime.slx

在 Trigger_FOC_Com 的基础上加入死区。每一相各有一个 Deadzone 子系统，用 Transport Delay 加逻辑判断，生成上桥臂两侧各留一个死区时间的互补驱动信号。可以用来对比有无死区时的相电流波形和 FFT 谐波分布。

### Trigger_FOC_Parameter.slx

参数化版本。电流传感器封装成一个独立模块，电机、逆变器、传感器等参数统一在初始化中设置，换电机或者换硬件时只改初始化部分，不用逐个进入子系统修改。

### Trigger_FOC_NFO.slx

在参数化模型的基础上加入非线性磁链观测器，观测器放在 NFO 子系统里，输入端电压和相电流，输出估计磁链和转子位置。这一版观测器只做观测，输出没有闭环回控制器，用来单独观察收敛过程，对照验证算法。

## 使用说明

- 需要 MATLAB/Simulink，建议 R2022b 及以上
- 模型里的电机参数按 MATLAB 预设填写，换成自己的电机时修改对应块参数
- 死区时间设置子系统的延迟时间按实际驱动板填写
- 仿真步长、powergui 的离散时间要和开关频率对应，否则载波和触发的时间关系会失准

## 补充说明

后续会按系列文章继续补充模型，新增的会追加到上表。
