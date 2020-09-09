#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "api.h"

#include <math.h>
#include <float.h>

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

typedef float Scalar;
typedef int   Integer;

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

typedef enum {
  AxisX, AxisY, AxisZ
} Axis;

typedef struct {
  Integer x, y;
} Int2;

typedef struct {
  Integer x, y, z;
} Int3;

typedef struct {
  Integer x, y, z, w;
} Int4;

typedef struct {
  Scalar x, y;
} Float2;

typedef struct {
  Scalar x, y, z;
} Float3;

typedef struct {
  Scalar x, y, z, w;
} Float4;

typedef struct {
  Scalar x, y, z, w;
} Quaternion;

typedef struct {
  union {
    Scalar m;
    Scalar mm;
    struct {
      Scalar m00;
    };
  };
} UnionTest;

typedef struct {
  union {
    Scalar m[9];
    Scalar mm[3][3];
    struct {
      Scalar m00, m01, m02;
      Scalar m10, m11, m12;
      Scalar m20, m21, m22;
    };
  };
} Float3x3;

typedef struct {
  union {
    Scalar m[16];
    Scalar mm[4][4];
    Float4 cols[4];
    struct {
      Scalar m00, m01, m02, m03;
      Scalar m10, m11, m12, m13;
      Scalar m20, m21, m22, m23;
      Scalar m30, m31, m32, m33;
    };
  };
} Float4x4;

typedef struct {
  Float3 min;
  Float3 max;
} Aabb;

typedef struct {
  Float3 normal;
  float  dist;
} Plane;

typedef struct {
  Float2 from;
  Float2 to;
} FloatRect;

typedef struct {
  Int2 from;
  Int2 to;
} IntRect;

typedef struct {
  Float3 center;
  float radius;
} Sphere;

typedef struct {
  Float3 pos;
  Float3 end;
  float radius;
} Capsule;

typedef struct {
  Float3 pos;
  Float3 end;
  float radius;
} Cone;

typedef struct {
  Float3 pos;
  Float3 end;
  float radius;
} Cylinder;

typedef struct {
  Float3 center;
  Float3 normal;
  float radius;
} Disk;

typedef struct {
  Float3 v0;
  Float3 v1;
  Float3 v2;
} Triangle;

typedef struct {
  Float3 pos;
  Float3 dir;
} Ray;

typedef struct {
  Float3 pos;
  Plane plane;
} Hit;

typedef struct {
  Float3      position;
  Float3      scale;
  Quaternion  rotation;
} XForm;

const extern Float3 DirRight;
const extern Float3 DirUp;
const extern Float3 DirForward;

const extern Float4x4 IdentityFloat4x4;

KB_API_INLINE Integer   neg_integer       (Integer a)                     { return -a;                    }
KB_API_INLINE Integer   add_integer       (Integer a, Integer b)          { return a + b;                 }
KB_API_INLINE Integer   sub_integer       (Integer a, Integer b)          { return a - b;                 }
KB_API_INLINE Integer   mul_integer       (Integer a, Integer b)          { return a * b;                 }
KB_API_INLINE bool      equal_integer     (Integer a, Integer b)          { return a == b;                }
KB_API_INLINE Integer   abs_integer       (Integer a)                     { return a < 0 ? -a : a;        }
KB_API_INLINE Integer   max_integer       (Integer a, Integer b)          { return a > b ? a : b;         }
KB_API_INLINE Integer   min_integer       (Integer a, Integer b)          { return b > a ? a : b;         }
KB_API_INLINE Integer   log2_integer      (Integer a)                     { return log2(a);               }

KB_API_INLINE Scalar    nms_scalar        (Scalar a, Scalar b, Scalar c)  { return c - a * b; }
KB_API_INLINE Scalar    mad_scalar        (Scalar a, Scalar b, Scalar c)  { return a * b + c; }
KB_API_INLINE Scalar    neg_scalar        (Scalar a)                      { return -a;                    }
KB_API_INLINE Scalar    add_scalar        (Scalar a, Scalar b)            { return a + b;                 }
KB_API_INLINE Scalar    sub_scalar        (Scalar a, Scalar b)            { return a - b;                 }
KB_API_INLINE Scalar    mul_scalar        (Scalar a, Scalar b)            { return a * b;                 }
KB_API_INLINE Scalar    dot_scalar        (Scalar a, Scalar b)            { return a * b;                 }

