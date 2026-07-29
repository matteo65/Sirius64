# Sirius64 PRNG
Sirius64 is a ultra-fast, minimalist, robust pseudo-random number generator (PRNG) designed for high-performance applications, simulations, and games. 

```C
uint64_t sirius64(uint64_t *state) 
{
	uint64_t z = (*state += 0x9e3779b97f4a7c15ull);
	z = 0x9e3779b97f4a7c15ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	return 0x9e3779b97f4a7c15ull * ((*state) ^ z ^ (z >> 17));
}
```
## Features
**Period**: $2^{64}$  
**State**: 64 bit  
**Output**: 64 bit  
**Seed**: all 64-bit values 

## SmokeRand v.0.47-gcc-linux Tests

## PractRand v.0.96 Tests

## TestU01 v.1.2.3 Tests
