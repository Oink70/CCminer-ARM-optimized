
/*
The MIT License (MIT)

Copyright (c) 2016 kste

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Optimized Implementations for Haraka256 and Haraka512
*/
#ifndef HARAKA_H_
#define HARAKA_H_

//#include "SSE2NEON.h"

#define NUMROUNDS 5

#ifdef _WIN32
typedef unsigned long long u64;
#else
typedef unsigned long u64;
#endif
typedef __m128i u128;

//extern u128 rc[40];

#define LOAD(src) _mm_load_si128((u128 *)(src))
#define STORE(dest,src) _mm_storeu_si128((u128 *)(dest),src)

#define AES2(s0, s1, rci) \
  s0 = _mm_aesenc_si128(s0, rc[rci]); \
  s1 = _mm_aesenc_si128(s1, rc[rci + 1]); \
  s0 = _mm_aesenc_si128(s0, rc[rci + 2]); \
  s1 = _mm_aesenc_si128(s1, rc[rci + 3]);

#define AES2_4x(s0, s1, s2, s3, rci) \
  AES2(s0[0], s0[1], rci); \
  AES2(s1[0], s1[1], rci); \
  AES2(s2[0], s2[1], rci); \
  AES2(s3[0], s3[1], rci);

#define AES2_8x(s0, s1, s2, s3, s4, s5, s6, s7, rci) \
  AES2_4x(s0, s1, s2, s3, rci); \
  AES2_4x(s4, s5, s6, s7, rci);

#define AES4(s0, s1, s2, s3, rci) \
  s0 = _mm_aesenc_si128(s0, rc[rci]); \
  s1 = _mm_aesenc_si128(s1, rc[rci + 1]); \
  s2 = _mm_aesenc_si128(s2, rc[rci + 2]); \
  s3 = _mm_aesenc_si128(s3, rc[rci + 3]); \
  s0 = _mm_aesenc_si128(s0, rc[rci + 4]); \
  s1 = _mm_aesenc_si128(s1, rc[rci + 5]); \
  s2 = _mm_aesenc_si128(s2, rc[rci + 6]); \
  s3 = _mm_aesenc_si128(s3, rc[rci + 7]); \

#define AES4_zero(s0, s1, s2, s3, rci) \
  s0 = _mm_aesenc_si128(s0, rc0[rci]); \
  s1 = _mm_aesenc_si128(s1, rc0[rci + 1]); \
  s2 = _mm_aesenc_si128(s2, rc0[rci + 2]); \
  s3 = _mm_aesenc_si128(s3, rc0[rci + 3]); \
  s0 = _mm_aesenc_si128(s0, rc0[rci + 4]); \
  s1 = _mm_aesenc_si128(s1, rc0[rci + 5]); \
  s2 = _mm_aesenc_si128(s2, rc0[rci + 6]); \
  s3 = _mm_aesenc_si128(s3, rc0[rci + 7]); \

#define AES4_4x(s0, s1, s2, s3, rci) \
  AES4(s0[0], s0[1], s0[2], s0[3], rci); \
  AES4(s1[0], s1[1], s1[2], s1[3], rci); \
  AES4(s2[0], s2[1], s2[2], s2[3], rci); \
  AES4(s3[0], s3[1], s3[2], s3[3], rci);

#define AES4_8x(s0, s1, s2, s3, s4, s5, s6, s7, rci) \
  AES4_4x(s0, s1, s2, s3, rci); \
  AES4_4x(s4, s5, s6, s7, rci);

//#define MIX2(s0, s1) \
//  tmp = _mm_unpacklo_epi32(s0, s1); \
//  s1 = _mm_unpackhi_epi32(s0, s1); \
//  s0 = tmp;

//#define MIX4(s0, s1, s2, s3) \
//  tmp  = _mm_unpacklo_epi32(s0, s1); \
//  s0 = _mm_unpackhi_epi32(s0, s1); \
//  s1 = _mm_unpacklo_epi32(s2, s3); \
//  s2 = _mm_unpackhi_epi32(s2, s3); \
//  s3 = _mm_unpacklo_epi32(s0, s2); \
//  s0 = _mm_unpackhi_epi32(s0, s2); \
//  s2 = _mm_unpackhi_epi32(s1, tmp); \
//  s1 = _mm_unpacklo_epi32(s1, tmp);

