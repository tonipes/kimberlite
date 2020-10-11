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

#define MAX(_a, _b) _a > _b ? _a : _b
#define MIN(_a, _b) _a < _b ? _a : _b
#define CLAMP(_t, _min, _max) _t > _max ? _max : (_t < _min ? _min : _t)

#define VECTOR_TYPE_2(name, type)               \
typedef union {                                 \
  struct { type x,     y;      };               \
  struct { type width, height; };               \
  struct { type u,     v;      };               \
  type m[2];                                    \
} name;

#define VECTOR_TYPE_3(name, type)               \
typedef union {                                 \
  struct { type x,     y,       z;      };      \
  struct { type r,     g,       b;      };      \
  struct { type width, height,  depth;  };      \
  struct { type u,     v,       w;      };      \
  type m[3];                                    \
} name;

#define VECTOR_TYPE_4(name, type)               \
typedef union {                                 \
  struct { type x,     y,       z,    w;  };    \
  struct { type r,     g,       b,    a;  };    \
  type m[4];                                    \
} name;

typedef float     Real32;
typedef double    Real64;
typedef bool      Bool;

typedef int8_t    Int8;
typedef int16_t   Int16;
typedef int32_t   Int32;
typedef int64_t   Int64;

typedef uint8_t   UInt8;
typedef uint16_t  UInt16;
typedef uint32_t  UInt32;
typedef uint64_t  UInt64;

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

typedef enum Axis {
  AxisX, AxisY, AxisZ
} Axis;

VECTOR_TYPE_2(Int2, Int32)
VECTOR_TYPE_3(Int3, Int32)
VECTOR_TYPE_4(Int4, Int32)

VECTOR_TYPE_2(Float2, Real32)
VECTOR_TYPE_3(Float3, Real32)
VECTOR_TYPE_4(Float4, Real32)

typedef struct Quaternion {
  Real32 x, y, z, w;
} Quaternion;

typedef struct Float3x3 {
  union {
    Real32 m[9];
    Real32 mm[3][3];
    struct {
      Real32 m00, m01, m02;
      Real32 m10, m11, m12;
      Real32 m20, m21, m22;
    };
  };
} Float3x3;

typedef struct Float4x4 {
  union {
    Real32 m[16];
    Real32 mm[4][4];
    Float4 cols[4];
    struct {
      Real32 m00, m01, m02, m03;
      Real32 m10, m11, m12, m13;
      Real32 m20, m21, m22, m23;
      Real32 m30, m31, m32, m33;
    };
  };
} Float4x4;

typedef struct Aabb {
  Float3 min;
  Float3 max;
} Aabb;

typedef struct Plane {
  Float3 normal;
  Real32 dist;
} Plane;

typedef struct FloatRect {
  Float2 from;
  Float2 to;
} FloatRect;

typedef struct IntRect {
  Int2 from;
  Int2 size;
} IntRect;

typedef struct Sphere {
  Float3 center;
  Real32 radius;
} Sphere;

typedef struct Capsule {
  Float3 pos;
  Float3 end;
  Real32 radius;
} Capsule;

typedef struct Cone {
  Float3 pos;
  Float3 end;
  Real32 radius;
} Cone;

typedef struct Cylinder {
  Float3 pos;
  Float3 end;
  Real32 radius;
} Cylinder;

typedef struct Disk {
  Float3 center;
  Float3 normal;
  Real32 radius;
} Disk;

typedef struct Triangle {
  Float3 v0;
  Float3 v1;
  Float3 v2;
} Triangle;

typedef struct Ray {
  Float3 pos;
  Float3 dir;
} Ray;

typedef struct Hit {
  Float3  pos;
  Plane   plane;
} Hit;

typedef struct XForm {
  Float3      position;
  Float3      scale;
  Quaternion  rotation;
} XForm;

const extern Float3 DirRight;
const extern Float3 DirUp;
const extern Float3 DirForward;

const extern Float4x4 IdentityFloat4x4;

KB_API_INLINE Int32     neg_integer       (Int32 a)                                           { return -a;                    }
KB_API_INLINE Int32     add_integer       (Int32 a, Int32 b)                                  { return a + b;                 }
KB_API_INLINE Int32     sub_integer       (Int32 a, Int32 b)                                  { return a - b;                 }
KB_API_INLINE Int32     mul_integer       (Int32 a, Int32 b)                                  { return a * b;                 }
KB_API_INLINE Bool      equal_integer     (Int32 a, Int32 b)                                  { return a == b;                }
KB_API_INLINE Int32     abs_integer       (Int32 a)                                           { return a < 0 ? -a : a;        }
KB_API_INLINE Int32     max_integer       (Int32 a, Int32 b)                                  { return a > b ? a : b;         }
KB_API_INLINE Int32     min_integer       (Int32 a, Int32 b)                                  { return b > a ? a : b;         }
KB_API_INLINE Int32     log2_integer      (Int32 a)                                           { return log2(a);               }

KB_API_INLINE Real32    nms_scalar        (Real32 a, Real32 b, Real32 c)                      { return c - a * b;             }
KB_API_INLINE Real32    mad_scalar        (Real32 a, Real32 b, Real32 c)                      { return a * b + c;             }
KB_API_INLINE Real32    neg_scalar        (Real32 a)                                          { return -a;                    }
KB_API_INLINE Real32    add_scalar        (Real32 a, Real32 b)                                { return a + b;                 }
KB_API_INLINE Real32    sub_scalar        (Real32 a, Real32 b)                                { return a - b;                 }
KB_API_INLINE Real32    mul_scalar        (Real32 a, Real32 b)                                { return a * b;                 }
KB_API_INLINE Real32    dot_scalar        (Real32 a, Real32 b)                                { return a * b;                 }

