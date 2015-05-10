#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cmath>
#include <ctime>
#include <windows.h>

#define MAX_MASS    100
#define MAX_X       1000
#define MAX_Y       1000
#define MAX_VX      10
#define MAX_VY      10
#define MAX_SIMU    1000
//#define G           6.67428
#define G 0.15
#define deltaT      0.05
#define sumT        1000
#define ITERATION   sumT/deltaT
#define PRECISION   3
#define RATIO       1
#define CLR_INV     1

using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

HWND console = GetConsoleWindow();
HDC handle = GetDC(console);
COLORREF color = RGB(255, 255, 255);

struct Star
{
    string name;
    double x, y;
    double TEMPx, TEMPy;
    double vx, vy;
    int mass;
};

string int2str(int input)
{
    stringstream ss;
    ss << input;
    return ss.str();
}

void randomInit(vector< Star > &input)
{
    srand(time(NULL));
    for(int i = 0; i < input.size(); i ++)
    {
        input[i].name = int2str(i+1);

        input[i].x = input[i].TEMPx = rand() % MAX_X;
        input[i].y = input[i].TEMPy = rand() % MAX_Y;
        input[i].mass = rand() % MAX_MASS + 1;

        //TODO: modify velocity direction
        input[i].vx = rand() % MAX_VX;
        input[i].vy = rand() % MAX_VY;
    }
}

void manualInit(vector< Star > &input)
{
    for(int i = 0; i < input.size(); i ++)
    {
        input[i].name = int2str(i+1);

        cout << endl << "Configuration for Star \"" << input[i].name << "\"" << endl;

        cout << " -Mass: ";
        while(!(cin >> input[i].mass))
        {
            cout << " *Please enter an integer. Re-type: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        cout << " -Coordinate: (x y) = ";
        while(!(cin >> input[i].x >> input[i].y))
        {
            cout << " *Please enter integers only. Re-type: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        input[i].TEMPx = input[i].x;
        input[i].TEMPy = input[i].y;

        cout << " -Velocity: (Vx Vy) = ";
        while(!(cin >> input[i].vx >> input[i].vy))
        {
            cout << " *Please enter integers only. Re-type: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

void init(vector< Star > &input)
{
    cout << endl  << "Would you like to manual configure the initial condition? (y/n) ";
    char selection;
    bool confirmResult;
    do
    {
        confirmResult = true;

        cin >> selection;
        switch(selection)
        {
            case 'y':
            case 'Y':
                manualInit(input);
                break;
            case 'n':
            case 'N':
                randomInit(input);
                break;
            default:
                cout << "Not an available option, please re-enter: ";
                confirmResult = false;
        }
    }while(!confirmResult);
}

void getAmount(int &storage)
{
    cout << "How many stars to simulate? ";
    bool firstTime = true;
    do
    {
        if(firstTime)
            firstTime = false;
        else
            cout << "Simulate amount exceed maximum limit (" << MAX_SIMU << "). Re-type: ";

        while(!(cin >> storage))
        {
            cout << "Please enter an integer. Re-type: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }while(storage > MAX_SIMU);

}

void printResult(vector< Star > &input)
{
    cout << "|----------------------------------------------------------------|" << endl;
    cout << "|" << right << setw(7) << "Star\t";
    cout << "|" << right << setw(7) << "Mass\t";
    cout << "|" << right << setw(10) << "\t(x,y)\t\t";
    cout << "|" << right << setw(10) << "\t(Vx,Vy)\t";
    cout << setw(10) << "|" << endl;
    cout << "|----------------------------------------------------------------|" << endl;

    for(int i = 0; i < input.size(); i ++)
    {
        cout << "|" << right << setw(4) << input[i].name << "\t";
        cout << "|" << right << setw(4) << input[i].mass << "\t";
        cout << "|" << right << setw(3) << "(" << fixed << setprecision(PRECISION) << input[i].x << "," << setprecision(PRECISION) << input[i].y << ")\t";
        cout << "|" << right << setw(3) << "(" << fixed << setprecision(PRECISION) << input[i].vx << "," << setprecision(PRECISION) << input[i].vy << ")\t";
        cout << setw(2) << "|" << endl;
    }

    cout << "|----------------------------------------------------------------|" << endl;
}

double distanceSQR(Star &input1, Star &input2)
{
    return (pow(input1.x - input2.x, 2) + pow(input1.y - input2.y, 2));
}

void applyVelocity(Star &input1, Star &input2, double acceleration)
{
    input1.vx += acceleration * (input2.x - input1.x) / pow(distanceSQR(input1, input2), 0.5) * deltaT;
    input1.vy += acceleration * (input2.y - input1.y) / pow(distanceSQR(input1, input2), 0.5) * deltaT;
}

void applyCoordinate(Star &input)
{
    input.TEMPx += input.vx * deltaT;
    input.TEMPy += input.vy * deltaT;
}

void saveCoordinate(vector< Star > &input)
{
    for(int i=0; i < input.size(); i++)
    {
        input[i].x = input[i].TEMPx;
        input[i].y = input[i].TEMPy;

        //TODO: isolate plot function
        //TODO: add color settings for different stars
        //TODO: add option for track enable/disable
        SetPixel(handle, input[i].x/RATIO, input[i].y/RATIO, color);
    }
}

void calculate(vector< Star > &input)
{
    for(int i=0; i < input.size(); i ++)
    {
        double acceleration = 0;
        for(int j=0; j < input.size(); j++)
        {
            if(j == i)
                continue;
            acceleration += G * input[j].mass / distanceSQR(input[i], input[j]);
            applyVelocity(input[i], input[j], acceleration);
        }

        applyCoordinate(input[i]);
    }
    saveCoordinate(input);
}

int main()
{
    int starAmount = 0;
    getAmount(starAmount);

    vector< Star > list(starAmount);

    init(list);

    cout << endl;

    cout << "============================ Initialize ==========================" << endl;
    cout << right << setw(10) << "G=" << G << setw(10) << "dT=" << deltaT << setw(10) << "T=" << sumT << endl;
    printResult(list);

    system("pause");
    system("cls");

    for(int k=0; k < ITERATION; k++)
    {
        calculate(list);
        Sleep(CLR_INV);
    }

    system("pause");

    cout << endl;
    cout << "============================= Result =============================" << endl;
    printResult(list);

    cout << endl;

    system("pause");

	ReleaseDC(console, handle);

    return 0;
}
