// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/math.h>

KB_API kb_float3 kb_quat_act(const kb_quat a, const kb_float3 b) {
  const kb_quat tmp0 = kb_quat_inv(a);
  const kb_quat qv   = { b.x, b.y, b.z, 0.0f };
  const kb_quat tmp1 = kb_quat_mul(tmp0, qv);
  const kb_quat res  = kb_quat_mul(tmp1, a);

  return { res.x, res.y, res.z };
}

KB_API kb_quat kb_quat_slerp(const kb_quat a, const kb_quat b, kb_float t) {
  const kb_float tt = 1 - t;
  kb_float dot = kb_quat_dot(a, b);

	kb_float th = kb_float_cos(dot);
	kb_float sn = kb_float_sin(th);

	kb_float wa = kb_float_sin(tt * th) / sn;
	kb_float wb = kb_float_sin(t  * th) / sn;
  
  kb_quat r {
	  wa * a.x + wb * b.x,
	  wa * a.y + wb * b.y,
	  wa * a.z + wb * b.z,
	  wa * a.w + wb * b.w,
  };

  return kb_quat_norm(r);
}

KB_API kb_float4x4 kb_look_at(const kb_float3 from, const kb_float3 to, const kb_float3 global_up) {
  const kb_float3 forward  = kb::norm(to - from);
  const kb_float3 right    = kb::norm(kb::cross(global_up, forward));
  const kb_float3 up       = kb::norm(kb::cross(forward, right));

  kb_float4x4 res = kb_float4x4_zero;
  res.m[ 0] = right.x;
  res.m[ 1] = up.x;
  res.m[ 2] = forward.x;
  res.m[ 4] = right.y;
  res.m[ 5] = up.y;
  res.m[ 6] = forward.y;
  res.m[ 8] = right.z;
  res.m[ 9] = up.z;
  res.m[10] = forward.z;
  res.m[12] = -kb_float3_dot(right,   from);
  res.m[13] = -kb_float3_dot(up,      from);
  res.m[14] = -kb_float3_dot(forward, from);
  res.m[15] = 1.0f;
  
  return res;  
}

KB_API kb_float4x4 kb_orthographic(kb_float left, kb_float right, kb_float top, kb_float bottom, kb_float near, kb_float far) {
		const kb_float aa = 2.0f / (right - left);
		const kb_float bb = 2.0f / (top - bottom);
		const kb_float cc = 1.0f / (far - near);

		const kb_float dd = (left + right ) / (left   - right);
		const kb_float ee = (top  + bottom) / (bottom - top  );

		const kb_float ff = near / (near - far);

    kb_float4x4 res = kb_float4x4_zero;
		res.m[ 0] = aa;
		res.m[ 5] = bb;
		res.m[10] = cc;
		res.m[12] = dd;
		res.m[13] = ee;
		res.m[14] = ff;
		res.m[15] = 1.0f;
    
    return res;
}

KB_API kb_float4x4 kb_perspective(kb_float fov, kb_float aspect, kb_float near, kb_float far) {
  const kb_float height = 1.0f / kb_float_tan(kb_deg_to_rad(fov) * 0.5f);
  const kb_float width  = height / aspect;

  const kb_float diff = far - near;
  const kb_float aa = far / diff;
  const kb_float bb = near * aa;

  kb_float4x4 res = kb_float4x4_zero;
  res.m[ 0] = -width;
	res.m[ 5] = height; // Flip proj y 
	res.m[10] = aa;
	res.m[11] = 1.0f;
	res.m[14] = -bb;

  return res;
}

KB_API kb_float4x4 kb_float4x4_translation(const kb_float3 v) {
  kb_float4x4 res = kb_float4x4_ident;
	
  res.m[12] = v.x;
	res.m[13] = v.y;
	res.m[14] = v.z;
  
  return res;
}

KB_API kb_float4x4 kb_float4x4_scaling(const kb_float3 v) {
  kb_float4x4 res = kb_float4x4_ident;
	
  res.m[0]  = v.x;
	res.m[5]  = v.y;
	res.m[10] = v.z;
  
  return res;
}

