// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "core.h"

#include <math.h>
#include <float.h>
#include <limits.h>
#include <float.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _ELEMW_OP1_T2(func) { func(a.x), func(a.y) }
#define _ELEMW_OP1_T3(func) { func(a.x), func(a.y), func(a.z) }
#define _ELEMW_OP1_T4(func) { func(a.x), func(a.y), func(a.z), func(a.w) }

#define _ELEMW_OP1_F_T2(func, f) { func(a.x, f), func(a.y, f) }
#define _ELEMW_OP1_F_T3(func, f) { func(a.x, f), func(a.y, f), func(a.z, f) }
#define _ELEMW_OP1_F_T4(func, f) { func(a.x, f), func(a.y, f), func(a.z, f), func(a.w, f) }

#define _ELEMW_COMB_OP1_T2(func, comb) func(a.x) comb func(a.y)
#define _ELEMW_COMB_OP1_T3(func, comb) func(a.x) comb func(a.y) comb func(a.z)
#define _ELEMW_COMB_OP1_T4(func, comb) func(a.x) comb func(a.y) comb func(a.z) comb func(a.w)

#define _ELEMW_OP2_T2(func) { func(a.x, b.x), func(a.y, b.y) }
#define _ELEMW_OP2_T3(func) { func(a.x, b.x), func(a.y, b.y), func(a.z, b.z) }
#define _ELEMW_OP2_T4(func) { func(a.x, b.x), func(a.y, b.y), func(a.z, b.z), func(a.w, b.w) }

#define _ELEMW_OP2_F_T2(func, f) { func(a.x, b.x, f), func(a.y, b.y, f) }
#define _ELEMW_OP2_F_T3(func, f) { func(a.x, b.x, f), func(a.y, b.y, f), func(a.z, b.z, f) }
#define _ELEMW_OP2_F_T4(func, f) { func(a.x, b.x, f), func(a.y, b.y, f), func(a.z, b.z, f), func(a.w, b.w, f) }

#define _ELEMW_COMB_OP2_T2(func, comb) func(a.x, b.x) comb func(a.y, b.y)
#define _ELEMW_COMB_OP2_T3(func, comb) func(a.x, b.x) comb func(a.y, b.y) comb func(a.z, b.z)
#define _ELEMW_COMB_OP2_T4(func, comb) func(a.x, b.x) comb func(a.y, b.y) comb func(a.z, b.z) comb func(a.w, b.w)

#define KB_MAX(_a, _b) _a > _b ? _a : _b
#define KB_MIN(_a, _b) _a < _b ? _a : _b
#define KB_CLAMP(_t, _min, _max) _t > _max ? _max : (_t < _min ? _min : _t)

#define VECTOR_TYPE_2(name, type) typedef struct name { type x, y; } name;
#define VECTOR_TYPE_3(name, type) typedef struct name { type x, y, z; } name;
#define VECTOR_TYPE_4(name, type) typedef struct name { type x, y, z, w; } name;

#define TYPE_OPERATORS(T, S) \
inline T    operator- (const T& a)                { return kb::neg(a); }              \
inline T    operator* (const T& a,  const T& b)   { return kb::mul(a, b); }           \
inline T    operator* (const T& a,  const S& b)   { return kb::scale(a, b); }         \
inline T    operator+ (const T& a,  const T& b)   { return kb::add(a, b); }           \
inline T    operator- (const T& a,  const T& b)   { return kb::sub(a, b); }           \
inline bool operator==(const T& a,  const T& b)   { return kb::equal(a, b); }         \
inline T&   operator+=(T& a,        const T& b)   { a = kb::add(a, b); return a; }    \
inline T&   operator-=(T& a,        const T& b)   { a = kb::sub(a, b); return a; }    \
inline T&   operator*=(T& a,        const T& b)   { a = kb::mul(a, b); return a; }    \
inline T&   operator*=(T& a,        const S& b)   { a = kb::scale(a, b); return a; }  \
inline T&   operator/=(T& a,        const T& b)   { a = kb::div(a, b); return a; }    \
inline T&   operator/=(T& a,        const S& b)   { a = kb::scale(a, b); return a; }  \

#define FUNC1_DEF_N(F, T1, func)          inline auto F(const T1& a) { return func(a); }
#define FUNC2_DEF_N(F, T1, T2, func)      inline auto F(const T1& a, const T2& b) { return func(a, b); }
#define FUNC3_DEF_N(F, T1, T2, T3, func)  inline auto F(const T1& a, const T2& b, const T3& c) { return func(a, b, c); }

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

static const kb_float3 kb_float2max    = { FLOATMAX, FLOATMAX };
static const kb_float3 kb_float2min    = { FLOATMIN, FLOATMIN };
static const kb_float3 kb_float3max    = { FLOATMAX, FLOATMAX, FLOATMAX };
static const kb_float3 kb_float3min    = { FLOATMIN, FLOATMIN, FLOATMIN };

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

KB_API_INLINE kb_int      kb_int_neg          (kb_int a)                                                  { return -a; }
KB_API_INLINE kb_int      kb_int_add          (kb_int a, kb_int b)                                        { return a + b; }
KB_API_INLINE kb_int      kb_int_sub          (kb_int a, kb_int b)                                        { return a - b; }
KB_API_INLINE kb_int      kb_int_mul          (kb_int a, kb_int b)                                        { return a * b; }
KB_API_INLINE kb_int      kb_int_div          (kb_int a, kb_int b)                                        { return a / b; }
KB_API_INLINE bool        kb_int_equal        (kb_int a, kb_int b)                                        { return a == b; }
KB_API_INLINE kb_int      kb_int_abs          (kb_int a)                                                  { return a < 0 ? -a : a; }
KB_API_INLINE kb_int      kb_int_sign         (kb_int a)                                                  { return (a > 0 ? 1 : 0) - (a < 0 ? 1 : 0); }
KB_API_INLINE kb_int      kb_int_max          (kb_int a, kb_int b)                                        { return a > b ? a : b; }
KB_API_INLINE kb_int      kb_int_min          (kb_int a, kb_int b)                                        { return b > a ? a : b; }
KB_API_INLINE kb_int      kb_int_log2         (kb_int a)                                                  { return log2(a); }
KB_API_INLINE kb_int      kb_int_clamp        (kb_int v, kb_int min, kb_int max)                          { return kb_int_max(kb_int_min(v, max), min); }
KB_API_INLINE kb_int      kb_int_square       (kb_int a)                                                  { return a * a; }

