#pragma once

#include "math.h"

#define FUNC1_DEF(F, RT, T1, func)          inline RT F(const T1& a) { return func(a); }
#define FUNC2_DEF(F, RT, T1, T2, func)      inline RT F(const T1& a, const T2& b) { return func(a, b); }
#define FUNC3_DEF(F, RT, T1, T2, T3, func)  inline RT F(const T1& a, const T2& b, const T3& c) { return func(a, b, c); }

FUNC1_DEF(kb_log2,       auto, Integer,           log2_integer         );


FUNC2_DEF(distance,   auto, Scalar, Scalar,   dist_scalar           );
FUNC2_DEF(equal,      auto, Scalar, Scalar,   equal_scalar          );
FUNC2_DEF(dot,        auto, Scalar, Scalar,   dot_scalar            );
FUNC2_DEF(min,        auto, Scalar, Scalar,   min_scalar            );
FUNC2_DEF(max,        auto, Scalar, Scalar,   max_scalar            );
// FUNC1_DEF(abs,        auto, Scalar,           abs_scalar            );
// FUNC1_DEF(floor,      auto, Scalar,           floor_scalar          );
FUNC1_DEF(kb_log2,       auto, Scalar,           log2_scalar           );
FUNC1_DEF(recip,      auto, Scalar,           recip_scalar          );
FUNC1_DEF(saturate,   auto, Scalar,           saturate_scalar       );
FUNC1_DEF(smoothstep, auto, Scalar,           smoothstep_scalar     );
FUNC1_DEF(square,     auto, Scalar,           square_scalar         );
FUNC2_DEF(step,       auto, Scalar, Scalar,   step_scalar           );

// FUNC1_DEF(exp,        auto, Scalar,           exp_scalar            );
// FUNC1_DEF(log,        auto, Scalar,           log_scalar            );
// FUNC1_DEF(sign,       auto, Scalar,           sign_scalar           );
// FUNC1_DEF(sqrt,       auto, Scalar,           sqrt_scalar           );
// FUNC1_DEF(frac,       auto, Scalar,           frac_scalar           );
// FUNC1_DEF(ceil,       auto, Scalar,           ceil_scalar           );
// FUNC1_DEF(round,      auto, Scalar,           round_scalar          );
// FUNC1_DEF(trunc,      auto, Scalar,           trunc_scalar          );

FUNC1_DEF(operator-,  auto, Float2,           neg_float2            );
FUNC2_DEF(operator*,  auto, Float2, Scalar,   scale_float2          );
FUNC2_DEF(operator*,  auto, Float2, Float2,   mul_float2            );
FUNC2_DEF(operator+,  auto, Float2, Float2,   add_float2            );
FUNC2_DEF(operator-,  auto, Float2, Float2,   sub_float2            );
FUNC2_DEF(operator==, auto, Float2, Float2,   equal_float2          );
FUNC2_DEF(distance,   auto, Float2, Float2,   dist_float2           );
FUNC2_DEF(equal,      auto, Float2, Float2,   equal_float2          );
FUNC2_DEF(dot,        auto, Float2, Float2,   dot_float2            );
FUNC2_DEF(min,        auto, Float2, Float2,   min_float2            );
FUNC2_DEF(max,        auto, Float2, Float2,   max_float2            );
FUNC1_DEF(abs,        auto, Float2,           abs_float2            );
FUNC1_DEF(floor,      auto, Float2,           floor_float2          );
FUNC1_DEF(length,     auto, Float2,           len_float2            );
FUNC1_DEF(norm,       auto, Float2,           norm_float2           );
FUNC1_DEF(recip,      auto, Float2,           recip_float2          );
FUNC1_DEF(saturate,   auto, Float2,           saturate_float2       );
FUNC1_DEF(smoothstep, auto, Float2,           smoothstep_float2     );
FUNC1_DEF(square,     auto, Float2,           square_float2         );
FUNC1_DEF(exp,        auto, Float2,           exp_float2            );
FUNC1_DEF(log,        auto, Float2,           log_float2            );
FUNC1_DEF(sign,       auto, Float2,           sign_float2           );
FUNC1_DEF(sqrt,       auto, Float2,           sqrt_float2           );
FUNC1_DEF(frac,       auto, Float2,           frac_float2           );
FUNC1_DEF(ceil,       auto, Float2,           ceil_float2           );
FUNC1_DEF(round,      auto, Float2,           round_float2          );
FUNC1_DEF(trunc,      auto, Float2,           trunc_float2          );

