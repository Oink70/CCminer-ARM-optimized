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
#include "sse2neon.h"
#include <stdio.h>
#include "haraka.h"
//#include <stdint.h>


//uint8x16_t _mm_aesenc_si128 (uint8x16_t a, uint8x16_t RoundKey)
//{
//   return vaesmcq_u8(vaeseq_u8(a, (uint8x16_t){})) ^ RoundKey;
//}
typedef unsigned int uint32_t;



void haraka512_keyed(unsigned char *out, const unsigned char *in, const __m128i *rc) {
  __m128i  tmp, t[4];
  //__m128i tmp;

  struct int64x2x4_t s;
  struct int64x2x4_t n;

  s = vld1q_s64_x4((int64_t *)in);

  //s[0] = LOAD(in);
  //s[1] = LOAD(in + 16);
  //s[2] = LOAD(in + 32);
  //s1[3] = LOAD(in + 48);

  AES4(s.val[0], s.val[1], s.val[2], s.val[3], 0);
  MIX4A(s.val[0], s.val[1], s.val[2], s.val[3]);

  AES4(n.val[0], n.val[1], n.val[2], n.val[3], 8);
  MIX4B(n.val[0], n.val[1], n.val[2], n.val[3]);

  AES4(s.val[0], s.val[1], s.val[2], s.val[3], 16);
  MIX4A(s.val[0], s.val[1], s.val[2], s.val[3]);

  AES4(n.val[0], n.val[1], n.val[2], n.val[3], 24);
  MIX4_LAST(n.val[0], n.val[1], n.val[2], n.val[3]);

  AES4_LAST(n.val[0], s.val[1], s.val[2], n.val[3], 32);


 // s[0] = _mm_xor_si128(s[0], LOAD(in));
 // s[1] = _mm_xor_si128(s[1], LOAD(in + 16));
 // s[2] = _mm_xor_si128(s[2], LOAD(in + 32));
 // s[3] = _mm_xor_si128(s[0], LOAD(in + 48));
  ((uint32_t*)&out[0])[7] = ((uint32_t*)&s.val[0])[10] ^ ((uint32_t*)&in[52])[0];

  //TRUNCSTORE(out, s[0],s[1], s[2], s[3]);
}
