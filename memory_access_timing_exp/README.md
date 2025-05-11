# Memory Access Timing Experiment

## Overview

This project measures the time required to access data stored in different parts of the CPU memory hierarchy:

* **Registers**
* **L1 Cache**
* **L2 Cache**
* **RAM**

It highlights how memory access latency increases significantly as data moves farther from the CPU core.

---

## Motivation

Modern CPUs operate much faster than memory systems.
Understanding cache behavior and memory latency is critical for building high-performance software.
This project demonstrates these hardware effects with practical measurements.

---

## How It Works

The program performs four experiments:

* **Registers Simulation**: Operates on a local copy of an integer to simulate register access.
* **L1 Cache Simulation**: Accesses a small array (\~32 KB) sequentially to stay within L1 cache.
* **L2 Cache Simulation**: Accesses a larger array (\~256 KB) to overflow L1 and utilize L2 cache.
* **RAM Simulation**: Accesses a large array (\~100 MB) randomly to ensure cache misses and RAM access.

After measuring:

* **Multiples relative to the fastest access** are calculated.
* An **ASCII bar graph** visualizes the access times.

---

## How to Build and Run

```bash
g++ -std=c++17 -O2 memory_experiment.cpp -o memory_experiment
./memory_experiment
```

* `-std=c++17` ensures compatibility with modern C++ features.
* `-O2` optimization allows the compiler to better utilize CPU registers.

---

## Example Output

```

>> g++ -std=c++17 -O2 memory_experiment.cpp -o memory_experiment
./memory_experiment
Memory Access Timing Experiment
--------------------------------
Registers Simulation: 0.0500384 seconds
L1 Cache Simulation:  1.3083e-05 seconds
L2 Cache Simulation:  0.000100167 seconds
RAM Simulation:       0.11041 seconds

Access Time Multiples (relative to fastest):
Registers: 3824.69x
L1 Cache: 1.00x
L2 Cache: 7.66x
RAM: 8439.21x

Access Time Graph (relative)
-----------------------------
 Registers | ====================== (0.050038s)
  L1 Cache |  (0.000013s)
  L2 Cache |  (0.000100s)
       RAM | ================================================== (0.110410s)

```

---

## Code Structure

| Function              | Responsibility                                        |
| :-------------------- | :---------------------------------------------------- |
| `measure_registers()` | Measure register-local operation time                 |
| `measure_cache()`     | Measure sequential array access time (cache friendly) |
| `measure_ram()`       | Measure random access time (forces RAM usage)         |
| `print_multiples()`   | Print access times relative to fastest layer          |
| `draw_graph()`        | Print an ASCII bar graph of access times              |
| `run_experiment()`    | Orchestrate the full benchmark                        |

---

## Key Concepts

| Memory Layer | Characteristics                                         |
| :----------- | :------------------------------------------------------ |
| Registers    | Fastest (1–2 CPU cycles), no memory access              |
| L1 Cache     | Very small (\~32 KB), extremely fast (\~3–5 cycles)     |
| L2 Cache     | Larger (\~256 KB–1 MB), moderate speed (\~10–20 cycles) |
| RAM          | Very large (GBs), very slow (\~200–500+ cycles)         |

* **CPU lookup flow**: Registers → L1 Cache → L2 Cache → L3 Cache (optional) → RAM
* **Cache miss** penalties grow with each level missed.

