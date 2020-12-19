// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/math.h>

const Float3 DirRight    = { 1.0f, 0.0f, 0.0f };
const Float3 DirUp       = { 0.0f, 1.0f, 0.0f };
const Float3 DirForward  = { 0.0f, 0.0f, 1.0f };

const Float3 Float2Max   = { FLOATMAX, FLOATMAX };
const Float3 Float2Min   = { FLOATMIN, FLOATMIN };
const Float3 Float3Max   = { FLOATMAX, FLOATMAX, FLOATMAX };
const Float3 Float3Min   = { FLOATMIN, FLOATMIN, FLOATMIN };

const Float4x4 IdentityFloat4x4 = { .m = {
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f,
}};

const Float4x4 ZeroFloat4x4 = { .m = {
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
}};

KB_API Float3 act_quat(const Quaternion a, const Float3 b) {
  const Quaternion tmp0 = inv_quat(a);
  const Quaternion qv   = { b.x, b.y, b.z, 0.0f };
  const Quaternion tmp1 = mul_quat(tmp0, qv);
  const Quaternion res  = mul_quat(tmp1, a);

  return { res.x, res.y, res.z };
}

KB_API Quaternion slerp_quat(const Quaternion a, const Quaternion b, Real32 t) {
  const float tt = 1 - t;
  Real32 dot = dot_quat(a, b);

	Real32 th = acos_scalar(dot);
	Real32 sn = sin_scalar(th);

	Real32 wa = sin_scalar(tt * th) / sn;
	Real32 wb = sin_scalar(t  * th) / sn;
  
  Quaternion r {
	  wa * a.x + wb * b.x,
	  wa * a.y + wb * b.y,
	  wa * a.z + wb * b.z,
	  wa * a.w + wb * b.w,
  };

  return norm_quat(r);
}

KB_API Float4x4 look_at(const Float3 from, const Float3 to, const Float3 global_up) {
  const Float3 forward  = norm(to - from);  
  const Float3 right    = norm(cross(global_up, forward));
  const Float3 up       = norm(cross(forward, right));

  Float4x4 res = ZeroFloat4x4;
  res.m[ 0] = right.x;
  res.m[ 1] = up.x;
  res.m[ 2] = forward.x;
  res.m[ 4] = right.y;
  res.m[ 5] = up.y;
  res.m[ 6] = forward.y;
  res.m[ 8] = right.z;
  res.m[ 9] = up.z;
  res.m[10] = forward.z;
  res.m[12] = -dot_float3(right,   from);
  res.m[13] = -dot_float3(up,      from);
  res.m[14] = -dot_float3(forward, from);
  res.m[15] = 1.0f;
  
  return res;  
}

KB_API Float4x4 orthographic(Real32 left, Real32 right, Real32 top, Real32 bottom, Real32 near, Real32 far) {
		const float aa = 2.0f / (right - left);
		const float bb = 2.0f / (top - bottom);
		const float cc = far - near;

		const float dd = (left + right ) / (left   - right);
		const float ee = (top  + bottom) / (bottom - top  );

		const float ff = near / (near - far);

    Float4x4 res = ZeroFloat4x4;  
		res.m[ 0] = aa;
		res.m[ 5] = bb;
		res.m[10] = cc;
		res.m[12] = dd;
		res.m[13] = ee;
		res.m[14] = ff;
		res.m[15] = 1.0f;
    
    return res;
}

KB_API Float4x4 perspective(Real32 fov, Real32 aspect, Real32 near, Real32 far) {
  const float height = 1.0f / tan_scalar(deg_to_rad(fov) * 0.5f);
  const float width  = height / aspect;

  const float diff = far - near;
  const float aa = far / diff;
  const float bb = near * aa;

  Float4x4 res = ZeroFloat4x4;  
  res.m[ 0] = -width;
	res.m[ 5] = height; // Flip proj y 
	res.m[10] = aa;
	res.m[11] = 1.0f;
	res.m[14] = -bb;

  return res;
}

KB_API Float4x4 transform_mtx(const Float3 v) {
  Float4x4 res = IdentityFloat4x4;
	
  res.m[12] = v.x;
	res.m[13] = v.y;
	res.m[14] = v.z;
  
  return res;
}

KB_API Float4x4 scaling_mtx(const Float3 v) {
  Float4x4 res = IdentityFloat4x4;
	
  res.m[0]  = v.x;
	res.m[5]  = v.y;
	res.m[10] = v.z;
  
  return res;
}

