/*
 * sirius64.h 
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

#ifndef SIRIUS64_H
#define SIRIUS64_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t sirius64(uint64_t *state);

#ifdef __cplusplus
}
#endif

#endif /* SIRIUS64_H */