KB_API_INLINE Scalar    sin_scalar        (Scalar f)                      { return sin(f);                }
KB_API_INLINE Scalar    cos_scalar        (Scalar f)                      { return cos(f);                }
KB_API_INLINE Scalar    tan_scalar        (Scalar f)                      { return tan(f);                }
KB_API_INLINE Scalar    sinh_scalar       (Scalar f)                      { return sinh(f);               }
KB_API_INLINE Scalar    asin_scalar       (Scalar f)                      { return asin(f);               }
KB_API_INLINE Scalar    acos_scalar       (Scalar f)                      { return acos(f);               }
KB_API_INLINE Scalar    atan_scalar       (Scalar f)                      { return atan(f);               }
KB_API_INLINE Scalar    atan2_scalar      (Scalar a, Scalar b)            { return atan2(a, b);           }
KB_API_INLINE Scalar    cosh_scalar       (Scalar f)                      { return cosh(f);               }

KB_API_INLINE Scalar    select_scalar     (Scalar a, Scalar b, bool s)    { return s ? b : a; }
KB_API_INLINE Scalar    step_scalar       (Scalar a, Scalar b)            { return select_scalar(0.0f, 1.0f, a >= b); }
KB_API_INLINE Scalar    sqrt_scalar       (Scalar f)                      { return sqrt(f);               }
KB_API_INLINE Scalar    floor_scalar      (Scalar f)                      { return floor(f);              }
KB_API_INLINE Scalar    frac_scalar       (Scalar f)                      { return f - floor_scalar(f);   }
KB_API_INLINE Scalar    ceil_scalar       (Scalar f)                      { return ceil(f);               }
KB_API_INLINE Scalar    round_scalar      (Scalar f)                      { return round(f);              }
KB_API_INLINE Scalar    trunc_scalar      (Scalar f)                      { return (Scalar) (Integer) f;  }
KB_API_INLINE Scalar    lerp_scalar       (Scalar a, Scalar b, Scalar c)  { return a + c * (b - a);       }
KB_API_INLINE Scalar    lerp_inv_scalar   (Scalar a, Scalar b, Scalar c)  { return (c - a) / (b - a);     }
KB_API_INLINE Scalar    sign_scalar       (Scalar a)                      { return (a > 0.0f ? 1.0f : 0.0f) - (a < 0.0f ? 1.0f : 0.0f); }
KB_API_INLINE Scalar    abs_scalar        (Scalar a)                      { return a < 0.0f ? -a : a;     }
KB_API_INLINE Scalar    max_scalar        (Scalar a, Scalar b)            { return a > b ? a : b;         }
KB_API_INLINE Scalar    min_scalar        (Scalar a, Scalar b)            { return b > a ? a : b;         }
KB_API_INLINE Scalar    dist_scalar       (Scalar a, Scalar b)            { return abs_scalar(a - b);     }  
KB_API_INLINE Scalar    pow_scalar        (Scalar a, Scalar b)            { return pow(a, b);             }
KB_API_INLINE Scalar    exp_scalar        (Scalar a)                      { return exp(a);                }
KB_API_INLINE Scalar    log_scalar        (Scalar a)                      { return log(a);                }
KB_API_INLINE Scalar    log2_scalar       (Scalar a)                      { return log2(a);               }
KB_API_INLINE Scalar    square_scalar     (Scalar a)                      { return a * a;                 }
KB_API_INLINE Scalar    recip_scalar      (Scalar a)                      { return 1.0f / a;              }
KB_API_INLINE Scalar    fmod_scalar       (Scalar a, Scalar b)            { return fmod(a, b);            }
KB_API_INLINE Scalar    mod_scalar        (Scalar a, Scalar b)            { return a - b * floor_scalar(a / b);           }
KB_API_INLINE Scalar    bias_scalar       (Scalar t, Scalar bias)         { return t / ( ( (1.0f/bias - 2.0f)*(1.0f - t) ) + 1.0f); }
KB_API_INLINE Scalar    gain_scalar       (Scalar t, Scalar gain)         { return t < 0.5f ? bias_scalar(t * 2.0f, gain) * 0.5f : bias_scalar(t * 2.0f - 1.0f, 1.0f - gain) * 0.5f + 0.5f; }
KB_API_INLINE Scalar    smoothstep_scalar (Scalar a)                      { return square_scalar(a) * (3.0f - 2.0f * a); }
KB_API_INLINE Scalar    clamp_scalar      (Scalar v, Scalar min, Scalar max) { return max_scalar(min_scalar(v, max), min); }
KB_API_INLINE Scalar    remap_scalar      (Scalar x, Scalar a, Scalar b, Scalar c, Scalar d) { return lerp_scalar(c, d, lerp_inv_scalar(a, b, x)); }
KB_API_INLINE Scalar    saturate_scalar   (Scalar a)                      { return clamp_scalar(a, 0.0f, 1.0f); }

