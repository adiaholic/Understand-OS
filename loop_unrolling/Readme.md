# Loop Unrolling Performance Experiments

This document contains the results of performance experiments comparing normal loops vs unrolled loops in C++.

## Experimental Setup
- **Compiler**: g++ with optimization flags (no optimization for first run, -O3 for second run)
- **Platform**: Linux
- **Measurements**: Execution time in microseconds (μs)
- **Trials**: 10 experiments per run

## Results Summary

### Run 1: Without Optimization
| Experiment | Normal Loop (μs) | Unrolled Loop (μs) | Faster |
|------------|------------------|-------------------|---------|
| 1          | 714,141         | 638,620          | Unrolled|
| 2          | 735,978         | 672,406          | Unrolled|
| 3          | 726,844         | 639,688          | Unrolled|
| 4          | 724,075         | 634,088          | Unrolled|
| 5          | 735,603         | 666,465          | Unrolled|
| 6          | 726,600         | 665,556          | Unrolled|
| 7          | 724,769         | 632,935          | Unrolled|
| 8          | 726,519         | 642,630          | Unrolled|
| 9          | 729,119         | 627,695          | Unrolled|
| 10         | 729,669         | 657,994          | Unrolled|

**Summary**: Unrolled loop faster in 10/10 trials (100% success rate)

### Assembly Generation Process

To analyze the performance differences at the instruction level, we generated assembly code for both implementations:

**Normal Loop Assembly Generation:**
```bash
g++ -S normal_loop.cpp
```

**Unrolled Loop Assembly Generation:**
```bash
g++ -S unloaded_loop.cpp
```

**What the -S flag does:**
The `-S` flag tells the compiler to stop after the compilation stage and output assembly language code instead of object code. This allows us to examine the exact CPU instructions generated for each C++ implementation, enabling detailed performance analysis at the hardware level.

### Step-by-Step Analysis: Why Unrolled Loop is Consistently Faster

#### 1. **Branch Instruction Frequency - The Primary Factor**

**Normal Loop (`normal_loop.s`):**
- Executes **1000 iterations** (i = 0 to 999)
- Each iteration processes 1 array element
- **1000 branch instructions** executed (`.L2` → `.L3` → `.L2`)

**Unrolled Loop (`unloaded_loop.s`):**
- Executes **250 iterations** (i increments by 4 each time)
- Each iteration processes 4 array elements
- **250 branch instructions** executed

**Impact**: 75% reduction in branch overhead is the major performance gain.

#### 2. **Assembly Code Analysis**

**Normal Loop `.L3` section (per iteration):** 

- ~11 instructions per array element processed

**Unrolled Loop `.L3` section (per iteration):**
- Processes arr[i], arr[i+1], arr[i+2], arr[i+3] in sequence
- Similar instruction pattern repeated 4 times
- ~44 instructions per 4 array elements 

#### 3. **Why More Instructions Can Be Faster**

**Branch Penalty Analysis:**
- Normal: 1000 × (~12 work instructions + branch penalty)
- Unrolled: 250 × (~50 work instructions + branch penalty)

**Modern ARM64 Pipeline Benefits:**
- Can execute 2-4 instructions simultaneously if there is no dependency between them
- Multiple array accesses can be pipelined for independent access. So, 250 × (~50 work instructions + branch penalty) becomes  250 × (4 work instructions + branch penalty) because the 4 of the array accesses can be pipelined

#### 4. **Performance Measurement Validation**

The experimental data shows:
- **Average normal loop time**: ~730,000 μs
- **Average unrolled loop time**: ~650,000 μs
- **Performance improvement**: ~10% consistently across all trials

This improvement correlates with the 75% reduction in branch instructions. 

**Conclusion**: The consistent 100% success rate of unrolled loops in unoptimized code demonstrates that manual loop unrolling provides significant performance benefits when compiler optimizations are not applied, primarily due to reduced branch instruction overhead.


### Run 2: With -O3 Optimization (Not to be discussed in detail in this document)
| Experiment | Normal Loop (μs) | Unrolled Loop (μs) | Faster |
|------------|------------------|-------------------|---------|
| 1          | 243,499         | 244,053          | Normal  |
| 2          | 255,205         | 237,862          | Unrolled|
| 3          | 235,995         | 235,663          | Unrolled|
| 4          | 248,675         | 228,366          | Unrolled|
| 5          | 235,330         | 234,551          | Unrolled|
| 6          | 236,433         | 235,702          | Unrolled|
| 7          | 234,441         | 237,295          | Normal  |
| 8          | 236,443         | 270,876          | Normal  |
| 9          | 243,031         | 243,902          | Normal  |
| 10         | 242,223         | 236,480          | Unrolled|

**Summary**: Unrolled loop faster in 6/10 trials (60% success rate)

## Key Findings

| Metric | Without Optimization | With -O3 Optimization |
|--------|---------------------|----------------------|
| Unrolled loop faster | 10/10 trials (100%) | 6/10 trials (60%) |
| Consistency | Consistent improvement | Variable performance |

## Conclusions

1. **Compiler optimization impact**: Loop unrolling shows more consistent benefits when compiler optimizations are disabled
2. **Performance variance**: With -O3, the compiler may already perform automatic optimizations that reduce the benefit of manual unrolling
3. **Manual unrolling value**: Most effective when working without aggressive compiler optimizations
4. **Consistency**: Unrolled loops show more predictable performance gains in unoptimized builds