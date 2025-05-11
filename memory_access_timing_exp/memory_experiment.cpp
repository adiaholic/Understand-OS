#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>  // For std::shuffle
#include <iomanip> // For formatting output

using namespace std;
using namespace chrono;

// Function to simulate register-only operations
double measure_registers(int value) {
    volatile int reg_copy = value;
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < 100000000; ++i) {
        reg_copy += 1;
    }
    auto end = high_resolution_clock::now();
    return duration<double>(end - start).count();
}

// Function to simulate cache access with sequential memory
double measure_cache(vector<int>& data, size_t size) {
    volatile int sum = 0;
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < size; ++i) {
        sum += data[i];
    }
    auto end = high_resolution_clock::now();
    return duration<double>(end - start).count();
}

// Function to simulate RAM access with random memory access
double measure_ram(vector<int>& data) {
    size_t N = data.size();
    vector<size_t> indices(N);
    for (size_t i = 0; i < N; ++i) indices[i] = i;

    random_device rd;
    mt19937 g(rd());
    shuffle(indices.begin(), indices.end(), g);

    volatile int sum = 0;
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        sum += data[indices[i]];
    }
    auto end = high_resolution_clock::now();
    return duration<double>(end - start).count();
}

// Function to print multiples relative to the fastest time
void print_multiples(const vector<string>& labels, const vector<double>& times) {
    double min_time = *min_element(times.begin(), times.end());
    cout << "\nAccess Time Multiples (relative to fastest):\n";
    for (size_t i = 0; i < labels.size(); ++i) {
        cout << labels[i] << ": " << fixed << setprecision(2) << times[i] / min_time << "x\n";
    }
}

// Function to draw ASCII bar graph
void draw_graph(const vector<string>& labels, const vector<double>& times) {
    double max_time = *max_element(times.begin(), times.end());
    double scale = 50.0 / max_time; // Scale bars to 50 chars max

    cout << "\nAccess Time Graph (relative)\n";
    cout << "-----------------------------\n";

    for (size_t i = 0; i < labels.size(); ++i) {
        cout << setw(10) << labels[i] << " | ";
        int bar_length = static_cast<int>(times[i] * scale);
        for (int j = 0; j < bar_length; ++j) cout << '=';
        cout << " (" << fixed << setprecision(6) << times[i] << "s)" << endl;
    }
}

// Main experiment controller
void run_experiment(vector<int>& data) {
    const size_t l1_size = 32 * 1024 / sizeof(int);
    const size_t l2_size = 256 * 1024 / sizeof(int);

    vector<string> labels = {"Registers", "L1 Cache", "L2 Cache", "RAM"};
    vector<double> times(4);

    times[0] = measure_registers(data[0]);
    cout << "Registers Simulation: " << times[0] << " seconds" << endl;

    times[1] = measure_cache(data, l1_size);
    cout << "L1 Cache Simulation:  " << times[1] << " seconds" << endl;

    times[2] = measure_cache(data, l2_size);
    cout << "L2 Cache Simulation:  " << times[2] << " seconds" << endl;

    times[3] = measure_ram(data);
    cout << "RAM Simulation:       " << times[3] << " seconds" << endl;

    print_multiples(labels, times);
    draw_graph(labels, times);
}

int main() {
    const size_t N = 100 * 1024 * 1024 / sizeof(int); // Allocate ~100MB
    vector<int> data(N, 1); // Initialize all elements to 1

    cout << "Memory Access Timing Experiment\n";
    cout << "--------------------------------\n";

    run_experiment(data);

    return 0;
}