KB_API_INLINE  Scalar wrap_scalar(Scalar v, Scalar wrap) {
  const float tmp = mod_scalar(v, wrap);
  return tmp < 0.0f ? wrap + tmp : tmp;
}

KB_API_INLINE  bool equal_scalar(Scalar a, Scalar b) {
  const float lhs = abs_scalar(a - b);
  const float rhs = FLT_EPSILON * max_scalar(max_scalar(1.0f, abs_scalar(a)), abs_scalar(b));
  return lhs <= rhs;
}

KB_API_INLINE Int2    add_int2        (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(add_integer); }
KB_API_INLINE Int2    sub_int2        (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(sub_integer); }
KB_API_INLINE Int2    mul_int2        (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(mul_integer);}
KB_API_INLINE Int2    min_int2        (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(min_integer);}
KB_API_INLINE Int2    max_int2        (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(max_integer);}
KB_API_INLINE Integer dot_int2        (const Int2 a, const Int2 b)                        { return ELEMWISE_COMB_OP2_T2(mul_integer, +); }
KB_API_INLINE bool    equal_int2      (const Int2 a, const Int2 b)                        { return ELEMWISE_COMB_OP2_T2(equal_integer, &&);}
KB_API_INLINE Int2    scale_int2      (const Int2 a, const Integer f)                     { return (Int2) ELEMWISE_OP1_F_T2(mul_integer, f);}
KB_API_INLINE Int2    neg_int2        (const Int2 a)                                      { return (Int2) ELEMWISE_OP1_T2(neg_integer); }
KB_API_INLINE Int2    abs_int2        (const Int2 a)                                      { return (Int2) ELEMWISE_OP1_T2(abs_integer);}
KB_API_INLINE Scalar  len_int2        (const Int2 a)                                      { return sqrt(dot_int2(a, a));}

KB_API_INLINE Int3    add_int3        (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(add_integer); }
KB_API_INLINE Int3    sub_int3        (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(sub_integer); }
KB_API_INLINE Int3    mul_int3        (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(mul_integer);}
KB_API_INLINE Int3    min_int3        (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(min_integer);}
KB_API_INLINE Int3    max_int3        (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(max_integer);}
KB_API_INLINE Integer dot_int3        (const Int3 a, const Int3 b)                        { return ELEMWISE_COMB_OP2_T3(mul_integer, +); }
KB_API_INLINE bool    equal_int3      (const Int3 a, const Int3 b)                        { return ELEMWISE_COMB_OP2_T3(equal_integer, &&);}
KB_API_INLINE Int3    scale_int3      (const Int3 a, const Integer f)                     { return (Int3) ELEMWISE_OP1_F_T3(mul_integer, f);}
KB_API_INLINE Int3    neg_int3        (const Int3 a)                                      { return (Int3) ELEMWISE_OP1_T3(neg_integer); }
KB_API_INLINE Int3    abs_int3        (const Int3 a)                                      { return (Int3) ELEMWISE_OP1_T3(abs_integer);}
KB_API_INLINE Scalar  len_int3        (const Int3 a)                                      { return sqrt(dot_int3(a, a));}

