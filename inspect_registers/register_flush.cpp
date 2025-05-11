#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

uint64_t gpr[31];
uint8_t fpr[512]; // 32 x 16 bytes (128 bits per vN register)

// Read General Purpose Registers (x0-x30)
void read_gpr() {
    asm volatile(
        "mov %0, x0\n\t" "mov %1, x1\n\t" "mov %2, x2\n\t" "mov %3, x3\n\t"
        "mov %4, x4\n\t" "mov %5, x5\n\t" "mov %6, x6\n\t" "mov %7, x7\n\t"
        : "=r"(gpr[0]), "=r"(gpr[1]), "=r"(gpr[2]), "=r"(gpr[3]),
          "=r"(gpr[4]), "=r"(gpr[5]), "=r"(gpr[6]), "=r"(gpr[7])
        : : 
    );
    asm volatile(
        "mov %0, x8\n\t" "mov %1, x9\n\t" "mov %2, x10\n\t" "mov %3, x11\n\t"
        "mov %4, x12\n\t" "mov %5, x13\n\t" "mov %6, x14\n\t" "mov %7, x15\n\t"
        : "=r"(gpr[8]), "=r"(gpr[9]), "=r"(gpr[10]), "=r"(gpr[11]),
          "=r"(gpr[12]), "=r"(gpr[13]), "=r"(gpr[14]), "=r"(gpr[15])
        : : 
    );
    asm volatile(
        "mov %0, x16\n\t" "mov %1, x17\n\t" "mov %2, x19\n\t" "mov %3, x20\n\t"
        "mov %4, x21\n\t" "mov %5, x22\n\t" "mov %6, x23\n\t" "mov %7, x24\n\t"
        : "=r"(gpr[16]), "=r"(gpr[17]), "=r"(gpr[19]), "=r"(gpr[20]),
          "=r"(gpr[21]), "=r"(gpr[22]), "=r"(gpr[23]), "=r"(gpr[24])
        : : 
    );
    asm volatile(
        "mov %0, x25\n\t" "mov %1, x26\n\t" "mov %2, x27\n\t" "mov %3, x28\n\t"
        : "=r"(gpr[25]), "=r"(gpr[26]), "=r"(gpr[27]), "=r"(gpr[28])
        : : 
    );
}

// Read Floating Point/SIMD Registers (v0-v31)
void read_fpr() {
    asm volatile(
        "str q0, [%0, #0]\n\t"  "str q1, [%0, #16]\n\t" "str q2, [%0, #32]\n\t" "str q3, [%0, #48]\n\t"
        "str q4, [%0, #64]\n\t" "str q5, [%0, #80]\n\t" "str q6, [%0, #96]\n\t" "str q7, [%0, #112]\n\t"
        "str q8, [%0, #128]\n\t" "str q9, [%0, #144]\n\t" "str q10, [%0, #160]\n\t" "str q11, [%0, #176]\n\t"
        "str q12, [%0, #192]\n\t" "str q13, [%0, #208]\n\t" "str q14, [%0, #224]\n\t" "str q15, [%0, #240]\n\t"
        "str q16, [%0, #256]\n\t" "str q17, [%0, #272]\n\t" "str q18, [%0, #288]\n\t" "str q19, [%0, #304]\n\t"
        "str q20, [%0, #320]\n\t" "str q21, [%0, #336]\n\t" "str q22, [%0, #352]\n\t" "str q23, [%0, #368]\n\t"
        "str q24, [%0, #384]\n\t" "str q25, [%0, #400]\n\t" "str q26, [%0, #416]\n\t" "str q27, [%0, #432]\n\t"
        "str q28, [%0, #448]\n\t" "str q29, [%0, #464]\n\t" "str q30, [%0, #480]\n\t" "str q31, [%0, #496]\n\t"
        : : "r"(fpr)
        : "memory"
    );
}

// Print General Purpose Registers
void print_gpr() {
    cout << "General Purpose Registers (x0-x30):" << endl;
    for (int i = 0; i <= 30; ++i) {
        cout << "X" << setw(2) << i << ": 0x" << hex << gpr[i] << dec << endl;
    }
}

