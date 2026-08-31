/*
 * sirius64gen.c
 *
 * Prints a continuous stream of random numbers 64bit generated with sirius64 PRNG to stdout
 *
 * Copyright(C) 2026 Matteo Zapparoli - zapparoli.matteo@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Compile: gcc -O3 sirius64gen.c -o sirius64gen
 *
 * Sintax:
 *  sirius64gen [-r | <seed> [-low32 | -mid32 | -high32]  [-reverse]]
 *
 *  sirius64gen                  Print help
 *  sirius64gen -r               Random seed (print value on stderr)
 *  sirius64gen <seed>           Seed must be non-zero
 *
 *  option -low32:  write on stdout the lowest 32 bit instead full 64 bit
 *  option -mid32:  write on stdout the middle 32 bit instead full 64 bit
 *  option -high32: write on stdout the highest 32 bit instead full 64 bit
 *  option -reverse: reverse bit
 *
 */

#include <stdio.h> // fwrite()
#include <string.h> // strcmp()
#include <stdint.h> // uint64_t
#include <inttypes.h> // PRIu64
#include <time.h> // clock_gettime()
#include <stdlib.h> // strtoull()
#include <errno.h> // errno
#include <ctype.h> // isdigit()

#define BUF_SIZE 65536

uint64_t sirius64(uint64_t *state)
{
	uint64_t z = (*state += 0x9e3779b97f4a7c15ull);
	z = 0x9e3779b97f4a7c15ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	return 0x9e3779b97f4a7c15ull * ((*state) ^ z ^ (z >> 17));
}

void usage(void)
{
	printf("sirius64gen - Continuous sirius64 stdout pseudo number 64bit generator in binary format\n");
	printf("Usage:\n");
	printf("      sirius64gen [-r | <seed> [-low32 | -mid32 | -high32] [-reverse]]\n");
	printf("\n");
	printf("      sirius64gen           Print help\n");
	printf("      sirius64gen -r        Random seed (print value on stderr)\n");
	printf("      sirius64gen <seed>    Seed must be non-zero\n");
	printf("\n");
	printf("option -low32: write on stdout the lowest 32 bit instead full 64 bit\n");
	printf("option -mid32:  write on stdout the middle 32 bit instead full 64 bit\n");
	printf("option -high32: write on stdout the highest 32 bit instead full 64 bit\n");
	printf("option -reverse: reverse bit\n");
}

// Return 1 id str a uint64_t, 0 else
int is_valid_uint64(const char *str, uint64_t *out_value)
{
	if (str == NULL || *str == '\0') return 0;

	// Controlla che la stringa contenga solo cifre
	for (const char *p = str; *p; p++) {
		if (!isdigit((unsigned char)*p)) return 0;
	}

	errno = 0;
	char *end;
	unsigned long long v = strtoull(str, &end, 10);

	// end non deve spostarsi oltre la fine
	if (*end != '\0') return 0;

	// Overflow rilevato da errno
	if (errno == ERANGE) return 0;

	// Controllo ulteriore: strtoull potrebbe essere > UINT64_MAX su alcune piattaforme
	if (v > UINT64_MAX) return 0;

	if (out_value) *out_value = (uint64_t)v;

	return 1;
}

// Return a simple "pseudo-random" based on clock
uint64_t random_from_clock(void)
{
	static uint64_t counter = 0;
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	uint64_t x = ((uint64_t)ts.tv_sec << 32) ^ (uint64_t)ts.tv_nsec;

	// aggiungi un contatore per garantire unicità
	x ^= (++counter * 0x9E3779B97F4A7C15ULL);

	// mixing forte (stile splitmix64)
	x ^= x >> 33;
	x *= 0xff51afd7ed558ccdULL;
	x ^= x >> 33;
	x *= 0xc4ceb9fe1a85ec53ULL;
	x ^= x >> 33;

	return x;
}

// Return -32 if option = "-low32"
//        +32 if option = "-high32"
//          1 if option = "-mid32"
//          0 else
int check_low_high(char *option)
{
	if(strcmp(option, "-low32") == 0) return -32;

	if(strcmp(option, "-high32") == 0) return +32;

	if(strcmp(option, "-mid32") == 0) return 1;

	return 0;
}

