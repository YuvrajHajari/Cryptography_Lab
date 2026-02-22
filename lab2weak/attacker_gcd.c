#include <stdio.h>

// Function to calculate Greatest Common Divisor
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int main() {
    int n = 9;
    printf("7.2 GCD Analysis Attack Output\n\n");
    printf("=== ATTACKER: GCD ANALYSIS ATTACK ===\n");
    printf("Target System Modulus n = %d\n\n", n);
    printf("Phase 1: Passive Traffic Sniffing\n");
    printf("Attacker captured from network:\n");
    printf("  - Modulus n = %d (PUBLIC)\n", n);
    printf("  - Various User_IDs and Tokens\n\n");
    printf("Phase 2: GCD Analysis\n");
    printf("Testing different User_IDs for common factors with n:\n\n");

    for (int id = 1; id <= 10; id++) {
        int common_factor = gcd(id, n);
        printf("User_ID = %d: gcd(%d, %d) = %d", id, id, n, common_factor);
        
        if (common_factor > 1 && common_factor < n) {
            printf(" <- VULNERABILITY DETECTED!\n");
            printf("  -> Factor discovered: %d\n", common_factor);
            printf("  -> Factorization: %d = %d x %d\n", n, common_factor, n/common_factor);
        } else if (common_factor == n) {
             printf(" <- VULNERABILITY DETECTED!\n");
             printf("  -> Factor discovered: %d\n", n);
             printf("  -> Factorization: %d = %d x 1\n", n, n);
        } else {
            printf(" (safe for this user)\n");
        }
    }

    printf("\n/!\\SYSTEM COMPROMISED!\n");
    printf("Prime factorization of n achieved\n");
    printf("phi(9) = 6\n");

    return 0;
}