KB_API_INLINE Int4    add_int4        (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(add_integer); }
KB_API_INLINE Int4    sub_int4        (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(sub_integer); }
KB_API_INLINE Int4    mul_int4        (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(mul_integer);}
KB_API_INLINE Int4    min_int4        (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(min_integer);}
KB_API_INLINE Int4    max_int4        (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(max_integer);}
KB_API_INLINE Integer dot_int4        (const Int4 a, const Int4 b)                        { return ELEMWISE_COMB_OP2_T4(mul_integer, +); }
KB_API_INLINE bool    equal_int4      (const Int4 a, const Int4 b)                        { return ELEMWISE_COMB_OP2_T4(equal_integer, &&);}
KB_API_INLINE Int4    scale_int4      (const Int4 a, const Integer f)                     { return (Int4) ELEMWISE_OP1_F_T4(mul_integer, f);}
KB_API_INLINE Int4    neg_int4        (const Int4 a)                                      { return (Int4) ELEMWISE_OP1_T4(neg_integer); }
KB_API_INLINE Int4    abs_int4        (const Int4 a)                                      { return (Int4) ELEMWISE_OP1_T4(abs_integer);}
KB_API_INLINE Scalar  len_int4        (const Int4 a)                                      { return sqrt(dot_int4(a, a));}

KB_API_INLINE Float2  abs_float2        (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(abs_scalar); }
KB_API_INLINE Float2  neg_float2        (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(neg_scalar); }
KB_API_INLINE Float2  floor_float2      (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(floor_scalar); }
KB_API_INLINE Float2  add_float2        (const Float2 a, const Float2 b)                  { return (Float2) ELEMWISE_OP2_T2(add_scalar); }
KB_API_INLINE Float2  sub_float2        (const Float2 a, const Float2 b)                  { return (Float2) ELEMWISE_OP2_T2(sub_scalar); }
KB_API_INLINE Float2  mul_float2        (const Float2 a, const Float2 b)                  { return (Float2) ELEMWISE_OP2_T2(mul_scalar); }
KB_API_INLINE Float2  min_float2        (const Float2 a, const Float2 b)                  { return (Float2) ELEMWISE_OP2_T2(min_scalar); }
KB_API_INLINE Float2  max_float2        (const Float2 a, const Float2 b)                  { return (Float2) ELEMWISE_OP2_T2(max_scalar); }
KB_API_INLINE Float2  scale_float2      (const Float2 a, const Scalar f)                  { return (Float2) ELEMWISE_OP1_F_T2(mul_scalar, f); }
KB_API_INLINE Float2  recip_float2      (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(recip_scalar); }
KB_API_INLINE Float2  lerp_float2       (const Float2 a, const Float2 b, const Scalar v)  { return (Float2) ELEMWISE_OP2_F_T2(lerp_scalar, v); }
KB_API_INLINE Float2  saturate_float2   (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(saturate_scalar); }
KB_API_INLINE Float2  smoothstep_float2 (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(smoothstep_scalar); }
KB_API_INLINE Float2  square_float2     (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(square_scalar); }
KB_API_INLINE Float2  exp_float2        (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(exp_scalar); }
KB_API_INLINE Float2  log_float2        (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(log_scalar); }
KB_API_INLINE Float2  sign_float2       (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(sign_scalar); }
KB_API_INLINE Float2  sqrt_float2       (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(sqrt_scalar); }
KB_API_INLINE Float2  frac_float2       (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(frac_scalar); }
KB_API_INLINE Float2  ceil_float2       (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(ceil_scalar); }
KB_API_INLINE Float2  round_float2      (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(round_scalar); }
KB_API_INLINE Float2  trunc_float2      (const Float2 a)                                  { return (Float2) ELEMWISE_OP1_T2(trunc_scalar); }
KB_API_INLINE Scalar  dot_float2        (const Float2 a, const Float2 b)                  { return ELEMWISE_COMB_OP2_T2(mul_scalar, +); }
KB_API_INLINE bool    equal_float2      (const Float2 a, const Float2 b)                  { return ELEMWISE_COMB_OP2_T2(equal_scalar, &&); }
KB_API_INLINE Scalar  len_float2        (const Float2 a)                                  { return sqrt(dot_float2(a, a)); }
KB_API_INLINE Float2  norm_float2       (const Float2 a)                                  { return scale_float2(a, recip_scalar(len_float2(a))); }
KB_API_INLINE Float2  div_float2        (const Float2 a, const Float2 b)                  { return mul_float2(a, recip_float2(b)); }
KB_API_INLINE Scalar  dist_float2       (const Float2 a, const Float2 b)                  { return len_float2(sub_float2(b, a)); }