KB_API_INLINE bool        kb_float_is_nan     (kb_float a)                                                { return isnan(a); }
KB_API_INLINE kb_float    kb_float_nms        (kb_float a, kb_float b, kb_float c)                        { return c - a * b; }
KB_API_INLINE kb_float    kb_float_mad        (kb_float a, kb_float b, kb_float c)                        { return a * b + c; }
KB_API_INLINE kb_float    kb_float_neg        (kb_float a)                                                { return -a; }
KB_API_INLINE kb_float    kb_float_add        (kb_float a, kb_float b)                                    { return a + b; }
KB_API_INLINE kb_float    kb_float_sub        (kb_float a, kb_float b)                                    { return a - b; }
KB_API_INLINE kb_float    kb_float_mul        (kb_float a, kb_float b)                                    { return a * b; }
KB_API_INLINE kb_float    kb_float_div        (kb_float a, kb_float b)                                    { return a / b; }
KB_API_INLINE kb_float    kb_float_dot        (kb_float a, kb_float b)                                    { return a * b; }
KB_API_INLINE kb_float    kb_float_sin        (kb_float f)                                                { return sin(f); }
KB_API_INLINE kb_float    kb_float_cos        (kb_float f)                                                { return cos(f); }
KB_API_INLINE kb_float    kb_float_tan        (kb_float f)                                                { return tan(f); }
KB_API_INLINE kb_float    kb_float_sinh       (kb_float f)                                                { return sinh(f); }
KB_API_INLINE kb_float    kb_float_asin       (kb_float f)                                                { return asin(f); }
KB_API_INLINE kb_float    kb_float_acos       (kb_float f)                                                { return acos(f); }
KB_API_INLINE kb_float    kb_float_atan       (kb_float f)                                                { return atan(f); }
KB_API_INLINE kb_float    kb_float_atan2      (kb_float a, kb_float b)                                    { return atan2(a, b); }
KB_API_INLINE kb_float    kb_float_cosh       (kb_float f)                                                { return cosh(f); }
KB_API_INLINE kb_float    kb_float_select     (kb_float a, kb_float b, bool s)                            { return s ? b : a; }
KB_API_INLINE kb_float    kb_float_step       (kb_float a, kb_float b)                                    { return kb_float_select(0.0f, 1.0f, a >= b); }
KB_API_INLINE kb_float    kb_float_sqrt       (kb_float f)                                                { return sqrt(f); }
KB_API_INLINE kb_float    kb_float_floor      (kb_float f)                                                { return floor(f); }
KB_API_INLINE kb_float    kb_float_frac       (kb_float f)                                                { return f - kb_float_floor(f); }
KB_API_INLINE kb_float    kb_float_ceil       (kb_float f)                                                { return ceil(f); }
KB_API_INLINE kb_float    kb_float_round      (kb_float f)                                                { return round(f); }
KB_API_INLINE kb_float    kb_float_trunc      (kb_float f)                                                { return (kb_float) (kb_int) f; }
KB_API_INLINE kb_float    kb_float_lerp       (kb_float a, kb_float b, kb_float c)                        { return a + c * (b - a); }
KB_API_INLINE kb_float    kb_float_lerp_inv   (kb_float a, kb_float b, kb_float c)                        { return (c - a) / (b - a); }
KB_API_INLINE kb_float    kb_float_sign       (kb_float a)                                                { return (a > 0.0f ? 1.0f : 0.0f) - (a < 0.0f ? 1.0f : 0.0f); }
KB_API_INLINE kb_float    kb_float_abs        (kb_float a)                                                { return a < 0.0f ? -a : a; }
KB_API_INLINE kb_float    kb_float_max        (kb_float a, kb_float b)                                    { return a > b ? a : b; }
KB_API_INLINE kb_float    kb_float_min        (kb_float a, kb_float b)                                    { return b > a ? a : b; }
KB_API_INLINE kb_float    kb_float_dist       (kb_float a, kb_float b)                                    { return kb_float_abs(a - b); }
KB_API_INLINE kb_float    kb_float_pow        (kb_float a, kb_float b)                                    { return pow(a, b); }
KB_API_INLINE kb_float    kb_float_exp        (kb_float a)                                                { return exp(a); }
KB_API_INLINE kb_float    kb_float_log        (kb_float a)                                                { return log(a); }
KB_API_INLINE kb_float    kb_float_log2       (kb_float a)                                                { return log2(a); }
KB_API_INLINE kb_float    kb_float_square     (kb_float a)                                                { return a * a; }
KB_API_INLINE kb_float    kb_float_recip      (kb_float a)                                                { return 1.0f / a; }
KB_API_INLINE kb_float    kb_float_fmod       (kb_float a, kb_float b)                                    { return fmod(a, b); }
KB_API_INLINE kb_float    kb_float_mod        (kb_float a, kb_float b)                                    { return a - b * kb_float_floor(a / b); }
KB_API_INLINE kb_float    kb_float_bias       (kb_float t, kb_float bias)                                 { return t / ( ( (1.0f / bias - 2.0f)*(1.0f - t) ) + 1.0f); }
KB_API_INLINE kb_float    kb_float_gain       (kb_float t, kb_float gain)                                 { return t < 0.5f ? kb_float_bias(t * 2.0f, gain) * 0.5f : kb_float_bias(t * 2.0f - 1.0f, 1.0f - gain) * 0.5f + 0.5f; }
KB_API_INLINE kb_float    kb_float_smoothstep (kb_float a)                                                { return kb_float_square(a) * (3.0f - 2.0f * a); }
KB_API_INLINE kb_float    kb_float_clamp      (kb_float v, kb_float min, kb_float max)                    { return kb_float_max(kb_float_min(v, max), min); }
KB_API_INLINE kb_float    kb_float_remap      (kb_float x, kb_float a, kb_float b, kb_float c, kb_float d){ return kb_float_lerp(c, d, kb_float_lerp_inv(a, b, x)); }
KB_API_INLINE kb_float    kb_float_saturate   (kb_float a)                                                { return kb_float_clamp(a, 0.0f, 1.0f); }
KB_API_INLINE kb_float    kb_float_wrap       (kb_float v, kb_float wrap)                                 { const kb_float tmp = kb_float_mod(v, wrap); return tmp < 0.0f ? wrap + tmp : tmp; }
KB_API_INLINE bool        kb_float_equal      (kb_float a, kb_float b)                                    { return kb_float_abs(a - b) < FLT_EPSILON * kb_float_max(kb_float_max(1.0f, kb_float_abs(a)), kb_float_abs(b)); }

