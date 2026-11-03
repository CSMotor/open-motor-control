/*
 * foc.h - 磁场定向控制（FOC）核心库
 * open-motor-control / 沉沙motor一下
 * https://www.zhihu.com/people/gun-ne-ni
 *
 * 纯 C99，无动态内存，中断安全。
 */

#ifndef FOC_H
#define FOC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 控制器状态 */
typedef struct {
  float vd;          /* d 轴电压指令 */
  float vq;          /* q 轴电压指令 */
  float duty_a;      /* A 相占空比 0~1 */
  float duty_b;
  float duty_c;
  float theta_e;     /* 电角度(rad) */
  float dt;          /* 控制周期(s) */
  uint8_t reverse;   /* 1 = 反转 */
} foc_ctx_t;

/* Clarke 变换：三相 -> alpha/beta */
void foc_clarke(const float ia, const float ib, const float ic,
                float *alpha, float *beta);

/* Park 变换：alpha/beta -> d/q */
void foc_park(float alpha, float beta, float theta,
              float *d, float *q);

/* 逆 Park 变换：d/q -> alpha/beta */
void foc_inv_park(float d, float q, float theta,
                  float *alpha, float *beta);

/* 七段式 SVPWM：alpha/beta -> 三相占空比 0~1 */
void foc_svpwm(float alpha, float beta, float vdc,
               float *da, float *db, float *dc);

/* 查表正弦，避免 libm 依赖；table 由 foc_init_sin_table() 填充 */
void foc_init_sin_table(void);
float foc_sin(float x);
float foc_cos(float x);

/* 初始化 */
void foc_init(foc_ctx_t *ctx, float dt);

/* 一个控制周期：输入三相电流、电角度、母线电压，输出占空比 */
void foc_step(foc_ctx_t *ctx,
              float ia, float ib, float ic,
              float theta_e, float vdc);

#ifdef __cplusplus
}
#endif

#endif /* FOC_H */
