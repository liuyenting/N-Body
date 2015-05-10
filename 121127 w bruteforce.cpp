#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <random>
#include <chrono>
#include <fstream>

#define VERSION         121127

#define MAX_MASS        100
#define MAX_X           500
#define MAX_Y           500
#define MAX_VX          10
#define MAX_VY          10

#define MAX_SIMU        1000
#define DEFAULT_G       6.74
#define DEFAULT_dT      0.02
#define DEFAULT_sumT    50

#define PRINT_LIM       500

#define DEFAULT_ctrX    200
#define DEFAULT_ctrY    200
#define RATIO           4
#define REFRESH         1

#define PRECISION       2

using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

HWND console = GetConsoleWindow();
HDC handle = GetDC(console);

int massRange[2];
int XcoordinateRange[2];
int YcoordinateRange[2];
int XvelocityRange[2];
int YvelocityRange[2];
double G, deltaT = 1, sumT = 1;
int centerX, centerY;
double zoom;
int refresh;
int precision, limit;

struct Star
{
    string name;
    double x, y;
    double TEMPx, TEMPy;
    double vx, vy;
    int mass;
    COLORREF color;
};

typedef mt19937 Generator;
typedef uniform_int_distribution<> Distribute;

template< typename T > string toString(const T& t)
{
    ostringstream os;
    os << t;
    return os.str();
}

void initEnvSetup()
{
    massRange[0] = 0;
    massRange[1] = MAX_MASS;

    XcoordinateRange[0] = 0;
    XcoordinateRange[1] = MAX_X;
    YcoordinateRange[0] = 0;
    YcoordinateRange[1] = MAX_Y;

    XvelocityRange[0] = 0;
    XvelocityRange[1] = MAX_VX;
    YvelocityRange[0] = 0;
    YvelocityRange[1] = MAX_VY;

    G = DEFAULT_G;
    deltaT = DEFAULT_dT;
    sumT = DEFAULT_sumT;

    centerX = DEFAULT_ctrX;
    centerY = DEFAULT_ctrY;
    zoom = RATIO;
    refresh = REFRESH;

    precision = PRECISION;
    limit = PRINT_LIM;
}

