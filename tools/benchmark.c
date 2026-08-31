/*
 * benchmark.c
 *
 * Run a benchmark comparing Sirius64 with other popular PRNG.
 *
 * Copyright(C) 2026 Matteo Zapparoli - zapparoli.matteo@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Compile: gcc -O3 -march=native -mtune=native benchmark.c -o benchmark
 *
 */ 
 
#include <stdio.h>
#include <stdint.h> // uint64_t
#include <x86intrin.h> // __rdtscp()
#include <time.h>

#define ITERATIONS 100000000

typedef struct {
	uint64_t (*rng)(void);
	const char *name;
	uint64_t cycles;
} prng_struct_t;

static inline uint64_t ticks(void)
{
	unsigned aux;
	return __rdtscp(&aux);
}

static inline uint64_t rotl64(uint64_t x, int r)
{
    return (x << r) | (x >> ((-r) & 63));
}

static inline uint64_t rotr64(uint64_t x, int r)
{
    return (x << ((-r) & 63)) | (x >> r);
}

void benchmark(prng_struct_t *p)
{
	volatile uint64_t sink = 0;

	uint64_t start = ticks();

	for(size_t i = 0; i < ITERATIONS; i++)
		sink ^= p->rng();

	uint64_t cycles = ticks() - start;

	if(cycles < p->cycles)
		p->cycles = cycles;
	
	if(sink == 0x123456789) printf("ignore\n");
}

// ====================== PRNGs ======================

/////////////////
uint64_t splitmix64(void)
{
    static uint64_t state = 0x123456789ABCDEF0ULL;
    uint64_t z = (state += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}
/////////////////
uint64_t xoshiro256ss(void)
{
    static uint64_t state[4] = {
        0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL,
        0xAABBCCDDEEFF0011ULL, 0x1122334455667788ULL
    };

    uint64_t result = state[1] * 5;
    result = (result << 7) | (result >> 57);
    result *= 9;

    uint64_t t = state[1] << 17;

    state[2] ^= state[0];
    state[3] ^= state[1];
    state[1] ^= state[2];
    state[0] ^= state[3];

    state[2] ^= t;
    state[3] = (state[3] << 45) | (state[3] >> 19);

    return result;
}
/////////////////
uint64_t xoshiro256pp(void)
{
	static uint64_t state[4] = {
        0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL,
        0xAABBCCDDEEFF0011ULL, 0x1122334455667788ULL
    };
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
/////////////////
uint64_t xoroshiro128pp(void)
{
	static uint64_t state[2] = {
		0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL
	};

    const uint64_t s0 = state[0];
    uint64_t s1 = state[1];
    const uint64_t result = rotl64(s0 + s1, 17) + s0;
    s1 ^= s0;
    state[0] = rotl64(s0, 49) ^ s1 ^ (s1 << 21); // a, b
    state[1] = rotl64(s1, 28); // c
    return result; 
}

/////////////////
uint64_t sirius64(void) 
{
	static uint64_t state = 0x123456789ABCDEF0ULL;
	
	uint64_t z = (state += 0x9e3779b97f4a7c15ull);
	z = 0x9e3779b97f4a7c15ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	return 0x9e3779b97f4a7c15ull * (state ^ z ^ (z >> 17));
}
/////////////////
static inline void _wymum(uint64_t *A, uint64_t *B) 
{
	__uint128_t r = *A; 
	r *= *B; 
	*A = (uint64_t)r; 
	*B = (uint64_t)(r >> 64);
}

//multiply and xor mix function, aka MUM
static inline uint64_t _wymix(uint64_t A, uint64_t B) 
{ 
	_wymum(&A, &B); 
	return A ^ B; 
}

// wyrand v.4
uint64_t wyrand(void) 
{
	static uint64_t state = 0x123456789ABCDEF0ULL;
	state += 0xa0761d6478bd642fULL; 
	return _wymix(state, state ^ 0xe7037ed1a0b428dbULL);
}
/////////////////
uint64_t romutrio(void)
{
	static uint64_t state_x = 0x123456789ABCDEF0ULL;
    static uint64_t state_y = 0xFEDCBA9876543210ULL;
    static uint64_t state_z = 0xAABBCCDDEEFF0011ULL;

    uint64_t x = state_x, y = state_y, z = state_z;
    state_x = 15241094284759029579ull * z;
    state_y = rotl64(y - x, 12);
    state_z = rotl64(z - y, 44);
    return x;
}
/////////////////
uint64_t pgc64_rxs_m_xs(void)
{
	static uint64_t state = 0x1234567890abcdef; ///< LCG state
    static uint64_t inc = 1;   ///< LCG increment, must be odd    
	const uint64_t word = ((state >> ((state >> 59) + 5)) ^ state) * 12605985483714917081ull;
	state = state * 6364136223846793005ull + inc;
	return (word >> 43) ^ word; 
}

int main(void)
{
	prng_struct_t prng[] = {
		{splitmix64,   "SplitMix64", UINT64_MAX},
		{xoshiro256ss, "xoshiro256**", UINT64_MAX},
		{xoshiro256pp, "xoshiro256++", UINT64_MAX},
		{xoroshiro128pp, "xoroshiro128++", UINT64_MAX},
		{sirius64, "Sirius64", UINT64_MAX},
		{wyrand, "wyrand v.4", UINT64_MAX},
		{romutrio, "romutrio", UINT64_MAX},
		{pgc64_rxs_m_xs, "Pgc64 RXS-M-XS", UINT64_MAX}
	};
	
	printf("========================================================\n");
    printf("===                 PRNG Benchmark                   ===\n");
    printf("========================================================\n\n");
	
	printf("| Generator      | Cycles/64-bit|Relative Speed|\n");
	printf("|----------------|--------------|--------------|\n");
	
	uint64_t min_cycle = UINT64_MAX;
	for(size_t i = 0; i < 10; i++) {
		for(size_t j = 0; j < sizeof(prng) / sizeof(prng_struct_t); j++) {
			benchmark(&prng[j]);
			if(prng[j].cycles < min_cycle) min_cycle = prng[j].cycles;
		}
	}
	
	for(size_t j = 0; j < sizeof(prng) / sizeof(prng_struct_t); j++) {
		double cyc_per_call = (double)prng[j].cycles / ITERATIONS;
		double rel_speed = 100.0 * (double)min_cycle / (double)prng[j].cycles;
		printf("| %-16s  | %.3f       | %.1f%%\n", prng[j].name, cyc_per_call, rel_speed);
	}

    return 0;
}
