#ifndef SHAMIR_H
#define SHAMIR_H

#include <vector>

struct Share {
    long long x;
    long long y;
};

// Added a verbose flag to toggle detailed terminal output
std::vector<Share> split_secret(long long secret, int n, int k, long long prime, bool verbose = false);
long long reconstruct_secret(const std::vector<Share>& shares, long long prime, bool verbose = false);

#endif // SHAMIR_H