//#define MIX4(s0, s1, s2, s3) \
//	tmp = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), 0, 4, 1, 5 ); \
//	s0 = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), 2, 6, 3, 7 ); \
//	s1 = __builtin_shufflevector(vreinterpretq_s32_s64(s2), vreinterpretq_s32_s64(s3), 0, 4, 1, 5 ); \
//	s2 = __builtin_shufflevector(vreinterpretq_s32_s64(s2), vreinterpretq_s32_s64(s3), 2, 6, 3, 7 ); \
//	s3 = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s2), 0, 4, 1, 5 ); \
//	s0 = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s2), 2, 6, 3, 7 ); \
//	s2 = __builtin_shufflevector(vreinterpretq_s32_s64(s1), vreinterpretq_s32_s64(tmp), 2, 6, 3, 7 ); \
//	s1 = __builtin_shufflevector(vreinterpretq_s32_s64(s1), vreinterpretq_s32_s64(tmp), -1, 4, 1, 5 ); 

//#define MIX4(s0, s1, s2, s3) \
//  s1 = s2.s0, s3.s0, s2.s1, s3.s1
//  s2 = s2.s2, s3.s2, s2.s3, s3.s3
//
//  s3 = s0.s2, s2.s2, s1.s2, s3.s2  //s3 & s0 are the same sequence wise
//  s0 = s0.s3, s2.s3, s1.s3, s3.s3  //s1 & s2 are the same sequence wise
//  s2 = s2.s1, s0.s1, s3.s1, s1.s1
//  s1 = s2.s0, s0.s0, s3.s0, s1.s0

#define MIX4A(s0, s1, s2, s3) \
  	n.val[0] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(n.val[0], 0, s0, 3), 1, s2, 3), 2, s1, 3), 3, s3, 3); \
  	n.val[1] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(n.val[1], 0, s2, 0), 1, s0, 0), 2, s3, 0), 3, s1, 0); \
  	n.val[2] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(n.val[2], 0, s2, 1), 1, s0, 1), 2, s3, 1), 3, s1, 1); \
  	n.val[3] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(n.val[3], 0, s0, 2), 1, s2, 2), 2, s1, 2), 3, s3, 2); 

#define MIX4B(s0, s1, s2, s3) \
  	s.val[0] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(s.val[0], 0, s0, 3), 1, s2, 3), 2, s1, 3), 3, s3, 3); \
  	s.val[1] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(s.val[1], 0, s2, 0), 1, s0, 0), 2, s3, 0), 3, s1, 0); \
  	s.val[2] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(s.val[2], 0, s2, 1), 1, s0, 1), 2, s3, 1), 3, s1, 1); \
  	s.val[3] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(s.val[3], 0, s0, 2), 1, s2, 2), 2, s1, 2), 3, s3, 2); 

//#define MIX4A(s0, s1, s2, s3) \
//	n.val[0] = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), 3, -1, 7, -1 ); \
//	n.val[0] = __builtin_shufflevector(vreinterpretq_s32_s64(n.val[0]), vreinterpretq_s32_s64(s2), 0, 7, 2, -1 ); \
//	n.val[0] = __builtin_shufflevector(vreinterpretq_s32_s64(n.val[0]), vreinterpretq_s32_s64(s3), 0, 1, 2, 7 ); \
//	n.val[1] = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), -1, 0, -1, 4 ); \
//	n.val[1] = __builtin_shufflevector(vreinterpretq_s32_s64(n.val[1]), vreinterpretq_s32_s64(s2), 4, 1, -1, 3 ); \
//	n.val[1] = __builtin_shufflevector(vreinterpretq_s32_s64(n.val[1]), vreinterpretq_s32_s64(s3), 0, 1, 4, 3 ); \
//	n.val[2] = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), -1, 1, -1, 5 ); \
//	n.val[2] = __builtin_shufflevector(vreinterpretq_s32_s64(n.val[2]), vreinterpretq_s32_s64(s2), 5, 1, -1, 3 ); \
//	n.val[2] = __builtin_shufflevector(vreinterpretq_s32_s64(n.val[2]), vreinterpretq_s32_s64(s3), 0, 1, 5, 3 ); \
//	n.val[3] = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), 2, -1, 6, -1 ); \
//	n.val[3] = __builtin_shufflevector(vreinterpretq_s32_s64(n.val[3]), vreinterpretq_s32_s64(s2), 0, 6, 2, -1 ); \
//	n.val[3] = __builtin_shufflevector(vreinterpretq_s32_s64(n.val[3]), vreinterpretq_s32_s64(s3), 0, 1, 2, 6 ); 

  
//uint8x16_t gets0 = { 12, 13, 14, 15, 44, 45, 46, 47, 28, 29, 30, 31, 60, 61, 62, 63 };
//uint8x16_t gets1 = { 32, 33, 34, 35, 0, 1, 2, 3, 48, 49, 50, 51, 16, 17, 18, 19 };
//uint8x16_t gets2 = { 36, 37, 38, 39, 4, 5, 6, 7, 52, 53, 54, 55, 20, 21, 22, 23};
//uint8x16_t gets3 = {  8,  9, 10, 11, 40, 41, 42, 43, 24, 25, 26, 27, 56, 57, 58, 59 };
// Using tbl was much slower when I tested it.  
// #define MIX4A(X) \
//      n.n8.val[0] = vqtbl4q_s8(  X, gets0  ); \
//      n.n8.val[1] = vqtbl4q_s8(  X, gets1  ); \
//      n.n8.val[2] = vqtbl4q_s8(  X, gets2  ); \
//      n.n8.val[3] = vqtbl4q_s8(  X, gets3  );

