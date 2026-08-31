# Sirius64 Pseudo-Random Number Generator
Sirius64 is a ultra-fast, robust pseudo-random number generator (PRNG) designed for high-performance applications, simulations, and games. 

```C
uint64_t sirius64(uint64_t *state) 
{
	uint64_t z = (*state += 0x9e3779b97f4a7c15ull);
	z = 0x9e3779b97f4a7c15ull * (z ^ (z >> 17));
	z = (z << 32) | (z >> 32);
	return 0x9e3779b97f4a7c15ull * ((*state) ^ z ^ (z >> 17));
}
```
# Labor Omnia Vincit
Sirius64 represents the culmination of more than six years of research dedicated to the design, analysis, and implementation of novel pseudorandom number generators and hash functions. During this journey, I designed and tested dozens of original algorithms, wrote thousands of lines of experimental code, and performed extensive validation against both theoretical expectations and empirical benchmarks.  

The path was rarely straightforward. Many prototypes were abandoned after revealing subtle statistical weaknesses, insufficient diffusion properties, or structural limitations. Others showed promise but ultimately failed to meet the standards required for publication-quality results. Each success was built upon numerous failures, refinements, and new insights gained through continuous experimentation.  

Throughout these years, I explored a wide range of techniques, from mixing functions and state-transition mechanisms to avalanche behavior, collision resistance, and large-scale statistical testing. Every design was evaluated not only for speed and simplicity, but also for its mathematical soundness and long-term statistical reliability.  

Sirius64 is therefore more than a single algorithm. It is the result of accumulated experience, countless experiments, extensive testing, and a genuine fascination with the challenge of generating high-quality randomness from deterministic processes.  

I hope that the work presented here may contribute, even in a small way, to the broader community of researchers, developers, and enthusiasts interested in random number generation. If the ideas, methods, results, or lessons learned from this project prove useful to others, then the effort invested over these years will have achieved its purpose.  

## Summary

**Sirius64 represents a major advancement in the landscape of non-cryptographic pseudo-random number generators (PRNGs), successfully reconciling extreme throughput with rigorous statistical quality.**  

In contrast to traditional high-speed generators that often exhibit structural vulnerabilities under prolonged empirical testing, Sirius64 demonstrates flawless empirical behavior. It successfully clears all standard comprehensive testing suites, including deep **PractRand** evaluations and strict **collision tests**, proving capable of generating ideal, indistinguishable white noise.  

Consequently, **until proven otherwise, Sirius64 stands as the fastest statistically sound PRNG in existence**, delivering near-optimal CPU cycle efficiency without compromising statistical integrity.   

This unique combination makes it an ideal candidate for high-performance computing, large-scale Monte Carlo simulations, and environments where execution speed and absolute statistical reliability are both critical constraints.

## Features
**Non-Cryptographic**  
**Period**: $2^{64}$  
**State**: 64 bit  
**Output**: 64 bit  
**Seed**: all 64-bit values  

