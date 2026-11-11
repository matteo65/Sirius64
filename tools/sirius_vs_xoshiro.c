/*
 * sirius_vs_xoshiro.c
 *
 * Run a benchmark comparing Sirius64 with xoshiro256++
 *
 * Copyright(C) 2026 Matteo Zapparoli - zapparoli.matteo@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Compile: gcc -O3 -march=native -fno-tree-vectorize sirius_vs_xoshiro.c -o sirius_vs_xoshiro
 * 
 * Copyright / Authors of PRNGs:
 *
 * sirius64     Copyright by Matteo Zapparoli
 * xoshiro256++ Public Domain by D.Blackman and S.Vigna
 *
 */
 
#include <stdio.h>
#include <stdint.h> // uint64_t
#include <x86intrin.h> // __rdtscp()
#include <time.h>

#define ITERATIONS 100000000

static inline uint64_t ticks(void)
{
	unsigned aux;
	return __rdtscp(&aux);
}

static inline uint64_t rotl64(uint64_t x, int r)
{
	return (x << r) | (x >> ((-r) & 63));
}

// ====================== PRNGs ======================

///////////////// xoshiro256pp
static inline uint64_t xoshiro256pp(uint64_t state[4])
{
	const uint64_t result = rotl64(state[0] + state[3], 23) + state[0];
	const uint64_t t = state[1] << 17;
	state[2] ^= state[0];
	state[3] ^= state[1];
	state[1] ^= state[2];
	state[0] ^= state[3];
	state[2] ^= t;
	state[3] = rotl64(state[3], 45);
	return result; 
}

///////////////// sirius64
static inline uint64_t sirius64(uint64_t *state) 
{
	uint64_t z = (*state += 0x9e3779b97f4a7c15ull);
	z = 0x9e3779b97f4a7c15ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	return 0x9e3779b97f4a7c15ull * (*state ^ z ^ (z >> 17));
}

int main(void)
{
	uint64_t sirius_min_cycle = UINT64_MAX;
	
	for(size_t i = 0; i < 20; i++) {
		uint64_t state = 0x123456789ABCDEF0ULL;
		uint64_t sink = 0;

		uint64_t start = ticks();

		for(size_t k = 0; k < ITERATIONS; k++)
			sink ^= sirius64(&state);

		uint64_t cycles = ticks() - start;

		asm volatile("" : "+r"(sink));

		if(cycles < sirius_min_cycle)
			sirius_min_cycle = cycles;
	}

	uint64_t xoshiro_min_cycle = UINT64_MAX;
	
	for(size_t i = 0; i < 20; i++) {
		uint64_t xoshiro_state[4] = {
			0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL,
			0xAABBCCDDEEFF0011ULL, 0x1122334455667788ULL
		};

		uint64_t sink = 0;

		uint64_t start = ticks();

		for(size_t k = 0; k < ITERATIONS; k++)
			sink ^= xoshiro256pp(xoshiro_state);

		uint64_t cycles = ticks() - start;
		
		asm volatile("" : "+r"(sink));

		if(cycles < xoshiro_min_cycle)
			xoshiro_min_cycle = cycles;
	}
	
	printf("\n\n");
	printf("| Generator    | Cycles/64-bit |\n");
	printf("|--------------|---------------|\n");
	printf("| xoshiro256++ | %.3f         |\n", (double)xoshiro_min_cycle / ITERATIONS);
	printf("| sirius64     | %.3f         |\n", (double)sirius_min_cycle / ITERATIONS);

	return 0;
}
