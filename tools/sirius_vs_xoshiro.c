/*
 * sirius_vs_xoshiro.c
 *
 * Run a benchmark comparing Sirius64 and xoshiro256++
 *
 * Copyright(C) 2026 Matteo Zapparoli - zapparoli.matteo@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Compile: gcc -O3 -march=native -fno-tree-vectorize sirius_vs_xoshiro.c -o sirius_vs_xoshiro -lm
 * 
 * Copyright / Authors of PRNGs:
 *
 * sirius64     Copyright by Matteo Zapparoli
 * xoshiro256++ Public Domain by D.Blackman and S.Vigna
 *
 * The following report presents a performance comparison between the 
 * Sirius64 and xoshiro256++ pseudorandom number generators (PRNGs).
 * Performance is measured in TSC ticks per 64-bit output, using sequential state-dependent 
 * generation. Each generator is benchmarked over 100 million iterations per session, 
 * with 20 measurement sessions performed on a CPU core pinned to a fixed logical processor.
 * The report includes the minimum, average, and maximum measured TSC ticks per generated 64-bit value.
 *
 * Performance was measured using a single state-dependent generation stream. 
 * Since every output depends on the updated internal state from the previous step, 
 * the benchmark represents the cost of serialized generation and is therefore closer 
 * to latency than to peak throughput.
 *
 */
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <x86intrin.h>

#define WARM_UP_ITERS     10000000   // 10 milioni invece di 100
#define SESSIONS          30
#define ITERATIONS        100000000
#define CPU_CORE 2

static inline uint64_t ticks(void)
{
    unsigned aux;
    _mm_lfence();
    uint64_t t = __rdtscp(&aux);
    _mm_lfence();
    return t;
}

static inline uint64_t rotl64(uint64_t x, int r)
{
    return (x << r) | (x >> ((-r) & 63));
}

// ====================== PRNGs ======================

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

static inline uint64_t sirius64(uint64_t *state)
{
    uint64_t z = (*state += 0x9e3779b97f4a7c15ull);
    z = 0x9e3779b97f4a7c15ull * (z ^ (z >> 17));
    z = (z << 32) | (z >> 32);
    return 0x9e3779b97f4a7c15ull * (*state ^ z ^ (z >> 17));
}

static uint64_t check_sirius(void)
{
    uint64_t state = 0x123456789ABCDEF0ULL;
    uint64_t sink = 0;
    uint64_t start = ticks();
    for (size_t k = 0; k < ITERATIONS; k++)
        sink ^= sirius64(&state);
    uint64_t end = ticks();
    asm volatile("" : "+r"(sink) : : "memory");
	
	return end - start;
}

static uint64_t check_xoshiro(void)
{
    uint64_t xstate[4] = {
        0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL,
        0xAABBCCDDEEFF0011ULL, 0x1122334455667788ULL
    };
    uint64_t sink = 0;
    uint64_t start = ticks();
    for (size_t k = 0; k < ITERATIONS; k++)
        sink ^= xoshiro256pp(xstate);
    uint64_t end = ticks();
    asm volatile("" : "+r"(sink) : : "memory");

	return end - start;
}

int main(void)
{
    // Pin to core CPU_CORE
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(CPU_CORE, &set);
    if (sched_setaffinity(0, sizeof(set), &set) != 0) {
        perror("sched_setaffinity");
        return 1;
    }
	
	double sirius_res[SESSIONS];
    double xoshiro_res[SESSIONS];

    uint64_t sirius_min = UINT64_MAX, sirius_max = 0;
    uint64_t xoshiro_min = UINT64_MAX, xoshiro_max = 0;
	
    // ---------- Warm-up ----------
	for (size_t i = 0; i < 3; i++) {
        uint64_t state = 0x123456789ABCDEF0ULL;
        uint64_t sink = 0;
        for (size_t k = 0; k < WARM_UP_ITERS; k++)
            sink ^= sirius64(&state);
        asm volatile("" : "+r"(sink));

        uint64_t xstate[4] = {
            0x123456789ABCDEF0ULL, 0xFEDCBA9876543210ULL,
            0xAABBCCDDEEFF0011ULL, 0x1122334455667788ULL
        };
        sink = 0;
        for (size_t k = 0; k < WARM_UP_ITERS; k++)
            sink ^= xoshiro256pp(xstate);
        asm volatile("" : "+r"(sink));
    }

    // ---------- Measurement ----------
    for (size_t i = 0; i < SESSIONS; i++) {
		uint64_t cycles;
		
		if ((i & 1) == 0) {
            cycles = check_sirius();
            sirius_res[i] = (double)cycles / ITERATIONS;
            if (cycles < sirius_min) sirius_min = cycles;
            if (cycles > sirius_max) sirius_max = cycles;

            cycles = check_xoshiro();
            xoshiro_res[i] = (double)cycles / ITERATIONS;
            if (cycles < xoshiro_min) xoshiro_min = cycles;
            if (cycles > xoshiro_max) xoshiro_max = cycles;
        } else {
            cycles = check_xoshiro();
            xoshiro_res[i] = (double)cycles / ITERATIONS;
            if (cycles < xoshiro_min) xoshiro_min = cycles;
            if (cycles > xoshiro_max) xoshiro_max = cycles;

            cycles = check_sirius();
            sirius_res[i] = (double)cycles / ITERATIONS;
            if (cycles < sirius_min) sirius_min = cycles;
            if (cycles > sirius_max) sirius_max = cycles;
        }
    }

	// ---------- Statistics ----------
    double sirius_sum = 0, xoshiro_sum = 0;
    for (size_t i = 0; i < SESSIONS; i++) {
        sirius_sum += sirius_res[i];
        xoshiro_sum += xoshiro_res[i];
    }
    double sirius_avg = sirius_sum / SESSIONS;
    double xoshiro_avg = xoshiro_sum / SESSIONS;

    double sirius_sq_diff = 0, xoshiro_sq_diff = 0;
    for (size_t i = 0; i < SESSIONS; i++) {
        sirius_sq_diff += pow(sirius_res[i] - sirius_avg, 2);
        xoshiro_sq_diff += pow(xoshiro_res[i] - xoshiro_avg, 2);
    }
    double sirius_stddev = sqrt(sirius_sq_diff / (SESSIONS - 1));
    double xoshiro_stddev = sqrt(xoshiro_sq_diff / (SESSIONS - 1));

    // ---------- Results ----------
    printf("Performance measured in TSC ticks per 64-bit output over %d sessions (%d iter/session).\n\n", SESSIONS, ITERATIONS);
    printf("| Generator    | Min TSC/64b | Avg TSC/64b | Max TSC/64b | StdDev      |\n");
    printf("|--------------|-------------|-------------|-------------|-------------|\n");
    printf("| xoshiro256++ | %11.3f | %11.3f | %11.3f | %11.3f |\n",
           (double)xoshiro_min / ITERATIONS, xoshiro_avg, (double)xoshiro_max / ITERATIONS, xoshiro_stddev);
    printf("| sirius64     | %11.3f | %11.3f | %11.3f | %11.3f |\n",
           (double)sirius_min / ITERATIONS, sirius_avg, (double)sirius_max / ITERATIONS, sirius_stddev);
    printf("\n");

    return 0;
}

