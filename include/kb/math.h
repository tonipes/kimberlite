// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>

#include <math.h>
#include <float.h>
#include <limits.h>
#include <float.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ELEMWISE_OP1_T2(func) { func(a.x), func(a.y) }
#define ELEMWISE_OP1_T3(func) { func(a.x), func(a.y), func(a.z) }
#define ELEMWISE_OP1_T4(func) { func(a.x), func(a.y), func(a.z), func(a.w) }

#define ELEMWISE_OP1_F_T2(func, f) { func(a.x, f), func(a.y, f) }
#define ELEMWISE_OP1_F_T3(func, f) { func(a.x, f), func(a.y, f), func(a.z, f) }
#define ELEMWISE_OP1_F_T4(func, f) { func(a.x, f), func(a.y, f), func(a.z, f), func(a.w, f) }

#define ELEMWISE_COMB_OP1_T2(func, comb) func(a.x) comb func(a.y)
#define ELEMWISE_COMB_OP1_T3(func, comb) func(a.x) comb func(a.y) comb func(a.z)
#define ELEMWISE_COMB_OP1_T4(func, comb) func(a.x) comb func(a.y) comb func(a.z) comb func(a.w)

#define ELEMWISE_OP2_T2(func) { func(a.x, b.x), func(a.y, b.y) }
#define ELEMWISE_OP2_T3(func) { func(a.x, b.x), func(a.y, b.y), func(a.z, b.z) }
#define ELEMWISE_OP2_T4(func) { func(a.x, b.x), func(a.y, b.y), func(a.z, b.z), func(a.w, b.w) }

#define ELEMWISE_OP2_F_T2(func, f) { func(a.x, b.x, f), func(a.y, b.y, f) }
#define ELEMWISE_OP2_F_T3(func, f) { func(a.x, b.x, f), func(a.y, b.y, f), func(a.z, b.z, f) }
#define ELEMWISE_OP2_F_T4(func, f) { func(a.x, b.x, f), func(a.y, b.y, f), func(a.z, b.z, f), func(a.w, b.w, f) }

#define ELEMWISE_COMB_OP2_T2(func, comb) func(a.x, b.x) comb func(a.y, b.y)
#define ELEMWISE_COMB_OP2_T3(func, comb) func(a.x, b.x) comb func(a.y, b.y) comb func(a.z, b.z)
#define ELEMWISE_COMB_OP2_T4(func, comb) func(a.x, b.x) comb func(a.y, b.y) comb func(a.z, b.z) comb func(a.w, b.w)

#define KB_MAX(_a, _b) _a > _b ? _a : _b
#define KB_MIN(_a, _b) _a < _b ? _a : _b
#define KB_CLAMP(_t, _min, _max) _t > _max ? _max : (_t < _min ? _min : _t)

#define VECTOR_TYPE_2(name, type) typedef struct name { type x, y; } name;
#define VECTOR_TYPE_3(name, type) typedef struct name { type x, y, z; } name;
#define VECTOR_TYPE_4(name, type) typedef struct name { type x, y, z, w; } name;

typedef int32_t   kb_int;
typedef float     kb_float;

#define PI         3.1415926535897932384626433832795f
#define PI2        6.2831853071795864769252867665590f
#define PIHALF     1.5707963267948966192313216916398f
#define PIQUARTER  0.7853981633974483096156608458199f
#define SQRT2      1.4142135623730950488016887242097f
#define LOGNAT10   2.3025850929940456840179914546844f
#define INVLOGNAT2 1.4426950408889634073599246810019f
#define LOGNAT2HI  0.6931471805599453094172321214582f
#define LOGNAT2LO  1.90821492927058770002e-10f
#define E          2.7182818284590452353602874713527f
#define FLOATMIN   1.175494e-38f
#define FLOATMAX   3.402823e+38f
#define EPSILON    FLT_EPSILON

VECTOR_TYPE_2(kb_int2, kb_int)
VECTOR_TYPE_3(kb_int3, kb_int)
VECTOR_TYPE_4(kb_int4, kb_int)

VECTOR_TYPE_2(kb_float2, kb_float)
VECTOR_TYPE_3(kb_float3, kb_float)
VECTOR_TYPE_4(kb_float4, kb_float)
VECTOR_TYPE_4(kb_quat,   kb_float)

typedef enum kb_axis {
  kb_axis_x, kb_axis_y, kb_axis_z
} kb_axis;

typedef struct kb_float3x3 {
  union {
    kb_float m[9];
    kb_float mm[3][3];
    kb_float3 cols[3];
    struct {
      kb_float m00, m01, m02;
      kb_float m10, m11, m12;
      kb_float m20, m21, m22;
    };
  };
} kb_float3x3;

typedef struct kb_float4x4 {
  union {
    kb_float m[16];
    kb_float mm[4][4];
    kb_float4 cols[4];
    struct {
      kb_float m00, m01, m02, m03;
      kb_float m10, m11, m12, m13;
      kb_float m20, m21, m22, m23;
      kb_float m30, m31, m32, m33;
    };
  };
} kb_float4x4;

typedef struct kb_aabb {
  kb_float3 min;
  kb_float3 max;
} kb_aabb;

typedef struct kb_aabb_int {
  kb_int3 min;
  kb_int3 max;
} kb_aabb_int;

typedef struct kb_plane {
  kb_float3 normal;
  kb_float dist;
} kb_plane;

typedef struct kb_rect_float {
  kb_float2 from;
  kb_float2 to;
} kb_rect_float;

typedef struct kb_rect_int {
  kb_int2 from;
  kb_int2 size;
} kb_rect_int;

typedef struct kb_sphere {
  kb_float3 center;
  kb_float radius;
} kb_sphere;

typedef struct kb_capsule {
  kb_float3 pos;
  kb_float3 end;
  kb_float radius;
} kb_capsule;

typedef struct kb_cone {
  kb_float3 pos;
  kb_float3 end;
  kb_float radius;
} kb_cone;

typedef struct kb_cylinder {
  kb_float3 pos;
  kb_float3 end;
  kb_float radius;
} kb_cylinder;

typedef struct kb_disk {
  kb_float3 center;
  kb_float3 normal;
  kb_float radius;
} kb_disk;

typedef struct kb_triangle {
  kb_float3 v0;
  kb_float3 v1;
  kb_float3 v2;
} kb_triangle;

typedef struct kb_ray {
  kb_float3 pos;
  kb_float3 dir;
} kb_ray;

typedef struct kb_hit {
  kb_float3  pos;
  kb_plane   plane;
} kb_hit;

typedef struct kb_xform {
  kb_float3      position;
  kb_float3      scale;
  kb_quat        rotation;
} kb_xform;

