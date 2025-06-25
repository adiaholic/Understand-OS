#include <iostream>
#include <vector>
#include <chrono>

int main() {
    const int N = 100000000;
    const int NUM_EXPERIMENTS = 10;
    
    int normal_faster = 0;
    int unrolled_faster = 0;
    
    std::cout << "Running " << NUM_EXPERIMENTS << " performance experiments...\n\n";
    
    for (int experiment = 1; experiment <= NUM_EXPERIMENTS; experiment++) {
        std::vector<int> a(N, 1);
        
        // Normal loop
        auto start1 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++) {
            a[i] += 1;
        }
        auto end1 = std::chrono::high_resolution_clock::now();

        // Reset vector for fair comparison
        std::fill(a.begin(), a.end(), 1);
        
        // Unrolled loop (by factor of 4)
        auto start2 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i += 4) {
            a[i] += 1;
            a[i + 1] += 1;
            a[i + 2] += 1;
            a[i + 3] += 1;
        }
        auto end2 = std::chrono::high_resolution_clock::now();

        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();

        std::cout << "Experiment " << experiment << ":\n";
        std::cout << "  Normal loop time: " << duration1 << " us\n";
        std::cout << "  Unrolled loop time: " << duration2 << " us\n";
        
        if (duration1 < duration2) {
            std::cout << "  Result: Normal loop performed better\n";
            normal_faster++;
        } else {
            std::cout << "  Result: Unrolled loop performed better\n";
            unrolled_faster++;
        }
        std::cout << "\n";
    }
    
    std::cout << "=== EXPERIMENTAL RESULTS ===\n";
    std::cout << "Normal loop faster: " << normal_faster << "/" << NUM_EXPERIMENTS << " trials\n";
    std::cout << "Unrolled loop faster: " << unrolled_faster << "/" << NUM_EXPERIMENTS << " trials\n";
    
    if (normal_faster > unrolled_faster) {
        std::cout << "Overall result: Normal loop shows better performance\n";
    } else if (unrolled_faster > normal_faster) {
        std::cout << "Overall result: Unrolled loop shows better performance\n";
    } else {
        std::cout << "Overall result: Both approaches show equivalent performance\n";
    }

    return 0;
}
