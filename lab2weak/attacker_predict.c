#include <stdio.h>

int main() {
    int n = 9;
    int user_id = 2;
    printf("--- ATTACK: Token Cycle Prediction (n=9) ---\n");
    
    // Demonstrate that k=1, k=7, and k=13 all yield the same token
    for (int k = 1; k <= 13; k++) {
        int token = 1;
        for(int i=0; i<k; i++) token = (token * user_id) % n;
        
        printf("Trying Key k=%d -> Token Result: %d", k, token);
        if (token == 2) printf(" [MATCH FOUND]");
        printf("\n");
    }
    printf("\nConclusion: Attacker can bypass 'k=7' by simply using 'k=1'.\n");
    return 0;
}