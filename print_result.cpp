#include "print_result.h"
#include "statistic.cpp"

#include <iostream>
#include <iomanip>

using namespace std;

print_result::print_result(vector< particle > &_data)
{
    data = _data;
    precision = 0;
}

print_result::print_result(vector< particle > &_data, int _precision)
{
    data = _data;
    precision = _precision;
}

void print_result::print_statistic(void)
{
    cout << "============================= Statistic =============================" << endl;
    cout << "|-------------------------------------------------------------------|" << endl;
    cout << "|" << left << setw(7) << " Star";
    cout << "|" << left << setw(29) << "         Summation";
    cout << "|" << left << setw(29) << "          Average";
    cout << "|" << endl<< setw(8) << "|" ;
    cout << "|" << left << setw(29) << "          Maximum";
    cout << "|" << left << setw(29) << "          Median";
    cout << "|" << endl;
    cout << "|-------------------------------------------------------------------|" << endl;

    for(int i = 0; i < data.size(); i ++)
    {
        cout << "|" << left << "  " << setw(5) << data[i].getName();

        vector< float > xSteps = data[i].getSingleStep().x;
        vector< float > ySteps = data[i].getSingleStep().y;
        statistic calculateX(xSteps);
        statistic calculateY(ySteps);

        cout << "|" << "     ( " << right << setw(6) << fixed << setprecision(precision) << calculateX.GetSummation();
        cout << " ," << right << setw(7) << setprecision(precision) << calculateY.GetSummation() << " )     ";

        cout << "|" << "     ( " << right << setw(6) << fixed << setprecision(precision) << calculateX.GetAverage();
        cout << " ," << right << setw(7) << setprecision(precision) << calculateY.GetAverage() << " )     ";

        cout << "|" << endl << "|" << left << "  " << setw(5) << "     ";

        cout << "|" << "     ( " << right << setw(6) << fixed << setprecision(precision) << calculateX.GetSummation();
        cout << " ," << right << setw(7) << setprecision(precision) << calculateY.GetMaximum() << " )     ";

        cout << "|" << "     ( " << right << setw(6) << fixed << setprecision(precision) << calculateX.GetAverage();
        cout << " ," << right << setw(7) << setprecision(precision) << calculateY.GetMedian() << " )     ";


        cout << "|" << endl;

        cout << "|-------------------------------------------------------------------|" << endl;
    }

    cout << endl;
}

void print_result::print_data(void)
{
    cout << "============================== Result ===============================" << endl;
    cout << "|-------------------------------------------------------------------|" << endl;
    cout << "|" << left << setw(7) << " Star";
    cout << "|" << left << setw(7) << " Mass";
    cout << "|" << left << setw(25) << "         ( x, y )";
    cout << "|" << left << setw(25) << "        ( Vx, Vy )";
    cout << "|" << endl;
    cout << "|-------------------------------------------------------------------|" << endl;

    for(int i = 0; i < data.size(); i ++)
    {
        cout << "|" << left << "  " << setw(5) << data[i].getName();

        cout << "|" << right << setw(5) << data[i].getMass() << "  ";

        cout << "|" << "  ( " << right << setw(8) << fixed << setprecision(precision) << data[i].getPosition().x;
        cout << "," << right << setw(8) << setprecision(precision) << data[i].getPosition().y << " )  ";

        cout << "|" << "  ( " << right << setw(8) << fixed << setprecision(precision) << data[i].getVelocity().x;
        cout << "," << right << setw(8) << setprecision(precision) << data[i].getVelocity().y << " )  ";

        cout << "|" << endl;
    }

    cout << "|-------------------------------------------------------------------|" << endl << endl;
}
