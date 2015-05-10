#include "menu.h"

#include <iostream>
#include <iomanip>

using namespace std;

float menu::getConstant(void)
{
    float arg;

    bool status;

    status = false;
    cout << " " << question << " ";
    while(!(cin >> arg))
    {
        cout << "  * Not an available input." << endl << endl;
        cout << " " << question << " ";

        cin.clear();
        cin.ignore(1000, '\n');
    }

    cout << endl;

    return arg;
}

float menu::getConstant(bool skipLine)
{
    float arg;

    bool status;

    status = false;
    cout << " " << question << " ";
    while(!(cin >> arg))
    {
        cout << "  * Not an available input." << endl << endl;
        cout << " " << question << " ";

        cin.clear();
        cin.ignore(1000, '\n');
    }

    if(skipLine)
        cout << endl;

    return arg;
}

DigitSet menu::getRange(void)
{
    DigitSet arg;

    bool status;

    status = false;
    cout << " " << question << " ";
    while(!(cin >> arg.x >> arg.y))
    {
        cout << "  * Not an available input." << endl << endl;
        cout << " " << question << " ";

        cin.clear();
        cin.ignore(1000, '\n');
    }

    cout << endl;

    if(arg.x > arg.y)
        swap(arg.x, arg.y);

    return arg;
}

DigitSet menu::getRange(bool skipLine)
{
    DigitSet arg;

    bool status;

    status = false;
    cout << " " << question << " ";
    while(!(cin >> arg.x >> arg.y))
    {
        cout << "  * Not an available input." << endl << endl;
        cout << " " << question << " ";

        cin.clear();
        cin.ignore(1000, '\n');
    }

    if(skipLine)
        cout << endl;

    if(arg.x > arg.y)
        swap(arg.x, arg.y);

    return arg;
}

int menu::getSelection(void)
{
    bool status;

    do
    {
        status = false;
        cout << " Please enter your selection: ";
        while(!(cin >> selection))
        {
            cout << "  * Not an available option." << endl << endl;
            cout << " Please enter your selection: ";

            cin.clear();
            cin.ignore(1000, '\n');
        }

        if(selection > option.size() || selection < 1)
        {
            status = true;
            cout << "  * Entered an out-of-range option." << endl << endl;
        }
    }
    while(status);

    cout << endl;

    return selection;
}

void menu::print(void)
{
    if(printVar)
    {
        cout << "|                                                                   |" << endl;
        printVar = false;
    }
    else
        printQuestion();

    for(int i = 0; i < option.size(); i ++)
        cout << "|  [" << i + 1 << "] " << left << setw(61) << option[i] << "|" << endl;
    cout << "|-------------------------------------------------------------------|" << endl << endl;
}

void menu::printQuestion(void)
{
    clrscr();
    cout << "|-------------------------------------------------------------------|" << endl;
    cout << "| " << left << setw(66) << question << "|" << endl;
    cout << "|-------------------------------------------------------------------|" << endl;
}

void menu::printQuestion(string _question)
{
    clrscr();
    cout << "|-------------------------------------------------------------------|" << endl;
    cout << "| " << left << setw(66) << _question << "|" << endl;
    cout << "|-------------------------------------------------------------------|" << endl;
}

void menu::printVariable(string title, float var)
{
    if(!printVar)
    {
        printVar = true;
        printQuestion();
    }
    cout << "|  - " << left << setw(25) << title << ":     " << left << setw(32) << var << "|" << endl;
}

void menu::printVariable(string title, float var1, float var2)
{
    if(!printVar)
    {
        printVar = true;
        printQuestion();
    }
    cout << "|  - " << left << setw(25) << title << ":  ";
    if(var1 != var2)
        cout << right << setw(5) << var1 << " ~ " << left << setw(27) << var2 << "|" << endl;
    else
        cout << right << setw(5) << var1 << " (Locked)" << setw(22) << "|" << endl;
}

void menu::clrscr(void)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD count;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);

    FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

    SetConsoleCursorPosition(hStdOut, coord);
}
