# Memory Access Timing Experiment

## Overview

This project measures the time taken to access data from different levels of the CPU memory hierarchy:

* **Registers**
* **L1 Cache**
* **L2 Cache**
* **RAM**

It benchmarks the access times and visualizes the results to highlight memory latency differences.

---

## How It Works

The program runs four tests:

* **Registers**: Operates on a local copy of an integer.
* **L1 Cache**: Sequentially accesses a small array (\~32 KB).
* **L2 Cache**: Sequentially accesses a medium array (\~256 KB).
* **RAM**: Randomly accesses a large array (\~100 MB).

The measured times are compared, and an ASCII graph visualizes the results.

---

## Build and Run

```bash
g++ -std=c++17 -O2 memory_experiment.cpp -o memory_experiment
./memory_experiment
```

---

## Output

```
Memory Access Timing Experiment
--------------------------------
Registers Simulation: 0.0416535 seconds
L1 Cache Simulation:  1.3167e-05 seconds
L2 Cache Simulation:  8.9708e-05 seconds
RAM Simulation:       0.115759 seconds

Access Time Multiples (relative to fastest):
Registers: 3163.47x
L1 Cache: 1.00x
L2 Cache: 6.81x
RAM: 8791.57x

Access Time Graph (relative)
-----------------------------
 Registers | ================= (0.041653s)
  L1 Cache |  (0.000013s)
  L2 Cache |  (0.000090s)
       RAM | ================================================== (0.115759s)
```

---

## Key Findings

* **L1 Cache** access was the fastest in this run.
* **Register operations** were slower than L1 cache access, likely due to loop overhead and operation count.
* **RAM access** was significantly slower — approximately **8791×** slower than L1 cache access.
* **L2 Cache** access was slower than L1 but much faster than RAM.
* The differences clearly demonstrate the impact of memory hierarchy on performance.

---

## Code Structure

| Function              | Purpose                                   |
| :-------------------- | :---------------------------------------- |
| `measure_registers()` | Measures register-local computation time  |
| `measure_cache()`     | Measures sequential cache access          |
| `measure_ram()`       | Measures random RAM access                |
| `print_multiples()`   | Prints relative access time multiples     |
| `draw_graph()`        | Draws ASCII visualization of access times |
| `run_experiment()`    | Coordinates the benchmark execution       |

