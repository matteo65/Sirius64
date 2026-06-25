# Sirius64 PRNG
Sirius64 is a ultra-fast, minimalist, robust pseudo-random number generator (PRNG) designed for high-performance applications, simulations, and games. 

```C
uint64_t sirius64(uint64_t *state) 
{
	uint64_t z = (*state += 0x9E3779B97F4A7C15ull);
	z = 0x9E3779B97F4A7BB9ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	return 0x9E3779B97F4A7C15ull * (z ^ (z >> 17));
}
```
The arithmetic is very simple; the state follows the well-known Weyl Sequence using the famous Golden Ratio constant; state management is therefore the same as SplitMix64 and similar to the PGC family of PRNGs, which instead use an LCG.
The real difference compared to SplitMix64 is in the random number generation: SplitMix64 "mixes" the bits and never produces a duplicate value, so one could even argue that it is a true PRNG, while Sirius64 performs operations that also generate duplicate numbers.
Note that in the second operation on z, the constant 0x9E3779B97F4A7BB9 was used, which is "almost" the Golden Ratio; it is the largest prime number less than the Golden Ratio; this constant allows the lower part of the output to be broken to avoid resonating with the other operations.

## Features
**Period**: $2^{64}$  
**State**: 64 bit  
**Output**: 64 bit  
**Seed**: all 64-bit values ​​except 0  

## SmokeRand v.0.47-gcc-linux Tests

## PractRand v.0.96 Tests

## TestU01 v.1.2.3 Tests
