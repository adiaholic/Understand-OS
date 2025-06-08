#include <iostream>
#include <unistd.h>  // For sleep()

int main() {
    size_t size = 10 * 1024 * 1024;  // 10 MB
    char* buffer = new char[size];

    // Touch every 4KB page to force allocation
    for (size_t i = 0; i < size; i += 4096) {
        buffer[i] = 'X';
    }

    std::cout << "Allocated and touched " << size << " bytes (approx. 10MB) of memory.\n";

    // Wait for 10 seconds
    while (true) {
        std::cout << "Memory hog running...\n";
        sleep(10);
    }

    delete[] buffer;
    return 0;
}