KB_API kb_float4x4 kb_float4x4_rotation(const kb_quat v) {
  kb_float4x4 res = kb_float4x4_ident;

  const kb_float x = v.x;
  const kb_float y = v.y;
  const kb_float z = v.z;
  const kb_float w = v.w;

  const kb_float xx  = x + x;
  const kb_float yy  = y + y;
  const kb_float zz  = z + z;
  const kb_float xxx = xx * x;
  const kb_float xxy = xx * y;
  const kb_float xxz = xx * z;
  const kb_float xxw = xx * w;
  const kb_float yyy = yy * y;
  const kb_float yyz = yy * z;
  const kb_float yyw = yy * w;
  const kb_float zzz = zz * z;
  const kb_float zzw = zz * w;

  res.m[ 0] = 1.0f - (yyy + zzz);
  res.m[ 1] =         xxy - zzw;
  res.m[ 2] =         xxz + yyw;

  res.m[ 4] =         xxy + zzw;
  res.m[ 5] = 1.0f - (xxx + zzz);
  res.m[ 6] =         yyz - xxw;

  res.m[ 8] =         xxz - yyw;
  res.m[ 9] =         yyz + xxw;
  res.m[10] = 1.0f - (xxx + yyy);

  return res;
}

KB_API void kb_float4x4_translate(kb_float4x4* mtx, const kb_float3 v) {
  *mtx = kb_float4x4_mul(*mtx, kb_float4x4_translation(v));
}

KB_API void kb_float4x4_scale(kb_float4x4* mtx, const kb_float3 v) {
  *mtx = kb_float4x4_mul(*mtx, kb_float4x4_scaling(v));
}

KB_API void kb_float4x4_rotate(kb_float4x4* mtx, const kb_quat v) {
  *mtx = kb_float4x4_mul(*mtx, kb_float4x4_rotation(v));
}

KB_API kb_float4x4 kb_float4x4_mul(const kb_float4x4 a, const kb_float4x4 b) {
  kb_float4x4 res = {};

  res.mm[0][0] = a.mm[0][0] * b.mm[0][0] + a.mm[0][1] * b.mm[1][0] + a.mm[0][2] * b.mm[2][0] + a.mm[0][3] * b.mm[3][0];
  res.mm[0][1] = a.mm[0][0] * b.mm[0][1] + a.mm[0][1] * b.mm[1][1] + a.mm[0][2] * b.mm[2][1] + a.mm[0][3] * b.mm[3][1];
  res.mm[0][2] = a.mm[0][0] * b.mm[0][2] + a.mm[0][1] * b.mm[1][2] + a.mm[0][2] * b.mm[2][2] + a.mm[0][3] * b.mm[3][2];
  res.mm[0][3] = a.mm[0][0] * b.mm[0][3] + a.mm[0][1] * b.mm[1][3] + a.mm[0][2] * b.mm[2][3] + a.mm[0][3] * b.mm[3][3];
  res.mm[1][0] = a.mm[1][0] * b.mm[0][0] + a.mm[1][1] * b.mm[1][0] + a.mm[1][2] * b.mm[2][0] + a.mm[1][3] * b.mm[3][0];
  res.mm[1][1] = a.mm[1][0] * b.mm[0][1] + a.mm[1][1] * b.mm[1][1] + a.mm[1][2] * b.mm[2][1] + a.mm[1][3] * b.mm[3][1];
  res.mm[1][2] = a.mm[1][0] * b.mm[0][2] + a.mm[1][1] * b.mm[1][2] + a.mm[1][2] * b.mm[2][2] + a.mm[1][3] * b.mm[3][2];
  res.mm[1][3] = a.mm[1][0] * b.mm[0][3] + a.mm[1][1] * b.mm[1][3] + a.mm[1][2] * b.mm[2][3] + a.mm[1][3] * b.mm[3][3];
  res.mm[2][0] = a.mm[2][0] * b.mm[0][0] + a.mm[2][1] * b.mm[1][0] + a.mm[2][2] * b.mm[2][0] + a.mm[2][3] * b.mm[3][0];
  res.mm[2][1] = a.mm[2][0] * b.mm[0][1] + a.mm[2][1] * b.mm[1][1] + a.mm[2][2] * b.mm[2][1] + a.mm[2][3] * b.mm[3][1];
  res.mm[2][2] = a.mm[2][0] * b.mm[0][2] + a.mm[2][1] * b.mm[1][2] + a.mm[2][2] * b.mm[2][2] + a.mm[2][3] * b.mm[3][2];
  res.mm[2][3] = a.mm[2][0] * b.mm[0][3] + a.mm[2][1] * b.mm[1][3] + a.mm[2][2] * b.mm[2][3] + a.mm[2][3] * b.mm[3][3];
  res.mm[3][0] = a.mm[3][0] * b.mm[0][0] + a.mm[3][1] * b.mm[1][0] + a.mm[3][2] * b.mm[2][0] + a.mm[3][3] * b.mm[3][0];
  res.mm[3][1] = a.mm[3][0] * b.mm[0][1] + a.mm[3][1] * b.mm[1][1] + a.mm[3][2] * b.mm[2][1] + a.mm[3][3] * b.mm[3][1];
  res.mm[3][2] = a.mm[3][0] * b.mm[0][2] + a.mm[3][1] * b.mm[1][2] + a.mm[3][2] * b.mm[2][2] + a.mm[3][3] * b.mm[3][2];
  res.mm[3][3] = a.mm[3][0] * b.mm[0][3] + a.mm[3][1] * b.mm[1][3] + a.mm[3][2] * b.mm[2][3] + a.mm[3][3] * b.mm[3][3];
  
  return res;
}