static const kb_float3 kb_dir_right     = { 1.0f, 0.0f, 0.0f };
static const kb_float3 kb_dir_up        = { 0.0f, 1.0f, 0.0f };
static const kb_float3 kb_dir_forward   = { 0.0f, 0.0f, 1.0f };

static const kb_float3 kb_float2_max    = { FLOATMAX, FLOATMAX };
static const kb_float3 kb_float2_min    = { FLOATMIN, FLOATMIN };
static const kb_float3 kb_float3_max    = { FLOATMAX, FLOATMAX, FLOATMAX };
static const kb_float3 kb_float3_min    = { FLOATMIN, FLOATMIN, FLOATMIN };

static const kb_float4x4 kb_float4x4_ident = { .m = {
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f,
}};

static const kb_float4x4 kb_float4x4_zero = { .m = {
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
}};

KB_API_INLINE kb_int    kb_int_neg       (kb_int a)                                            { return -a; }
KB_API_INLINE kb_int    kb_int_add       (kb_int a, kb_int b)                                  { return a + b; }
KB_API_INLINE kb_int    kb_int_sub       (kb_int a, kb_int b)                                  { return a - b; }
KB_API_INLINE kb_int    kb_int_mul       (kb_int a, kb_int b)                                  { return a * b; }
KB_API_INLINE kb_int    kb_int_div       (kb_int a, kb_int b)                                  { return a / b; }
KB_API_INLINE bool      kb_int_equal     (kb_int a, kb_int b)                                  { return a == b; }
KB_API_INLINE kb_int    kb_int_abs       (kb_int a)                                            { return a < 0 ? -a : a; }
KB_API_INLINE kb_int    kb_int_sign      (kb_int a)                                            { return (a > 0 ? 1 : 0) - (a < 0 ? 1 : 0); }
KB_API_INLINE kb_int    kb_int_max       (kb_int a, kb_int b)                                  { return a > b ? a : b; }
KB_API_INLINE kb_int    kb_int_min       (kb_int a, kb_int b)                                  { return b > a ? a : b; }
KB_API_INLINE kb_int    kb_int_log2      (kb_int a)                                            { return log2(a); }
KB_API_INLINE kb_int    kb_int_clamp     (kb_int v, kb_int min, kb_int max)                    { return kb_int_max(kb_int_min(v, max), min); }
KB_API_INLINE kb_int    kb_int_square    (kb_int a)                                            { return a * a; }

KB_API_INLINE kb_float    nms_scalar      (kb_float a, kb_float b, kb_float c)                  { return c - a * b; }
KB_API_INLINE kb_float    mad_scalar      (kb_float a, kb_float b, kb_float c)                  { return a * b + c; }
KB_API_INLINE kb_float    neg_scalar      (kb_float a)                                          { return -a; }
KB_API_INLINE kb_float    add_scalar      (kb_float a, kb_float b)                              { return a + b; }
KB_API_INLINE kb_float    sub_scalar      (kb_float a, kb_float b)                              { return a - b; }
KB_API_INLINE kb_float    mul_scalar      (kb_float a, kb_float b)                              { return a * b; }
KB_API_INLINE kb_float    div_scalar      (kb_float a, kb_float b)                              { return a / b; }
KB_API_INLINE kb_float    dot_scalar      (kb_float a, kb_float b)                              { return a * b; }
KB_API_INLINE kb_float    sin_scalar      (kb_float f)                                          { return sin(f); }
KB_API_INLINE kb_float    cos_scalar      (kb_float f)                                          { return cos(f); }
KB_API_INLINE kb_float    tan_scalar      (kb_float f)                                          { return tan(f); }
KB_API_INLINE kb_float    sinh_scalar     (kb_float f)                                          { return sinh(f); }
KB_API_INLINE kb_float    asin_scalar     (kb_float f)                                          { return asin(f); }
KB_API_INLINE kb_float    acos_scalar     (kb_float f)                                          { return acos(f); }
KB_API_INLINE kb_float    atan_scalar     (kb_float f)                                          { return atan(f); }
KB_API_INLINE kb_float    atan2_scalar    (kb_float a, kb_float b)                              { return atan2(a, b); }
KB_API_INLINE kb_float    cosh_scalar     (kb_float f)                                          { return cosh(f); }

KB_API_INLINE kb_float    select_scalar     (kb_float a, kb_float b, bool s)                    { return s ? b : a; }
KB_API_INLINE kb_float    step_scalar       (kb_float a, kb_float b)                            { return select_scalar(0.0f, 1.0f, a >= b); }
KB_API_INLINE kb_float    sqrt_scalar       (kb_float f)                                        { return sqrt(f); }
KB_API_INLINE kb_float    floor_scalar      (kb_float f)                                        { return floor(f); }
KB_API_INLINE kb_float    frac_scalar       (kb_float f)                                        { return f - floor_scalar(f); }
KB_API_INLINE kb_float    ceil_scalar       (kb_float f)                                        { return ceil(f); }
KB_API_INLINE kb_float    round_scalar      (kb_float f)                                        { return round(f); }
KB_API_INLINE kb_float    trunc_scalar      (kb_float f)                                        { return (kb_float) (kb_int) f; }
KB_API_INLINE kb_float    lerp_scalar       (kb_float a, kb_float b, kb_float c)                { return a + c * (b - a); }
KB_API_INLINE kb_float    lerp_inv_scalar   (kb_float a, kb_float b, kb_float c)                { return (c - a) / (b - a); }
KB_API_INLINE kb_float    sign_scalar       (kb_float a)                                        { return (a > 0.0f ? 1.0f : 0.0f) - (a < 0.0f ? 1.0f : 0.0f); }
KB_API_INLINE kb_float    abs_scalar        (kb_float a)                                        { return a < 0.0f ? -a : a; }
KB_API_INLINE kb_float    max_scalar        (kb_float a, kb_float b)                            { return a > b ? a : b; }
KB_API_INLINE kb_float    min_scalar        (kb_float a, kb_float b)                            { return b > a ? a : b; }
KB_API_INLINE kb_float    dist_scalar       (kb_float a, kb_float b)                            { return abs_scalar(a - b); }
KB_API_INLINE kb_float    pow_scalar        (kb_float a, kb_float b)                            { return pow(a, b); }
KB_API_INLINE kb_float    exp_scalar        (kb_float a)                                        { return exp(a); }
KB_API_INLINE kb_float    log_scalar        (kb_float a)                                        { return log(a); }
KB_API_INLINE kb_float    log2_scalar       (kb_float a)                                        { return log2(a); }
KB_API_INLINE kb_float    square_scalar     (kb_float a)                                        { return a * a; }
KB_API_INLINE kb_float    recip_scalar      (kb_float a)                                        { return 1.0f / a; }
KB_API_INLINE kb_float    fmod_scalar       (kb_float a, kb_float b)                            { return fmod(a, b); }
KB_API_INLINE kb_float    mod_scalar        (kb_float a, kb_float b)                            { return a - b * floor_scalar(a / b); }
KB_API_INLINE kb_float    bias_scalar       (kb_float t, kb_float bias)                         { return t / ( ( (1.0f/bias - 2.0f)*(1.0f - t) ) + 1.0f); }
KB_API_INLINE kb_float    gain_scalar       (kb_float t, kb_float gain)                         { return t < 0.5f ? bias_scalar(t * 2.0f, gain) * 0.5f : bias_scalar(t * 2.0f - 1.0f, 1.0f - gain) * 0.5f + 0.5f; }
KB_API_INLINE kb_float    smoothstep_scalar (kb_float a)                                        { return square_scalar(a) * (3.0f - 2.0f * a); }
KB_API_INLINE kb_float    clamp_scalar      (kb_float v, kb_float min, kb_float max)            { return max_scalar(min_scalar(v, max), min); }
KB_API_INLINE kb_float    remap_scalar      (kb_float x, kb_float a, kb_float b, kb_float c, kb_float d) { return lerp_scalar(c, d, lerp_inv_scalar(a, b, x)); }
KB_API_INLINE kb_float    saturate_scalar   (kb_float a)                                        { return clamp_scalar(a, 0.0f, 1.0f); }
KB_API_INLINE kb_float    wrap_scalar       (kb_float v, kb_float wrap)                         { const kb_float tmp = mod_scalar(v, wrap); return tmp < 0.0f ? wrap + tmp : tmp; }
KB_API_INLINE bool        equal_scalar      (kb_float a, kb_float b)                            { return abs_scalar(a - b) < FLT_EPSILON * max_scalar(max_scalar(1.0f, abs_scalar(a)), abs_scalar(b)); }