KB_API Float4x4 rotation_mtx(const Quaternion v) {
  Float4x4 res = IdentityFloat4x4;

  const float x = v.x;
  const float y = v.y;
  const float z = v.z;
  const float w = v.w;

  const float xx  = x + x;
  const float yy  = y + y;
  const float zz  = z + z;
  const float xxx = xx * x;
  const float xxy = xx * y;
  const float xxz = xx * z;
  const float xxw = xx * w;
  const float yyy = yy * y;
  const float yyz = yy * z;
  const float yyw = yy * w;
  const float zzz = zz * z;
  const float zzw = zz * w;

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

KB_API void translate(Float4x4* mtx, const Float3 v) {
  *mtx = mul_float4x4(*mtx, transform_mtx(v));
}

KB_API void scale(Float4x4* mtx, const Float3 v) {
  *mtx = mul_float4x4(*mtx, scaling_mtx(v));
}

KB_API void rotate(Float4x4* mtx, const Quaternion v) {
  *mtx = mul_float4x4(*mtx, rotation_mtx(v));
}

KB_API Float4x4 mul_float4x4(const Float4x4 a, const Float4x4 b) {
  Float4x4 res = {};

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

KB_API Float3 get_point(Axis axis, float x, float y) {
  Float3 result;

  switch (axis) {
    case Axis::AxisX:
      result = { 0.0f, x, y };
      break;

    case Axis::AxisY:
      result = { y, 0.0f, x };
      break;

    default:
      result = { x, y, 0.0f };
      break;
  }

  return result;
}

KB_API void tangent_frame(const Float3 n, Float3* t, Float3* b) {
  const float nx = n.x;
  const float ny = n.y;
  const float nz = n.z;

  if (abs_scalar(nx) > abs_scalar(nz)) {
    float inv_len = 1.0f / sqrt_scalar(nx*nx + nz*nz);
    t->x = -nz * inv_len;
    t->y =  0.0f;
    t->z =  nx * inv_len;
  } else {
    float inv_len = 1.0f / sqrt_scalar(ny*ny + nz*nz);
    t->x =  0.0f;
    t->y =  nz * inv_len;
    t->z = -ny * inv_len;
  }

  *b = cross_float3(n, *t);
}

KB_API void tangent_frame_with_spin(const Float3 n, float spin, Float3* t, Float3* b) {
  tangent_frame(n, t, b);
  
  const float sa = sin_scalar(spin);
  const float ca = cos_scalar(spin);

  t->x = -sa * b->x + ca * t->x;
  t->y = -sa * b->y + ca * t->y;
  t->z = -sa * b->z + ca * t->z;

  *b = cross_float3(n, *t); 
}

uint32_t float_rgba_to_uint(const Float4 color) {
  uint32_t r = 255 * color.x;
  uint32_t g = 255 * color.y;
  uint32_t b = 255 * color.z;
  uint32_t a = 255 * color.w;

  return (a << 24) | (b << 16) | (g << 8) | (r << 0);
}

KB_API Float4x4 transpose_float4x4(const Float4x4 a) {
  Float4x4 res = {};

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

KB_API Float4x4 inv_float4x4(const Float4x4 a) {
  const float xx = a.m[ 0];
  const float xy = a.m[ 1];
  const float xz = a.m[ 2];
  const float xw = a.m[ 3];
  const float yx = a.m[ 4];
  const float yy = a.m[ 5];
  const float yz = a.m[ 6];
  const float yw = a.m[ 7];
  const float zx = a.m[ 8];
  const float zy = a.m[ 9];
  const float zz = a.m[10];
  const float zw = a.m[11];
  const float wx = a.m[12];
  const float wy = a.m[13];
  const float wz = a.m[14];
  const float ww = a.m[15];

  float det = 0.0f;
  det += xx * ( yy * (zz * ww - zw * wz) - yz * (zy * ww - zw * wy) + yw * (zy * wz - zz * wy) );
  det -= xy * ( yx * (zz * ww - zw * wz) - yz * (zx * ww - zw * wx) + yw * (zx * wz - zz * wx) );
  det += xz * ( yx * (zy * ww - zw * wy) - yy * (zx * ww - zw * wx) + yw * (zx * wy - zy * wx) );
  det -= xw * ( yx * (zy * wz - zz * wy) - yy * (zx * wz - zz * wx) + yz * (zx * wy - zy * wx) );

  float inv_det = 1.0f / det;
  
  Float4x4 res = {};

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

KB_API Float4 act_float4x4(const Float4x4 mtx, const Float4 vec) {
  return {
    vec.x * mtx.m[ 0] + vec.y * mtx.m[4] + vec.z * mtx.m[ 8] + vec.w * mtx.m[12],
		vec.x * mtx.m[ 1] + vec.y * mtx.m[5] + vec.z * mtx.m[ 9] + vec.w * mtx.m[13],
		vec.x * mtx.m[ 2] + vec.y * mtx.m[6] + vec.z * mtx.m[10] + vec.w * mtx.m[14],
		vec.x * mtx.m[ 3] + vec.y * mtx.m[7] + vec.z * mtx.m[11] + vec.w * mtx.m[15],
  };
}

KB_API float ray_plane_intersection(const Ray ray, const Plane plane) {
  float denom = dot_float3(plane.normal, ray.dir);
  
  if (denom > FLT_EPSILON) {
    Float3 p = sub_float3((scale_float3(plane.normal, plane.dist)), ray.pos);
    return dot_float3(p, plane.normal) / denom;
  }

  return NAN;
}