KB_API_INLINE kb_int2      kb_int2_add          (const kb_int2 a, const kb_int2 b)                        { return (kb_int2) _ELEMW_OP2_T2(kb_int_add); }
KB_API_INLINE kb_int2      kb_int2_sub          (const kb_int2 a, const kb_int2 b)                        { return (kb_int2) _ELEMW_OP2_T2(kb_int_sub); }
KB_API_INLINE kb_int2      kb_int2_mul          (const kb_int2 a, const kb_int2 b)                        { return (kb_int2) _ELEMW_OP2_T2(kb_int_mul); }
KB_API_INLINE kb_int2      kb_int2_div          (const kb_int2 a, const kb_int2 b)                        { return (kb_int2) _ELEMW_OP2_T2(kb_int_div); }
KB_API_INLINE kb_int2      kb_int2_min          (const kb_int2 a, const kb_int2 b)                        { return (kb_int2) _ELEMW_OP2_T2(kb_int_min); }
KB_API_INLINE kb_int2      kb_int2_max          (const kb_int2 a, const kb_int2 b)                        { return (kb_int2) _ELEMW_OP2_T2(kb_int_max); }
KB_API_INLINE kb_int2      kb_int2_scale        (const kb_int2 a, const kb_int f)                         { return (kb_int2) _ELEMW_OP1_F_T2(kb_int_mul, f); }
KB_API_INLINE kb_int2      kb_int2_neg          (const kb_int2 a)                                         { return (kb_int2) _ELEMW_OP1_T2(kb_int_neg); }
KB_API_INLINE kb_int2      kb_int2_abs          (const kb_int2 a)                                         { return (kb_int2) _ELEMW_OP1_T2(kb_int_abs); }
KB_API_INLINE kb_int2      kb_int2_sign         (const kb_int2 a)                                         { return (kb_int2) _ELEMW_OP1_T2(kb_int_sign); }
KB_API_INLINE kb_int2      kb_int2_square       (const kb_int2 a)                                         { return (kb_int2) _ELEMW_OP1_T2(kb_int_square); }
KB_API_INLINE kb_int       kb_int2_dot          (const kb_int2 a, const kb_int2 b)                        { return _ELEMW_COMB_OP2_T2(kb_int_mul, +); }
KB_API_INLINE bool         kb_int2_equal        (const kb_int2 a, const kb_int2 b)                        { return _ELEMW_COMB_OP2_T2(kb_int_equal, &&); }
KB_API_INLINE float        kb_int2_len          (const kb_int2 a)                                         { return kb_float_sqrt(kb_int2_dot(a, a)); }

KB_API_INLINE kb_int3      kb_int3_add          (const kb_int3 a, const kb_int3 b)                        { return (kb_int3) _ELEMW_OP2_T3(kb_int_add); }
KB_API_INLINE kb_int3      kb_int3_sub          (const kb_int3 a, const kb_int3 b)                        { return (kb_int3) _ELEMW_OP2_T3(kb_int_sub); }
KB_API_INLINE kb_int3      kb_int3_mul          (const kb_int3 a, const kb_int3 b)                        { return (kb_int3) _ELEMW_OP2_T3(kb_int_mul); }
KB_API_INLINE kb_int3      kb_int3_div          (const kb_int3 a, const kb_int3 b)                        { return (kb_int3) _ELEMW_OP2_T3(kb_int_div); }
KB_API_INLINE kb_int3      kb_int3_min          (const kb_int3 a, const kb_int3 b)                        { return (kb_int3) _ELEMW_OP2_T3(kb_int_min); }
KB_API_INLINE kb_int3      kb_int3_max          (const kb_int3 a, const kb_int3 b)                        { return (kb_int3) _ELEMW_OP2_T3(kb_int_max); }
KB_API_INLINE kb_int3      kb_int3_scale        (const kb_int3 a, const kb_int f)                         { return (kb_int3) _ELEMW_OP1_F_T3(kb_int_mul, f); }
KB_API_INLINE kb_int3      kb_int3_neg          (const kb_int3 a)                                         { return (kb_int3) _ELEMW_OP1_T3(kb_int_neg); }
KB_API_INLINE kb_int3      kb_int3_abs          (const kb_int3 a)                                         { return (kb_int3) _ELEMW_OP1_T3(kb_int_abs); }
KB_API_INLINE kb_int3      kb_int3_sign         (const kb_int3 a)                                         { return (kb_int3) _ELEMW_OP1_T3(kb_int_sign); }
KB_API_INLINE kb_int3      kb_int3_square       (const kb_int3 a)                                         { return (kb_int3) _ELEMW_OP1_T3(kb_int_square); }
KB_API_INLINE kb_int       kb_int3_dot          (const kb_int3 a, const kb_int3 b)                        { return _ELEMW_COMB_OP2_T3(kb_int_mul, +); }
KB_API_INLINE bool         kb_int3_equal        (const kb_int3 a, const kb_int3 b)                        { return _ELEMW_COMB_OP2_T3(kb_int_equal, &&); }
KB_API_INLINE kb_float     kb_int3_len          (const kb_int3 a)                                         { return kb_float_sqrt(kb_int3_dot(a, a)); }

KB_API_INLINE kb_int4      kb_int4_add          (const kb_int4 a, const kb_int4 b)                        { return (kb_int4) _ELEMW_OP2_T4(kb_int_add); }
KB_API_INLINE kb_int4      kb_int4_sub          (const kb_int4 a, const kb_int4 b)                        { return (kb_int4) _ELEMW_OP2_T4(kb_int_sub); }
KB_API_INLINE kb_int4      kb_int4_mul          (const kb_int4 a, const kb_int4 b)                        { return (kb_int4) _ELEMW_OP2_T4(kb_int_mul); }
KB_API_INLINE kb_int4      kb_int4_div          (const kb_int4 a, const kb_int4 b)                        { return (kb_int4) _ELEMW_OP2_T4(kb_int_div); }
KB_API_INLINE kb_int4      kb_int4_min          (const kb_int4 a, const kb_int4 b)                        { return (kb_int4) _ELEMW_OP2_T4(kb_int_min); }
KB_API_INLINE kb_int4      kb_int4_max          (const kb_int4 a, const kb_int4 b)                        { return (kb_int4) _ELEMW_OP2_T4(kb_int_max); }
KB_API_INLINE kb_int4      kb_int4_scale        (const kb_int4 a, const kb_int f)                         { return (kb_int4) _ELEMW_OP1_F_T4(kb_int_mul, f); }
KB_API_INLINE kb_int4      kb_int4_neg          (const kb_int4 a)                                         { return (kb_int4) _ELEMW_OP1_T4(kb_int_neg); }
KB_API_INLINE kb_int4      kb_int4_abs          (const kb_int4 a)                                         { return (kb_int4) _ELEMW_OP1_T4(kb_int_abs); }
KB_API_INLINE kb_int4      kb_int4_sign         (const kb_int4 a)                                         { return (kb_int4) _ELEMW_OP1_T4(kb_int_sign); }
KB_API_INLINE kb_int4      kb_int4_square       (const kb_int4 a)                                         { return (kb_int4) _ELEMW_OP1_T4(kb_int_square); }
KB_API_INLINE kb_int       kb_int4_dot          (const kb_int4 a, const kb_int4 b)                        { return _ELEMW_COMB_OP2_T4(kb_int_mul, +); }
KB_API_INLINE bool         kb_int4_equal        (const kb_int4 a, const kb_int4 b)                        { return _ELEMW_COMB_OP2_T4(kb_int_equal, &&); }
KB_API_INLINE kb_float     kb_int4_len          (const kb_int4 a)                                         { return kb_float_sqrt(kb_int4_dot(a, a)); }

