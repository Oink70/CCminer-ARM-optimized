/*
 * This uses veriations of the clhash algorithm for Verus Coin, licensed
 * with the Apache-2.0 open source license.
 * 
 * Copyright (c) 2018 Michael Toutonghi
 * Distributed under the Apache 2.0 software license, available in the original form for clhash
 * here: https://github.com/lemire/clhash/commit/934da700a2a54d8202929a826e2763831bd43cf7#diff-9879d6db96fd29134fc802214163b95a
 * 
 * CLHash is a very fast hashing function that uses the
 * carry-less multiplication and SSE instructions.
 *
 * Original CLHash code (C) 2017, 2018 Daniel Lemire and Owen Kaser
 * Faster 64-bit universal hashing
 * using carry-less multiplications, Journal of Cryptographic Engineering (to appear)
 *
 * Best used on recent x64 processors (Haswell or better).
 *
 **/

#include "sse2neon.h"
uint64_t verusclhash_port2_1(void * random, const unsigned char buf[64], uint64_t keyMask, uint16_t *  __restrict fixrand, uint16_t * __restrict fixrandex);
uint64_t verusclhash_port2_2(void * random, const unsigned char buf[64], uint64_t keyMask, uint16_t *  __restrict fixrand, uint16_t * __restrict fixrandex, __m128i *g_prand, __m128i *g_prandex);
