#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <climits>

class Random {
public:
    // Public methods
    unsigned int nextInt() {
        const unsigned int t = seed_[0] ^ (seed_[0] << 11);
        seed_[0] = seed_[1];
        seed_[1] = seed_[2];
        seed_[2] = seed_[3];
        return seed_[3] = (seed_[3] ^ (seed_[3] >> 19)) ^ (t ^ (t >> 8));
    }

    unsigned int nextInt(int n) {
        return nextInt() % n;
    }

    double nextReal() {
        return (double)nextInt() / UINT_MAX;
    }

    Random(const unsigned int initial_seed) {
        unsigned int s = initial_seed;
        for (int i = 1; i <= 4; i++){
            seed_[i-1] = s = 1812433253U * (s^(s>>30)) + i;
        }
    }

private:
    // Private parameters
    unsigned int seed_[4];
};

#endif // _RANDOM_H_
