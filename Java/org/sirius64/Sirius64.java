package org.sirius64;

/**
 * Sirius64 - Pseudo Random Number Generator 64 bit output, 64 bit state
 * 
 * Copyright(C) 2026 Matteo Zapparoli - zapparoli.matteo@gmail.com
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Minimalist implementation
 * 
 */

public class Sirius64 {

	private long state;
	
	public Sirius64(long seed) {
		this.state = seed;
	}
	
	public long next() {
	    long z = (state += 0x9e3779b97f4a7c15L);
		z = 0x9E3779B97F4A7BB9L * (z ^ (z >>> 17));
		z = (z << 32) | (z >>> 32);
		return 0x9e3779b97f4a7c15L * (z ^ (z >>> 17));
	}
	
}