KB_API_INLINE kb_int2      add_int2          (const kb_int2 a, const kb_int2 b)                 { return (kb_int2) ELEMWISE_OP2_T2(kb_int_add); }
KB_API_INLINE kb_int2      sub_int2          (const kb_int2 a, const kb_int2 b)                 { return (kb_int2) ELEMWISE_OP2_T2(kb_int_sub); }
KB_API_INLINE kb_int2      mul_int2          (const kb_int2 a, const kb_int2 b)                 { return (kb_int2) ELEMWISE_OP2_T2(kb_int_mul); }
KB_API_INLINE kb_int2      div_int2          (const kb_int2 a, const kb_int2 b)                 { return (kb_int2) ELEMWISE_OP2_T2(kb_int_div); }
KB_API_INLINE kb_int2      min_int2          (const kb_int2 a, const kb_int2 b)                 { return (kb_int2) ELEMWISE_OP2_T2(kb_int_min); }
KB_API_INLINE kb_int2      max_int2          (const kb_int2 a, const kb_int2 b)                 { return (kb_int2) ELEMWISE_OP2_T2(kb_int_max); }
KB_API_INLINE kb_int       dot_int2          (const kb_int2 a, const kb_int2 b)                 { return ELEMWISE_COMB_OP2_T2(kb_int_mul, +); }
KB_API_INLINE bool         equal_int2        (const kb_int2 a, const kb_int2 b)                 { return ELEMWISE_COMB_OP2_T2(kb_int_equal, &&); }
KB_API_INLINE kb_int2      scale_int2        (const kb_int2 a, const kb_int f)                  { return (kb_int2) ELEMWISE_OP1_F_T2(kb_int_mul, f); }
KB_API_INLINE kb_int2      neg_int2          (const kb_int2 a)                                  { return (kb_int2) ELEMWISE_OP1_T2(kb_int_neg); }
KB_API_INLINE kb_int2      abs_int2          (const kb_int2 a)                                  { return (kb_int2) ELEMWISE_OP1_T2(kb_int_abs); }
KB_API_INLINE float        len_int2          (const kb_int2 a)                                  { return sqrt_scalar(dot_int2(a, a)); }
KB_API_INLINE kb_int2      sign_int2         (const kb_int2 a)                                  { return (kb_int2) ELEMWISE_OP1_T2(kb_int_sign); }
KB_API_INLINE kb_int2      square_int2       (const kb_int2 a)                                  { return (kb_int2) ELEMWISE_OP1_T2(kb_int_square); }

KB_API_INLINE kb_int3      add_int3          (const kb_int3 a, const kb_int3 b)                 { return (kb_int3) ELEMWISE_OP2_T3(kb_int_add); }
KB_API_INLINE kb_int3      sub_int3          (const kb_int3 a, const kb_int3 b)                 { return (kb_int3) ELEMWISE_OP2_T3(kb_int_sub); }
KB_API_INLINE kb_int3      mul_int3          (const kb_int3 a, const kb_int3 b)                 { return (kb_int3) ELEMWISE_OP2_T3(kb_int_mul); }
KB_API_INLINE kb_int3      div_int3          (const kb_int3 a, const kb_int3 b)                 { return (kb_int3) ELEMWISE_OP2_T3(kb_int_div); }
KB_API_INLINE kb_int3      min_int3          (const kb_int3 a, const kb_int3 b)                 { return (kb_int3) ELEMWISE_OP2_T3(kb_int_min); }
KB_API_INLINE kb_int3      max_int3          (const kb_int3 a, const kb_int3 b)                 { return (kb_int3) ELEMWISE_OP2_T3(kb_int_max); }
KB_API_INLINE kb_int       dot_int3          (const kb_int3 a, const kb_int3 b)                 { return ELEMWISE_COMB_OP2_T3(kb_int_mul, +); }
KB_API_INLINE bool         equal_int3        (const kb_int3 a, const kb_int3 b)                 { return ELEMWISE_COMB_OP2_T3(kb_int_equal, &&); }
KB_API_INLINE kb_int3      scale_int3        (const kb_int3 a, const kb_int f)                  { return (kb_int3) ELEMWISE_OP1_F_T3(kb_int_mul, f); }
KB_API_INLINE kb_int3      neg_int3          (const kb_int3 a)                                  { return (kb_int3) ELEMWISE_OP1_T3(kb_int_neg); }
KB_API_INLINE kb_int3      abs_int3          (const kb_int3 a)                                  { return (kb_int3) ELEMWISE_OP1_T3(kb_int_abs); }
KB_API_INLINE kb_float     len_int3          (const kb_int3 a)                                  { return sqrt_scalar(dot_int3(a, a)); }
KB_API_INLINE kb_int3      sign_int3         (const kb_int3 a)                                  { return (kb_int3) ELEMWISE_OP1_T3(kb_int_sign); }
KB_API_INLINE kb_int3      square_int3       (const kb_int3 a)                                  { return (kb_int3) ELEMWISE_OP1_T3(kb_int_square); }

