#ifndef MENU_H
#define MENU_H

#include "type.h"

#include <string>
#include <vector>
#include <sstream>
#include <windows.h>

using namespace std;

template< typename T > string toString(const T& t)
{
    ostringstream os;
    os << t;
    return os.str();
}

template< typename T > DigitSet toDigitSet(const T& t1, const T& t2)
{
    DigitSet temp;
    temp.x = t1;
    temp.y = t2;
    return temp;
}

class menu
{
public:
    menu(void)
    {
        question = "NaQ";
        printVar = false;
    }
    menu(string _question)
    {
        question = _question;
        printVar = false;
    }
    menu(string _question, vector< string > &_option)
    {
        question = _question;
        option = _option;
        printVar = false;
    }
    menu(int _version, vector< string > &_option)
    {
        question = "    N-Body Simulation w Brute Force :: " + toString(_version) + " :: by Andy";
        option = _option;
        printVar = false;
    }
    void setReload(string _question)
    {
        question = _question;
    }
    void setReload(vector< string > &_option)
    {
        option = _option;
    }
    void setReload(string _question, vector< string > &_option)
    {
        question = _question;
        option = _option;
    }
    int getSelection(void);
    DigitSet getRange(void);
    DigitSet getRange(bool skipLine);
    void print(void);
    void clrscr(void);
    void printQuestion(void);
    void printQuestion(string _question);
    void printVariable(string title, float var);
    void printVariable(string title, float var1, float var2);
    void setPrecision(int _precision)
    {
        precision = _precision;
    }
    float getConstant(void);
    float getConstant(bool skipLine);
protected:
private:
    string question;
    int version;
    vector< string > option;
    int selection;
    int precision;
    //TODO: need a new flag settings, indicate the need of clrscr
    bool printVar;
};

#endif // MENU_H