FUNC1_DEF(operator-,    auto, Float3,           neg_float3          );
FUNC2_DEF(operator*,    auto, Float3, Scalar,   scale_float3        );
FUNC2_DEF(operator*,    auto, Float3, Float3,   mul_float3          );
FUNC2_DEF(operator+,    auto, Float3, Float3,   add_float3          );
FUNC2_DEF(operator-,    auto, Float3, Float3,   sub_float3          );
FUNC2_DEF(operator==,   auto, Float3, Float3,   equal_float3        );
FUNC2_DEF(distance,     auto, Float3, Float3,   dist_float3         );
FUNC2_DEF(equal,        auto, Float3, Float3,   equal_float3        );
FUNC2_DEF(dot,          auto, Float3, Float3,   dot_float3          );
FUNC2_DEF(min,          auto, Float3, Float3,   min_float3          );
FUNC2_DEF(max,          auto, Float3, Float3,   max_float3          );
FUNC2_DEF(cross,        auto, Float3, Float3,   cross_float3        );
FUNC1_DEF(abs,          auto, Float3,           abs_float3          );
FUNC1_DEF(floor,        auto, Float3,           floor_float3        );
FUNC1_DEF(length,       auto, Float3,           len_float3          );
FUNC1_DEF(norm,         auto, Float3,           norm_float3         );
FUNC1_DEF(recip,        auto, Float3,           recip_float3        );
FUNC1_DEF(saturate,     auto, Float3,           saturate_float3     );
FUNC1_DEF(smoothstep,   auto, Float3,           smoothstep_float3   );
FUNC1_DEF(square,       auto, Float3,           square_float3       );
FUNC1_DEF(exp,          auto, Float3,           exp_float3          );
FUNC1_DEF(log,          auto, Float3,           log_float3          );
FUNC1_DEF(sign,         auto, Float3,           sign_float3         );
FUNC1_DEF(sqrt,         auto, Float3,           sqrt_float3         );
FUNC1_DEF(frac,         auto, Float3,           frac_float3         );
FUNC1_DEF(ceil,         auto, Float3,           ceil_float3         );
FUNC1_DEF(round,        auto, Float3,           round_float3        );
FUNC1_DEF(trunc,        auto, Float3,           trunc_float3        );

FUNC1_DEF(operator-,    auto, Float4,           neg_float4          );
FUNC2_DEF(operator*,    auto, Float4, Scalar,   scale_float4        );
FUNC2_DEF(operator*,    auto, Float4, Float4,   mul_float4          );
FUNC2_DEF(operator+,    auto, Float4, Float4,   add_float4          );
FUNC2_DEF(operator-,    auto, Float4, Float4,   sub_float4          );
FUNC2_DEF(operator==,   auto, Float4, Float4,   equal_float4        );
FUNC2_DEF(distance,     auto, Float4, Float4,   dist_float4         );
FUNC2_DEF(equal,        auto, Float4, Float4,   equal_float4        );
FUNC2_DEF(dot,          auto, Float4, Float4,   dot_float4          );
FUNC2_DEF(min,          auto, Float4, Float4,   min_float4          );
FUNC2_DEF(max,          auto, Float4, Float4,   max_float4          );
FUNC1_DEF(abs,          auto, Float4,           abs_float4          );
FUNC1_DEF(floor,        auto, Float4,           floor_float4        );
FUNC1_DEF(length,       auto, Float4,           len_float4          );
FUNC1_DEF(norm,         auto, Float4,           norm_float4         );
FUNC1_DEF(recip,        auto, Float4,           recip_float4        );
FUNC1_DEF(saturate,     auto, Float4,           saturate_float4     );
FUNC1_DEF(smoothstep,   auto, Float4,           smoothstep_float4   );
FUNC1_DEF(square,       auto, Float4,           square_float4       );
FUNC1_DEF(exp,          auto, Float4,           exp_float4          );
FUNC1_DEF(log,          auto, Float4,           log_float4          );
FUNC1_DEF(sign,         auto, Float4,           sign_float4         );
FUNC1_DEF(sqrt,         auto, Float4,           sqrt_float4         );
FUNC1_DEF(frac,         auto, Float4,           frac_float4         );
FUNC1_DEF(ceil,         auto, Float4,           ceil_float4         );
FUNC1_DEF(round,        auto, Float4,           round_float4        );
FUNC1_DEF(trunc,        auto, Float4,           trunc_float4        );