KB_API_INLINE kb_int4      add_int4          (const kb_int4 a, const kb_int4 b)                 { return (kb_int4) ELEMWISE_OP2_T4(kb_int_add); }
KB_API_INLINE kb_int4      sub_int4          (const kb_int4 a, const kb_int4 b)                 { return (kb_int4) ELEMWISE_OP2_T4(kb_int_sub); }
KB_API_INLINE kb_int4      mul_int4          (const kb_int4 a, const kb_int4 b)                 { return (kb_int4) ELEMWISE_OP2_T4(kb_int_mul); }
KB_API_INLINE kb_int4      div_int4          (const kb_int4 a, const kb_int4 b)                 { return (kb_int4) ELEMWISE_OP2_T4(kb_int_div); }
KB_API_INLINE kb_int4      min_int4          (const kb_int4 a, const kb_int4 b)                 { return (kb_int4) ELEMWISE_OP2_T4(kb_int_min); }
KB_API_INLINE kb_int4      max_int4          (const kb_int4 a, const kb_int4 b)                 { return (kb_int4) ELEMWISE_OP2_T4(kb_int_max); }
KB_API_INLINE kb_int       dot_int4          (const kb_int4 a, const kb_int4 b)                 { return ELEMWISE_COMB_OP2_T4(kb_int_mul, +); }
KB_API_INLINE bool         equal_int4        (const kb_int4 a, const kb_int4 b)                 { return ELEMWISE_COMB_OP2_T4(kb_int_equal, &&); }
KB_API_INLINE kb_int4      scale_int4        (const kb_int4 a, const kb_int f)                  { return (kb_int4) ELEMWISE_OP1_F_T4(kb_int_mul, f); }
KB_API_INLINE kb_int4      neg_int4          (const kb_int4 a)                                  { return (kb_int4) ELEMWISE_OP1_T4(kb_int_neg); }
KB_API_INLINE kb_int4      abs_int4          (const kb_int4 a)                                  { return (kb_int4) ELEMWISE_OP1_T4(kb_int_abs); }
KB_API_INLINE kb_float     len_int4          (const kb_int4 a)                                  { return sqrt_scalar(dot_int4(a, a)); }
KB_API_INLINE kb_int4      sign_int4         (const kb_int4 a)                                  { return (kb_int4) ELEMWISE_OP1_T4(kb_int_sign); }
KB_API_INLINE kb_int4      square_int4       (const kb_int4 a)                                  { return (kb_int4) ELEMWISE_OP1_T4(kb_int_square); }

KB_API_INLINE kb_float2    abs_float2        (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(abs_scalar); }
KB_API_INLINE kb_float2    neg_float2        (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(neg_scalar); }
KB_API_INLINE kb_float2    floor_float2      (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(floor_scalar); }
KB_API_INLINE kb_float2    add_float2        (const kb_float2 a, const kb_float2 b)             { return (kb_float2) ELEMWISE_OP2_T2(add_scalar); }
KB_API_INLINE kb_float2    sub_float2        (const kb_float2 a, const kb_float2 b)             { return (kb_float2) ELEMWISE_OP2_T2(sub_scalar); }
KB_API_INLINE kb_float2    mul_float2        (const kb_float2 a, const kb_float2 b)             { return (kb_float2) ELEMWISE_OP2_T2(mul_scalar); }
KB_API_INLINE kb_float2    min_float2        (const kb_float2 a, const kb_float2 b)             { return (kb_float2) ELEMWISE_OP2_T2(min_scalar); }
KB_API_INLINE kb_float2    max_float2        (const kb_float2 a, const kb_float2 b)             { return (kb_float2) ELEMWISE_OP2_T2(max_scalar); }
KB_API_INLINE kb_float2    scale_float2      (const kb_float2 a, const kb_float f)              { return (kb_float2) ELEMWISE_OP1_F_T2(mul_scalar, f); }
KB_API_INLINE kb_float2    recip_float2      (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(recip_scalar); }
KB_API_INLINE kb_float2    lerp_float2       (const kb_float2 a, const kb_float2 b, const kb_float v) { return (kb_float2) ELEMWISE_OP2_F_T2(lerp_scalar, v); }
KB_API_INLINE kb_float2    saturate_float2   (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(saturate_scalar); }
KB_API_INLINE kb_float2    smoothstep_float2 (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(smoothstep_scalar); }
KB_API_INLINE kb_float2    square_float2     (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(square_scalar); }
KB_API_INLINE kb_float2    exp_float2        (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(exp_scalar); }
KB_API_INLINE kb_float2    log_float2        (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(log_scalar); }
KB_API_INLINE kb_float2    sign_float2       (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(sign_scalar); }
KB_API_INLINE kb_float2    sqrt_float2       (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(sqrt_scalar); }
KB_API_INLINE kb_float2    frac_float2       (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(frac_scalar); }
KB_API_INLINE kb_float2    ceil_float2       (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(ceil_scalar); }
KB_API_INLINE kb_float2    round_float2      (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(round_scalar); }
KB_API_INLINE kb_float2    trunc_float2      (const kb_float2 a)                                { return (kb_float2) ELEMWISE_OP1_T2(trunc_scalar); }
KB_API_INLINE kb_float     dot_float2        (const kb_float2 a, const kb_float2 b)             { return ELEMWISE_COMB_OP2_T2(mul_scalar, +); }
KB_API_INLINE bool         equal_float2      (const kb_float2 a, const kb_float2 b)             { return ELEMWISE_COMB_OP2_T2(equal_scalar, &&); }
KB_API_INLINE kb_float     len_float2        (const kb_float2 a)                                { return sqrt_scalar(dot_float2(a, a)); }
KB_API_INLINE kb_float2    norm_float2       (const kb_float2 a)                                { return scale_float2(a, recip_scalar(len_float2(a))); }
KB_API_INLINE kb_float2    div_float2        (const kb_float2 a, const kb_float2 b)             { return mul_float2(a, recip_float2(b)); }
KB_API_INLINE kb_float     dist_float2       (const kb_float2 a, const kb_float2 b)             { return len_float2(sub_float2(b, a)); }

