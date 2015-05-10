#ifndef PRINT_RESULT_H
#define PRINT_RESULT_H

#include "type.h"
#include "particle.h"
#include <vector>

class print_result
{
public:
    print_result(vector< particle > &_data);
    print_result(vector< particle > &_data, int _precision);
    void SetData(vector< particle > &_data)
    {
        data = _data;
    }
    void print_statistic(void);
    void print_data(void);
    int GetPrecision()
    {
        return precision;
    }
    void SetPrecision(int _precision)
    {
        precision = _precision;
    }
private:
    vector< particle > data;
    int precision;
};

#endif // PRINT_RESULT_H