FUNC1_DEF(operator-,  auto, Int2,             neg_int2      );
FUNC2_DEF(operator*,  auto, Int2, Integer,    scale_int2    );
FUNC2_DEF(operator*,  auto, Int2, Int2,       mul_int2      );
FUNC2_DEF(operator+,  auto, Int2, Int2,       add_int2      );
FUNC2_DEF(operator-,  auto, Int2, Int2,       sub_int2      );
FUNC2_DEF(operator==, auto, Int2, Int2,       equal_int2    );
FUNC2_DEF(equal,      auto, Int2, Int2,       equal_int2    );
FUNC2_DEF(dot,        auto, Int2, Int2,       dot_int2      );
FUNC2_DEF(min,        auto, Int2, Int2,       min_int2      );
FUNC2_DEF(max,        auto, Int2, Int2,       max_int2      );
FUNC1_DEF(abs,        auto, Int2,             abs_int2      );
FUNC1_DEF(length,     auto, Int2,             len_int2      );

FUNC1_DEF(operator-,  auto, Int3,             neg_int3      );
FUNC2_DEF(operator*,  auto, Int3, Integer,    scale_int3    );
FUNC2_DEF(operator*,  auto, Int3, Int3,       mul_int3      );
FUNC2_DEF(operator+,  auto, Int3, Int3,       add_int3      );
FUNC2_DEF(operator-,  auto, Int3, Int3,       sub_int3      );
FUNC2_DEF(operator==, auto, Int3, Int3,       equal_int3    );
FUNC2_DEF(equal,      auto, Int3, Int3,       equal_int3    );
FUNC2_DEF(dot,        auto, Int3, Int3,       dot_int3      );
FUNC2_DEF(min,        auto, Int3, Int3,       min_int3      );
FUNC2_DEF(max,        auto, Int3, Int3,       max_int3      );
FUNC1_DEF(abs,        auto, Int3,             abs_int3      );
FUNC1_DEF(length,     auto, Int3,             len_int3      );

FUNC1_DEF(operator-,  auto, Int4,             neg_int4      );
FUNC2_DEF(operator*,  auto, Int4, Integer,    scale_int4    );
FUNC2_DEF(operator*,  auto, Int4, Int4,       mul_int4      );
FUNC2_DEF(operator+,  auto, Int4, Int4,       add_int4      );
FUNC2_DEF(operator-,  auto, Int4, Int4,       sub_int4      );
FUNC2_DEF(operator==, auto, Int4, Int4,       equal_int4    );
FUNC2_DEF(equal,      auto, Int4, Int4,       equal_int4    );
FUNC2_DEF(dot,        auto, Int4, Int4,       dot_int4      );
FUNC2_DEF(min,        auto, Int4, Int4,       min_int4      );
FUNC2_DEF(max,        auto, Int4, Int4,       max_int4      );
FUNC1_DEF(abs,        auto, Int4,             abs_int4      );
FUNC1_DEF(length,     auto, Int4,             len_int4      );

FUNC3_DEF(lerp,       auto, Scalar, Scalar, Scalar, lerp_scalar      );
FUNC3_DEF(lerp,       auto, Float2, Float2, Scalar, lerp_float2      );
FUNC3_DEF(lerp,       auto, Float3, Float3, Scalar, lerp_float3      );
FUNC3_DEF(lerp,       auto, Float4, Float4, Scalar, lerp_float4      );