KB_API_INLINE kb_float3    abs_float3        (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(abs_scalar); }
KB_API_INLINE kb_float3    neg_float3        (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(neg_scalar); }
KB_API_INLINE kb_float3    floor_float3      (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(floor_scalar); }
KB_API_INLINE kb_float3    add_float3        (const kb_float3 a, const kb_float3 b)             { return (kb_float3) ELEMWISE_OP2_T3(add_scalar); }
KB_API_INLINE kb_float3    sub_float3        (const kb_float3 a, const kb_float3 b)             { return (kb_float3) ELEMWISE_OP2_T3(sub_scalar); }
KB_API_INLINE kb_float3    mul_float3        (const kb_float3 a, const kb_float3 b)             { return (kb_float3) ELEMWISE_OP2_T3(mul_scalar); }
KB_API_INLINE kb_float3    min_float3        (const kb_float3 a, const kb_float3 b)             { return (kb_float3) ELEMWISE_OP2_T3(min_scalar); }
KB_API_INLINE kb_float3    max_float3        (const kb_float3 a, const kb_float3 b)             { return (kb_float3) ELEMWISE_OP2_T3(max_scalar); }
KB_API_INLINE kb_float3    scale_float3      (const kb_float3 a, const kb_float f)              { return (kb_float3) ELEMWISE_OP1_F_T3(mul_scalar, f); }
KB_API_INLINE kb_float3    recip_float3      (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(recip_scalar); }
KB_API_INLINE kb_float3    lerp_float3       (const kb_float3 a, const kb_float3 b, const kb_float v)     { return (kb_float3) ELEMWISE_OP2_F_T3(lerp_scalar, v); }
KB_API_INLINE kb_float3    saturate_float3   (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(saturate_scalar); }
KB_API_INLINE kb_float3    smoothstep_float3 (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(smoothstep_scalar); }
KB_API_INLINE kb_float3    square_float3     (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(square_scalar); }
KB_API_INLINE kb_float3    exp_float3        (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(exp_scalar); }
KB_API_INLINE kb_float3    log_float3        (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(log_scalar); }
KB_API_INLINE kb_float3    sign_float3       (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(sign_scalar); }
KB_API_INLINE kb_float3    sqrt_float3       (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(sqrt_scalar); }
KB_API_INLINE kb_float3    frac_float3       (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(frac_scalar); }
KB_API_INLINE kb_float3    ceil_float3       (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(ceil_scalar); }
KB_API_INLINE kb_float3    round_float3      (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(round_scalar); }
KB_API_INLINE kb_float3    trunc_float3      (const kb_float3 a)                                { return (kb_float3) ELEMWISE_OP1_T3(trunc_scalar); }
KB_API_INLINE kb_float     dot_float3        (const kb_float3 a, const kb_float3 b)             { return ELEMWISE_COMB_OP2_T3(mul_scalar, +); }
KB_API_INLINE bool         equal_float3      (const kb_float3 a, const kb_float3 b)             { return ELEMWISE_COMB_OP2_T3(equal_scalar, &&); }
KB_API_INLINE kb_float     len_float3        (const kb_float3 a)                                { return sqrt_scalar(dot_float3(a, a)); }
KB_API_INLINE kb_float3    norm_float3       (const kb_float3 a)                                { return scale_float3(a, recip_scalar(len_float3(a))); }
KB_API_INLINE kb_float3    div_float3        (const kb_float3 a, const kb_float3 b)             { return mul_float3(a, recip_float3(b)); }
KB_API_INLINE kb_float     dist_float3       (const kb_float3 a, const kb_float3 b)             { return len_float3(sub_float3(b, a)); }

KB_API_INLINE kb_float4    abs_float4        (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(abs_scalar); }
KB_API_INLINE kb_float4    neg_float4        (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(neg_scalar); }
KB_API_INLINE kb_float4    floor_float4      (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(floor_scalar); }
KB_API_INLINE kb_float4    add_float4        (const kb_float4 a, const kb_float4 b)             { return (kb_float4) ELEMWISE_OP2_T4(add_scalar); }
KB_API_INLINE kb_float4    sub_float4        (const kb_float4 a, const kb_float4 b)             { return (kb_float4) ELEMWISE_OP2_T4(sub_scalar); }
KB_API_INLINE kb_float4    mul_float4        (const kb_float4 a, const kb_float4 b)             { return (kb_float4) ELEMWISE_OP2_T4(mul_scalar); }
KB_API_INLINE kb_float4    min_float4        (const kb_float4 a, const kb_float4 b)             { return (kb_float4) ELEMWISE_OP2_T4(min_scalar); }
KB_API_INLINE kb_float4    max_float4        (const kb_float4 a, const kb_float4 b)             { return (kb_float4) ELEMWISE_OP2_T4(max_scalar); }
KB_API_INLINE kb_float4    scale_float4      (const kb_float4 a, const kb_float f)              { return (kb_float4) ELEMWISE_OP1_F_T4(mul_scalar, f); }
KB_API_INLINE kb_float4    recip_float4      (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(recip_scalar); }
KB_API_INLINE kb_float4    lerp_float4       (const kb_float4 a, const kb_float4 b, const kb_float v)     { return (kb_float4) ELEMWISE_OP2_F_T4(lerp_scalar, v); }
KB_API_INLINE kb_float4    saturate_float4   (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(saturate_scalar); }
KB_API_INLINE kb_float4    smoothstep_float4 (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(smoothstep_scalar); }
KB_API_INLINE kb_float4    square_float4     (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(square_scalar); }
KB_API_INLINE kb_float4    exp_float4        (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(exp_scalar); }
KB_API_INLINE kb_float4    log_float4        (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(log_scalar); }
KB_API_INLINE kb_float4    sign_float4       (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(sign_scalar); }
KB_API_INLINE kb_float4    sqrt_float4       (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(sqrt_scalar); }
KB_API_INLINE kb_float4    frac_float4       (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(frac_scalar); }
KB_API_INLINE kb_float4    ceil_float4       (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(ceil_scalar); }
KB_API_INLINE kb_float4    round_float4      (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(round_scalar); }
KB_API_INLINE kb_float4    trunc_float4      (const kb_float4 a)                                { return (kb_float4) ELEMWISE_OP1_T4(trunc_scalar); }
KB_API_INLINE kb_float     dot_float4        (const kb_float4 a, const kb_float4 b)             { return ELEMWISE_COMB_OP2_T4(mul_scalar, +); }
KB_API_INLINE bool         equal_float4      (const kb_float4 a, const kb_float4 b)             { return ELEMWISE_COMB_OP2_T4(equal_scalar, &&); }
KB_API_INLINE kb_float     len_float4        (const kb_float4 a)                                { return sqrt_scalar(dot_float4(a, a)); }
KB_API_INLINE kb_float4    norm_float4       (const kb_float4 a)                                { return scale_float4(a, recip_scalar(len_float4(a))); }
KB_API_INLINE kb_float4    div_float4        (const kb_float4 a, const kb_float4 b)             { return mul_float4(a, recip_float4(b)); }
KB_API_INLINE kb_float     dist_float4       (const kb_float4 a, const kb_float4 b)             { return len_float4(sub_float4(b, a)); }