KB_API_INLINE kb_float2    kb_float2_abs        (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_abs); }
KB_API_INLINE kb_float2    kb_float2_neg        (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_neg); }
KB_API_INLINE kb_float2    kb_float2_floor      (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_floor); }
KB_API_INLINE kb_float2    kb_float2_add        (const kb_float2 a, const kb_float2 b)                    { return (kb_float2) _ELEMW_OP2_T2(kb_float_add); }
KB_API_INLINE kb_float2    kb_float2_sub        (const kb_float2 a, const kb_float2 b)                    { return (kb_float2) _ELEMW_OP2_T2(kb_float_sub); }
KB_API_INLINE kb_float2    kb_float2_mul        (const kb_float2 a, const kb_float2 b)                    { return (kb_float2) _ELEMW_OP2_T2(kb_float_mul); }
KB_API_INLINE kb_float2    kb_float2_min        (const kb_float2 a, const kb_float2 b)                    { return (kb_float2) _ELEMW_OP2_T2(kb_float_min); }
KB_API_INLINE kb_float2    kb_float2_max        (const kb_float2 a, const kb_float2 b)                    { return (kb_float2) _ELEMW_OP2_T2(kb_float_max); }
KB_API_INLINE kb_float2    kb_float2_scale      (const kb_float2 a, const kb_float f)                     { return (kb_float2) _ELEMW_OP1_F_T2(kb_float_mul, f); }
KB_API_INLINE kb_float2    kb_float2_recip      (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_recip); }
KB_API_INLINE kb_float2    kb_float2_lerp       (const kb_float2 a, const kb_float2 b, const kb_float v)  { return (kb_float2) _ELEMW_OP2_F_T2(kb_float_lerp, v); }
KB_API_INLINE kb_float2    kb_float2_saturate   (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_saturate); }
KB_API_INLINE kb_float2    kb_float2_smoothstep (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_smoothstep); }
KB_API_INLINE kb_float2    kb_float2_square     (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_square); }
KB_API_INLINE kb_float2    kb_float2_exp        (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_exp); }
KB_API_INLINE kb_float2    kb_float2_log        (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_log); }
KB_API_INLINE kb_float2    kb_float2_sign       (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_sign); }
KB_API_INLINE kb_float2    kb_float2_sqrt       (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_sqrt); }
KB_API_INLINE kb_float2    kb_float2_frac       (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_frac); }
KB_API_INLINE kb_float2    kb_float2_ceil       (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_ceil); }
KB_API_INLINE kb_float2    kb_float2_round      (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_round); }
KB_API_INLINE kb_float2    kb_float2_trunc      (const kb_float2 a)                                       { return (kb_float2) _ELEMW_OP1_T2(kb_float_trunc); }
KB_API_INLINE kb_float     kb_float2_dot        (const kb_float2 a, const kb_float2 b)                    { return _ELEMW_COMB_OP2_T2(kb_float_mul, +); }
KB_API_INLINE bool         kb_float2_equal      (const kb_float2 a, const kb_float2 b)                    { return _ELEMW_COMB_OP2_T2(kb_float_equal, &&); }
KB_API_INLINE kb_float     kb_float2_len        (const kb_float2 a)                                       { return kb_float_sqrt(kb_float2_dot(a, a)); }
KB_API_INLINE kb_float2    kb_float2_norm       (const kb_float2 a)                                       { return kb_float2_scale(a, kb_float_recip(kb_float2_len(a))); }
KB_API_INLINE kb_float2    kb_float2_div        (const kb_float2 a, const kb_float2 b)                    { return kb_float2_mul(a, kb_float2_recip(b)); }
KB_API_INLINE kb_float     kb_float2_dist       (const kb_float2 a, const kb_float2 b)                    { return kb_float2_len(kb_float2_sub(b, a)); }

KB_API_INLINE kb_float3    kb_float3_abs        (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_abs); }
KB_API_INLINE kb_float3    kb_float3_neg        (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_neg); }
KB_API_INLINE kb_float3    kb_float3_floor      (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_floor); }
KB_API_INLINE kb_float3    kb_float3_add        (const kb_float3 a, const kb_float3 b)                    { return (kb_float3) _ELEMW_OP2_T3(kb_float_add); }
KB_API_INLINE kb_float3    kb_float3_sub        (const kb_float3 a, const kb_float3 b)                    { return (kb_float3) _ELEMW_OP2_T3(kb_float_sub); }
KB_API_INLINE kb_float3    kb_float3_mul        (const kb_float3 a, const kb_float3 b)                    { return (kb_float3) _ELEMW_OP2_T3(kb_float_mul); }
KB_API_INLINE kb_float3    kb_float3_min        (const kb_float3 a, const kb_float3 b)                    { return (kb_float3) _ELEMW_OP2_T3(kb_float_min); }
KB_API_INLINE kb_float3    kb_float3_max        (const kb_float3 a, const kb_float3 b)                    { return (kb_float3) _ELEMW_OP2_T3(kb_float_max); }
KB_API_INLINE kb_float3    kb_float3_scale      (const kb_float3 a, const kb_float f)                     { return (kb_float3) _ELEMW_OP1_F_T3(kb_float_mul, f); }
KB_API_INLINE kb_float3    kb_float3_recip      (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_recip); }
KB_API_INLINE kb_float3    kb_float3_lerp       (const kb_float3 a, const kb_float3 b, const kb_float v)  { return (kb_float3) _ELEMW_OP2_F_T3(kb_float_lerp, v); }
KB_API_INLINE kb_float3    kb_float3_saturate   (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_saturate); }
KB_API_INLINE kb_float3    kb_float3_smoothstep (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_smoothstep); }
KB_API_INLINE kb_float3    kb_float3_square     (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_square); }
KB_API_INLINE kb_float3    kb_float3_exp        (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_exp); }
KB_API_INLINE kb_float3    kb_float3_log        (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_log); }
KB_API_INLINE kb_float3    kb_float3_sign       (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_sign); }
KB_API_INLINE kb_float3    kb_float3_sqrt       (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_sqrt); }
KB_API_INLINE kb_float3    kb_float3_frac       (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_frac); }
KB_API_INLINE kb_float3    kb_float3_ceil       (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_ceil); }
KB_API_INLINE kb_float3    kb_float3_round      (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_round); }
KB_API_INLINE kb_float3    kb_float3_trunc      (const kb_float3 a)                                       { return (kb_float3) _ELEMW_OP1_T3(kb_float_trunc); }
KB_API_INLINE kb_float     kb_float3_dot        (const kb_float3 a, const kb_float3 b)                    { return _ELEMW_COMB_OP2_T3(kb_float_mul, +); }
KB_API_INLINE bool         kb_float3_equal      (const kb_float3 a, const kb_float3 b)                    { return _ELEMW_COMB_OP2_T3(kb_float_equal, &&); }
KB_API_INLINE kb_float     kb_float3_len        (const kb_float3 a)                                       { return kb_float_sqrt(kb_float3_dot(a, a)); }
KB_API_INLINE kb_float3    kb_float3_norm       (const kb_float3 a)                                       { return kb_float3_scale(a, kb_float_recip(kb_float3_len(a))); }
KB_API_INLINE kb_float3    kb_float3_div        (const kb_float3 a, const kb_float3 b)                    { return kb_float3_mul(a, kb_float3_recip(b)); }
KB_API_INLINE kb_float     kb_float3_dist       (const kb_float3 a, const kb_float3 b)                    { return kb_float3_len(kb_float3_sub(b, a)); }
KB_API_INLINE kb_float3    kb_float3_cross      (const kb_float3 a, const kb_float3 b)                    { return (kb_float3) { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };}