KB_API_INLINE Float3  abs_float3        (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(abs_scalar); }
KB_API_INLINE Float3  neg_float3        (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(neg_scalar); }
KB_API_INLINE Float3  floor_float3      (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(floor_scalar); }
KB_API_INLINE Float3  add_float3        (const Float3 a, const Float3 b)                  { return (Float3) ELEMWISE_OP2_T3(add_scalar); }
KB_API_INLINE Float3  sub_float3        (const Float3 a, const Float3 b)                  { return (Float3) ELEMWISE_OP2_T3(sub_scalar); }
KB_API_INLINE Float3  mul_float3        (const Float3 a, const Float3 b)                  { return (Float3) ELEMWISE_OP2_T3(mul_scalar); }
KB_API_INLINE Float3  min_float3        (const Float3 a, const Float3 b)                  { return (Float3) ELEMWISE_OP2_T3(min_scalar); }
KB_API_INLINE Float3  max_float3        (const Float3 a, const Float3 b)                  { return (Float3) ELEMWISE_OP2_T3(max_scalar); }
KB_API_INLINE Float3  scale_float3      (const Float3 a, const Scalar f)                  { return (Float3) ELEMWISE_OP1_F_T3(mul_scalar, f); }
KB_API_INLINE Float3  recip_float3      (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(recip_scalar); }
KB_API_INLINE Float3  lerp_float3       (const Float3 a, const Float3 b, const Scalar v)  { return (Float3) ELEMWISE_OP2_F_T3(lerp_scalar, v); }
KB_API_INLINE Float3  saturate_float3   (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(saturate_scalar); }
KB_API_INLINE Float3  smoothstep_float3 (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(smoothstep_scalar); }
KB_API_INLINE Float3  square_float3     (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(square_scalar); }
KB_API_INLINE Float3  exp_float3        (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(exp_scalar); }
KB_API_INLINE Float3  log_float3        (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(log_scalar); }
KB_API_INLINE Float3  sign_float3       (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(sign_scalar); }
KB_API_INLINE Float3  sqrt_float3       (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(sqrt_scalar); }
KB_API_INLINE Float3  frac_float3       (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(frac_scalar); }
KB_API_INLINE Float3  ceil_float3       (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(ceil_scalar); }
KB_API_INLINE Float3  round_float3      (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(round_scalar); }
KB_API_INLINE Float3  trunc_float3      (const Float3 a)                                  { return (Float3) ELEMWISE_OP1_T3(trunc_scalar); }
KB_API_INLINE Scalar  dot_float3        (const Float3 a, const Float3 b)                  { return ELEMWISE_COMB_OP2_T3(mul_scalar, +); }
KB_API_INLINE bool    equal_float3      (const Float3 a, const Float3 b)                  { return ELEMWISE_COMB_OP2_T3(equal_scalar, &&); }
KB_API_INLINE Scalar  len_float3        (const Float3 a)                                  { return sqrt(dot_float3(a, a)); }
KB_API_INLINE Float3  norm_float3       (const Float3 a)                                  { return scale_float3(a, recip_scalar(len_float3(a))); }
KB_API_INLINE Float3  div_float3        (const Float3 a, const Float3 b)                  { return mul_float3(a, recip_float3(b)); }
KB_API_INLINE Scalar  dist_float3       (const Float3 a, const Float3 b)                  { return len_float3(sub_float3(b, a)); }

