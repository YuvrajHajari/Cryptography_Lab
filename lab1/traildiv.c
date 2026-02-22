#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

bool isPrimeTrial(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int main() {
    long long num;
    if (scanf("%lld", &num) != 1) return 1;

    long long iterations = 1000000; // Run it a million times to make it measurable
    clock_t start = clock();
    
    for (long long i = 0; i < iterations; i++) {
        isPrimeTrial(num);
    }
    
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    FILE *f = fopen("trial_data.txt", "a");
    if (f != NULL) {
        fprintf(f, "%lld,%f\n", num, time_taken);
        fclose(f);
    }

    // Add a print statement so you can see it in the terminal too
    printf("Tested %lld: Time for 1M iterations = %f seconds\n", num, time_taken);

    return 0;
}