KB_API_INLINE kb_float4    kb_float4_abs        (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_abs); }
KB_API_INLINE kb_float4    kb_float4_neg        (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_neg); }
KB_API_INLINE kb_float4    kb_float4_floor      (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_floor); }
KB_API_INLINE kb_float4    kb_float4_add        (const kb_float4 a, const kb_float4 b)                    { return (kb_float4) _ELEMW_OP2_T4(kb_float_add); }
KB_API_INLINE kb_float4    kb_float4_sub        (const kb_float4 a, const kb_float4 b)                    { return (kb_float4) _ELEMW_OP2_T4(kb_float_sub); }
KB_API_INLINE kb_float4    kb_float4_mul        (const kb_float4 a, const kb_float4 b)                    { return (kb_float4) _ELEMW_OP2_T4(kb_float_mul); }
KB_API_INLINE kb_float4    kb_float4_min        (const kb_float4 a, const kb_float4 b)                    { return (kb_float4) _ELEMW_OP2_T4(kb_float_min); }
KB_API_INLINE kb_float4    kb_float4_max        (const kb_float4 a, const kb_float4 b)                    { return (kb_float4) _ELEMW_OP2_T4(kb_float_max); }
KB_API_INLINE kb_float4    kb_float4_scale      (const kb_float4 a, const kb_float f)                     { return (kb_float4) _ELEMW_OP1_F_T4(kb_float_mul, f); }
KB_API_INLINE kb_float4    kb_float4_recip      (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_recip); }
KB_API_INLINE kb_float4    kb_float4_lerp       (const kb_float4 a, const kb_float4 b, const kb_float v)  { return (kb_float4) _ELEMW_OP2_F_T4(kb_float_lerp, v); }
KB_API_INLINE kb_float4    kb_float4_saturate   (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_saturate); }
KB_API_INLINE kb_float4    kb_float4_smoothstep (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_smoothstep); }
KB_API_INLINE kb_float4    kb_float4_square     (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_square); }
KB_API_INLINE kb_float4    kb_float4_exp        (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_exp); }
KB_API_INLINE kb_float4    kb_float4_log        (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_log); }
KB_API_INLINE kb_float4    kb_float4_sign       (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_sign); }
KB_API_INLINE kb_float4    kb_float4_sqrt       (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_sqrt); }
KB_API_INLINE kb_float4    kb_float4_frac       (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_frac); }
KB_API_INLINE kb_float4    kb_float4_ceil       (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_ceil); }
KB_API_INLINE kb_float4    kb_float4_round      (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_round); }
KB_API_INLINE kb_float4    kb_float4_trunc      (const kb_float4 a)                                       { return (kb_float4) _ELEMW_OP1_T4(kb_float_trunc); }
KB_API_INLINE kb_float     kb_float4_dot        (const kb_float4 a, const kb_float4 b)                    { return _ELEMW_COMB_OP2_T4(kb_float_mul, +); }
KB_API_INLINE bool         kb_float4_equal      (const kb_float4 a, const kb_float4 b)                    { return _ELEMW_COMB_OP2_T4(kb_float_equal, &&); }
KB_API_INLINE kb_float     kb_float4_len        (const kb_float4 a)                                       { return kb_float_sqrt(kb_float4_dot(a, a)); }
KB_API_INLINE kb_float4    kb_float4_norm       (const kb_float4 a)                                       { return kb_float4_scale(a, kb_float_recip(kb_float4_len(a))); }
KB_API_INLINE kb_float4    kb_float4_div        (const kb_float4 a, const kb_float4 b)                    { return kb_float4_mul(a, kb_float4_recip(b)); }
KB_API_INLINE kb_float     kb_float4_dist       (const kb_float4 a, const kb_float4 b)                    { return kb_float4_len(kb_float4_sub(b, a)); }

