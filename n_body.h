#ifndef N_BODY_H
#define N_BODY_H

#include "type.h"
#include "file.cpp"
#include "print_result.cpp"

#include <vector>
#include <string>
#include <iostream>

#define MAX_SIMU_AMOUNT     1000

#define DEFAULT_SUMT        10
#define DEFAULT_DELTAT      0.1
#define DEFAULT_GRAVITY     6.67428
//TODO: define default boundary

using namespace std;

class n_body
{
public:
    n_body();
    void setBoundary(Boundary &_boundary) { boundary = _boundary; }
    Boundary getBoundary(void) { return boundary; };
    void setGravity(float _gravity) { gravity = _gravity; }
    float getGravity(void) { return gravity; }
    void setSumT(float _sumT) { sumT = _sumT; }
    float getSumT(void) { return sumT; }
    void setDeltaT(float _deltaT) { deltaT = _deltaT; }
    float getDeltaT(void) { return deltaT; }
    void calculate(void);
    void generate(int type)
    {
        switch(type)
        {
        case 1:
            getAmount();
            byRandom();
            break;
        case 2:
            getAmount();
            byManual();
            break;
        case 3:
            byFile();
            break;
        }

        for(int i = 0; i < data.size(); i++)
            data[i].setCondition(gravity, deltaT);
    }
    //TODO: edit precision
    void printResult(void)
    {
        print_result print(data, 2);
        print.print_data();
        //print.print_statistic();
    }
private:
    int amount;
    float gravity;
    float deltaT, sumT;
    vector< particle > data;
    Boundary boundary;
    void byRandom();
    void byManual();
    void byFile();
    void getAmount();
};

#endif // N_BODY_H