int menuSelection(vector< string > &items)
{
    cout << "-----------------------------------------" << endl;

    for(int i = 0; i < items.size(); i ++)
        cout << "  [" << i+1 << "]" << " " << items[i] << endl;

    bool proper;
    int selection;
    do
    {
        cout << endl << "Please choose an action: ";
        while(!(cin >> selection))
        {
            cout << " *Please enter an INTEGER as your option." << endl;
            cout << endl << "Please choose an action: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        if(selection < 0 || selection > items.size())
            proper = false;
        else
            return selection;

        cout << " *Please enter an AVAILABLE option." << endl;
    }
    while(!proper);

    return 0;
}

void modify_simulation_settings()
{
    bool proper = true;
    int selection;

    system("cls");

    cout << "Current configuration for simulation: " << endl << endl;
    cout << " - Mass boundary: " << massRange[0] << " - " << massRange[1] << " ...[1]" << endl;
    cout << " - Coordinates boundary" << endl;
    cout << "   - X axle: " << XcoordinateRange[0] << " - " << XcoordinateRange[1] << " ...[2]" << endl;
    cout << "   - Y axle: " << YcoordinateRange[0] << " - " << YcoordinateRange[1] << " ...[3]" << endl;
    cout << "  - Velocity boundary" << endl;
    cout << "   - X quantity: " << XvelocityRange[0] << " - " << XvelocityRange[1] << " ...[4]" << endl;
    cout << "   - Y quantity: " << YvelocityRange[0] << " - " << YvelocityRange[1] << " ...[5]" << endl;
    cout << " - Gravity constant: G = " << G << " ...[6]" << endl;
    cout << " - Time specificaiton" << endl;
    cout << "   - Total simulation duration: " << sumT << " ...[7]" << endl;
    cout << "   - Single step duration: " << deltaT << " ...[8]" << endl;

    cout << endl;
    cout << " [0] Return main menu" << endl;
    cout << " [9] Reset to default" << endl;

    do
    {
        cout << endl << "Please choose an item: ";
        while(!(cin >> selection))
        {
            cout << " *Please enter an INTEGER as your option." << endl;
            cout << endl << "Please choose an item: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        proper = true;
        if(selection < 0 || selection > 9)
        {
            proper = false;
            cout << " *Pleaes enter an AVAILABLE option." << endl;
        }

    }
    while(!proper);

    int upperBound = 0, lowerBound = 0;
    double input = 0;

    if(selection < 6 && selection != 0)
    {
        cout << endl << "Please enter boudaries in (min max): ";
        while(!(cin >> lowerBound >> upperBound))
        {
            cout << " *Enter INTEGERS only." << endl;
            cout << endl << "Please enter boudaries in (A B): ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        if(lowerBound > upperBound)
        {
            int temp = lowerBound;
            lowerBound = upperBound;
            upperBound = temp;
        }
        else if(lowerBound == upperBound)
        {
            upperBound++;
        }
    }
    else if(selection > 5 && selection != 9)
    {
        cout << endl << "Please enter the variable: ";
        while(!(cin >> input))
        {
            cout << " *Enter a NUMERIC VALUE only." << endl;
            cout << endl << "Please enter the variable: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    if(selection != 4 && selection != 5)
    {
        if(input < 0 || upperBound < 0 || lowerBound < 0)
        {
            cout << " *Control variable less than 0 is not acceptable." << endl << endl;
            selection = -1;
            system("pause");
        }
    }

    switch(selection)
    {
    case 1:
        massRange[0] = lowerBound;
        massRange[1] = upperBound;
        break;
    case 2:
        XcoordinateRange[0] = lowerBound;
        XcoordinateRange[1] = upperBound;
        break;
    case 3:
        YcoordinateRange[0] = lowerBound;
        YcoordinateRange[1] = upperBound;
        break;
    case 4:
        XvelocityRange[0] = lowerBound;
        XvelocityRange[1] = upperBound;
        break;
    case 5:
        YvelocityRange[0] = lowerBound;
        YvelocityRange[1] = upperBound;
        break;
    case 6:
        G = input;
        break;
    case 7:
        sumT = input;
        break;
    case 8:
        deltaT = input;
        break;
    case 9:
        initEnvSetup();
        break;
    }

    if(selection != 0)
        modify_simulation_settings();
}

void modify_display_settings()
{
    bool proper = true;
    int selection;

    system("cls");

    cout << "Current configuration for plotting: " << endl << endl;
    cout << " - Refresh time: " << refresh << " ms ...[1]" << endl;
    cout << " - Plotter center: (x y) = " << "( " << centerX << ", " << centerY << " )" << " ...[2]" << endl;
    cout << " - Plot zoom(reciprocal): " << zoom << " ...[3]" << endl;
    cout << " - Float points precision: " << precision << " ...[4]" << endl;
    cout << " - List warning threshold: " << limit << " ...[5]" << endl;

    cout << endl;
    cout << " [0] Return main menu" << endl;
    cout << " [6] Reset to default" << endl;

    do
    {
        cout << endl << "Please choose an item: ";
        while(!(cin >> selection))
        {
            cout << " *Please enter an INTEGER as your option." << endl;
            cout << endl << "Please choose an item: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        proper = true;
        if(selection < 0 || selection > 6)
        {
            proper = false;
            cout << " *Pleaes enter an AVAILABLE option." << endl;
        }

    }
    while(!proper);

    int inputX = 0, inputY = 0;
    double input = 0;

    if(selection == 2)
    {
        cout << endl << "Please enter the coordinate in (x y): ";
        while(!(cin >> inputX >> inputY))
        {
            cout << " *Enter INTEGERS only." << endl;
            cout << endl << "Please enter the coordinate in (x y): ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    else if(selection != 0 && selection != 6)
    {
        cout << endl << "Please enter the variable: ";
        while(!(cin >> input))
        {
            cout << " *Enter a NUMERIC VALUE only." << endl;
            cout << endl << "Please enter the variable: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    if(input < 0)
    {
        cout << " *Control variable less than 0 is not acceptable." << endl << endl;
        selection = -1;
        system("pause");
    }

    switch(selection)
    {
    case 1:
        refresh = input;
        break;
    case 2:
        centerX = inputX;
        centerY = inputY;
        break;
    case 3:
        zoom = input;
        break;
    case 4:
        precision = input;
        break;
    case 5:
        limit = input;
        break;
    case 6:
        initEnvSetup();
        break;
    }

    if(selection != 0)
        modify_display_settings();
}

void randomColor(COLORREF &color)
{
    color = RGB(rand() % 255, rand() % 255, rand() % 255);
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
        {
            cout << "Simulate amount exceed maximum limit (" << MAX_SIMU << ").";
            cout << endl << "How many stars to simulate? ";
        }

        while(!(cin >> storage))
        {
            cout << "Please enter an INTEGER." << endl;
            cout << endl << "How many stars to simulate? ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    while(storage > MAX_SIMU);
}

int generateRandom(int min, int max, Generator &g)
{
    Distribute d(min, max);
    return d(g);
}

vector< Star > init_cnd_random()
{
    cout << endl;

    int amount;
    getAmount(amount);

    vector< Star > input(amount);
    Generator g(time(0));

    for(int i = 0; i < input.size(); i ++)
    {
        input[i].name = toString(i+1);

        input[i].x = input[i].TEMPx = generateRandom(XcoordinateRange[0], XcoordinateRange[1], g);
        input[i].y = input[i].TEMPy = generateRandom(YcoordinateRange[0], YcoordinateRange[1], g);
        input[i].mass = generateRandom(massRange[0], massRange[1], g);

        input[i].vx = generateRandom(XvelocityRange[0], XvelocityRange[1], g);
        input[i].vy = generateRandom(YvelocityRange[0], YvelocityRange[1], g);

        randomColor(input[i].color);
    }

    cout << " ...Generation complete!" << endl;

    return input;
}

bool fileExist(const string& fileName_in)
{
    DWORD ftyp = GetFileAttributesA(fileName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return false;
    return true;
}

vector< Star > init_cnd_file()
{
    cout << endl;
    cout << "Please enter the file path below." << endl;

    cout << "> ";
    string path;
    cin >> path;
    vector< Star > input;

    if(fileExist(path))
    {
        ifstream inFile (path);

        while(!inFile.eof())
        {
            Star temp;
            temp.name = toString(input.size() + 1);
            inFile >> temp.mass >> temp.x >> temp.y >> temp.vx >> temp.vy;
            temp.TEMPx = temp.x;
            temp.TEMPy = temp.y;
            randomColor(temp.color);
            input.push_back(temp);
        }

        input.erase(input.end());
    }
    else
    {
        vector< string > file_not_exist;
        file_not_exist.push_back("Re-enter file path");
        file_not_exist.push_back("Generate by random");

        cout << endl << "  Can't open file from this path." << endl;
        switch(menuSelection(file_not_exist))
        {
        case 1:
            input = init_cnd_file();
            break;
        default:
            input = init_cnd_random();
            break;
        }
    }

    return input;
}

vector< Star > init_cnd_manual()
{
    cout << endl;

    int amount;
    getAmount(amount);

    vector< Star > input(amount);

    for(int i = 0; i < input.size(); i ++)
    {
        input[i].name = toString(i+1);

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

        randomColor(input[i].color);
    }

    return input;
}

void printResult(vector< Star > &input)
{
    cout << endl;
    cout << "============================== Result ===============================" << endl;
    cout << "|-------------------------------------------------------------------|" << endl;
    cout << "|" << left << setw(7) << " Star";
    cout << "|" << left << setw(7) << " Mass";
    cout << "|" << left << setw(25) << "         ( x, y )";
    cout << "|" << left << setw(25) << "        ( Vx, Vy )";
    cout << "|" << endl;
    cout << "|-------------------------------------------------------------------|" << endl;

    for(int i = 0; i < input.size(); i ++)
    {
        cout << "|" << left << "  " << setw(5) << input[i].name;
        cout << "|" << right << setw(5) << input[i].mass << "  ";
        cout << "|" << "  ( " << right << setw(8) << fixed << setprecision(precision) << input[i].x;
        cout << "," << right << setw(8) << setprecision(precision) << input[i].y << " )  ";
        cout << "|" << "  ( " << right << setw(8) << fixed << setprecision(precision) << input[i].vx;
        cout << "," << right << setw(8) << setprecision(precision) << input[i].vy << " )  ";
        cout << "|" << endl;
    }

    cout << "|-------------------------------------------------------------------|" << endl << endl;
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

void plot(Star &input)
{
    SetPixel(handle, input.x/zoom + centerX, input.y/zoom + centerY, input.color);
}

void saveCoordinate(vector< Star > &input)
{
    for(int i=0; i < input.size(); i++)
    {
        input[i].x = input[i].TEMPx;
        input[i].y = input[i].TEMPy;

        //TODO: add option for track enable/disable
        plot(input[i]);
    }

    Sleep(refresh);
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

int start_simulation()
{
    system("cls");

    cout << "    Import stars initial condition" << endl;
    vector< string > import_init_cnd;
    import_init_cnd.push_back("Return main menu");
    import_init_cnd.push_back("Import from a local file");
    import_init_cnd.push_back("Generate it randomly with limited range");
    import_init_cnd.push_back("Configure them manually");

    vector< Star > list;

    switch(menuSelection(import_init_cnd))
    {
    case 1:
        return 0;
        break;
    case 2:
        list = init_cnd_file();
        break;
    case 3:
        list = init_cnd_random();
        break;
    case 4:
        list = init_cnd_manual();
        break;
    }

    vector< string > display_result;
    display_result.push_back("Skip");
    display_result.push_back("Continue");
    bool print;
    cout << endl << "  Would you like to verify data?" << endl;
    switch(menuSelection(display_result))
    {
    case 2:
        print = true;
        break;
    default:
        print = false;
        break;
    }
    if(print && list.size() > PRINT_LIM)
    {
        cout << endl << "  LARGE amount of data, continue?" << endl;
        switch(menuSelection(display_result))
        {
        case 1:
            print = false;
            break;
        }
    }

    if(print)
    {
        printResult(list);
        system("pause");
    }

    system("cls");

    cout << "Processing...";

    for(int i = 0; i < (int)(sumT/deltaT); i ++ )
        calculate(list);

    cout << " COMPLETE!" << endl;

    system("pause");

    for(int i = 0; i < 100; i ++)
        cout << endl;
    system("cls");

    vector< string > display_processed;
    display_processed.push_back("Yes");
    display_processed.push_back("No");
    cout << "  Would you like to review data?" << endl;
    switch(menuSelection(display_processed))
    {
    case 1:
        print = true;
        break;
    default:
        print = false;
        break;
    }
    if(print)
    {
        printResult(list);
        system("pause");
    }

    return 0;
}

int main()
{
    initEnvSetup();

    vector< string > main_menu;
    main_menu.push_back("Exit");
    main_menu.push_back("Simulation settings");
    main_menu.push_back("Display settings");
    main_menu.push_back("Start simulation");

    bool exit = false;

    do
    {
        system("cls");

        cout << "    N-Body Simulation " << VERSION << " by Andy     " << endl;
        switch(menuSelection(main_menu))
        {
        case 1:
            exit = true;
            break;
        case 2:
            modify_simulation_settings();
            break;
        case 3:
            modify_display_settings();
            break;
        case 4:
            start_simulation();
            break;
        }
    }
    while(!exit);

    return 0;
}
