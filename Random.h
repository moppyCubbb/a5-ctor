#ifndef _RANDOM_H
#define _RANDOM_H

class Random{
      unsigned int seed;
  bool hasSeed;
    public:
    Random();
    void setSeed(unsigned int seed);
    int randomInt(int min, int max);
};

#endif
