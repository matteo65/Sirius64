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

## SmokeRand v.0.48-gcc-linux Tests

## PractRand v.0.96 Tests

Two complementary PractRand test campaigns were performed using independently generated random seeds.  
The **broad campaign** provides extensive seed coverage, while the **deep campaign** provides substantial testing depth.  
Together, these approaches assess both cross-seed robustness and long-range statistical behavior.

## Broad Campaign
- 200 independent `RNG_test` runs.
- Each run covered the range from **1 GB to 1 TB**.
- Designed to evaluate consistency across a large number of initial states and seeds.
- Report available on [test_practrand/practrand_test.txt](https://github.com/matteo65/Sirius64/blob/main/test_practrand/practrand_test.txt)

Total PractRand reports: **200**  
Reports without anomalies: **90**  
Reports with anomalies: **110**  
Total Test Stages: **2200**  
Total Tests: **646200**  

### Anomaly Counting
|Anomaly|    #| Percentage|Min p-value|Max p-value|
|-------|-----|----------|---|---|
|unusual|147|0.0227%|3.9e-6|1 - 3.0e-5|
|mildly suspicious|9|0.0014%|3.2e-5|1 - 2.4e-5|
|suspicious|1|0.0002%|1 - 2.5e-5|1 - 2.5e-5|
|very suspicious|0|0%|-|-|
|FAIL|0|0%|-|-|
|**Total**|**157**|**0.0243%**|-|-|

### Anomaly for Test Stage
|Test Stage|Unusual|Mildly Suspicious|Suspicious|Very Suspicious|FAIL|**Total**|
|----------|-------|-----------------|----------|---------------|----|---------|
|1 GB|7|1|0|0|0|**8**|
|2 GB|12|1|0|0|0|**13**|
|4 GB|22|4|0|0|0|**26**|
|8 GB|16|2|0|0|0|**18**|
|16 GB|16|1|0|0|0|**17**|
|32 GB|11|0|0|0|0|**11**|
|64 GB|13|0|0|0|0|**13**|
|128 GB|11|0|0|0|0|**11**|
|256 GB|19|0|1|0|0|**20**|
|512 GB|13|0|0|0|0|**13**|
|1 TB|7|0|0|0|0|**7**|
|**Total**|**147**|**9**|**1**|**0**|**0**|**157**|

### Anomaly Ranking
<details>
  <summary><b>Show</b></summary>

	
|Anomaly|    #|
|-------|-----|
|[Low4/64]DC6-9x1Bytes-1|16|
|DC6-9x1Bytes-1|15|
|[Low16/64]DC6-9x1Bytes-1|15|
|[Low1/64]DC6-9x1Bytes-1|12|
|[Low16/64]BCFN(2+0,13-0U)|5|
|[Low1/64]FPF/16:cross|4|
|BCFN(2+1,13-0U)|4|
|[Low16/64]Gap-16:A|4|
|Gap-16:B|4|
|[Low16/64]BCFN(2+1,13-0U)|4|
|FPF/16:all|4|
|BCFN(2+0,13-0U)|3|
|[Low4/64]Gap-16:A|3|
|[Low4/64]FPF/16:all|3|
|[Low16/64]FPF/16:cross|3|
|[Low1/64]Gap-16:A|3|
|[Low1/64]Gap-16:B|3|
|[Low16/64]FPF/16:all|3|
|[Low1/64]BCFN(2+1,13-3U)|3|
|FPF/16:cross|3|
|[Low1/64]FPF/16:all|3|
|[Low1/64]BCFN(2+0,13-0U)|2|
|[Low4/64]BCFN(2+2,13-3U)|2|
|Gap-16:A|2|
|[Low1/64]BCFN(2+1,13-1U)|2|
|[Low4/64]BCFN(2+1,13-0U)|2|
|[Low4/64]BCFN(2+2,13-0U)|2|
|[Low4/64]BCFN(2+0,13-0U)|2|
|[Low4/64]mod3n(5):(0,9-1)|1|
|[Low4/64]BCFN(2+10,13-5U)|1|
|[Low4/64]FPF/16:cross|1|
|BRank(12):score:4096|1|
|[Low16/64]mod3n(5):(3,9-1)|1|
|[Low1/64]BCFN(2+1,13-2U)|1|
|BCFN(2+2,13-0U)|1|
|[Low4/64]BCFN(2+2,13-1U)|1|
|[Low1/64]BCFN(2+6,13-2U)|1|
|BCFN(2+5,13-0U)|1|
|[Low4/64]mod3n(5):(3,9-0)|1|
|[Low4/64]BCFN(2+3,13-0U)|1|
|BRank(12):score:6144|1|
|[Low16/64]Gap-16:B|1|
|BDayS2(4,24)[64+0]|1|
|[Low16/64]BCFN(2+2,13-0U)|1|
|BRank(12):score:2560|1|
|BRank(12):score:5120|1|
|[Low1/64]BCFN(2+2,13-3U)|1|
|[Low1/64]BCFN(2+3,13-1U)|1|
|[Low4/64]BCFN(2+1,13-2U)|1|
|[Low1/64]BCFN(2+0,13-3U)|1|
|[Low1/64]BCFN(2+5,13-5U)|1|
|[Low4/64]BCFN(2+6,13-3U)|1|
|[Low16/64]BCFN(2+0,13-1U)|1|
|[Low16/64]BCFN(2+1,13-2U)|1|
|**Total**|**157**|


</details>

### Top/Bottom p-value distribuition of anomalies
This table displays the breakdown of p-values ​​close to 1 (top) and close to 0 (bottom); in an ideal uniform distribution the values ​​should be equal.  
| Top   |Bottom|**Total**|
|-------|------|---------|
|91|66|**157**|

**No reproducible weakness was observed in this campaign.**

## Deep Campaign
- 10 independent `RNG_test` runs.
- Each run covered the range from **1 GB to 64 TB**.
- Designed to detect weaknesses that may emerge only at very large output volumes.


## TestU01 v.1.2.3 Tests