/*
The MIT License (MIT)

Copyright (c) 2016 kste

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Optimized Implementations for Haraka256 and Haraka512
*/
#ifdef HARAKA_H_


//#include "immintrin.h"

#define NUMROUNDS 5

#ifdef _WIN32
typedef unsigned long long u64;
#else
typedef unsigned long u64;
#endif
typedef __m128i u128;

extern __m128i rc[40];

#define LOAD(src) _mm_load_si128((u128 *)(src))
#define STORE(dest,src) _mm_storeu_si128((u128 *)(dest),src)

#define AES2(s0, s1, rci) \
  s0 = _mm_aesenc_si128(s0, rc[rci]); \
  s1 = _mm_aesenc_si128(s1, rc[rci + 1]); \
  s0 = _mm_aesenc_si128(s0, rc[rci + 2]); \
  s1 = _mm_aesenc_si128(s1, rc[rci + 3]);

#define AES2_4x(s0, s1, s2, s3, rci) \
  AES2(s0[0], s0[1], rci); \
  AES2(s1[0], s1[1], rci); \
  AES2(s2[0], s2[1], rci); \
  AES2(s3[0], s3[1], rci);

#define AES2_8x(s0, s1, s2, s3, s4, s5, s6, s7, rci) \
  AES2_4x(s0, s1, s2, s3, rci); \
  AES2_4x(s4, s5, s6, s7, rci);

//#define AES4(s0, s1, s2, s3, rci) \
//  s0 = _mm_aesenc_si128(s0, rc[rci]); \
//  s1 = _mm_aesenc_si128(s1, rc[rci + 1]); \
//  s2 = _mm_aesenc_si128(s2, rc[rci + 2]); \
//  s3 = _mm_aesenc_si128(s3, rc[rci + 3]); \
//  s0 = _mm_aesenc_si128(s0, rc[rci + 4]); \
//  s1 = _mm_aesenc_si128(s1, rc[rci + 5]); \
//  s2 = _mm_aesenc_si128(s2, rc[rci + 6]); \
//  s3 = _mm_aesenc_si128(s3, rc[rci + 7]); \

#define AES4_LAST(s0, s1, s2, s3, rci) \
  s2 = _mm_aesenc_si128(s2, rc[rci + 2]); \
  s2 = _mm_aesenc_si128(s2, rc[rci + 6]); 


#define AES4_zero(s0, s1, s2, s3, rci) \
  s0 = _mm_aesenc_si128(s0, rc0[rci]); \
  s1 = _mm_aesenc_si128(s1, rc0[rci + 1]); \
  s2 = _mm_aesenc_si128(s2, rc0[rci + 2]); \
  s3 = _mm_aesenc_si128(s3, rc0[rci + 3]); \
  s0 = _mm_aesenc_si128(s0, rc0[rci + 4]); \
  s1 = _mm_aesenc_si128(s1, rc0[rci + 5]); \
  s2 = _mm_aesenc_si128(s2, rc0[rci + 6]); \
  s3 = _mm_aesenc_si128(s3, rc0[rci + 7]); \

