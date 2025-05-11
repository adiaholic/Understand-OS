# 🧠 Understanding ARM64 Register Flushing

This project demonstrates how to read and flush both General-Purpose Registers (GPRs) and Floating-Point/SIMD Registers on ARM64 architecture using C++ and inline assembly.

---

## 📋 Overview

* **Read**: Capture the current contents of General-Purpose Registers (GPRs: x0–x30) and Floating-Point/SIMD Registers (FPRs: v0–v31).
* **Flush**: Attempt to zero these registers safely.
* **Re-Read**: Print the register contents after flushing to verify changes.

---

## 🛠️ Setup & Execution

1. **Compile**:

   ```bash
   g++ -o register_flush register_flush.cpp
   ```

2. **Run**:

   ```bash
   ./register_flush
   ```

---

## 🔍 Observations and Key Findings

### 1. General-Purpose Registers (GPRs)

* **Flushing is Temporary**:
  Even after issuing instructions to zero registers, the compiler/runtime **restores** certain GPRs automatically (specifically x19–x28).
  This happens because of the **ARM64 ABI** which requires these registers to be preserved across function calls.

* **Reserved System Registers**:
  Some registers like:

  * `x18` (Platform Register)
  * `x29` (Frame Pointer)
  * `x30` (Link Register)

  are **reserved by the OS/runtime** and **must not be flushed**.
  Flushing them caused **warnings**, but more importantly, could lead to **undefined behavior** (crashes or memory corruption).

* **Compiler Warnings**:
  The compiler warned when we tried to flush system-critical registers.
  **Warning**: *"inline asm clobber list contains reserved registers: X18, FP"*

* **State Cannot Be Fully Controlled**:
  Due to ABI, the true state of some registers is out of the application's control after inline assembly.

---

### 2. Floating-Point/SIMD Registers (FPRs)

* **Flushing Successful**:
  Floating-Point Registers (`v0`–`v31`) could be reliably flushed using `movi vN.16b, #0`.

* **Minor Residual Noise**:
  Even after flushing, tiny nonzero values like `3.45846e-323` sometimes appeared when interpreting the lower 64 bits as `double`.
  This is harmless and caused by:

  * Timing of register reading
  * Cache/memory effects
  * Immediate reuse by runtime

* **Fully Controllable**:
  Unlike GPRs, the FPRs can be completely zeroed out by flushing and immediately re-reading after.

---

### 3. Different Purposes of GPRs vs FPRs

| Type                                   | Purpose                                                   |
| :------------------------------------- | :-------------------------------------------------------- |
| General-Purpose Registers (x0–x30)     | Handle integer operations, memory addresses, control flow |
| Floating-Point/SIMD Registers (v0–v31) | Handle floating-point math, SIMD (vector) computations    |

* Their design and management by the CPU and OS are **completely different**.

---

## ⚠️ Important Notes

| Topic                                           | Insight                                                                                       |
| :---------------------------------------------- | :-------------------------------------------------------------------------------------------- |
| Cannot permanently flush GPRs                   | Compiler/OS restores critical registers.                                                      |
| Flushing system-reserved registers is dangerous | Must skip `x18`, `x29`, `x30`.                                                                |
| Flushing FPRs is reliable                       | No ABI constraint on `v0`–`v31`.                                                              |
| C++ inline assembly is limited                  | Cannot fully control register save/restore behavior without naked functions or pure assembly. |

---

# 🚀 Conclusion

* **GPRs**: Partial, temporary control possible. Full control not possible from C++.
* **FPRs**: Full flush and control possible with correct timing and memory handling.
* **System Behavior**: Following ARM64 ABI is mandatory to avoid undefined behavior.

---
