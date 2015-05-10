#include "random.h"

void random::setBoundary(int _min, int _max)
{
    minimum = _min;
    maximum = _max;
}

float random::getFloat(void)
{
    std::uniform_real_distribution< float > distribution(minimum, maximum);
    return distribution(engine);
}

int random::getInt(void)
{
    std::tr1::uniform_int<int> distribution(minimum, maximum);
    //std::uniform_int_distribution< int > distribution(minimum, maximum);
    return distribution(engine);
}
