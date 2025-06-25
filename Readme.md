# Understand OS

This repository contains a collection of small projects exploring low-level systems programming concepts.

## Projects

* [`loop_unrolling/`](./loop_unrolling/):
  Compare performance of normal vs unrolled loops to analyze branch overhead and instruction pipelining.

* [`inspect_registers/`](./inspect_registers/):
  Read and flush CPU registers on ARM64; observe system constraints and behavior.

* [`hard_links/`](./hard_links/):
  Trace and analyze system calls (`linkat`, `unlinkat`, `openat`) involved in creating, deleting, and reading hard links.

* [`processes_and_cgroups/`](./processes_and_cgroups/):
  Create cgroups for processes and experiment with memory limits to trigger OOM conditions.