KB_API_INLINE Float4  abs_float4        (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(abs_scalar); }
KB_API_INLINE Float4  neg_float4        (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(neg_scalar); }
KB_API_INLINE Float4  floor_float4      (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(floor_scalar); }
KB_API_INLINE Float4  add_float4        (const Float4 a, const Float4 b)                  { return (Float4) ELEMWISE_OP2_T4(add_scalar); }
KB_API_INLINE Float4  sub_float4        (const Float4 a, const Float4 b)                  { return (Float4) ELEMWISE_OP2_T4(sub_scalar); }
KB_API_INLINE Float4  mul_float4        (const Float4 a, const Float4 b)                  { return (Float4) ELEMWISE_OP2_T4(mul_scalar); }
KB_API_INLINE Float4  min_float4        (const Float4 a, const Float4 b)                  { return (Float4) ELEMWISE_OP2_T4(min_scalar); }
KB_API_INLINE Float4  max_float4        (const Float4 a, const Float4 b)                  { return (Float4) ELEMWISE_OP2_T4(max_scalar); }
KB_API_INLINE Float4  scale_float4      (const Float4 a, const Scalar f)                  { return (Float4) ELEMWISE_OP1_F_T4(mul_scalar, f); }
KB_API_INLINE Float4  recip_float4      (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(recip_scalar); }
KB_API_INLINE Float4  lerp_float4       (const Float4 a, const Float4 b, const Scalar v)  { return (Float4) ELEMWISE_OP2_F_T4(lerp_scalar, v); }
KB_API_INLINE Float4  saturate_float4   (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(saturate_scalar); }
KB_API_INLINE Float4  smoothstep_float4 (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(smoothstep_scalar); }
KB_API_INLINE Float4  square_float4     (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(square_scalar); }
KB_API_INLINE Float4  exp_float4        (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(exp_scalar); }
KB_API_INLINE Float4  log_float4        (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(log_scalar); }
KB_API_INLINE Float4  sign_float4       (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(sign_scalar); }
KB_API_INLINE Float4  sqrt_float4       (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(sqrt_scalar); }
KB_API_INLINE Float4  frac_float4       (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(frac_scalar); }
KB_API_INLINE Float4  ceil_float4       (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(ceil_scalar); }
KB_API_INLINE Float4  round_float4      (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(round_scalar); }
KB_API_INLINE Float4  trunc_float4      (const Float4 a)                                  { return (Float4) ELEMWISE_OP1_T4(trunc_scalar); }
KB_API_INLINE Scalar  dot_float4        (const Float4 a, const Float4 b)                  { return ELEMWISE_COMB_OP2_T4(mul_scalar, +); }
KB_API_INLINE bool    equal_float4      (const Float4 a, const Float4 b)                  { return ELEMWISE_COMB_OP2_T4(equal_scalar, &&); }
KB_API_INLINE Scalar  len_float4        (const Float4 a)                                  { return sqrt(dot_float4(a, a)); }
KB_API_INLINE Float4  norm_float4       (const Float4 a)                                  { return scale_float4(a, recip_scalar(len_float4(a))); }
KB_API_INLINE Float4  div_float4        (const Float4 a, const Float4 b)                  { return mul_float4(a, recip_float4(b)); }
KB_API_INLINE Scalar  dist_float4       (const Float4 a, const Float4 b)                  { return len_float4(sub_float4(b, a)); }

KB_API Float3       act_quat          (const Quaternion a, const Float3 b);
// KB_API Quaternion   euler_quat        (const Float3 euler);
KB_API Quaternion   slerp_quat        (const Quaternion a, const Quaternion b, Scalar t);

KB_API_INLINE Quaternion  norm_quat     (const Quaternion a);

KB_API_INLINE Quaternion  neg_quat      (const Quaternion a)                                      { return (Quaternion) ELEMWISE_OP1_T4(neg_scalar); }
KB_API_INLINE Quaternion  add_quat      (const Quaternion a, const Quaternion b)                  { return (Quaternion) ELEMWISE_OP2_T4(add_scalar); }
KB_API_INLINE Quaternion  sub_quat      (const Quaternion a, const Quaternion b)                  { return (Quaternion) ELEMWISE_OP2_T4(sub_scalar); }
KB_API_INLINE Quaternion  scale_quat    (const Quaternion a, const Scalar f)                      { return (Quaternion) ELEMWISE_OP1_F_T4(mul_scalar, f); }
KB_API_INLINE Quaternion  lerp_quat     (const Quaternion a, const Quaternion b, const Scalar v)  { return norm_quat((Quaternion) ELEMWISE_OP2_F_T4(lerp_scalar, v)) ; }
KB_API_INLINE Scalar      dot_quat      (const Quaternion a, const Quaternion b)                  { return ELEMWISE_COMB_OP2_T4(mul_scalar, +); }
KB_API_INLINE bool        equal_quat    (const Quaternion a, const Quaternion b)                  { return ELEMWISE_COMB_OP2_T4(equal_scalar, &&); }
KB_API_INLINE Scalar      len_quat      (const Quaternion a)                                      { return sqrt(dot_quat(a, a)); }
KB_API_INLINE Quaternion  conj_quat     (const Quaternion a)                                      { return (Quaternion) { -a.x, -a.y, -a.z, +a.w }; }
KB_API_INLINE Quaternion  recip_quat    (const Quaternion a)                                      { return scale_quat(conj_quat(a), 1.0f / dot_quat(a, a)); }