KB_API kb_float3 kb_point_on_plane(kb_axis axis, kb_float x, kb_float y) {
  kb_float3 result;

  switch (axis) {
    case kb_axis::kb_axis_x:
      result = { 0.0f, x, y };
      break;

    case kb_axis::kb_axis_y:
      result = { y, 0.0f, x };
      break;

    default:
      result = { x, y, 0.0f };
      break;
  }

  return result;
}

KB_API void kb_tangent_frame(const kb_float3 n, kb_float3* t, kb_float3* b) {
  const kb_float nx = n.x;
  const kb_float ny = n.y;
  const kb_float nz = n.z;

  if (kb_float_abs(nx) > kb_float_abs(nz)) {
    kb_float inv_len = 1.0f / kb_float_sqrt(nx*nx + nz*nz);
    t->x = -nz * inv_len;
    t->y =  0.0f;
    t->z =  nx * inv_len;
  } else {
    kb_float inv_len = 1.0f / kb_float_sqrt(ny*ny + nz*nz);
    t->x =  0.0f;
    t->y =  nz * inv_len;
    t->z = -ny * inv_len;
  }

  *b = kb_float3_cross(n, *t);
}

KB_API void kb_tangent_frame_with_spin(const kb_float3 n, kb_float spin, kb_float3* t, kb_float3* b) {
  kb_tangent_frame(n, t, b);
  
  const kb_float sa = kb_float_sin(spin);
  const kb_float ca = kb_float_cos(spin);

  t->x = -sa * b->x + ca * t->x;
  t->y = -sa * b->y + ca * t->y;
  t->z = -sa * b->z + ca * t->z;

  *b = kb_float3_cross(n, *t); 
}

uint32_t kb_color_rgba_to_uint(const kb_float4 color) {
  uint32_t r = 255 * color.x;
  uint32_t g = 255 * color.y;
  uint32_t b = 255 * color.z;
  uint32_t a = 255 * color.w;

  return (a << 24) | (b << 16) | (g << 8) | (r << 0);
}

KB_API kb_float4x4 kb_float4x4_transpose(const kb_float4x4 a) {
  kb_float4x4 res = {};

  res.m[ 0] = a.m[ 0];
  res.m[ 4] = a.m[ 1];
  res.m[ 8] = a.m[ 2];
  res.m[12] = a.m[ 3];
  res.m[ 1] = a.m[ 4];
  res.m[ 5] = a.m[ 5];
  res.m[ 9] = a.m[ 6];
  res.m[13] = a.m[ 7];
  res.m[ 2] = a.m[ 8];
  res.m[ 6] = a.m[ 9];
  res.m[10] = a.m[10];
  res.m[14] = a.m[11];
  res.m[ 3] = a.m[12];
  res.m[ 7] = a.m[13];
  res.m[11] = a.m[14];
  res.m[15] = a.m[15];
  
  return res;
}