uint64_t bit_reverse64(uint64_t x)
{
    x = ((x & 0x5555555555555555ULL) << 1) | ((x & 0xAAAAAAAAAAAAAAAAULL) >> 1);
    x = ((x & 0x3333333333333333ULL) << 2) | ((x & 0xCCCCCCCCCCCCCCCCULL) >> 2);
    x = ((x & 0x0F0F0F0F0F0F0F0FULL) << 4) | ((x & 0xF0F0F0F0F0F0F0F0ULL) >> 4);
    x = ((x & 0x00FF00FF00FF00FFULL) << 8) | ((x & 0xFF00FF00FF00FF00ULL) >> 8);
    x = ((x & 0x0000FFFF0000FFFFULL) << 16) | ((x & 0xFFFF0000FFFF0000ULL) >> 16);
    return (x << 32) | (x >> 32);
}

uint32_t bit_reverse32(uint32_t x) {
    x = ((x & 0x55555555U) << 1)  | ((x & 0xAAAAAAAAU) >> 1);
    x = ((x & 0x33333333U) << 2)  | ((x & 0xCCCCCCCCU) >> 2);
    x = ((x & 0x0F0F0F0FU) << 4)  | ((x & 0xF0F0F0F0U) >> 4);
    x = ((x & 0x00FF00FFU) << 8)  | ((x & 0xFF00FF00U) >> 8);
    return (x << 16) | (x >> 16);
}

void invalid_arguments(void)
{
	fprintf(stderr, "*** Error: invalid arguments\n");
}

int main(int argc, char *argv[])
{
	uint64_t state;
	int random_option = 0;
	int low_high_option = 0;
	int reverse_option = 0;

	if(argc <= 1) {
		usage();
		return 0;
	}
	
	// Check second argument: -reverse or -low32 or -mid32 or -high32
	if(argc > 2) {
		reverse_option = strcmp(argv[2], "-reverse") == 0;
		if(reverse_option) {
			if(argc > 3) {
				invalid_arguments();
				return 1;
			}
		}
		else {
			low_high_option = check_low_high(argv[2]);
			if(argc == 4) {
				reverse_option = strcmp(argv[3], "-reverse") == 0;
			}
			if(argc > 4 || !low_high_option && !reverse_option) {
				invalid_arguments();
				return 1;
			}
		}
	}

	// Check first argument: -r or <number>
	random_option = strcmp(argv[1], "-r") == 0;
	if(random_option) {
		state = random_from_clock();
		fprintf(stderr, "sirius64gen: seed = %" PRIu64 "\n", state);
	}
	else if(!is_valid_uint64(argv[1], &state)) {
		invalid_arguments();
		return 1;
	}

	if(low_high_option == -32) { // 4 byte bassi
		uint32_t buffer[BUF_SIZE];
		for(;;) {
			for(int i = 0; i < BUF_SIZE; i++) {
				uint32_t r = (uint32_t)sirius64(&state);
				buffer[i] = reverse_option ? bit_reverse32(r) : r;
			}
			fwrite(buffer, sizeof(uint32_t), BUF_SIZE, stdout);
		}
	}
	else if(low_high_option == +32) { // 4 byte alti
		uint32_t buffer[BUF_SIZE];
		for(;;) {
			for(int i = 0; i < BUF_SIZE; i++) {
				uint32_t r = (uint32_t)(sirius64(&state) >> 32);
				buffer[i] = reverse_option ? bit_reverse32(r) : r;
			}
			fwrite(buffer, sizeof(uint32_t), BUF_SIZE, stdout);
		}
	}
	else if(low_high_option == 1) { // 4 byte centrali
		uint32_t buffer[BUF_SIZE];
		for(;;) {
			for(int i = 0; i < BUF_SIZE; i++) {
				uint32_t r = (uint32_t)(sirius64(&state) >> 16);
				buffer[i] = reverse_option ? bit_reverse32(r) : r;
			}
			fwrite(buffer, sizeof(uint32_t), BUF_SIZE, stdout);
		}
	}
	else { // Tutti gli 8 byte
		uint64_t buffer[BUF_SIZE];
		for(;;) {
			for(int i = 0; i < BUF_SIZE; i++) {
				uint64_t r = sirius64(&state);
				buffer[i] = reverse_option ? bit_reverse64(r) : r;
			}
			fwrite(buffer, sizeof(uint64_t), BUF_SIZE, stdout);
		}
	}
	return 0;
}
