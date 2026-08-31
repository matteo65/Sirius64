/*
 * testu01_sirius64_mid32.c
 *
 * Execute TestU01 SmallCrush, Crush and BigCrush on sirius64 (middle 32 bits)
 *
 * Copyright(C) 2026 Matteo Zapparoli - zapparoli.matteo@gmail.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Compile: gcc -std=c99 -Wall -O3 -o testu01_sirius64_mid32 testu01_sirius64_mid32.c -ltestu01 -lm
 *
 * Sintax:
 *  testu01_sirius64_mid32 [(small | crush | big) <seed>]
 *
 * No args prints help
 *
 */
 
#include <stdio.h>
#include <stdint.h> // uint64_t
#include <stdlib.h> // strcmp()
#include <string.h> // strtoull()
#include <errno.h> // errno
#include <ctype.h> // isdigit()

#include "TestU01.h"

static uint64_t state;

unsigned int sirius64mid32(void) 
{
	uint64_t z = (state += 0x9e3779b97f4a7c15ull);
	z = 0x9e3779b97f4a7c15ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	z = 0x9e3779b97f4a7c15ull * (state ^ z ^ (z >> 17));

	return (unsigned int)(z >> 16);
}

// Return 1 if str is not a uint64_t, 0 else
int isnt_valid_uint64(const char *str, uint64_t *out_value)
{
	if (str == NULL || *str == '\0') return 1;

	// Controlla che la stringa contenga solo cifre
	for (const char *p = str; *p; p++) {
		if (!isdigit((unsigned char) *p)) return 1;
	}

	errno = 0;
	char *end;
	unsigned long long v = strtoull(str, &end, 10);

	// end non deve spostarsi oltre la fine
	if (*end != '\0') return 1;

	// Overflow rilevato da errno
	if (errno == ERANGE) return 1;

	// Controllo ulteriore: strtoull potrebbe essere > UINT64_MAX su alcune piattaforme
	if (v > UINT64_MAX) return 1;

	if (out_value) *out_value = (uint64_t)v;

	return 0;
}

void usage(void)
{
	printf("testu01_sirius64_mid32 - Run TestU01 SmallCrush, Crush and BigCrush on sirius64 (middle 32 bits)\n\n");
	printf("Usage: testu01_sirius64_mid32 [(small | crush | big) <seed>]\n\n");
	printf("The seed must be non-zero\n");
}

int main(int argc, char *argv[])
{
	char *test_type;

	if(argc == 1) {
		usage();
		return 0;
	}
	
	if(argc != 3) {
		fprintf(stderr, "*** Error: invalid arguments\n");
		return 1;
	}
	
	if (strcmp(argv[1], "small") == 0) test_type = "small";
	else if (strcmp(argv[1], "crush") == 0) test_type = "crush";
	else if (strcmp(argv[1], "big") == 0) test_type = "big";
	else {
		fprintf(stderr, "*** Error: invalid argument %s\n", argv[1]);
		return 1;
	}
	
	if(isnt_valid_uint64(argv[2], &state)) {
		fprintf(stderr, "*** Error: invalid arguments\n");
		return 1;
	}
	
	if(state == 0) {
		fprintf(stderr, "*** Error: seed must be non-zero\n");
		return 1; 
	}

	unif01_Gen *gen = unif01_CreateExternGenBits("Sirius64mid32", sirius64mid32);

	printf("=== TestU01 - Generator: Sirius64mid32 ===\n");
	printf("Seed: %llu\n", (unsigned long long) state);

	if (strcmp(test_type, "small") == 0) {
		bbattery_SmallCrush(gen);
	}
	else if (strcmp(test_type, "crush") == 0) {
		bbattery_Crush(gen);
	}
	else if (strcmp(test_type, "big") == 0) {
		bbattery_BigCrush(gen);
	}

	unif01_DeleteExternGenBits(gen);

	return 0;
}