#define AES4_4x(s0, s1, s2, s3, rci) \
  AES4(s0[0], s0[1], s0[2], s0[3], rci); \
  AES4(s1[0], s1[1], s1[2], s1[3], rci); \
  AES4(s2[0], s2[1], s2[2], s2[3], rci); \
  AES4(s3[0], s3[1], s3[2], s3[3], rci);

#define AES4_8x(s0, s1, s2, s3, s4, s5, s6, s7, rci) \
  AES4_4x(s0, s1, s2, s3, rci); \
  AES4_4x(s4, s5, s6, s7, rci);

#define MIX2(s0, s1) \
  tmp = _mm_unpacklo_epi32(s0, s1); \
  s1 = _mm_unpackhi_epi32(s0, s1); \
  s0 = tmp;

//#define MIX4(s0, s1, s2, s3) \
//  tmp  = _mm_unpacklo_epi32(s0, s1); \
//  s0 = _mm_unpackhi_epi32(s0, s1); \
//  s1 = _mm_unpacklo_epi32(s2, s3); \
//  s2 = _mm_unpackhi_epi32(s2, s3); \
//  s3 = _mm_unpacklo_epi32(s0, s2); \
//  s0 = _mm_unpackhi_epi32(s0, s2); \
//  s2 = _mm_unpackhi_epi32(s1, tmp); \
//  s1 = _mm_unpacklo_epi32(s1, tmp);

//#define MIX4_LAST(s0, s1, s2, s3) \
//  tmp  = _mm_unpacklo_epi32(s0, s1); \
//  s1 = _mm_unpacklo_epi32(s2, s3); \
//  s2 = _mm_unpackhi_epi32(s1, tmp); 

//#define MIX4_LAST(s0, s1, s2, s3) \
//  tmp  = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), -1, -1, 1, 5); \
//  s1 = _mm_unpacklo_epi32(s2, s3); \
//  s2 = _mm_unpackhi_epi32(s1, tmp); 

//#define MIX4_LAST(s0, s1, s2, s3) \
//  tmp  = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), -1, -1, 1, 5); \
//  s1 = __builtin_shufflevector(vreinterpretq_s32_s64(s2), vreinterpretq_s32_s64(s3), 0, 4, 1, 5); \
//  s2 = __builtin_shufflevector(vreinterpretq_s32_s64(s1), vreinterpretq_s32_s64(tmp), 2, 6, 3, 7); 
  
#define MIX4_LAST(s0, s1, s2, s3) \
  	s.val[1] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(s.val[1], 0, s2, 0), 1, s3, 0), 2, s2, 1), 3, s3, 1); \
  	s.val[2] = vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(vcopyq_laneq_s32(s.val[2], 0, s2, 1), 1, s0, 1), 2, s3, 1), 3, s1, 1); 
//  	s.val[0] = s0; \
//  	s.val[3] = s3; 

//tmp = s0.0, s1.0, s0.1, s1.1
//s1 = s2.0, s3.0, s2.1, s3.1
//s2 = s2.1, s0.1, s3.1, s1.1
//s0 unchanged
//s3 unchanged

#define TRUNCSTORE(out, s0, s1, s2, s3) \
  *(u64*)(out) = *(((u64*)&s0 + 1)); \
  *(u64*)(out + 8) = *(((u64*)&s1 + 1)); \
  *(u64*)(out + 16) = *(((u64*)&s2 + 0)); \
  *(u64*)(out + 24) = *(((u64*)&s3 + 0));

void load_constants();
void test_implementations();

void load_constants();

void haraka256(unsigned char *out, const unsigned char *in);
void haraka256_keyed(unsigned char *out, const unsigned char *in, const u128 *rc);
void haraka256_4x(unsigned char *out, const unsigned char *in);
void haraka256_8x(unsigned char *out, const unsigned char *in);

void haraka512(unsigned char *out, const unsigned char *in);
void haraka512_zero(unsigned char *out, const unsigned char *in);
void haraka512_keyed(unsigned char *out, const unsigned char *in, const u128 *rc);
void haraka512_4x(unsigned char *out, const unsigned char *in);
void haraka512_8x(unsigned char *out, const unsigned char *in);

#endif


void load_constants();
void test_implementations();

void load_constants();

void haraka256(unsigned char *out, const unsigned char *in);
void haraka256_keyed(unsigned char *out, const unsigned char *in, const u128 *rc);
void haraka256_4x(unsigned char *out, const unsigned char *in);
void haraka256_8x(unsigned char *out, const unsigned char *in);

#endif