KB_API_INLINE Real32    sin_scalar        (Real32 f)                                          { return sin(f);                }
KB_API_INLINE Real32    cos_scalar        (Real32 f)                                          { return cos(f);                }
KB_API_INLINE Real32    tan_scalar        (Real32 f)                                          { return tan(f);                }
KB_API_INLINE Real32    sinh_scalar       (Real32 f)                                          { return sinh(f);               }
KB_API_INLINE Real32    asin_scalar       (Real32 f)                                          { return asin(f);               }
KB_API_INLINE Real32    acos_scalar       (Real32 f)                                          { return acos(f);               }
KB_API_INLINE Real32    atan_scalar       (Real32 f)                                          { return atan(f);               }
KB_API_INLINE Real32    atan2_scalar      (Real32 a, Real32 b)                                { return atan2(a, b);           }
KB_API_INLINE Real32    cosh_scalar       (Real32 f)                                          { return cosh(f);               }

KB_API_INLINE Real32    select_scalar     (Real32 a, Real32 b, Bool s)                        { return s ? b : a; }
KB_API_INLINE Real32    step_scalar       (Real32 a, Real32 b)                                { return select_scalar(0.0f, 1.0f, a >= b); }
KB_API_INLINE Real32    sqrt_scalar       (Real32 f)                                          { return sqrt(f);               }
KB_API_INLINE Real32    floor_scalar      (Real32 f)                                          { return floor(f);              }
KB_API_INLINE Real32    frac_scalar       (Real32 f)                                          { return f - floor_scalar(f);   }
KB_API_INLINE Real32    ceil_scalar       (Real32 f)                                          { return ceil(f);               }
KB_API_INLINE Real32    round_scalar      (Real32 f)                                          { return round(f);              }
KB_API_INLINE Real32    trunc_scalar      (Real32 f)                                          { return (Real32) (Int32) f; }
KB_API_INLINE Real32    lerp_scalar       (Real32 a, Real32 b, Real32 c)                      { return a + c * (b - a);       }
KB_API_INLINE Real32    lerp_inv_scalar   (Real32 a, Real32 b, Real32 c)                      { return (c - a) / (b - a);     }
KB_API_INLINE Real32    sign_scalar       (Real32 a)                                          { return (a > 0.0f ? 1.0f : 0.0f) - (a < 0.0f ? 1.0f : 0.0f); }
KB_API_INLINE Real32    abs_scalar        (Real32 a)                                          { return a < 0.0f ? -a : a;     }
KB_API_INLINE Real32    max_scalar        (Real32 a, Real32 b)                                { return a > b ? a : b;         }
KB_API_INLINE Real32    min_scalar        (Real32 a, Real32 b)                                { return b > a ? a : b;         }
KB_API_INLINE Real32    dist_scalar       (Real32 a, Real32 b)                                { return abs_scalar(a - b);     }  
KB_API_INLINE Real32    pow_scalar        (Real32 a, Real32 b)                                { return pow(a, b);             }
KB_API_INLINE Real32    exp_scalar        (Real32 a)                                          { return exp(a);                }
KB_API_INLINE Real32    log_scalar        (Real32 a)                                          { return log(a);                }
KB_API_INLINE Real32    log2_scalar       (Real32 a)                                          { return log2(a);               }
KB_API_INLINE Real32    square_scalar     (Real32 a)                                          { return a * a;                 }
KB_API_INLINE Real32    recip_scalar      (Real32 a)                                          { return 1.0f / a;              }
KB_API_INLINE Real32    fmod_scalar       (Real32 a, Real32 b)                                { return fmod(a, b);            }
KB_API_INLINE Real32    mod_scalar        (Real32 a, Real32 b)                                { return a - b * floor_scalar(a / b);           }
KB_API_INLINE Real32    bias_scalar       (Real32 t, Real32 bias)                             { return t / ( ( (1.0f/bias - 2.0f)*(1.0f - t) ) + 1.0f); }
KB_API_INLINE Real32    gain_scalar       (Real32 t, Real32 gain)                             { return t < 0.5f ? bias_scalar(t * 2.0f, gain) * 0.5f : bias_scalar(t * 2.0f - 1.0f, 1.0f - gain) * 0.5f + 0.5f; }
KB_API_INLINE Real32    smoothstep_scalar (Real32 a)                                          { return square_scalar(a) * (3.0f - 2.0f * a); }
KB_API_INLINE Real32    clamp_scalar      (Real32 v, Real32 min, Real32 max)                  { return max_scalar(min_scalar(v, max), min); }
KB_API_INLINE Real32    remap_scalar      (Real32 x, Real32 a, Real32 b, Real32 c, Real32 d)  { return lerp_scalar(c, d, lerp_inv_scalar(a, b, x)); }
KB_API_INLINE Real32    saturate_scalar   (Real32 a)                                          { return clamp_scalar(a, 0.0f, 1.0f); }
KB_API_INLINE Real32    wrap_scalar       (Real32 v, Real32 wrap)                             { const Real32 tmp = mod_scalar(v, wrap); return tmp < 0.0f ? wrap + tmp : tmp; }
KB_API_INLINE Bool      equal_scalar      (Real32 a, Real32 b)                                { return abs_scalar(a - b) < FLT_EPSILON * max_scalar(max_scalar(1.0f, abs_scalar(a)), abs_scalar(b)); }