KB_API        kb_quat      kb_quat_inv          (const kb_quat a);
KB_API        kb_quat      kb_quat_norm         (const kb_quat a);
KB_API        kb_quat      kb_quat_mul          (const kb_quat a, const kb_quat b);
KB_API        kb_quat      kb_quat_euler        (const kb_float3 euler);
KB_API        kb_quat      kb_quat_axis_angle   (const kb_float3 axis, kb_float angle);
KB_API        kb_quat      kb_quat_norm         (const kb_quat a);
KB_API        kb_float3    kb_quat_act          (const kb_quat a, const kb_float3 b);
KB_API        kb_quat      kb_quat_slerp        (const kb_quat a, const kb_quat b, kb_float t);
KB_API_INLINE kb_quat      kb_quat_neg          (const kb_quat a)                                         { return (kb_quat) _ELEMW_OP1_T4(kb_float_neg); }
KB_API_INLINE kb_quat      kb_quat_add          (const kb_quat a, const kb_quat b)                        { return (kb_quat) _ELEMW_OP2_T4(kb_float_add); }
KB_API_INLINE kb_quat      kb_quat_sub          (const kb_quat a, const kb_quat b)                        { return (kb_quat) _ELEMW_OP2_T4(kb_float_sub); }
KB_API_INLINE kb_quat      kb_quat_scale        (const kb_quat a, const kb_float f)                       { return (kb_quat) _ELEMW_OP1_F_T4(kb_float_mul, f); }
KB_API_INLINE kb_quat      kb_quat_lerp         (const kb_quat a, const kb_quat b, const kb_float v)      { return kb_quat_norm((kb_quat) _ELEMW_OP2_F_T4(kb_float_lerp, v)) ; }
KB_API_INLINE kb_float     kb_quat_dot          (const kb_quat a, const kb_quat b)                        { return _ELEMW_COMB_OP2_T4(kb_float_mul, +); }
KB_API_INLINE bool         kb_quat_equal        (const kb_quat a, const kb_quat b)                        { return _ELEMW_COMB_OP2_T4(kb_float_equal, &&); }
KB_API_INLINE kb_float     kb_quat_len          (const kb_quat a)                                         { return kb_float_sqrt(kb_quat_dot(a, a)); }
KB_API_INLINE kb_quat      kb_quat_conj         (const kb_quat a)                                         { return (kb_quat) { -a.x, -a.y, -a.z, +a.w }; }
KB_API_INLINE kb_quat      kb_quat_recip        (const kb_quat a)                                         { return kb_quat_scale(kb_quat_conj(a), 1.0f / kb_quat_dot(a, a)); }
KB_API_INLINE kb_quat      kb_quat_div          (const kb_quat a, const kb_quat b)                        { return kb_quat_mul(a, kb_quat_inv(b)); }

KB_API        kb_float4    kb_float4x4_act          (const kb_float4x4 a, const kb_float4 b);
KB_API        kb_float4x4  kb_float4x4_transform    (const kb_float3 pos, const kb_float3 scale, const kb_quat rot);
KB_API        kb_float4x4  kb_float4x4_mul          (const kb_float4x4 a, const kb_float4x4 b);
KB_API        kb_float4x4  kb_float4x4_inv          (const kb_float4x4 a);
KB_API        kb_float4x4  kb_float4x4_transpose    (const kb_float4x4 a);
KB_API        void         kb_float4x4_translate    (kb_float4x4* mtx, const kb_float3 pos);
KB_API        void         kb_float4x4_scale        (kb_float4x4* mtx, const kb_float3 scale);
KB_API        void         kb_float4x4_rotate       (kb_float4x4* mtx, const kb_quat rotation);
KB_API        kb_float4x4  kb_float4x4_translation  (const kb_float3 v);
KB_API        kb_float4x4  kb_float4x4_scaling      (const kb_float3 v);
KB_API        kb_float4x4  kb_float4x4_rotation     (const kb_quat v);

