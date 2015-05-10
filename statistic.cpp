#include "statistic.h"
#include "sorting.cpp"

#include <algorithm>

statistic::statistic(vector< float > &_data)
{
    data = _data;
    summation = -1;
}

float statistic::GetSummation(void)
{
    summation = 0;
    for(int i = 0; i < data.size(); i ++)
        summation += data[i];
    return summation;
}

float statistic::GetAverage(void)
{
    return ((summation == -1)?GetSummation():summation)/data.size();
}

float statistic::GetMaximum(void)
{
    return *max_element(data.begin(), data.end());
}

float statistic::GetMedian(void)
{
    sorting sorted(data);
    vector< float > temp = sorted.mergeSort();
    if(data.size() % 2 == 0)
        median = (temp[temp.size()/2] + temp[temp.size()/2 + 1])/2;
    else
        median = temp[temp.size()/2 + 1];
    return median;
}