KB_API_INLINE Int2      add_int2          (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(add_integer);           }
KB_API_INLINE Int2      sub_int2          (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(sub_integer);           }
KB_API_INLINE Int2      mul_int2          (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(mul_integer);           }
KB_API_INLINE Int2      min_int2          (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(min_integer);           }
KB_API_INLINE Int2      max_int2          (const Int2 a, const Int2 b)                        { return (Int2) ELEMWISE_OP2_T2(max_integer);           }
KB_API_INLINE Int32     dot_int2          (const Int2 a, const Int2 b)                        { return ELEMWISE_COMB_OP2_T2(mul_integer, +);          }
KB_API_INLINE Bool      equal_int2        (const Int2 a, const Int2 b)                        { return ELEMWISE_COMB_OP2_T2(equal_integer, &&);       }
KB_API_INLINE Int2      scale_int2        (const Int2 a, const Int32 f)                       { return (Int2) ELEMWISE_OP1_F_T2(mul_integer, f);      }
KB_API_INLINE Int2      neg_int2          (const Int2 a)                                      { return (Int2) ELEMWISE_OP1_T2(neg_integer);           }
KB_API_INLINE Int2      abs_int2          (const Int2 a)                                      { return (Int2) ELEMWISE_OP1_T2(abs_integer);           }
KB_API_INLINE Real32    len_int2          (const Int2 a)                                      { return sqrt(dot_int2(a, a));                          }

KB_API_INLINE Int3      add_int3          (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(add_integer);           }
KB_API_INLINE Int3      sub_int3          (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(sub_integer);           }
KB_API_INLINE Int3      mul_int3          (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(mul_integer);           }
KB_API_INLINE Int3      min_int3          (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(min_integer);           }
KB_API_INLINE Int3      max_int3          (const Int3 a, const Int3 b)                        { return (Int3) ELEMWISE_OP2_T3(max_integer);           }
KB_API_INLINE Int32     dot_int3          (const Int3 a, const Int3 b)                        { return ELEMWISE_COMB_OP2_T3(mul_integer, +);          }
KB_API_INLINE Bool      equal_int3        (const Int3 a, const Int3 b)                        { return ELEMWISE_COMB_OP2_T3(equal_integer, &&);       }
KB_API_INLINE Int3      scale_int3        (const Int3 a, const Int32 f)                       { return (Int3) ELEMWISE_OP1_F_T3(mul_integer, f);      }
KB_API_INLINE Int3      neg_int3          (const Int3 a)                                      { return (Int3) ELEMWISE_OP1_T3(neg_integer);           }
KB_API_INLINE Int3      abs_int3          (const Int3 a)                                      { return (Int3) ELEMWISE_OP1_T3(abs_integer);           }
KB_API_INLINE Real32    len_int3          (const Int3 a)                                      { return sqrt(dot_int3(a, a));                          }

KB_API_INLINE Int4      add_int4          (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(add_integer);           }
KB_API_INLINE Int4      sub_int4          (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(sub_integer);           }
KB_API_INLINE Int4      mul_int4          (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(mul_integer);           }
KB_API_INLINE Int4      min_int4          (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(min_integer);           }
KB_API_INLINE Int4      max_int4          (const Int4 a, const Int4 b)                        { return (Int4) ELEMWISE_OP2_T4(max_integer);           }
KB_API_INLINE Int32     dot_int4          (const Int4 a, const Int4 b)                        { return ELEMWISE_COMB_OP2_T4(mul_integer, +);          }
KB_API_INLINE Bool      equal_int4        (const Int4 a, const Int4 b)                        { return ELEMWISE_COMB_OP2_T4(equal_integer, &&);       }
KB_API_INLINE Int4      scale_int4        (const Int4 a, const Int32 f)                       { return (Int4) ELEMWISE_OP1_F_T4(mul_integer, f);      }
KB_API_INLINE Int4      neg_int4          (const Int4 a)                                      { return (Int4) ELEMWISE_OP1_T4(neg_integer);           }
KB_API_INLINE Int4      abs_int4          (const Int4 a)                                      { return (Int4) ELEMWISE_OP1_T4(abs_integer);           }
KB_API_INLINE Real32    len_int4          (const Int4 a)                                      { return sqrt(dot_int4(a, a));                          }

