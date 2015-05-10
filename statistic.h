#ifndef STATISTIC_H
#define STATISTIC_H

#include "type.h"

class statistic
{
public:
    statistic(vector< float > &_data);
    float GetSummation(void);
    float GetAverage(void);
    float GetMaximum(void);
    float GetMedian(void);
private:
    vector< float > data;
    float summation;
    float median;
};

#endif // STATISTIC_H
