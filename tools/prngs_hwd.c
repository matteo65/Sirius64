#include <stdint.h> // uint64_t

/* Sample file for hwd.c (sirius64). */

static uint64_t state = 0x123456789ABCDEFull;

static inline uint64_t next() {
	uint64_t z = (state += 0x9e3779b97f4a7c15ull);
	z = 0x9e3779b97f4a7c15ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	return 0x9e3779b97f4a7c15ull * (state ^ z ^ (z >> 17));
}