KB_API        kb_quat       inv_quat         (const kb_quat a);
KB_API        kb_quat       norm_quat        (const kb_quat a);
KB_API        kb_quat       mul_quat         (const kb_quat a, const kb_quat b);
KB_API        kb_quat       euler_quat       (const kb_float3 euler);
KB_API        kb_quat       axis_angle_quat  (const kb_float3 axis, kb_float angle);
KB_API        kb_quat       norm_quat        (const kb_quat a);
KB_API_INLINE kb_quat       neg_quat         (const kb_quat a)                                  { return (kb_quat) ELEMWISE_OP1_T4(neg_scalar); }
KB_API_INLINE kb_quat       add_quat         (const kb_quat a, const kb_quat b)                 { return (kb_quat) ELEMWISE_OP2_T4(add_scalar); }
KB_API_INLINE kb_quat       sub_quat         (const kb_quat a, const kb_quat b)                 { return (kb_quat) ELEMWISE_OP2_T4(sub_scalar); }
KB_API_INLINE kb_quat       scale_quat       (const kb_quat a, const kb_float f)                { return (kb_quat) ELEMWISE_OP1_F_T4(mul_scalar, f); }
KB_API_INLINE kb_quat       lerp_quat        (const kb_quat a, const kb_quat b, const kb_float v)     { return norm_quat((kb_quat) ELEMWISE_OP2_F_T4(lerp_scalar, v)) ; }
KB_API_INLINE kb_float      dot_quat         (const kb_quat a, const kb_quat b)                 { return ELEMWISE_COMB_OP2_T4(mul_scalar, +); }
KB_API_INLINE bool          equal_quat       (const kb_quat a, const kb_quat b)                 { return ELEMWISE_COMB_OP2_T4(equal_scalar, &&); }
KB_API_INLINE kb_float      len_quat         (const kb_quat a)                                  { return sqrt_scalar(dot_quat(a, a)); }
KB_API_INLINE kb_quat       conj_quat        (const kb_quat a)                                  { return (kb_quat) { -a.x, -a.y, -a.z, +a.w }; }
KB_API_INLINE kb_quat       recip_quat       (const kb_quat a)                                  { return scale_quat(conj_quat(a), 1.0f / dot_quat(a, a)); }
KB_API_INLINE kb_quat       div_quat         (const kb_quat a, const kb_quat b)                 { return mul_quat(a, inv_quat(b)); }

KB_API_INLINE kb_float      uint16_to_float  (int16_t value)                                    { return remap_scalar((kb_float) value, (kb_float) INT16_MIN, (float) INT16_MAX, -1.0f, 1.0f); }
KB_API_INLINE bool          kb_activated     (uint8_t* curr, uint8_t* prev, size_t i)           { return !prev[i] && curr[i]; }
KB_API_INLINE bool          kb_deactivated   (uint8_t* curr, uint8_t* prev, size_t i)           { return prev[i] && !curr[i]; }
KB_API_INLINE kb_float      deg_to_rad       (kb_float deg)                                     { return deg * PI / 180.0f; }
KB_API_INLINE kb_float      rad_to_deg       (kb_float rad)                                     { return rad * 180.0f / PI; }

KB_API kb_float3            act_quat                  (const kb_quat a, const kb_float3 b);
KB_API kb_quat              slerp_quat                (const kb_quat a, const kb_quat b, kb_float t);
KB_API bool                 aabb_contains             (kb_aabb_int aabb, kb_int3 pos);
KB_API kb_float3            cross_float3              (const kb_float3 a, const kb_float3 b);
KB_API kb_float4            act_float4x4              (const kb_float4x4 a, const kb_float4 b);
KB_API kb_float4x4          xform                     (const kb_float3 pos, const kb_float3 scale, const kb_quat rot);
KB_API kb_float4x4          mul_float4x4              (const kb_float4x4 a, const kb_float4x4 b);
KB_API kb_float4x4          inv_float4x4              (const kb_float4x4 a);
KB_API kb_float4x4          transpose_float4x4        (const kb_float4x4 a);
KB_API void                 translate                 (kb_float4x4* mtx, const kb_float3 pos);
KB_API void                 scale                     (kb_float4x4* mtx, const kb_float3 scale);
KB_API void                 rotate                    (kb_float4x4* mtx, const kb_quat rotation);
KB_API kb_float4x4          transform_mtx             (const kb_float3 v);
KB_API kb_float4x4          scaling_mtx               (const kb_float3 v);
KB_API kb_float4x4          rotation_mtx              (const kb_quat v);
KB_API kb_float4x4          look_at                   (const kb_float3 from, const kb_float3 to, const kb_float3 up);
KB_API kb_float4x4          perspective               (kb_float fov, kb_float aspect, kb_float near, kb_float far);
KB_API kb_float4x4          orthographic              (kb_float left, kb_float right, kb_float top, kb_float bottom, kb_float near, kb_float far);
KB_API kb_float3            get_point                 (kb_axis axis, kb_float x, kb_float y);
KB_API void                 tangent_frame             (const kb_float3 normal, kb_float3* tangent, kb_float3* bitangent);
KB_API void                 tangent_frame_with_spin   (const kb_float3 normal, float angle, kb_float3* tangent, kb_float3* bitangent);
KB_API uint32_t             float_rgba_to_uint        (const kb_float4 color);
KB_API float                ray_plane_intersection    (const kb_ray ray, const kb_plane plane);
KB_API kb_float3            unproject                 (kb_float4x4 unproj, kb_float3 point);
KB_API kb_ray               unproject_view            (kb_float4x4 unproj, kb_float2 p);
KB_API uint64_t             align_up                  (uint64_t a, uint64_t align);
KB_API kb_float2            circle_point              (kb_float angle);
KB_API kb_float2            squircle_point            (kb_float angle);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#define TYPE_OPERATORS(T, TT) \
inline T    operator- (const T& a)                    { return kb::neg(a); } \
inline T    operator* (const T& a,  const T& b)       { return kb::mul(a, b); } \
inline T    operator* (const T& a,  const TT& b)      { return kb::scale(a, b); } \
inline T    operator+ (const T& a,  const T& b)       { return kb::add(a, b); } \
inline T    operator- (const T& a,  const T& b)       { return kb::sub(a, b); } \
inline bool operator==(const T& a,  const T& b)       { return kb::equal(a, b); } \
inline T&   operator+=(T& a,        const T& b)       { a = kb::add(a, b); return a; } \
inline T&   operator-=(T& a,        const T& b)       { a = kb::sub(a, b); return a; } \
inline T&   operator*=(T& a,        const T& b)       { a = kb::mul(a, b); return a; } \
inline T&   operator/=(T& a,        const T& b)       { a = kb::div(a, b); return a; } \
inline T&   operator/=(T& a,        const TT& b)      { a = kb::scale(a, b); return a; } \