## SmokeRand v.0.48-gcc-linux Tests
We decided to use [SmokeRand](https://github.com/alvoskov/SmokeRand) (**v. 0.48-gcc-linux64**) as an initial test tool rather than Dieharder as it is a new tool that we found to be more reliable and accurate.  
We ran 12 full runs with 64-bit inputs; 10 runs with random seeds, and 2 runs with limit seeds (0 and UINT64_MAX).  
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
|12|UINT64_MAX          |       -         |

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
- 12 independent `RNG_test` runs.
- Each run covered the range from **1 GB to 64 TB**.
- Designed to detect weaknesses that may emerge only at very large output volumes.
- Output files available on [test_practrand/](https://github.com/matteo65/Sirius64/blob/main/test_practrand/)

|# |         Seed       | Anomalies       |
|--|--------------------|-----------------|
|1 |9743679751792029932 | -               |
|2 |2183565420831062164 | -               |
|3 |13370065392227974123| 1GB test: FPF/16:all p-value = 1-3.8e-4 unusual|
|4 |2238462460366867871 | -               |
|5 |14342525201357417549| -               |
|6 |1541033473771210326 | -               |
|7 |8685395623620954169 | -               |
|8 |4950362692890351856 | -               |
|9 |9067208584599398542 | -               |
|10|10659961338205357269| -               |
|11|0                   | -               |
|12|UINT64_MAX          | 128 GB test: FPF/16:all p-value = 4.7e-4 unusual<br>16 TB test: DC6-9x1Bytes-1 p-value = 1-3.4e-3 unusual|

No failures or suspicious results were observed. Three isolated "unusual" p-values appeared across the entire test campaign, all at different seeds and data volumes, with no recurrence or progressive deterioration. The observed behaviour is consistent with the expected statistical false-positive rate of PractRand.

## TestU01 v.1.2.3 Tests
A total of **300 BigCrush** runs were performed using 100 independent seeds. For each seed, the High32, Mid32 and Low32 portions of the 64-bit output stream were tested separately, producing 48,000 individual TestU01 statistics.

No failures were observed. A total of 104 anomalies were detected, compared to 96.0 expected under the null hypothesis of perfect randomness.  

Anomalies were evenly distributed across High32, Mid32 and Low32 outputs (33, 36 and 35 respectively), with no evidence of concentration in any specific test family. The balance between upper-tail and lower-tail p-values (54 vs 50) was close to ideal, and no p-values below 10⁻⁶ were observed.

Overall, the results are fully consistent with the behavior expected from a high-quality random number generator and provide no evidence of systematic statistical weaknesses.

Output files available on [test bigbrush/](https://github.com/matteo65/Sirius64/tree/main/testu01_bigcrush/)

Number of session tests: **100**  
Number of big_crush runs: **300**  
Nomber of TestU01 statistics: **48000**  
Number of failures: **0**  
Number of anomalies: **104**  
Number of anomalies expected: **96.0**  

|Runs|high32|mid32|low32|**Total**|
|----|------|-----|-----|---------|
|With anomalies|30|29|28|**87**|
|Without anomalies|70|71|72|**213**|
|**Total**|**100**|**100**|**100**|**300**|

### Details of anomalies for each session test
<details>
  <summary><b>Show</b></summary>



| Test | high32 | mid32 | low32 |
|------|--------|--------|--------|
|00|25 ClosePairs NJumps, t = 16 (6.1e-6)|-|-|
|01|-|64 WeightDistrib, r = 26 (0.9995)|-|
|02|-|-|82 LempelZiv, r = 0 (0.9994)|
|03|84 Fourier3, r = 0 (0.9997)|-|66 MatrixRank, L=30, r=0 (0.9996)|
|04|-|62 WeightDistrib, r = 0 (4.6e-4)|-|
|05|-|-|-|
|06|-|25 ClosePairs mNP1, t = 16 (0.9998);<br>54 SampleMean, r = 10 (8.4e-4)|-|
|07|22 ClosePairs mNP2, t = 3 (6.2e-4)|-|-|
|08|11 CollisionOver, t = 21 (1.4e-5)|-|-|
|09|97 HammingIndep, L=300, r=0 (0.9997)|-|14 BirthdaySpacings, t = 3 (0.9994)|
|10|24 ClosePairs mNP, t = 9 (7.5e-4);<br>81 LinearComp, r = 29 (5.9e-4)|-|-|
|11|-|-|-|
|12|-|-|-|
|13|8 CollisionOver, t = 7 (0.9993)|-|-|
|14|36 Gap, r = 0 (6.5e-4);<br>43 Permutation, t = 10 (4.6e-4)|-|-|
|15|-|-|7 CollisionOver, t = 7 (0.9996);<br>68 MatrixRank, L=1000, r=0 (0.9991);<br>77 RandomWalk1 H (L=1000, r=20) (0.9999)|
|16|97 HammingIndep, L=300, r=0 (3.0e-4)|-|-|
|17|-|24 ClosePairs mNP2S, t = 9 (0.9995)|11 CollisionOver, t = 21 (0.9997)|
|18|-|-|-|
|19|-|-|-|
|20|-|-|-|
|21|-|12 CollisionOver, t = 21 (6.5e-4)|-|
|22|-|-|97 HammingIndep, L=300, r=0 (1.3e-4)|
|23|-|-|54 SampleMean, r = 10 (7.7e-4)|
|24|-|11 CollisionOver, t = 21 (0.9994);<br>31 CouponCollector, r = 10 (9.6e-5)|90 HammingWeight2, r = 0 (0.9998)|
|25|-|-|-|
|26|-|-|-|
|27|-|-|62 WeightDistrib, r = 0 (1 - 2.4e-5)|
|28|102 Run of bits, r = 27 (6.1e-4)|-|-|
|29|105 AutoCor, d=1, r=27 (0.9998)|-|-|
|30|-|-|-|
|31|-|-|-|
|32|-|-|24 ClosePairs mNP1, t = 9 (0.9996)|
|33|-|-|40 Permutation, t = 3 (9.7e-4)|
|34|-|41 Permutation, t = 5 (2.6e-4)|-|
|35|-|-|76 RandomWalk1 R (L=1000, r=0) (0.9995)|
|36|-|-|-|
|37|-|-|-|
|38|4 CollisionOver, t = 2 (0.9997)|-|50 SampleProd, t = 8 (0.9990);<br>83 LempelZiv, r = 15 (0.9999)|
|39|-|-|-|
|40|-|-|-|
|41|61 WeightDistrib, r = 28 (1.0e-4)|56 SampleCorr, k = 2 (0.9991)|47 MaxOft AD, t = 16 (1.4e-4)|
|42|27 SimpPoker, r = 27 (7.6e-4)|-|-|
|43|-|47 MaxOft AD, t = 16 (0.9992)|-|
|44|24 ClosePairs mNP2, t = 9 (0.9990)|35 Gap, r = 25 (0.9991);<br>56 SampleCorr, k = 2 (2.9e-5)|-|
|45|-|25 ClosePairs mNP1, t = 16 (0.9993)|-|
|46|-|22 ClosePairs mNP2, t = 3 (6.3e-4)|-|
|47|-|-|-|
|48|-|-|26 SimpPoker, r = 0 (0.9997)|
|49|78 RandomWalk1 J (L=10000, r=0) (0.9998)|24 ClosePairs mNP1, t = 9 (0.9991)|48 MaxOft AD, t = 24 (0.9993)|
|50|-|14 BirthdaySpacings, t = 3 (1.8e-4);<br>106 AutoCor, d=3, r=27 (8.7e-5)|47 MaxOft, t = 16 (7.9e-4);<br>76 RandomWalk1 H (L=1000, r=0) (0.9996);<br>88 PeriodsInStrings, r = 0 (0.9992)|
|51|-|-|-|
|52|-|-|-|
|53|11 CollisionOver, t = 21 (4.1e-4)|-|-|
|54|90 HammingWeight2, r = 0 (1.3e-4)|97 HammingIndep, L=300, r=0 (5.8e-4)|-|
|55|-|-|-|
|56|-|-|-|
|57|76 RandomWalk1 C (L=1000, r=0) (0.9992)|99 HammingIndep, L=1200, r=0 (0.9995)|-|
|58|-|-|-|
|59|-|-|-|
|60|-|55 SampleCorr, k = 1 (0.9999)|-|
|61|77 RandomWalk1 M (L=1000, r=20) (0.9993)|-|-|
|62|77 RandomWalk1 H (L=1000, r=20) (7.5e-4)|-|-|
|63|-|-|55 SampleCorr, k = 1 (1.4e-4)|
|64|81 LinearComp, r = 0 (9.3e-4)|-|-|
|65|-|69 MatrixRank, L=1000, r=26 (9.9e-4)|-|
|66|77 RandomWalk1 J (L=1000, r=20) (0.9993)|-|-|
|67|-|-|37 Gap, r = 20 (0.9991)|
|68|-|-|-|
|69|-|-|6 CollisionOver, t = 3 (2.1e-4)|
|70|-|11 CollisionOver, t = 21 (1.4e-4)|-|
|71|-|11 CollisionOver, t = 21 (0.9992)|-|
|72|-|-|-|
|73|-|-|-|
|74|-|24 ClosePairs mNP1, t = 9 (0.9996);<br>30 CouponCollector, r = 0 (0.9998)|-|
|75|-|56 SampleCorr, k = 2 (0.9998)|-|
|76|49 MaxOft, t = 32 (1.7e-4);<br>66 MatrixRank, L=30, r=0 (3.8e-4)|-|-|
|77|8 CollisionOver, t = 7 (0.9998)|78 RandomWalk1 C (L=10000, r=0) (5.5e-4);<br>103 AutoCor, d=1, r=0 (0.9991)|-|
|78|11 CollisionOver, t = 21 (1.6e-5)|-|-|
|79|36 Gap, r = 0 (0.9994)|78 RandomWalk1 R (L=10000, r=0) (0.9996)|60 WeightDistrib, r = 20 (0.9992)|
|80|-|-|-|
|81|25 ClosePairs mNP2, t = 16 (5.7e-4)|15 BirthdaySpacings, t = 4 (0.9992)|-|
|82|-|-|104 AutoCor, d=3, r=0 (0.9993)|
|83|-|74 RandomWalk1 M (L=50, r=0) (7.0e-4)|6 CollisionOver, t = 3 (3.1e-4);<br>36 Gap, r = 0 (3.2e-4)|
|84|-|-|-|
|85|-|-|-|
|86|-|-|-|
|87|-|-|-|
|88|-|-|77 RandomWalk1 R (L=1000, r=20) (6.7e-4);<br>102 Run of bits, r = 27 (4.7e-4)|
|89|58 AppearanceSpacings, r = 27 (3.7e-5)|-|-|
|90|-|12 CollisionOver, t = 21 (0.9999)|29 SimpPoker, r = 25 (0.9997)|
|91|-|9 CollisionOver, t = 14 (4.5e-4);<br>11 CollisionOver, t = 21 (9.2e-5)|-|
|92|-|-|-|
|93|-|-|74 RandomWalk1 R (L=50, r=0) (5.4e-4)|
|94|77 RandomWalk1 C (L=1000, r=20) (8.3e-5)|-|-|
|95|-|-|-|
|96|-|55 SampleCorr, k = 1 (6.7e-4)|46 MaxOft AD, t = 8 (0.9990)|
|97|-|-|-|
|98|-|-|-|
|99|-|-|55 SampleCorr, k = 1 (0.9996)|


</details>

### Ranking by frequency of anomalies
<details>
  <summary><b>Show</b></summary>


|Anomaly|high32|mid32|low32|
|-------|------|-----|-----|
|11 CollisionOver, t = 21|3|4|1|
|24 ClosePairs mNP, t = 9|2|3|1|
|77 RandomWalk1 H (L=1000, r=20)|4|0|2|
|25 ClosePairs NJumps, t = 16|2|2|0|
|97 HammingIndep, L=300, r=0|2|1|1|
|55 SampleCorr, k = 1|0|2|2|
|36 Gap, r = 0|2|0|1|
|76 RandomWalk1 R (L=1000, r=0)|1|0|2|
|56 SampleCorr, k = 2|0|3|0|
|47 MaxOft AD, t = 16|0|1|2|
|78 RandomWalk1 J (L=10000, r=0)|1|2|0|
|66 MatrixRank, L=30, r=0|1|0|1|
|62 WeightDistrib, r = 0|0|1|1|
|54 SampleMean, r = 10|0|1|1|
|22 ClosePairs mNP2, t = 3|1|1|0|
|14 BirthdaySpacings, t = 3|0|1|1|
|81 LinearComp, r = 29|2|0|0|
|8 CollisionOver, t = 7|2|0|0|
|12 CollisionOver, t = 21|0|2|0|
|90 HammingWeight2, r = 0|1|0|1|
|102 Run of bits, r = 27|1|0|1|
|6 CollisionOver, t = 3|0|0|2|
|74 RandomWalk1 M (L=50, r=0)|0|1|1|
|64 WeightDistrib, r = 26|0|1|0|
|82 LempelZiv, r = 0|0|0|1|
|84 Fourier3, r = 0|1|0|0|
|43 Permutation, t = 10|1|0|0|
|7 CollisionOver, t = 7|0|0|1|
|68 MatrixRank, L=1000, r=0|0|0|1|
|31 CouponCollector, r = 10|0|1|0|
|105 AutoCor, d=1, r=27|1|0|0|
|40 Permutation, t = 3|0|0|1|
|41 Permutation, t = 5|0|1|0|
|4 CollisionOver, t = 2|1|0|0|
|50 SampleProd, t = 8|0|0|1|
|83 LempelZiv, r = 15|0|0|1|
|61 WeightDistrib, r = 28|1|0|0|
|27 SimpPoker, r = 27|1|0|0|
|35 Gap, r = 25|0|1|0|
|26 SimpPoker, r = 0|0|0|1|
|48 MaxOft AD, t = 24|0|0|1|
|106 AutoCor, d=3, r=27|0|1|0|
|88 PeriodsInStrings, r = 0|0|0|1|
|99 HammingIndep, L=1200, r=0|0|1|0|
|69 MatrixRank, L=1000, r=26|0|1|0|
|37 Gap, r = 20|0|0|1|
|30 CouponCollector, r = 0|0|1|0|
|49 MaxOft, t = 32|1|0|0|
|103 AutoCor, d=1, r=0|0|1|0|
|60 WeightDistrib, r = 20|0|0|1|
|15 BirthdaySpacings, t = 4|0|1|0|
|104 AutoCor, d=3, r=0|0|0|1|
|58 AppearanceSpacings, r = 27|1|0|0|
|29 SimpPoker, r = 25|0|0|1|
|9 CollisionOver, t = 14|0|1|0|
|46 MaxOft AD, t = 8|0|0|1|
|**Total**|**33**|**36**|**35**|
|**Expected**|**32.0**|**32.0**|**32.0**|


</details>

### Interval p-value distribution
This table summarizes the distribution of anomalous p-values by order of magnitude.  
For p-values close to **0**, the p-value itself is used. For p-values close to **1**, the residual value **(1 − p)** is used instead. In other words, all anomalies are measured by their distance from the nearest distribution boundary (0 or 1).  
Examples:  
- p = 0.00042 → residual value = 0.00042
- p = 0.99958 → residual value = 0.00042
- p = 0.999991 → residual value = 9.0 × 10⁻⁶

This normalization allows upper-tail and lower-tail anomalies to be analyzed together and compared within the same magnitude intervals.  
For a perfectly random generator, the residual values are expected to follow a uniform distribution, implying that the number of anomalies should decrease by approximately a factor of ten for each successive interval.  

|p-value Interval|Found|**Expected**|
|----------------|-----|--------|
|[1.0e-4, 1.0e-3)|90|**86.4**|
|[1.0e-5, 1.0e-4)|13|**8.6**|
|[1.0e-6, 1.0e-5)|1|**0.9**|
|[1.0e-7, 1.0e-6)|0|**0.1**|
|[1.0e-8, 1.0e-7)|0|**0.0**|
|[0, 1.0e-7)     |0|**0.0**|
|**Total**       |**104**|**96.0**|

### Top/Bottom p-value distribuition
This table displays the breakdown of p-values ​​close to 1 (top) and close to 0 (bottom); in an ideal uniform distribution the values ​​should be equal.  
|    |high32 | mid32 | low32 |**Total**|
|----|-------|-------|-------|---------|
|Top |12|19|23|**54**|
|Bottom|21|17|12|**50**|
|**Total**|**33**|**36**|**35**|**104**|

### Statistical of p-values
|    |high32 | mid32 | low32 |
|----|-------|-------|-------|
|Top Average|0.9995|0.9995|0.9995|
|Top Max Value|0.9998|0.9999|1 - 2.4e-5|
|Top Min Value|0.9990|0.9991|0.9990|
|Top Standard Deviation|0.000927|0.001236|0.001379|
|Bottom Average|3.97e-4|4.36e-4|4.55e-4|
|Bottom Max Value|9.3e-4|9.9e-4|9.7e-4|
|Bottom Min Value|6.1e-6|2.9e-5|1.3e-4|
|Bottom Standard Deviation|0.001338|0.001191|0.000966|

## Collision Count
Ten independent collision-counting runs were performed using different initial seeds. Exact collision counts were measured using **ColFinder**, an open-source tool developed specifically for large-scale PRNG collision analysis.

For **16 × 10^9** generated 64-bit values, the theoretical expected number of collisions is **6.94**. The observed average was **6.8** collisions, showing excellent agreement with the random-mapping model. Individual runs produced between 2 and 13 collisions, a range fully consistent with the expected Poisson distribution governing collision events in a 64-bit output space.

**Report directory:** [test_collision/](https://github.com/matteo65/Sirius64/tree/main/test_collision)  
**ColFinder repository:** [Colfinder](https://github.com/matteo65/colfinder)  

|#|Seed|# Collisions|
|--|------------------|-|
|01|0x1234567890abcdef|5|
|02|0                 |7|
|03|1                 |6|
|04|UINT64_MAX        |8|
|05|123456789         |6|
|06|15171584865647022546|13|
|07|11223344556677889900|2|
|08|2776390552391494841|5|
|09|7562801862664434757|8|
|10|13777961059468951567|8|
|  |   **AVERAGE**          |**6.8**|

## Hamming–Weight Dependencies Test
A long-run validation (seed = 0x123456789ABCDEF) is being conducted and has currently reached 7 PB without failures.  
Testing will continue and results will be updated as additional milestones are achieved.

- Test reference on [Testing Hamming–Weight Dependencies](https://prng.di.unimi.it/hwd.php)
- Report available on [test_hwd/sirius64_hwd.txt](https://github.com/matteo65/Sirius64/blob/main/test_hwd/sirius64_hwd.txt)

### Current result
```text
mix3 extreme = 1.42962 (sig = 00002000) weight 1 (16), p-value = 0.93
mix3 extreme = 2.78010 (sig = 02100000) weight 2 (112), p-value = 0.457
mix3 extreme = 2.71222 (sig = 00002011) weight 3 (448), p-value = 0.95
mix3 extreme = 3.81522 (sig = 10200011) weight 4 (1120), p-value = 0.141
mix3 extreme = 4.32730 (sig = 11211010) weight >=5 (4864), p-value = 0.0708
bits per word = 64 (analyzing bits); min category p-value = 0.0708

processed 7e+15 bytes in 2.16e+06 seconds (3.237 GB/s, 11.65 TB/h). Sun Aug 30 10:45:14 2026

p = 0.307
```


## Test Conclusion
Sirius64 has successfully passed extensive empirical validation including TestU01 BigCrush, multiple PractRand runs up to 64 TB, collision-counting experiments consistent with theoretical expectations, and more than 7 PiB of Hamming-Weight Dependency testing without significant anomalies.  
The combined evidence suggests no detectable statistical weaknesses within the tested range.  

## Benchmark
### Characteristics of the computer where the benchmark was run
**OS:** Ubuntu 24.04.4 LTS  
**Kernel:** Linux 6.6.87.2-microsoft-standard-WSL2  
**gcc:** (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0  
**CPU:**  
Architecture: x86_64  
CPU op-mode(s): 32-bit, 64-bit  
Address sizes: 46 bits physical, 48 bits virtual  
Byte Order: Little Endian  
CPU(s): 16  
On-line CPU(s) list:  0-15    
Vendor ID:  GenuineIntel  
Model name:  Intel(R) Core(TM) Ultra 7 265H  
CPU family: 6  
Model: 197  

|Generator     |Cycles/64-bit|Relative Speed|
|--------------|-------------|--------------|
|SplitMix64    |    5.111    |    100%      |
|Sirius64      |    5.143    |    99%       |
|wyrand v.4    |    5.438    |    93%       |
|xoshiro256**  |    6.364    |    80%       |
|xoroshiro128++|    6.368    |    80%       |
|xoshiro256++  |    6.445    |    79%       |

Sirius64 achieves performance comparable to SplitMix64, requiring only 5.14 CPU cycles per generated 64-bit value. Benchmark results show Sirius64 running within 1% of SplitMix64 and significantly faster than xoshiro256**, xoshiro256++, xoroshiro128++, and wyRand v4
 
 
 


