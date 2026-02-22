#include <stdio.h>
#include <time.h>

long long modInverse(long long a, long long m) {
    long long m0 = m, t, q, x0 = 0, x1 = 1;
    while (a > 1) {
        if (m == 0) return 0; // Prevent division by zero
        q = a / m;
        t = m; m = a % m, a = t;
        t = x0; x0 = x1 - q * x0; x1 = t;
    }
    return (x1 < 0) ? x1 + m0 : x1;
}

int main() {
    int m[3], a[3];
    // Read input: m1 a1 m2 a2 m3 a3
    if (scanf("%d %d %d %d %d %d", &m[0], &a[0], &m[1], &a[1], &m[2], &a[2]) != 6) return 1;

    // We run the calculation 1,000,000 times to make it measurable
    long long iterations = 1000000;
    clock_t start = clock();
    
    long long final_res = 0;
    long long M = (long long)m[0] * m[1] * m[2];

    for (long long j = 0; j < iterations; j++) {
        long long res = 0;
        for (int i = 0; i < 3; i++) {
            long long Mi = M / m[i];
            res += a[i] * modInverse(Mi, m[i]) * Mi;
        }
        final_res = res % M;
    }
    
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Save to file
    FILE *f = fopen("crt_data.txt", "a");
    if (f != NULL) {
        fprintf(f, "%lld,%f\n", M, time_taken);
        fclose(f);
    }

    return 0;
}