KB_API_INLINE bool         kb_activated               (uint8_t* curr, uint8_t* prev, size_t i)         { return !prev[i] && curr[i]; }
KB_API_INLINE bool         kb_deactivated             (uint8_t* curr, uint8_t* prev, size_t i)         { return prev[i] && !curr[i]; }
KB_API_INLINE kb_float     kb_deg_to_rad              (kb_float deg)                                   { return deg * PI / 180.0f; }
KB_API_INLINE kb_float     kb_rad_to_deg              (kb_float rad)                                   { return rad * 180.0f / PI; }
KB_API        bool         kb_aabb_contains           (kb_aabb_int aabb, kb_int3 pos);
KB_API        kb_float4x4  kb_look_at                 (const kb_float3 from, const kb_float3 to, const kb_float3 up);
KB_API        kb_float4x4  kb_perspective             (kb_float fov, kb_float aspect, kb_float near, kb_float far);
KB_API        kb_float4x4  kb_orthographic            (kb_float left, kb_float right, kb_float top, kb_float bottom, kb_float near, kb_float far);
KB_API        kb_float3    kb_point_on_plane          (kb_axis axis, kb_float x, kb_float y);
KB_API        void         kb_tangent_frame           (const kb_float3 normal, kb_float3* tangent, kb_float3* bitangent);
KB_API        void         kb_tangent_frame_with_spin (const kb_float3 normal, float angle, kb_float3* tangent, kb_float3* bitangent);
KB_API        uint32_t     kb_color_rgba_to_uint      (const kb_float4 color);
KB_API        float        kb_ray_plane_intersection  (const kb_ray ray, const kb_plane plane);
KB_API        kb_float3    kb_unproject               (kb_float4x4 unproj, kb_float3 point);
KB_API        kb_ray       kb_unproject_view          (kb_float4x4 unproj, kb_float2 p);
KB_API        uint64_t     kb_align_up                (uint64_t a, uint64_t align);
KB_API        kb_float2    kb_circle_point            (kb_float angle);
KB_API        kb_float2    kb_squircle_point          (kb_float angle);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace kb {
  using integer     = kb_int;
  using scalar      = kb_float;
  using int2        = kb_int2;
  using int3        = kb_int3;
  using int4        = kb_int4;
  using float2      = kb_float2;
  using float3      = kb_float3;
  using float4      = kb_float4;
  using quat        = kb_quat;
  using float4x4    = kb_float4x4;
  using float3x3    = kb_float3x3;
  using aabb        = kb_aabb;
  using aabb_int    = kb_aabb_int;
  using plane       = kb_plane;
  using rect_float  = kb_rect_float;
  using rect_int    = kb_rect_int;
  using sphere      = kb_sphere;
  using capsule     = kb_capsule;
  using cone        = kb_cone;
  using cylinder    = kb_cylinder;
  using disk        = kb_disk;
  using triangle    = kb_triangle;
  using ray         = kb_ray;
  using hit         = kb_hit;
  using xform       = kb_xform;

  FUNC1_DEF_N(log2,   integer,    kb_int_log2);
  FUNC1_DEF_N(log2,   scalar,     kb_float_log2);
  
  FUNC1_DEF_N(abs,    scalar,     kb_float_abs);
  FUNC1_DEF_N(abs,    float2,     kb_float2_abs);
  FUNC1_DEF_N(abs,    float3,     kb_float3_abs);
  FUNC1_DEF_N(abs,    float4,     kb_float4_abs);
  FUNC1_DEF_N(abs,    integer,    kb_int_abs);
  FUNC1_DEF_N(abs,    int2,       kb_int2_abs);
  FUNC1_DEF_N(abs,    int3,       kb_int3_abs);
  FUNC1_DEF_N(abs,    int4,       kb_int4_abs);

  FUNC1_DEF_N(sign,   scalar,     kb_float_sign);
  FUNC1_DEF_N(sign,   float2,     kb_float2_sign);
  FUNC1_DEF_N(sign,   float3,     kb_float3_sign);
  FUNC1_DEF_N(sign,   float4,     kb_float4_sign);
  FUNC1_DEF_N(sign,   integer,    kb_int_sign);
  FUNC1_DEF_N(sign,   int2,       kb_int2_sign);
  FUNC1_DEF_N(sign,   int3,       kb_int3_sign);
  FUNC1_DEF_N(sign,   int4,       kb_int4_sign);

  FUNC1_DEF_N(square,  scalar,    kb_float_square);
  FUNC1_DEF_N(square,  float2,    kb_float2_square);
  FUNC1_DEF_N(square,  float3,    kb_float3_square);
  FUNC1_DEF_N(square,  float4,    kb_float4_square);
  FUNC1_DEF_N(square,  integer,   kb_int_square);
  FUNC1_DEF_N(square,  int2,      kb_int2_square);
  FUNC1_DEF_N(square,  int3,      kb_int3_square);
  FUNC1_DEF_N(square,  int4,      kb_int4_square);

  FUNC1_DEF_N(neg,     scalar,    kb_float_neg);
  FUNC1_DEF_N(neg,     float2,    kb_float2_neg);
  FUNC1_DEF_N(neg,     float3,    kb_float3_neg);
  FUNC1_DEF_N(neg,     float4,    kb_float4_neg);
  FUNC1_DEF_N(neg,     quat,      kb_quat_neg);
  FUNC1_DEF_N(neg,     integer,   kb_int_neg);
  FUNC1_DEF_N(neg,     int2,      kb_int2_neg);
  FUNC1_DEF_N(neg,     int3,      kb_int3_neg);
  FUNC1_DEF_N(neg,     int4,      kb_int4_neg);

  FUNC2_DEF_N(add,  scalar,   scalar,   kb_float_add);
  FUNC2_DEF_N(add,  float2,   float2,   kb_float2_add);
  FUNC2_DEF_N(add,  float3,   float3,   kb_float3_add);
  FUNC2_DEF_N(add,  float4,   float4,   kb_float4_add);
  FUNC2_DEF_N(add,  quat,     quat,     kb_quat_add);
  FUNC2_DEF_N(add,  integer,  integer,  kb_int_add);
  FUNC2_DEF_N(add,  int2,     int2,     kb_int2_add);
  FUNC2_DEF_N(add,  int3,     int3,     kb_int3_add);
  FUNC2_DEF_N(add,  int4,     int4,     kb_int4_add);

  FUNC2_DEF_N(sub,  scalar,   scalar,   kb_float_sub);
  FUNC2_DEF_N(sub,  float2,   float2,   kb_float2_sub);
  FUNC2_DEF_N(sub,  float3,   float3,   kb_float3_sub);
  FUNC2_DEF_N(sub,  float4,   float4,   kb_float4_sub);
  FUNC2_DEF_N(sub,  quat,     quat,     kb_quat_sub);
  FUNC2_DEF_N(sub,  integer,  integer,  kb_int_sub);
  FUNC2_DEF_N(sub,  int2,     int2,     kb_int2_sub);
  FUNC2_DEF_N(sub,  int3,     int3,     kb_int3_sub);
  FUNC2_DEF_N(sub,  int4,     int4,     kb_int4_sub);

  FUNC2_DEF_N(mul,  scalar,   scalar,   kb_float_mul);
  FUNC2_DEF_N(mul,  float2,   float2,   kb_float2_mul);
  FUNC2_DEF_N(mul,  float3,   float3,   kb_float3_mul);
  FUNC2_DEF_N(mul,  float4,   float4,   kb_float4_mul);
  FUNC2_DEF_N(mul,  quat,     quat,     kb_quat_mul);
  FUNC2_DEF_N(mul,  integer,  integer,  kb_int_mul);
  FUNC2_DEF_N(mul,  int2,     int2,     kb_int2_mul);
  FUNC2_DEF_N(mul,  int3,     int3,     kb_int3_mul);
  FUNC2_DEF_N(mul,  int4,     int4,     kb_int4_mul);

  FUNC2_DEF_N(div,  scalar,   scalar,   kb_float_div);
  FUNC2_DEF_N(div,  float2,   float2,   kb_float2_div);
  FUNC2_DEF_N(div,  float3,   float3,   kb_float3_div);
  FUNC2_DEF_N(div,  float4,   float4,   kb_float4_div);
  FUNC2_DEF_N(div,  quat,     quat,     kb_quat_div);
  FUNC2_DEF_N(div,  integer,  integer,  kb_int_div);
  FUNC2_DEF_N(div,  int2,     int2,     kb_int2_div);
  FUNC2_DEF_N(div,  int3,     int3,     kb_int3_div);
  FUNC2_DEF_N(div,  int4,     int4,     kb_int4_div);

  FUNC2_DEF_N(scale,  scalar,   scalar,   kb_float_mul);
  FUNC2_DEF_N(scale,  float2,   scalar,   kb_float2_scale);
  FUNC2_DEF_N(scale,  float3,   scalar,   kb_float3_scale);
  FUNC2_DEF_N(scale,  float4,   scalar,   kb_float4_scale);
  FUNC2_DEF_N(scale,  quat,     scalar,   kb_quat_scale);
  FUNC2_DEF_N(scale,  integer,  integer,  kb_int_mul);
  FUNC2_DEF_N(scale,  int2,     integer,  kb_int2_scale);
  FUNC2_DEF_N(scale,  int3,     integer,  kb_int3_scale);
  FUNC2_DEF_N(scale,  int4,     integer,  kb_int4_scale);

  FUNC2_DEF_N(equal,  scalar,   scalar,   kb_float_equal);
  FUNC2_DEF_N(equal,  float2,   float2,   kb_float2_equal);
  FUNC2_DEF_N(equal,  float3,   float3,   kb_float3_equal);
  FUNC2_DEF_N(equal,  float4,   float4,   kb_float4_equal);
  FUNC2_DEF_N(equal,  quat,     quat,     kb_quat_equal);
  FUNC2_DEF_N(equal,  integer,  integer,  kb_int_equal);
  FUNC2_DEF_N(equal,  int2,     int2,     kb_int2_equal);
  FUNC2_DEF_N(equal,  int3,     int3,     kb_int3_equal);
  FUNC2_DEF_N(equal,  int4,     int4,     kb_int4_equal);

  FUNC2_DEF_N(min,  scalar,   scalar,   kb_float_min);
  FUNC2_DEF_N(min,  float2,   float2,   kb_float2_min);
  FUNC2_DEF_N(min,  float3,   float3,   kb_float3_min);
  FUNC2_DEF_N(min,  float4,   float4,   kb_float4_min);
  FUNC2_DEF_N(min,  integer,  integer,  kb_int_min);
  FUNC2_DEF_N(min,  int2,     int2,     kb_int2_min);
  FUNC2_DEF_N(min,  int3,     int3,     kb_int3_min);
  FUNC2_DEF_N(min,  int4,     int4,     kb_int4_min);

  FUNC2_DEF_N(max,  scalar,   scalar,   kb_float_max);
  FUNC2_DEF_N(max,  float2,   float2,   kb_float2_max);
  FUNC2_DEF_N(max,  float3,   float3,   kb_float3_max);
  FUNC2_DEF_N(max,  float4,   float4,   kb_float4_max);
  FUNC2_DEF_N(max,  integer,  integer,  kb_int_max);
  FUNC2_DEF_N(max,  int2,     int2,     kb_int2_max);
  FUNC2_DEF_N(max,  int3,     int3,     kb_int3_max);
  FUNC2_DEF_N(max,  int4,     int4,     kb_int4_max);

  FUNC1_DEF_N(exp,    scalar,     kb_float_exp);
  FUNC1_DEF_N(exp,    float2,     kb_float2_exp);
  FUNC1_DEF_N(exp,    float3,     kb_float3_exp);
  FUNC1_DEF_N(exp,    float4,     kb_float4_exp);

  FUNC1_DEF_N(floor,  scalar,     kb_float_floor);
  FUNC1_DEF_N(floor,  float2,     kb_float2_floor);
  FUNC1_DEF_N(floor,  float3,     kb_float3_floor);
  FUNC1_DEF_N(floor,  float4,     kb_float4_floor);

  FUNC1_DEF_N(log,    scalar,     kb_float_log);
  FUNC1_DEF_N(log,    float2,     kb_float2_log);
  FUNC1_DEF_N(log,    float3,     kb_float3_log);
  FUNC1_DEF_N(log,    float4,     kb_float4_log);

  FUNC1_DEF_N(sqrt,   scalar,     kb_float_sqrt);
  FUNC1_DEF_N(sqrt,   float2,     kb_float2_sqrt);
  FUNC1_DEF_N(sqrt,   float3,     kb_float3_sqrt);
  FUNC1_DEF_N(sqrt,   float4,     kb_float4_sqrt);

  FUNC1_DEF_N(frac,   scalar,     kb_float_frac);
  FUNC1_DEF_N(frac,   float2,     kb_float2_frac);
  FUNC1_DEF_N(frac,   float3,     kb_float3_frac);
  FUNC1_DEF_N(frac,   float4,     kb_float4_frac);

  FUNC1_DEF_N(ceil,   scalar,     kb_float_ceil);
  FUNC1_DEF_N(ceil,   float2,     kb_float2_ceil);
  FUNC1_DEF_N(ceil,   float3,     kb_float3_ceil);
  FUNC1_DEF_N(ceil,   float4,     kb_float4_ceil);

  FUNC1_DEF_N(round,  scalar,     kb_float_round);
  FUNC1_DEF_N(round,  float2,     kb_float2_round);
  FUNC1_DEF_N(round,  float3,     kb_float3_round);
  FUNC1_DEF_N(round,  float4,     kb_float4_round);

  FUNC1_DEF_N(trunc,  scalar,     kb_float_trunc);
  FUNC1_DEF_N(trunc,  float2,     kb_float2_trunc);
  FUNC1_DEF_N(trunc,  float3,     kb_float3_trunc);
  FUNC1_DEF_N(trunc,  float4,     kb_float4_trunc);

  FUNC1_DEF_N(recip,  scalar,       kb_float_recip);
  FUNC1_DEF_N(recip,  float2,       kb_float2_recip);
  FUNC1_DEF_N(recip,  float3,       kb_float3_recip);
  FUNC1_DEF_N(recip,  float4,       kb_float4_recip);
  FUNC1_DEF_N(recip,  kb_float4x4,  kb_float4x4_inv);
  
  FUNC1_DEF_N(saturate,  scalar,  kb_float_saturate);
  FUNC1_DEF_N(saturate,  float2,  kb_float2_saturate);
  FUNC1_DEF_N(saturate,  float3,  kb_float3_saturate);
  FUNC1_DEF_N(saturate,  float4,  kb_float4_saturate);

  FUNC1_DEF_N(smoothstep,  scalar,  kb_float_smoothstep);
  FUNC1_DEF_N(smoothstep,  float2,  kb_float2_smoothstep);
  FUNC1_DEF_N(smoothstep,  float3,  kb_float3_smoothstep);
  FUNC1_DEF_N(smoothstep,  float4,  kb_float4_smoothstep);

  FUNC1_DEF_N(norm, float2,  kb_float2_norm);
  FUNC1_DEF_N(norm, float3,  kb_float3_norm);
  FUNC1_DEF_N(norm, float4,  kb_float4_norm);

  FUNC1_DEF_N(len, float2,  kb_float2_len);
  FUNC1_DEF_N(len, float3,  kb_float3_len);
  FUNC1_DEF_N(len, float4,  kb_float4_len);

  FUNC2_DEF_N(dist,  scalar,   scalar,    kb_float_dist);
  FUNC2_DEF_N(dist,  float2,   float2,    kb_float2_dist);
  FUNC2_DEF_N(dist,  float3,   float3,    kb_float3_dist);
  FUNC2_DEF_N(dist,  float4,   float4,    kb_float4_dist);

  FUNC3_DEF_N(lerp,  scalar, scalar, scalar, kb_float_lerp);
  FUNC3_DEF_N(lerp,  float2, float2, scalar, kb_float2_lerp);
  FUNC3_DEF_N(lerp,  float3, float3, scalar, kb_float3_lerp);
  FUNC3_DEF_N(lerp,  float4, float4, scalar, kb_float4_lerp);

  FUNC2_DEF_N(cross, float3,   float3,    kb_float3_cross);
};

TYPE_OPERATORS(kb::float2, kb::scalar);
TYPE_OPERATORS(kb::float3, kb::scalar);
TYPE_OPERATORS(kb::float4, kb::scalar);
TYPE_OPERATORS(kb::quat,   kb::scalar);
TYPE_OPERATORS(kb::int2,   kb::integer);
TYPE_OPERATORS(kb::int3,   kb::integer);
TYPE_OPERATORS(kb::int4,   kb::integer);

FUNC2_DEF_N(operator*, kb::quat,      kb::float3,   kb_quat_act);
FUNC2_DEF_N(operator*, kb::float4x4,  kb::float4,   kb_float4x4_act);
FUNC2_DEF_N(operator*, kb::float4x4,  kb::float4x4, kb_float4x4_mul);

#endif
