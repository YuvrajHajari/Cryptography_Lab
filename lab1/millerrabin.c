#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Helper function to multiply (a * b) % mod without overflow
long long mulMod(long long a, long long b, long long mod) {
    long long res = 0;
    a %= mod;
    while (b > 0) {
        if (b % 2 == 1) res = (res + a) % mod;
        a = (a * 2) % mod;
        b /= 2;
    }
    return res;
}

// Function to calculate (base^exp) % mod
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = mulMod(res, base, mod);
        base = mulMod(base, base, mod);
        exp /= 2;
    }
    return res;
}

int millerTest(long long d, long long n) {
    if (n <= 4) return (n == 2 || n == 3); 

    long long a = 2 + rand() % (n - 4);
    long long x = power(a, d, n);

    if (x == 1 || x == n - 1) return 1;

    while (d != n - 1) {
        x = mulMod(x, x, n);
        d *= 2;

        if (x == 1) return 0;
        if (x == n - 1) return 1;
    }
    return 0;
}

int isMillerRabin(long long n, int k) {
    if (n <= 1 || n == 4) return 0;
    if (n <= 3) return 1;

    long long d = n - 1;
    while (d % 2 == 0) d /= 2;

    for (int i = 0; i < k; i++) {
        if (!millerTest(d, n)) return 0;
    }
    return 1;
}

int main() {
    long long num;
    printf("Enter a number to test: ");
    if (scanf("%lld", &num) != 1) return 1;

    srand(time(NULL));

    long long iterations = 1000000;
    clock_t start = clock();
    
    int result = 0;
    for (long long i = 0; i < iterations; i++) {
        result = isMillerRabin(num, 5);
    }
    
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (result)
        printf("%lld is Probably Prime\n", num);
    else
        printf("%lld is Composite\n", num);
    
    printf("Time taken for 1M iterations: %f seconds\n", time_taken);

    FILE *f = fopen("miller_data.txt", "a");
    if (f != NULL) {
        fprintf(f, "%lld,%f\n", num, time_taken);
        fclose(f);
    }

    return 0;
}