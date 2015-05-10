#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <ctime>

typedef std::mt19937 Generator;

class random
{
public:
    random() { }
    random(int _seed) { engine.seed(_seed); }
    int getInt(void);
    float getFloat(void);
    void setBoundary(int _min, int _max);
private:
    int minimum, maximum;
    Generator engine;
};

#endif // RANDOM_H
