#include "Random.h"
#include <cstdlib>

Random::Random() : hasSeed{false} {}

void Random::setSeed(unsigned int seed) {
     this-> seed = seed;
      hasSeed = true;
  srand(seed);
}

int Random::randomInt(int min, int max) {
  //if (hasSeed) {
  //      srand(seed);
  //  }
    return rand() % (max - min + 1) + min;
}
