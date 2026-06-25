/*
 * sirius64.c 
 *
 * Sirius64 - Pseudo Random Number Generator 64 bit output, 64 bit state
 * 
 * Copyright(C) 2026 Matteo Zapparoli - zapparoli.matteo@gmail.com
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Minimalist implementation
 *
 */

#include "sirius64.h"

uint64_t sirius64(uint64_t *state) 
{
	uint64_t z = (*state += 0x9E3779B97F4A7C15ull);
	z = 0x9E3779B97F4A7BB9ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	return 0x9E3779B97F4A7C15ull * (z ^ (z >> 17));
}