KB_API_INLINE Quaternion inv_quat(const Quaternion a) {
  // TODO: Check
  const Quaternion conj = conj_quat(a);
  return scale_quat(conj, 1.0f / len_quat(conj));
}

KB_API_INLINE Quaternion  norm_quat(const Quaternion a) {
  // TODO: Check
  const float norm = dot_quat(a, a);
  if (0.0f < norm) {
    return scale_quat(a, 1.0f / sqrt(norm));
  }
  return (Quaternion) { 0.0f, 0.0f, 0.0f, 1.0f};
}

KB_API_INLINE  Quaternion mul_quat(const Quaternion a, const Quaternion b) {
  return (Quaternion) {
    a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
    a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
    a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
    a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
  };
}

KB_API_INLINE Quaternion euler_quat(const Float3 euler) {
  const float hx = euler.x * 0.5f;
  const float hy = euler.y * 0.5f;
  const float hz = euler.z * 0.5f;
  
  const Quaternion x = { sin(hx), 0.0f, 0.0f, cos(hx) };
  const Quaternion y = { 0.0f, sin(hy), 0.0f, cos(hy) };
  const Quaternion z = { 0.0f, 0.0f, sin(hz), cos(hz) };

  return mul_quat(x, mul_quat(y, z));
}

KB_API_INLINE Quaternion axis_angle_quat(const Float3 axis, Scalar angle) {
  const float half = angle * 0.5f;
  const float s = sin(half);

  return (Quaternion) {
    axis.x * s,
    axis.y * s,
    axis.z * s,
    cos(half),
  };
}

KB_API_INLINE Float3  cross_float3    (const Float3 a, const Float3 b) {
  return (Float3) { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

KB_API Float4       act_float4x4    (const Float4x4 a, const Float4 b);

KB_API Float4x4     xform           (const Float3 pos, const Float3 scale, const Quaternion rot);

KB_API Float4x4     mul_float4x4    (const Float4x4 a, const Float4x4 b);
KB_API Float4x4     inv_float4x4    (const Float4x4 a);
KB_API Float4x4     transpose_float4x4(const Float4x4 a);

KB_API void         translate       (Float4x4* mtx, const Float3 pos);
KB_API void         scale           (Float4x4* mtx, const Float3 scale);
KB_API void         rotate          (Float4x4* mtx, const Quaternion rotation);

KB_API Float4x4     transform_mtx   (const Float3 v);
KB_API Float4x4     scaling_mtx     (const Float3 v);
KB_API Float4x4     rotation_mtx    (const Quaternion v);

KB_API Float4x4     look_at         (const Float3 from, const Float3 to, const Float3 up);
KB_API Float4x4     perspective     (Scalar fov, Scalar aspect, Scalar near, Scalar far);
KB_API Float4x4     orthographic    (Scalar left, Scalar right, Scalar top, Scalar bottom, Scalar near, Scalar far);

KB_API Float3       get_point       (Axis axis, float x, float y);

KB_API void         tangent_frame(const Float3 normal, Float3* tangent, Float3* bitangent);
KB_API void         tangent_frame_with_spin(const Float3 normal, float angle, Float3* tangent, Float3* bitangent);

KB_API uint32_t     float_rgba_to_uint(const Float4 color);

KB_API float        ray_plane_intersection(const Ray ray, const Plane plane);

KB_API_INLINE float deg_to_rad(float deg) { return deg * PI / 180.0f; }
KB_API_INLINE float rad_to_deg(float rad) { return rad * 180.0f / PI; }

KB_API_INLINE uint32_t align_up(uint32_t a, uint32_t align) {
  const uint32_t mask = uint32_t(align - 1);
  return uint32_t( (a + mask) & ~mask);
}

#ifdef __cplusplus
}
#endif