#define FUNC1_DEF_N(F, T1, func)          inline auto F(const T1& a) { return func(a); }
#define FUNC2_DEF_N(F, T1, T2, func)      inline auto F(const T1& a, const T2& b) { return func(a, b); }
#define FUNC3_DEF_N(F, T1, T2, T3, func)  inline auto F(const T1& a, const T2& b, const T3& c) { return func(a, b, c); }

namespace kb {
  using integer = kb_int;
  using scalar  = kb_float;
  using int2    = kb_int2;
  using int3    = kb_int3;
  using int4    = kb_int4;
  using float2  = kb_float2;
  using float3  = kb_float3;
  using float4  = kb_float4;
  using quat    = kb_quat;

FUNC1_DEF_N(log2,   integer,    kb_int_log2);
FUNC1_DEF_N(log2,   scalar,     log2_scalar);

FUNC1_DEF_N(abs,    scalar,     abs_scalar);
FUNC1_DEF_N(abs,    float2,     abs_float2);
FUNC1_DEF_N(abs,    float3,     abs_float3);
FUNC1_DEF_N(abs,    float4,     abs_float4);
FUNC1_DEF_N(abs,    integer,    kb_int_abs);
FUNC1_DEF_N(abs,    int2,       abs_int2);
FUNC1_DEF_N(abs,    int3,       abs_int3);
FUNC1_DEF_N(abs,    int4,       abs_int4);

FUNC1_DEF_N(sign,   scalar,     sign_scalar);
FUNC1_DEF_N(sign,   float2,     sign_float2);
FUNC1_DEF_N(sign,   float3,     sign_float3);
FUNC1_DEF_N(sign,   float4,     sign_float4);
FUNC1_DEF_N(sign,   integer,    kb_int_sign);
FUNC1_DEF_N(sign,   int2,       sign_int2);
FUNC1_DEF_N(sign,   int3,       sign_int3);
FUNC1_DEF_N(sign,   int4,       sign_int4);

FUNC1_DEF_N(square,  scalar,    square_scalar);
FUNC1_DEF_N(square,  float2,    square_float2);
FUNC1_DEF_N(square,  float3,    square_float3);
FUNC1_DEF_N(square,  float4,    square_float4);
FUNC1_DEF_N(square,  integer,   kb_int_square);
FUNC1_DEF_N(square,  int2,      square_int2);
FUNC1_DEF_N(square,  int3,      square_int3);
FUNC1_DEF_N(square,  int4,      square_int4);

FUNC1_DEF_N(neg,      scalar,   neg_scalar);
FUNC1_DEF_N(neg,      float2,   neg_float2);
FUNC1_DEF_N(neg,      float3,   neg_float3);
FUNC1_DEF_N(neg,      float4,   neg_float4);
FUNC1_DEF_N(neg,      quat,     neg_quat);
FUNC1_DEF_N(neg,      integer,  kb_int_neg);
FUNC1_DEF_N(neg,      int2,     neg_int2);
FUNC1_DEF_N(neg,      int3,     neg_int3);
FUNC1_DEF_N(neg,      int4,     neg_int4);

FUNC2_DEF_N(add,  scalar,   scalar,   add_scalar);
FUNC2_DEF_N(add,  float2,   float2,   add_float2);
FUNC2_DEF_N(add,  float3,   float3,   add_float3);
FUNC2_DEF_N(add,  float4,   float4,   add_float4);
FUNC2_DEF_N(add,  quat,     quat,     add_quat);
FUNC2_DEF_N(add,  integer,  integer,  kb_int_add);
FUNC2_DEF_N(add,  int2,     int2,     add_int2);
FUNC2_DEF_N(add,  int3,     int3,     add_int3);
FUNC2_DEF_N(add,  int4,     int4,     add_int4);

FUNC2_DEF_N(sub,  scalar,   scalar,   sub_scalar);
FUNC2_DEF_N(sub,  float2,   float2,   sub_float2);
FUNC2_DEF_N(sub,  float3,   float3,   sub_float3);
FUNC2_DEF_N(sub,  float4,   float4,   sub_float4);
FUNC2_DEF_N(sub,  quat,     quat,     sub_quat);
FUNC2_DEF_N(sub,  integer,  integer,  kb_int_sub);
FUNC2_DEF_N(sub,  int2,     int2,     sub_int2);
FUNC2_DEF_N(sub,  int3,     int3,     sub_int3);
FUNC2_DEF_N(sub,  int4,     int4,     sub_int4);

FUNC2_DEF_N(mul,  scalar,   scalar,   mul_scalar);
FUNC2_DEF_N(mul,  float2,   float2,   mul_float2);
FUNC2_DEF_N(mul,  float3,   float3,   mul_float3);
FUNC2_DEF_N(mul,  float4,   float4,   mul_float4);
FUNC2_DEF_N(mul,  quat,     quat,     mul_quat);
FUNC2_DEF_N(mul,  integer,  integer,  kb_int_mul);
FUNC2_DEF_N(mul,  int2,     int2,     mul_int2);
FUNC2_DEF_N(mul,  int3,     int3,     mul_int3);
FUNC2_DEF_N(mul,  int4,     int4,     mul_int4);

FUNC2_DEF_N(div,  scalar,   scalar,   div_scalar);
FUNC2_DEF_N(div,  float2,   float2,   div_float2);
FUNC2_DEF_N(div,  float3,   float3,   div_float3);
FUNC2_DEF_N(div,  float4,   float4,   div_float4);
FUNC2_DEF_N(div,  quat,     quat,     div_quat);
FUNC2_DEF_N(div,  integer,  integer,  kb_int_div);
FUNC2_DEF_N(div,  int2,     int2,     div_int2);
FUNC2_DEF_N(div,  int3,     int3,     div_int3);
FUNC2_DEF_N(div,  int4,     int4,     div_int4);

FUNC2_DEF_N(scale,  scalar,   scalar,   mul_scalar);
FUNC2_DEF_N(scale,  float2,   scalar,   scale_float2);
FUNC2_DEF_N(scale,  float3,   scalar,   scale_float3);
FUNC2_DEF_N(scale,  float4,   scalar,   scale_float4);
FUNC2_DEF_N(scale,  quat,     scalar,   scale_quat);
FUNC2_DEF_N(scale,  integer,  integer,  kb_int_mul);
FUNC2_DEF_N(scale,  int2,     integer,  scale_int2);
FUNC2_DEF_N(scale,  int3,     integer,  scale_int3);
FUNC2_DEF_N(scale,  int4,     integer,  scale_int4);

FUNC2_DEF_N(equal,  scalar,   scalar,   equal_scalar);
FUNC2_DEF_N(equal,  float2,   float2,   equal_float2);
FUNC2_DEF_N(equal,  float3,   float3,   equal_float3);
FUNC2_DEF_N(equal,  float4,   float4,   equal_float4);
FUNC2_DEF_N(equal,  quat,     quat,     equal_quat);
FUNC2_DEF_N(equal,  integer,  integer,  kb_int_equal);
FUNC2_DEF_N(equal,  int2,     int2,     equal_int2);
FUNC2_DEF_N(equal,  int3,     int3,     equal_int3);
FUNC2_DEF_N(equal,  int4,     int4,     equal_int4);

FUNC2_DEF_N(min,  scalar,   scalar,   min_scalar);
FUNC2_DEF_N(min,  float2,   float2,   min_float2);
FUNC2_DEF_N(min,  float3,   float3,   min_float3);
FUNC2_DEF_N(min,  float4,   float4,   min_float4);
FUNC2_DEF_N(min,  integer,  integer,  kb_int_min);
FUNC2_DEF_N(min,  int2,     int2,     min_int2);
FUNC2_DEF_N(min,  int3,     int3,     min_int3);
FUNC2_DEF_N(min,  int4,     int4,     min_int4);

FUNC2_DEF_N(max,  scalar,   scalar,   max_scalar);
FUNC2_DEF_N(max,  float2,   float2,   max_float2);
FUNC2_DEF_N(max,  float3,   float3,   max_float3);
FUNC2_DEF_N(max,  float4,   float4,   max_float4);
FUNC2_DEF_N(max,  integer,  integer,  kb_int_max);
FUNC2_DEF_N(max,  int2,     int2,     max_int2);
FUNC2_DEF_N(max,  int3,     int3,     max_int3);
FUNC2_DEF_N(max,  int4,     int4,     max_int4);

FUNC1_DEF_N(exp,    scalar,     exp_scalar);
FUNC1_DEF_N(exp,    float2,     exp_float2);
FUNC1_DEF_N(exp,    float3,     exp_float3);
FUNC1_DEF_N(exp,    float4,     exp_float4);

FUNC1_DEF_N(floor,  scalar,     floor_scalar);
FUNC1_DEF_N(floor,  float2,     floor_float2);
FUNC1_DEF_N(floor,  float3,     floor_float3);
FUNC1_DEF_N(floor,  float4,     floor_float4);

FUNC1_DEF_N(log,    scalar,     log_scalar);
FUNC1_DEF_N(log,    float2,     log_float2);
FUNC1_DEF_N(log,    float3,     log_float3);
FUNC1_DEF_N(log,    float4,     log_float4);

FUNC1_DEF_N(sqrt,   scalar,     sqrt_scalar);
FUNC1_DEF_N(sqrt,   float2,     sqrt_float2);
FUNC1_DEF_N(sqrt,   float3,     sqrt_float3);
FUNC1_DEF_N(sqrt,   float4,     sqrt_float4);

FUNC1_DEF_N(frac,   scalar,     frac_scalar);
FUNC1_DEF_N(frac,   float2,     frac_float2);
FUNC1_DEF_N(frac,   float3,     frac_float3);
FUNC1_DEF_N(frac,   float4,     frac_float4);

FUNC1_DEF_N(ceil,   scalar,     ceil_scalar);
FUNC1_DEF_N(ceil,   float2,     ceil_float2);
FUNC1_DEF_N(ceil,   float3,     ceil_float3);
FUNC1_DEF_N(ceil,   float4,     ceil_float4);

FUNC1_DEF_N(round,  scalar,     round_scalar);
FUNC1_DEF_N(round,  float2,     round_float2);
FUNC1_DEF_N(round,  float3,     round_float3);
FUNC1_DEF_N(round,  float4,     round_float4);

FUNC1_DEF_N(trunc,  scalar,     trunc_scalar);
FUNC1_DEF_N(trunc,  float2,     trunc_float2);
FUNC1_DEF_N(trunc,  float3,     trunc_float3);
FUNC1_DEF_N(trunc,  float4,     trunc_float4);

FUNC1_DEF_N(recip,  scalar,     recip_scalar);
FUNC1_DEF_N(recip,  float2,     recip_float2);
FUNC1_DEF_N(recip,  float3,     recip_float3);
FUNC1_DEF_N(recip,  float4,     recip_float4);

FUNC1_DEF_N(saturate,  scalar,  saturate_scalar);
FUNC1_DEF_N(saturate,  float2,  saturate_float2);
FUNC1_DEF_N(saturate,  float3,  saturate_float3);
FUNC1_DEF_N(saturate,  float4,  saturate_float4);

FUNC1_DEF_N(smoothstep,  scalar,  smoothstep_scalar);
FUNC1_DEF_N(smoothstep,  float2,  smoothstep_float2);
FUNC1_DEF_N(smoothstep,  float3,  smoothstep_float3);
FUNC1_DEF_N(smoothstep,  float4,  smoothstep_float4);

FUNC1_DEF_N(norm, float2,  norm_float2);
FUNC1_DEF_N(norm, float3,  norm_float3);
FUNC1_DEF_N(norm, float4,  norm_float4);

FUNC1_DEF_N(len, float2,  len_float2);
FUNC1_DEF_N(len, float3,  len_float3);
FUNC1_DEF_N(len, float4,  len_float4);

FUNC2_DEF_N(dist,  scalar,   scalar,    dist_scalar);
FUNC2_DEF_N(dist,  float2,   float2,    dist_float2);
FUNC2_DEF_N(dist,  float3,   float3,    dist_float3);
FUNC2_DEF_N(dist,  float4,   float4,    dist_float4);

FUNC3_DEF_N(lerp,  scalar, scalar, scalar, lerp_scalar);
FUNC3_DEF_N(lerp,  float2, float2, scalar, lerp_float2);
FUNC3_DEF_N(lerp,  float3, float3, scalar, lerp_float3);
FUNC3_DEF_N(lerp,  float4, float4, scalar, lerp_float4);

FUNC2_DEF_N(cross, float3,   float3,    cross_float3);

};

TYPE_OPERATORS(kb::float2, kb::scalar);
TYPE_OPERATORS(kb::float3, kb::scalar);
TYPE_OPERATORS(kb::float4, kb::scalar);
TYPE_OPERATORS(kb::quat,   kb::scalar);
TYPE_OPERATORS(kb::int2,   kb::integer);
TYPE_OPERATORS(kb::int3,   kb::integer);
TYPE_OPERATORS(kb::int4,   kb::integer);

FUNC2_DEF_N(operator*, kb::quat, kb::float3, act_quat);

#endif