KB_API_INLINE Float2    abs_float2        (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(abs_scalar);          }
KB_API_INLINE Float2    neg_float2        (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(neg_scalar);          }
KB_API_INLINE Float2    floor_float2      (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(floor_scalar);        }
KB_API_INLINE Float2    add_float2        (const Float2 a, const Float2 b)                    { return (Float2) ELEMWISE_OP2_T2(add_scalar);          }
KB_API_INLINE Float2    sub_float2        (const Float2 a, const Float2 b)                    { return (Float2) ELEMWISE_OP2_T2(sub_scalar);          }
KB_API_INLINE Float2    mul_float2        (const Float2 a, const Float2 b)                    { return (Float2) ELEMWISE_OP2_T2(mul_scalar);          }
KB_API_INLINE Float2    min_float2        (const Float2 a, const Float2 b)                    { return (Float2) ELEMWISE_OP2_T2(min_scalar);          }
KB_API_INLINE Float2    max_float2        (const Float2 a, const Float2 b)                    { return (Float2) ELEMWISE_OP2_T2(max_scalar);          }
KB_API_INLINE Float2    scale_float2      (const Float2 a, const Real32 f)                    { return (Float2) ELEMWISE_OP1_F_T2(mul_scalar, f);     }
KB_API_INLINE Float2    recip_float2      (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(recip_scalar);        }
KB_API_INLINE Float2    lerp_float2       (const Float2 a, const Float2 b, const Real32 v)    { return (Float2) ELEMWISE_OP2_F_T2(lerp_scalar, v);    }
KB_API_INLINE Float2    saturate_float2   (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(saturate_scalar);     }
KB_API_INLINE Float2    smoothstep_float2 (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(smoothstep_scalar);   }
KB_API_INLINE Float2    square_float2     (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(square_scalar);       }
KB_API_INLINE Float2    exp_float2        (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(exp_scalar);          }
KB_API_INLINE Float2    log_float2        (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(log_scalar);          }
KB_API_INLINE Float2    sign_float2       (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(sign_scalar);         }
KB_API_INLINE Float2    sqrt_float2       (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(sqrt_scalar);         }
KB_API_INLINE Float2    frac_float2       (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(frac_scalar);         }
KB_API_INLINE Float2    ceil_float2       (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(ceil_scalar);         }
KB_API_INLINE Float2    round_float2      (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(round_scalar);        }
KB_API_INLINE Float2    trunc_float2      (const Float2 a)                                    { return (Float2) ELEMWISE_OP1_T2(trunc_scalar);        }
KB_API_INLINE Real32    dot_float2        (const Float2 a, const Float2 b)                    { return ELEMWISE_COMB_OP2_T2(mul_scalar, +);           }
KB_API_INLINE Bool      equal_float2      (const Float2 a, const Float2 b)                    { return ELEMWISE_COMB_OP2_T2(equal_scalar, &&);        }
KB_API_INLINE Real32    len_float2        (const Float2 a)                                    { return sqrt(dot_float2(a, a));                        }
KB_API_INLINE Float2    norm_float2       (const Float2 a)                                    { return scale_float2(a, recip_scalar(len_float2(a)));  }
KB_API_INLINE Float2    div_float2        (const Float2 a, const Float2 b)                    { return mul_float2(a, recip_float2(b));                }
KB_API_INLINE Real32    dist_float2       (const Float2 a, const Float2 b)                    { return len_float2(sub_float2(b, a));                  }

KB_API_INLINE Float3    abs_float3        (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(abs_scalar);          }
KB_API_INLINE Float3    neg_float3        (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(neg_scalar);          }
KB_API_INLINE Float3    floor_float3      (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(floor_scalar);        }
KB_API_INLINE Float3    add_float3        (const Float3 a, const Float3 b)                    { return (Float3) ELEMWISE_OP2_T3(add_scalar);          }
KB_API_INLINE Float3    sub_float3        (const Float3 a, const Float3 b)                    { return (Float3) ELEMWISE_OP2_T3(sub_scalar);          }
KB_API_INLINE Float3    mul_float3        (const Float3 a, const Float3 b)                    { return (Float3) ELEMWISE_OP2_T3(mul_scalar);          }
KB_API_INLINE Float3    min_float3        (const Float3 a, const Float3 b)                    { return (Float3) ELEMWISE_OP2_T3(min_scalar);          }
KB_API_INLINE Float3    max_float3        (const Float3 a, const Float3 b)                    { return (Float3) ELEMWISE_OP2_T3(max_scalar);          }
KB_API_INLINE Float3    scale_float3      (const Float3 a, const Real32 f)                    { return (Float3) ELEMWISE_OP1_F_T3(mul_scalar, f);     }
KB_API_INLINE Float3    recip_float3      (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(recip_scalar);        }
KB_API_INLINE Float3    lerp_float3       (const Float3 a, const Float3 b, const Real32 v)    { return (Float3) ELEMWISE_OP2_F_T3(lerp_scalar, v);    }
KB_API_INLINE Float3    saturate_float3   (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(saturate_scalar);     }
KB_API_INLINE Float3    smoothstep_float3 (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(smoothstep_scalar);   }
KB_API_INLINE Float3    square_float3     (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(square_scalar);       }
KB_API_INLINE Float3    exp_float3        (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(exp_scalar);          }
KB_API_INLINE Float3    log_float3        (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(log_scalar);          }
KB_API_INLINE Float3    sign_float3       (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(sign_scalar);         }
KB_API_INLINE Float3    sqrt_float3       (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(sqrt_scalar);         }
KB_API_INLINE Float3    frac_float3       (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(frac_scalar);         }
KB_API_INLINE Float3    ceil_float3       (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(ceil_scalar);         }
KB_API_INLINE Float3    round_float3      (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(round_scalar);        }
KB_API_INLINE Float3    trunc_float3      (const Float3 a)                                    { return (Float3) ELEMWISE_OP1_T3(trunc_scalar);        }
KB_API_INLINE Real32    dot_float3        (const Float3 a, const Float3 b)                    { return ELEMWISE_COMB_OP2_T3(mul_scalar, +);           }
KB_API_INLINE Bool      equal_float3      (const Float3 a, const Float3 b)                    { return ELEMWISE_COMB_OP2_T3(equal_scalar, &&);        }
KB_API_INLINE Real32    len_float3        (const Float3 a)                                    { return sqrt(dot_float3(a, a));                        }
KB_API_INLINE Float3    norm_float3       (const Float3 a)                                    { return scale_float3(a, recip_scalar(len_float3(a)));  }
KB_API_INLINE Float3    div_float3        (const Float3 a, const Float3 b)                    { return mul_float3(a, recip_float3(b));                }
KB_API_INLINE Real32    dist_float3       (const Float3 a, const Float3 b)                    { return len_float3(sub_float3(b, a));                  }