// Print Floating Point/SIMD Registers
void print_fpr() {
    cout << "\nFloating Point/SIMD Registers (v0-v31, lower 64 bits interpreted as double):" << endl;
    for (int i = 0; i < 32; ++i) {
        double val;
        memcpy(&val, fpr + i * 16, sizeof(double)); // Only lower 64 bits
        cout << "V" << setw(2) << i << ": " << val << endl;
    }
}

// Flush (zero) General Purpose Registers (skip x18, x29, x30)
void flush_gpr() {
    asm volatile(
        "mov x0, xzr\n\t" "mov x1, xzr\n\t" "mov x2, xzr\n\t" "mov x3, xzr\n\t"
        "mov x4, xzr\n\t" "mov x5, xzr\n\t" "mov x6, xzr\n\t" "mov x7, xzr\n\t"
        "mov x8, xzr\n\t" "mov x9, xzr\n\t" "mov x10, xzr\n\t" "mov x11, xzr\n\t"
        "mov x12, xzr\n\t" "mov x13, xzr\n\t" "mov x14, xzr\n\t" "mov x15, xzr\n\t"
        "mov x16, xzr\n\t" "mov x17, xzr\n\t"
        "mov x19, xzr\n\t" "mov x20, xzr\n\t" "mov x21, xzr\n\t" "mov x22, xzr\n\t"
        "mov x23, xzr\n\t" "mov x24, xzr\n\t" "mov x25, xzr\n\t" "mov x26, xzr\n\t"
        "mov x27, xzr\n\t" "mov x28, xzr\n\t"
        : : : "x0","x1","x2","x3","x4","x5","x6","x7",
              "x8","x9","x10","x11","x12","x13","x14","x15",
              "x16","x17",
              "x19","x20","x21","x22","x23","x24","x25","x26","x27","x28"
    );
}

// Flush (zero) Floating Point/SIMD Registers
void flush_fpr() {
    asm volatile(
        "movi v0.16b, #0\n\t" "movi v1.16b, #0\n\t" "movi v2.16b, #0\n\t" "movi v3.16b, #0\n\t"
        "movi v4.16b, #0\n\t" "movi v5.16b, #0\n\t" "movi v6.16b, #0\n\t" "movi v7.16b, #0\n\t"
        "movi v8.16b, #0\n\t" "movi v9.16b, #0\n\t" "movi v10.16b, #0\n\t" "movi v11.16b, #0\n\t"
        "movi v12.16b, #0\n\t" "movi v13.16b, #0\n\t" "movi v14.16b, #0\n\t" "movi v15.16b, #0\n\t"
        "movi v16.16b, #0\n\t" "movi v17.16b, #0\n\t" "movi v18.16b, #0\n\t" "movi v19.16b, #0\n\t"
        "movi v20.16b, #0\n\t" "movi v21.16b, #0\n\t" "movi v22.16b, #0\n\t" "movi v23.16b, #0\n\t"
        "movi v24.16b, #0\n\t" "movi v25.16b, #0\n\t" "movi v26.16b, #0\n\t" "movi v27.16b, #0\n\t"
        "movi v28.16b, #0\n\t" "movi v29.16b, #0\n\t" "movi v30.16b, #0\n\t" "movi v31.16b, #0\n\t"
        : : : "v0","v1","v2","v3","v4","v5","v6","v7",
              "v8","v9","v10","v11","v12","v13","v14","v15",
              "v16","v17","v18","v19","v20","v21","v22","v23",
              "v24","v25","v26","v27","v28","v29","v30","v31"
    );
}

int main() {
    cout << "Dumping CPU register values BEFORE flushing:\n\n";

    read_gpr();
    read_fpr();
    print_gpr();
    print_fpr();

    flush_gpr();
    flush_fpr();
    memset(fpr, 0, sizeof(fpr)); // Clear local buffer

    cout << "\n\nDumping CPU register values AFTER flushing:\n\n";

    read_gpr();
    read_fpr();
    print_gpr();
    print_fpr();

    return 0;
}
