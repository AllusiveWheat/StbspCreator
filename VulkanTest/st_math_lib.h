#pragma once

#include <immintrin.h>

inline __m128 magnitude_ps(__m128 vec) {
	__m128 sqr = _mm_mul_ps(vec,vec);
	__m128 magnitude = _mm_add_ps(sqr,_mm_shuffle_ps(sqr,sqr,_MM_SHUFFLE(2,3,0,1)));
	return _mm_sqrt_ps(_mm_add_ps(magnitude,_mm_shuffle_ps(magnitude,magnitude,_MM_SHUFFLE(0,1,2,3))));
}

inline __m128 dotProduct_ps(__m128 a, __m128 b) {
	__m128 c = _mm_mul_ps(a,b);
	c = _mm_add_ps(c,_mm_shuffle_ps(c,c,_MM_SHUFFLE(2,3,0,1)));
	return _mm_add_ps(c,_mm_shuffle_ps(c,c,_MM_SHUFFLE(0,1,2,3)));
}

inline __m128 normalize_ps(__m128 a) {
	return _mm_div_ps(a,magnitude_ps(a));
}

inline __m128 abs_ps(__m128 a) {
	return _mm_and_ps(a,_mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
}

inline __m128 crossProduct_ps(__m128 u, __m128 v) {
	return _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(u, u, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2))),
		_mm_mul_ps(_mm_shuffle_ps(u, u, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1))));
}

inline __m128 faceNormal_ps(__m128 v0, __m128 v1, __m128 v2) {
	__m128 u = _mm_sub_ps(v1,v0);
	__m128 v = _mm_sub_ps(v2,v0);
	return normalize_ps(crossProduct_ps(u,v));
}

inline __m128 faceCenter_ps(__m128 v0, __m128 v1, __m128 v2) {
	return _mm_div_ps(_mm_add_ps(_mm_add_ps(v0,v1),v2),_mm_set1_ps(3.f));
}

inline bool boundingBoxCollides(__m128 point, __m128i box) {
	__m128 boxOrigin = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(box, box), 0x10u));
	__m128 boxExtends = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(box, box), 0x10u));

	__m128 cos = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(boxOrigin,boxOrigin,_MM_SHUFFLE(3,3,3,3)),
		_mm_set_ps(0,0,-0.000031,-0.000031)),_mm_set_ps(0,1,0,0));
	__m128 sin = _mm_mul_ps(_mm_shuffle_ps(boxExtends, boxExtends, _MM_SHUFFLE(3,3,3,3)), _mm_set_ps(0,0,-0.000031,0.000031));

	__m128 originOffset = _mm_sub_ps(point,boxOrigin);

	__m128 rotatedOriginOffset = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(originOffset, originOffset, _MM_SHUFFLE(2,3,0,1)), sin), _mm_mul_ps(originOffset, cos));
	
	uint32_t mask = _mm_movemask_ps(_mm_cmpgt_ps(abs_ps(rotatedOriginOffset),boxExtends));
	if(mask&0x7)
		return false;

	return true;
}

inline bool cpu_supports_avx() {
#ifdef _MSC_VER
	int info[4];
	__cpuid(info, 1);
	bool avx_cpu = (info[2] & (1 << 28)) != 0;
	bool osxsave = (info[2] & (1 << 27)) != 0;
	if (!avx_cpu || !osxsave) return false;

	uint64_t xcr0 = _xgetbv(_XCR_XFEATURE_ENABLED_MASK);
	return (xcr0 & 0x6) == 0x6;
#else 
	if (__builtin_cpu_supports("avx"))
	{
		return true;
	}
	return false;
#endif
}


inline __m128 loadVector3(Vector3* vec) {
	if (cpu_supports_avx())
	{
		return _mm_maskload_ps(&vec->x,_mm_set_epi32(0,~0,~0,~0));
	}
	else
	{
		return _mm_set_ps(0, vec->z, vec->y, vec->x);
	}
}

}