KB_API_INLINE Float4    abs_float4        (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(abs_scalar); }
KB_API_INLINE Float4    neg_float4        (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(neg_scalar); }
KB_API_INLINE Float4    floor_float4      (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(floor_scalar); }
KB_API_INLINE Float4    add_float4        (const Float4 a, const Float4 b)                    { return (Float4) ELEMWISE_OP2_T4(add_scalar); }
KB_API_INLINE Float4    sub_float4        (const Float4 a, const Float4 b)                    { return (Float4) ELEMWISE_OP2_T4(sub_scalar); }
KB_API_INLINE Float4    mul_float4        (const Float4 a, const Float4 b)                    { return (Float4) ELEMWISE_OP2_T4(mul_scalar); }
KB_API_INLINE Float4    min_float4        (const Float4 a, const Float4 b)                    { return (Float4) ELEMWISE_OP2_T4(min_scalar); }
KB_API_INLINE Float4    max_float4        (const Float4 a, const Float4 b)                    { return (Float4) ELEMWISE_OP2_T4(max_scalar); }
KB_API_INLINE Float4    scale_float4      (const Float4 a, const Real32 f)                    { return (Float4) ELEMWISE_OP1_F_T4(mul_scalar, f); }
KB_API_INLINE Float4    recip_float4      (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(recip_scalar); }
KB_API_INLINE Float4    lerp_float4       (const Float4 a, const Float4 b, const Real32 v)    { return (Float4) ELEMWISE_OP2_F_T4(lerp_scalar, v); }
KB_API_INLINE Float4    saturate_float4   (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(saturate_scalar); }
KB_API_INLINE Float4    smoothstep_float4 (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(smoothstep_scalar); }
KB_API_INLINE Float4    square_float4     (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(square_scalar); }
KB_API_INLINE Float4    exp_float4        (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(exp_scalar); }
KB_API_INLINE Float4    log_float4        (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(log_scalar); }
KB_API_INLINE Float4    sign_float4       (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(sign_scalar); }
KB_API_INLINE Float4    sqrt_float4       (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(sqrt_scalar); }
KB_API_INLINE Float4    frac_float4       (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(frac_scalar); }
KB_API_INLINE Float4    ceil_float4       (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(ceil_scalar); }
KB_API_INLINE Float4    round_float4      (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(round_scalar); }
KB_API_INLINE Float4    trunc_float4      (const Float4 a)                                    { return (Float4) ELEMWISE_OP1_T4(trunc_scalar); }
KB_API_INLINE Real32    dot_float4        (const Float4 a, const Float4 b)                    { return ELEMWISE_COMB_OP2_T4(mul_scalar, +); }
KB_API_INLINE Bool      equal_float4      (const Float4 a, const Float4 b)                    { return ELEMWISE_COMB_OP2_T4(equal_scalar, &&); }
KB_API_INLINE Real32    len_float4        (const Float4 a)                                    { return sqrt(dot_float4(a, a)); }
KB_API_INLINE Float4    norm_float4       (const Float4 a)                                    { return scale_float4(a, recip_scalar(len_float4(a))); }
KB_API_INLINE Float4    div_float4        (const Float4 a, const Float4 b)                    { return mul_float4(a, recip_float4(b)); }
KB_API_INLINE Real32    dist_float4       (const Float4 a, const Float4 b)                    { return len_float4(sub_float4(b, a)); }

KB_API_INLINE Quaternion  norm_quat       (const Quaternion a);
KB_API_INLINE Quaternion  neg_quat        (const Quaternion a)                                      { return (Quaternion) ELEMWISE_OP1_T4(neg_scalar); }
KB_API_INLINE Quaternion  add_quat        (const Quaternion a, const Quaternion b)                  { return (Quaternion) ELEMWISE_OP2_T4(add_scalar); }
KB_API_INLINE Quaternion  sub_quat        (const Quaternion a, const Quaternion b)                  { return (Quaternion) ELEMWISE_OP2_T4(sub_scalar); }
KB_API_INLINE Quaternion  scale_quat      (const Quaternion a, const Real32 f)                      { return (Quaternion) ELEMWISE_OP1_F_T4(mul_scalar, f); }
KB_API_INLINE Quaternion  lerp_quat       (const Quaternion a, const Quaternion b, const Real32 v)  { return norm_quat((Quaternion) ELEMWISE_OP2_F_T4(lerp_scalar, v)) ; }
KB_API_INLINE Real32      dot_quat        (const Quaternion a, const Quaternion b)                  { return ELEMWISE_COMB_OP2_T4(mul_scalar, +); }
KB_API_INLINE Bool        equal_quat      (const Quaternion a, const Quaternion b)                  { return ELEMWISE_COMB_OP2_T4(equal_scalar, &&); }
KB_API_INLINE Real32      len_quat        (const Quaternion a)                                      { return sqrt(dot_quat(a, a)); }
KB_API_INLINE Quaternion  conj_quat       (const Quaternion a)                                      { return (Quaternion) { -a.x, -a.y, -a.z, +a.w }; }
KB_API_INLINE Quaternion  recip_quat      (const Quaternion a)                                      { return scale_quat(conj_quat(a), 1.0f / dot_quat(a, a)); }

