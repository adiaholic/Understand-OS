#include <iostream>

int main() {
    const int N = 1000;  // Define array size
    int a[N];            // Declare array

    // Your original loop
	for (int i = 0; i < N; i += 4) {
		a[i] += 1;
		a[i + 1] += 1;
		a[i + 2] += 1;
		a[i + 3] += 1;
	}

    return 0;
}