KB_API kb_float4x4 kb_float4x4_inv(const kb_float4x4 a) {
  const kb_float xx = a.m[ 0];
  const kb_float xy = a.m[ 1];
  const kb_float xz = a.m[ 2];
  const kb_float xw = a.m[ 3];
  const kb_float yx = a.m[ 4];
  const kb_float yy = a.m[ 5];
  const kb_float yz = a.m[ 6];
  const kb_float yw = a.m[ 7];
  const kb_float zx = a.m[ 8];
  const kb_float zy = a.m[ 9];
  const kb_float zz = a.m[10];
  const kb_float zw = a.m[11];
  const kb_float wx = a.m[12];
  const kb_float wy = a.m[13];
  const kb_float wz = a.m[14];
  const kb_float ww = a.m[15];

  kb_float det = 0.0f;
  det += xx * ( yy * (zz * ww - zw * wz) - yz * (zy * ww - zw * wy) + yw * (zy * wz - zz * wy) );
  det -= xy * ( yx * (zz * ww - zw * wz) - yz * (zx * ww - zw * wx) + yw * (zx * wz - zz * wx) );
  det += xz * ( yx * (zy * ww - zw * wy) - yy * (zx * ww - zw * wx) + yw * (zx * wy - zy * wx) );
  det -= xw * ( yx * (zy * wz - zz * wy) - yy * (zx * wz - zz * wx) + yz * (zx * wy - zy * wx) );

  kb_float inv_det = 1.0f / det;
  
  kb_float4x4 res = {};

  res.m[ 0] = +( yy * (zz * ww - wz * zw) - yz * (zy * ww - wy * zw) + yw * (zy * wz - wy * zz) ) * inv_det;
  res.m[ 1] = -( xy * (zz * ww - wz * zw) - xz * (zy * ww - wy * zw) + xw * (zy * wz - wy * zz) ) * inv_det;
  res.m[ 2] = +( xy * (yz * ww - wz * yw) - xz * (yy * ww - wy * yw) + xw * (yy * wz - wy * yz) ) * inv_det;
  res.m[ 3] = -( xy * (yz * zw - zz * yw) - xz * (yy * zw - zy * yw) + xw * (yy * zz - zy * yz) ) * inv_det;

  res.m[ 4] = -( yx * (zz * ww - wz * zw) - yz * (zx * ww - wx * zw) + yw * (zx * wz - wx * zz) ) * inv_det;
  res.m[ 5] = +( xx * (zz * ww - wz * zw) - xz * (zx * ww - wx * zw) + xw * (zx * wz - wx * zz) ) * inv_det;
  res.m[ 6] = -( xx * (yz * ww - wz * yw) - xz * (yx * ww - wx * yw) + xw * (yx * wz - wx * yz) ) * inv_det;
  res.m[ 7] = +( xx * (yz * zw - zz * yw) - xz * (yx * zw - zx * yw) + xw * (yx * zz - zx * yz) ) * inv_det;

  res.m[ 8] = +( yx * (zy * ww - wy * zw) - yy * (zx * ww - wx * zw) + yw * (zx * wy - wx * zy) ) * inv_det;
  res.m[ 9] = -( xx * (zy * ww - wy * zw) - xy * (zx * ww - wx * zw) + xw * (zx * wy - wx * zy) ) * inv_det;
  res.m[10] = +( xx * (yy * ww - wy * yw) - xy * (yx * ww - wx * yw) + xw * (yx * wy - wx * yy) ) * inv_det;
  res.m[11] = -( xx * (yy * zw - zy * yw) - xy * (yx * zw - zx * yw) + xw * (yx * zy - zx * yy) ) * inv_det;

  res.m[12] = -( yx * (zy * wz - wy * zz) - yy * (zx * wz - wx * zz) + yz * (zx * wy - wx * zy) ) * inv_det;
  res.m[13] = +( xx * (zy * wz - wy * zz) - xy * (zx * wz - wx * zz) + xz * (zx * wy - wx * zy) ) * inv_det;
  res.m[14] = -( xx * (yy * wz - wy * yz) - xy * (yx * wz - wx * yz) + xz * (yx * wy - wx * yy) ) * inv_det;
  res.m[15] = +( xx * (yy * zz - zy * yz) - xy * (yx * zz - zx * yz) + xz * (yx * zy - zx * yy) ) * inv_det;
  
  return res;
}

KB_API kb_float4 kb_float4x4_act(const kb_float4x4 mtx, const kb_float4 vec) {
  return {
    vec.x * mtx.m[ 0] + vec.y * mtx.m[4] + vec.z * mtx.m[ 8] + vec.w * mtx.m[12],
		vec.x * mtx.m[ 1] + vec.y * mtx.m[5] + vec.z * mtx.m[ 9] + vec.w * mtx.m[13],
		vec.x * mtx.m[ 2] + vec.y * mtx.m[6] + vec.z * mtx.m[10] + vec.w * mtx.m[14],
		vec.x * mtx.m[ 3] + vec.y * mtx.m[7] + vec.z * mtx.m[11] + vec.w * mtx.m[15],
  };
}

