# Sirius64 Pseudo Random Number Generator
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
We decided to use [SmokeRand](https://github.com/alvoskov/SmokeRand) (**v. 0.48-gcc-linux64**) as an initial test tool rather than Dieharder as it is a new tool that we found to be more reliable and accurate.  
We ran 12 full runs with 64-bit inputs; 10 runs with random seeds, and 2 runs with limit seeds (0 and MAX_UINT64).  
The output files are in the [test_smokerand/](https://github.com/matteo65/Sirius64/tree/main/test_smokerand) directory.
Each full run performs 50 tests, for a total of 600 tests.
No failures were detected, and one suspect were found, a number consistent with the expected value.
All runs scored a quality of 4 (good) on a scale from 0 to 4.  

### SmokeRand Test Summary Table

|# |         Seed       | Anomalies       |
|--|--------------------|-----------------|
|01|2369157498668639969 |       -         |
|02|14269143917408480699|       -         |
|03|4380167771877769980 |       -         |
|04|16790869163370235529|       -         ||
|05|11162697988881170641| test: **bspace8_8d_high** p-value: **1 - 5.63e-04** SUSPICIOUS|
|06|15018305529109891896|       -         |
|07|4094093812534875946 |       -         |
|08|2259750328877818529 |       -         |
|09|5627138766949811142 |       -         |
|10|10129161356813192160|       -         |
|11|0                   |       -         |
|12|MAX_UINT64          |       -         |

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

## Collision Analysis
Ten independent collision-counting runs were performed using different initial seeds. Exact collision counts were measured using ColFinder, an open-source tool developed specifically for large-scale PRNG collision analysis.

For **16 × 10^9** generated 64-bit values, the theoretical expected number of collisions is **6.94**. The observed average was **6.8** collisions, showing excellent agreement with the random-mapping model. Individual runs produced between 2 and 13 collisions, a range fully consistent with the expected Poisson distribution governing collision events in a 64-bit output space.

Report directory: [test_collision](https://github.com/matteo65/Sirius64/tree/main/test_collision)
ColFinder repository: [Colfinder](https://github.com/matteo65/colfinder)

|#|Seed|# Collisions|
|--|------------------|-|
|01|0x1234567890abcdef|5|
|02|0                 |7|
|03|1                 |6|
|04|MAX_UINT64        |8|
|05|123456789         |6|
|06|15171584865647022546|13|
|07|11223344556677889900|2|
|08|2776390552391494841|5|
|09|7562801862664434757|8|
|10|13777961059468951567|8|
|  |   **AVERAGE**          |**6.8**|