KB_API_INLINE Real32      uint16_to_float (int16_t value)                                           { return remap_scalar(Real32(value), Real32(INT16_MIN), Real32(INT16_MAX), -1.0f, 1.0f); }
KB_API_INLINE bool        kb_activated    (uint8_t* curr, uint8_t* prev, size_t i)                  { return !prev[i] && curr[i]; }
KB_API_INLINE bool        kb_deactivated  (uint8_t* curr, uint8_t* prev, size_t i)                  { return prev[i] && !curr[i]; }

KB_API Float3             act_quat        (const Quaternion a, const Float3 b);
KB_API Quaternion         slerp_quat      (const Quaternion a, const Quaternion b, Real32 t);

KB_API_INLINE Quaternion inv_quat(const Quaternion a) {
  // TODO: Check
  const Quaternion conj = conj_quat(a);
  return scale_quat(conj, 1.0f / len_quat(conj));
}

KB_API_INLINE Quaternion  norm_quat(const Quaternion a) {
  // TODO: Check
  const Real32 norm = dot_quat(a, a);
  if (0.0f < norm) {
    return scale_quat(a, 1.0f / sqrt_scalar(norm));
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
  const Real32 hx = euler.x * 0.5f;
  const Real32 hy = euler.y * 0.5f;
  const Real32 hz = euler.z * 0.5f;
  
  const Quaternion x = { sin_scalar(hx), 0.0f, 0.0f, cos_scalar(hx) };
  const Quaternion y = { 0.0f, sin_scalar(hy), 0.0f, cos_scalar(hy) };
  const Quaternion z = { 0.0f, 0.0f, sin_scalar(hz), cos_scalar(hz) };

  return mul_quat(x, mul_quat(y, z));
}

KB_API_INLINE Quaternion axis_angle_quat(const Float3 axis, Real32 angle) {
  const float half = angle * 0.5f;
  const float s = sin_scalar(half);

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

KB_API Float4       act_float4x4      (const Float4x4 a, const Float4 b);

KB_API Float4x4     xform             (const Float3 pos, const Float3 scale, const Quaternion rot);

KB_API Float4x4     mul_float4x4      (const Float4x4 a, const Float4x4 b);
KB_API Float4x4     inv_float4x4      (const Float4x4 a);
KB_API Float4x4     transpose_float4x4(const Float4x4 a);

KB_API void         translate         (Float4x4* mtx, const Float3 pos);
KB_API void         scale             (Float4x4* mtx, const Float3 scale);
KB_API void         rotate            (Float4x4* mtx, const Quaternion rotation);

KB_API Float4x4     transform_mtx     (const Float3 v);
KB_API Float4x4     scaling_mtx       (const Float3 v);
KB_API Float4x4     rotation_mtx      (const Quaternion v);

KB_API Float4x4     look_at           (const Float3 from, const Float3 to, const Float3 up);
KB_API Float4x4     perspective       (Real32 fov, Real32 aspect, Real32 near, Real32 far);
KB_API Float4x4     orthographic      (Real32 left, Real32 right, Real32 top, Real32 bottom, Real32 near, Real32 far);

KB_API Float3       get_point         (Axis axis, float x, float y);

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

KB_API_INLINE Float2 circle_point(float angle) {
  return { 
    sin_scalar(angle), 
    cos_scalar(angle) 
  };
}

KB_API_INLINE Float2 squircle_point(float angle) {
  float sa = sin_scalar(angle);
  float ca = cos_scalar(angle);

  return {
    sqrt_scalar(abs_scalar(sa)) * sign_scalar(sa),
    sqrt_scalar(abs_scalar(ca)) * sign_scalar(ca)
  };
}

#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus

#define FUNC1_DEF(F, RT, T1, func)          inline RT F(const T1& a) { return func(a); }
#define FUNC2_DEF(F, RT, T1, T2, func)      inline RT F(const T1& a, const T2& b) { return func(a, b); }
#define FUNC3_DEF(F, RT, T1, T2, T3, func)  inline RT F(const T1& a, const T2& b, const T3& c) { return func(a, b, c); }

FUNC1_DEF(kb_log2,        auto, Int32,            log2_integer          );
FUNC1_DEF(kb_log2,        auto, Real32,           log2_scalar           );

FUNC2_DEF(distance,       auto, Real32, Real32,   dist_scalar           );
FUNC2_DEF(equal,          auto, Real32, Real32,   equal_scalar          );
FUNC2_DEF(dot,            auto, Real32, Real32,   dot_scalar            );
FUNC2_DEF(min,            auto, Real32, Real32,   min_scalar            );
FUNC2_DEF(max,            auto, Real32, Real32,   max_scalar            );
FUNC1_DEF(recip,          auto, Real32,           recip_scalar          );
FUNC1_DEF(saturate,       auto, Real32,           saturate_scalar       );
FUNC1_DEF(smoothstep,     auto, Real32,           smoothstep_scalar     );
FUNC1_DEF(square,         auto, Real32,           square_scalar         );
FUNC2_DEF(step,           auto, Real32, Real32,   step_scalar           );

// FUNC1_DEF(abs,         auto, Real32,           abs_scalar            );
// FUNC1_DEF(floor,       auto, Real32,           floor_scalar          );
// FUNC1_DEF(exp,         auto, Real32,           exp_scalar            );
// FUNC1_DEF(log,         auto, Real32,           log_scalar            );
// FUNC1_DEF(sign,        auto, Real32,           sign_scalar           );
// FUNC1_DEF(sqrt,        auto, Real32,           sqrt_scalar           );
// FUNC1_DEF(frac,        auto, Real32,           frac_scalar           );
// FUNC1_DEF(ceil,        auto, Real32,           ceil_scalar           );
// FUNC1_DEF(round,       auto, Real32,           round_scalar          );
// FUNC1_DEF(trunc,       auto, Real32,           trunc_scalar          );

FUNC1_DEF(operator-,      auto, Float2,           neg_float2            );
FUNC2_DEF(operator*,      auto, Float2, Real32,   scale_float2          );
FUNC2_DEF(operator*,      auto, Float2, Float2,   mul_float2            );
FUNC2_DEF(operator+,      auto, Float2, Float2,   add_float2            );
FUNC2_DEF(operator-,      auto, Float2, Float2,   sub_float2            );
FUNC2_DEF(operator==,     auto, Float2, Float2,   equal_float2          );
FUNC2_DEF(distance,       auto, Float2, Float2,   dist_float2           );
FUNC2_DEF(equal,          auto, Float2, Float2,   equal_float2          );
FUNC2_DEF(dot,            auto, Float2, Float2,   dot_float2            );
FUNC2_DEF(min,            auto, Float2, Float2,   min_float2            );
FUNC2_DEF(max,            auto, Float2, Float2,   max_float2            );
FUNC1_DEF(abs,            auto, Float2,           abs_float2            );
FUNC1_DEF(floor,          auto, Float2,           floor_float2          );
FUNC1_DEF(length,         auto, Float2,           len_float2            );
FUNC1_DEF(norm,           auto, Float2,           norm_float2           );
FUNC1_DEF(recip,          auto, Float2,           recip_float2          );
FUNC1_DEF(saturate,       auto, Float2,           saturate_float2       );
FUNC1_DEF(smoothstep,     auto, Float2,           smoothstep_float2     );
FUNC1_DEF(square,         auto, Float2,           square_float2         );
FUNC1_DEF(exp,            auto, Float2,           exp_float2            );
FUNC1_DEF(log,            auto, Float2,           log_float2            );
FUNC1_DEF(sign,           auto, Float2,           sign_float2           );
FUNC1_DEF(sqrt,           auto, Float2,           sqrt_float2           );
FUNC1_DEF(frac,           auto, Float2,           frac_float2           );
FUNC1_DEF(ceil,           auto, Float2,           ceil_float2           );
FUNC1_DEF(round,          auto, Float2,           round_float2          );
FUNC1_DEF(trunc,          auto, Float2,           trunc_float2          );

FUNC1_DEF(operator-,      auto, Float3,           neg_float3          );
FUNC2_DEF(operator*,      auto, Float3, Real32,   scale_float3        );
FUNC2_DEF(operator*,      auto, Float3, Float3,   mul_float3          );
FUNC2_DEF(operator+,      auto, Float3, Float3,   add_float3          );
FUNC2_DEF(operator-,      auto, Float3, Float3,   sub_float3          );
FUNC2_DEF(operator==,     auto, Float3, Float3,   equal_float3        );
FUNC2_DEF(distance,       auto, Float3, Float3,   dist_float3         );
FUNC2_DEF(equal,          auto, Float3, Float3,   equal_float3        );
FUNC2_DEF(dot,            auto, Float3, Float3,   dot_float3          );
FUNC2_DEF(min,            auto, Float3, Float3,   min_float3          );
FUNC2_DEF(max,            auto, Float3, Float3,   max_float3          );
FUNC2_DEF(cross,          auto, Float3, Float3,   cross_float3        );
FUNC1_DEF(abs,            auto, Float3,           abs_float3          );
FUNC1_DEF(floor,          auto, Float3,           floor_float3        );
FUNC1_DEF(length,         auto, Float3,           len_float3          );
FUNC1_DEF(norm,           auto, Float3,           norm_float3         );
FUNC1_DEF(recip,          auto, Float3,           recip_float3        );
FUNC1_DEF(saturate,       auto, Float3,           saturate_float3     );
FUNC1_DEF(smoothstep,     auto, Float3,           smoothstep_float3   );
FUNC1_DEF(square,         auto, Float3,           square_float3       );
FUNC1_DEF(exp,            auto, Float3,           exp_float3          );
FUNC1_DEF(log,            auto, Float3,           log_float3          );
FUNC1_DEF(sign,           auto, Float3,           sign_float3         );
FUNC1_DEF(sqrt,           auto, Float3,           sqrt_float3         );
FUNC1_DEF(frac,           auto, Float3,           frac_float3         );
FUNC1_DEF(ceil,           auto, Float3,           ceil_float3         );
FUNC1_DEF(round,          auto, Float3,           round_float3        );
FUNC1_DEF(trunc,          auto, Float3,           trunc_float3        );

FUNC1_DEF(operator-,      auto, Float4,           neg_float4          );
FUNC2_DEF(operator*,      auto, Float4, Real32,   scale_float4        );
FUNC2_DEF(operator*,      auto, Float4, Float4,   mul_float4          );
FUNC2_DEF(operator+,      auto, Float4, Float4,   add_float4          );
FUNC2_DEF(operator-,      auto, Float4, Float4,   sub_float4          );
FUNC2_DEF(operator==,     auto, Float4, Float4,   equal_float4        );
FUNC2_DEF(distance,       auto, Float4, Float4,   dist_float4         );
FUNC2_DEF(equal,          auto, Float4, Float4,   equal_float4        );
FUNC2_DEF(dot,            auto, Float4, Float4,   dot_float4          );
FUNC2_DEF(min,            auto, Float4, Float4,   min_float4          );
FUNC2_DEF(max,            auto, Float4, Float4,   max_float4          );
FUNC1_DEF(abs,            auto, Float4,           abs_float4          );
FUNC1_DEF(floor,          auto, Float4,           floor_float4        );
FUNC1_DEF(length,         auto, Float4,           len_float4          );
FUNC1_DEF(norm,           auto, Float4,           norm_float4         );
FUNC1_DEF(recip,          auto, Float4,           recip_float4        );
FUNC1_DEF(saturate,       auto, Float4,           saturate_float4     );
FUNC1_DEF(smoothstep,     auto, Float4,           smoothstep_float4   );
FUNC1_DEF(square,         auto, Float4,           square_float4       );
FUNC1_DEF(exp,            auto, Float4,           exp_float4          );
FUNC1_DEF(log,            auto, Float4,           log_float4          );
FUNC1_DEF(sign,           auto, Float4,           sign_float4         );
FUNC1_DEF(sqrt,           auto, Float4,           sqrt_float4         );
FUNC1_DEF(frac,           auto, Float4,           frac_float4         );
FUNC1_DEF(ceil,           auto, Float4,           ceil_float4         );
FUNC1_DEF(round,          auto, Float4,           round_float4        );
FUNC1_DEF(trunc,          auto, Float4,           trunc_float4        );

FUNC1_DEF(operator-,      auto, Int2,             neg_int2      );
FUNC2_DEF(operator*,      auto, Int2, Int32,      scale_int2    );
FUNC2_DEF(operator*,      auto, Int2, Int2,       mul_int2      );
FUNC2_DEF(operator+,      auto, Int2, Int2,       add_int2      );
FUNC2_DEF(operator-,      auto, Int2, Int2,       sub_int2      );
FUNC2_DEF(operator==,     auto, Int2, Int2,       equal_int2    );
FUNC2_DEF(equal,          auto, Int2, Int2,       equal_int2    );
FUNC2_DEF(dot,            auto, Int2, Int2,       dot_int2      );
FUNC2_DEF(min,            auto, Int2, Int2,       min_int2      );
FUNC2_DEF(max,            auto, Int2, Int2,       max_int2      );
FUNC1_DEF(abs,            auto, Int2,             abs_int2      );
FUNC1_DEF(length,         auto, Int2,             len_int2      );

FUNC1_DEF(operator-,      auto, Int3,             neg_int3      );
FUNC2_DEF(operator*,      auto, Int3, Int32,      scale_int3    );
FUNC2_DEF(operator*,      auto, Int3, Int3,       mul_int3      );
FUNC2_DEF(operator+,      auto, Int3, Int3,       add_int3      );
FUNC2_DEF(operator-,      auto, Int3, Int3,       sub_int3      );
FUNC2_DEF(operator==,     auto, Int3, Int3,       equal_int3    );
FUNC2_DEF(equal,          auto, Int3, Int3,       equal_int3    );
FUNC2_DEF(dot,            auto, Int3, Int3,       dot_int3      );
FUNC2_DEF(min,            auto, Int3, Int3,       min_int3      );
FUNC2_DEF(max,            auto, Int3, Int3,       max_int3      );
FUNC1_DEF(abs,            auto, Int3,             abs_int3      );
FUNC1_DEF(length,         auto, Int3,             len_int3      );

FUNC1_DEF(operator-,      auto, Int4,             neg_int4      );
FUNC2_DEF(operator*,      auto, Int4, Int32,      scale_int4    );
FUNC2_DEF(operator*,      auto, Int4, Int4,       mul_int4      );
FUNC2_DEF(operator+,      auto, Int4, Int4,       add_int4      );
FUNC2_DEF(operator-,      auto, Int4, Int4,       sub_int4      );
FUNC2_DEF(operator==,     auto, Int4, Int4,       equal_int4    );
FUNC2_DEF(equal,          auto, Int4, Int4,       equal_int4    );
FUNC2_DEF(dot,            auto, Int4, Int4,       dot_int4      );
FUNC2_DEF(min,            auto, Int4, Int4,       min_int4      );
FUNC2_DEF(max,            auto, Int4, Int4,       max_int4      );
FUNC1_DEF(abs,            auto, Int4,             abs_int4      );
FUNC1_DEF(length,         auto, Int4,             len_int4      );

FUNC3_DEF(lerp,           auto, Real32, Real32, Real32, lerp_scalar      );
FUNC3_DEF(lerp,           auto, Float2, Float2, Real32, lerp_float2      );
FUNC3_DEF(lerp,           auto, Float3, Float3, Real32, lerp_float3      );
FUNC3_DEF(lerp,           auto, Float4, Float4, Real32, lerp_float4      );

FUNC2_DEF(operator*,      auto, Float4x4, Float4x4,   mul_float4x4     );
FUNC2_DEF(operator*,      auto, Quaternion, Float3,   act_quat         );
FUNC2_DEF(operator*,      auto, Quaternion, Quaternion,   mul_quat     );

#endif