KB_API kb_float kb_ray_plane_intersection(const kb_ray ray, const kb_plane plane) {
  kb_float denom = kb_float3_dot(plane.normal, ray.dir);
  
  if (denom > FLT_EPSILON) {
    kb_float3 p = kb_float3_sub((kb_float3_scale(plane.normal, plane.dist)), ray.pos);
    return kb_float3_dot(p, plane.normal) / denom;
  }

  return NAN;
}

KB_API kb_float3 kb_unproject(kb_float4x4 unproj, kb_float3 point) {
  kb_float4 v = kb_float4x4_act(unproj, kb_float4 { point.x, point.y, point.z, 1.0 });
  return kb_float3_scale(kb_float3 { v.x, v.y, v.z }, (1.0 / v.w));
}

KB_API kb_ray kb_unproject_view(kb_float4x4 unproj, kb_float2 p) {
  const kb_float3 orig = kb_unproject(unproj, kb_float3 { 0, 0, -1 });
  kb_float3 pp = kb_unproject(unproj, kb_float3 { p.x, p.y, 0.9999f });
  kb_float3 dir = kb_float3_sub(orig, pp);

  return {orig, dir};
}

KB_API kb_quat kb_quat_inv(const kb_quat a) {
  // TODO: Check
  const kb_quat conj = kb_quat_conj(a);
  return kb_quat_scale(conj, 1.0f / kb_quat_len(conj));
}

KB_API kb_quat  kb_quat_norm(const kb_quat a) {
  // TODO: Check
  const kb_float norm = kb_quat_dot(a, a);
  if (0.0f < norm) {
    return kb_quat_scale(a, 1.0f / kb_float_sqrt(norm));
  }
  return (kb_quat) { 0.0f, 0.0f, 0.0f, 1.0f};
}

KB_API  kb_quat kb_quat_mul(const kb_quat a, const kb_quat b) {
  return (kb_quat) {
    a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
    a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
    a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
    a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
  };
}

KB_API kb_quat kb_quat_euler(const kb_float3 euler) {
  const kb_float hx = euler.x * 0.5f;
  const kb_float hy = euler.y * 0.5f;
  const kb_float hz = euler.z * 0.5f;
  
  const kb_quat x = { kb_float_sin(hx), 0.0f, 0.0f, kb_float_cos(hx) };
  const kb_quat y = { 0.0f, kb_float_sin(hy), 0.0f, kb_float_cos(hy) };
  const kb_quat z = { 0.0f, 0.0f, kb_float_sin(hz), kb_float_cos(hz) };

  return kb_quat_mul(x, kb_quat_mul(y, z));
}

KB_API kb_quat kb_quat_axis_angle(const kb_float3 axis, kb_float angle) {
  const kb_float half = angle * 0.5f;
  const kb_float s = kb_float_sin(half);

  return (kb_quat) {
    axis.x * s,
    axis.y * s,
    axis.z * s,
    cos(half),
  };
}

KB_API bool kb_aabb_contains(kb_aabb_int aabb, kb_int3 pos) {
  return true
    && pos.x >= aabb.min.x && pos.x < aabb.max.x
    && pos.y >= aabb.min.y && pos.y < aabb.max.y
    && pos.z >= aabb.min.z && pos.z < aabb.max.z
  ;
}

KB_API uint64_t kb_align_up(uint64_t a, uint64_t align) {
  const uint64_t mask = (uint64_t)(align - 1);
  return (uint64_t)( (a + mask) & ~mask);
}

KB_API kb_float2 kb_circle_point(kb_float angle) {
  return (kb_float2) { 
    kb_float_sin(angle), 
    kb_float_cos(angle) 
  };
}

KB_API kb_float2 kb_squircle_point(kb_float angle) {
  kb_float sa = kb_float_sin(angle);
  kb_float ca = kb_float_cos(angle);

  return (kb_float2) {
    kb_float_sqrt(kb_float_abs(sa)) * kb_float_sign(sa),
    kb_float_sqrt(kb_float_abs(ca)) * kb_float_sign(ca)
  };
}
