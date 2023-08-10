#ifndef SPX_HARAKA_H
#define SPX_HARAKA_H

//#include "immintrin.h"
#include "sse2neon.h"
#include "arm_neon.h"
// typedef int32x4_t __m128i;

#define NUMROUNDS 5

#ifdef _WIN32
typedef unsigned long long u64;
#else
typedef unsigned long u64;
#endif

//extern void aesenc(unsigned char *s, const unsigned char *rk);
void aesenc(unsigned char *s, const unsigned char *rk) ;
#define AES2_EMU(s0, s1, rci) \
  aesenc((unsigned char *)&s0, (unsigned char *)&(rc[rci])); \
  aesenc((unsigned char *)&s1, (unsigned char *)&(rc[rci + 1])); \
  aesenc((unsigned char *)&s0, (unsigned char *)&(rc[rci + 2])); \
  aesenc((unsigned char *)&s1, (unsigned char *)&(rc[rci + 3]));

typedef unsigned int uint32_t;

static inline __m128i _mm_unpacklo_epi32_emu(__m128i a, __m128i b)
{
    uint32_t result[4];
    uint32_t *tmp1 = (uint32_t *)&a, *tmp2 = (uint32_t *)&b;
    result[0] = tmp1[0];
    result[1] = tmp2[0];
    result[2] = tmp1[1];
    result[3] = tmp2[1];
    return *(__m128i *)result;
}

static inline __m128i _mm_unpackhi_epi32_emu(__m128i a, __m128i b)
{
    uint32_t result[4];
    uint32_t *tmp1 = (uint32_t *)&a, *tmp2 = (uint32_t *)&b;
    result[0] = tmp1[2];
    result[1] = tmp2[2];
    result[2] = tmp1[3];
    result[3] = tmp2[3];
    return *(__m128i *)result;
}

#define MIX2_EMU1(s0, s1) \
  temp1b = _mm_unpacklo_epi32(s0, s1); \
  temp2b = _mm_unpackhi_epi32(s0, s1); 

#define MIX2_EMU2(s0, s1) \
  temp1a = _mm_unpacklo_epi32(s0, s1); \
  temp2a = _mm_unpackhi_epi32(s0, s1); 

#define MIX2_EMU(s0, s1) \
  tmp = _mm_unpacklo_epi32(s0, s1); \
  s1 = _mm_unpackhi_epi32(s0, s1); \
  s0 = tmp;
  
//#define MIX2_EMU(s0, s1) \
//  tmp = _mm_unpacklo_epi32(s0, s1); \
//  s1 = _mm_unpackhi_epi32(s0, s1); \
//  s0 = __builtin_shufflevector(vreinterpretq_s32_s64(tmp), vreinterpretq_s32_s64(tmp), -1, 1, 2, 3);

//#define MIX2_EMU(s0, s1) \
//  tmp = __builtin_shufflevector(vreinterpretq_s32_s64(s1), vreinterpretq_s32_s64(s1), 0, 1, -1, -1) ; \
//  s1 = _mm_unpackhi_epi32(s0, s1); \
//  s0 = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(tmp), -1, 4, 1, 5);  

// s0 = s0.s0, s1.s0, s0.s1, s1.s1
// s1 = s0.s2, s1.s2, s0.s3, s1.s3

//best
#define MIX2_EMU(s0, s1) \
  tmp = __builtin_shufflevector(s1, s1, 0, -1) ; \
  s1 = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), 2, 6, 3, -1); \
  s0 = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(tmp), -1, 4, 1, 5);  

//#define MIX2_EMU(s0, s1) \
//  tmp = __builtin_shufflevector(vreinterpretq_s32_s64(s1), vreinterpretq_s32_s64(s1), 0, 1, -1, -1) ; \
//  s1 = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(s1), 2, 6, 3, -1);  \
//  s0 = __builtin_shufflevector(vreinterpretq_s32_s64(s0), vreinterpretq_s32_s64(tmp), -1, 4, 1, 5);  

//  s0 = _mm_unpacklo_epi32(s0, tmp);
  
//#define MIX2_EMU(s0, s1) \
//  tmp = vcopyq_laneq_s64(tmp, 0, s1, 0); \
//  s1 = _mm_unpackhi_epi32(s0, s1); \
//  s0 = _mm_unpacklo_epi32(s0, tmp);
  
/* load constants */

void load_constants_port();

/* Tweak constants with seed */
void tweak_constants(const unsigned char *pk_seed, const unsigned char *sk_seed, 
	                 unsigned long long seed_length);

/* Haraka Sponge */
void haraka_S(unsigned char *out, unsigned long long outlen,
              const unsigned char *in, unsigned long long inlen);

/* Applies the 512-bit Haraka permutation to in. */
void haraka512_perm(unsigned char *out, const unsigned char *in);

/* Implementation of Haraka-512 */
void haraka512_port(unsigned char *out, const unsigned char *in);

/* Implementation of Haraka-512 */
void haraka512_port_keyed(unsigned char *out, const unsigned char *in, const __m128i *rc);

/* Applies the 512-bit Haraka permutation to in, using zero key. */
void haraka512_perm_zero(unsigned char *out, const unsigned char *in);

/* Implementation of Haraka-512, using zero key */
void haraka512_port_zero(unsigned char *out, const unsigned char *in);

/* Implementation of Haraka-256 */
void haraka256_port(unsigned char *out, const unsigned char *in);

/* Implementation of Haraka-256 using sk.seed constants */
void haraka256_sk(unsigned char *out, const unsigned char *in);


#endif
