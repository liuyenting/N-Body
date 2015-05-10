#ifndef FILE_H
#define FILE_H

#include "type.h"
#include "particle.cpp"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class file
{
public:
    file()
    {
        bool first = true;
        do
        {
            if(first)
                first = false;
            else
                cout << "  * Can't read from this path." << endl << endl;
            cout << " Please enter the file path below." << endl;
            cout << " > ";
            cin >> path;
        }
        while( !fileExist(path) );

        cout << endl;
    }
    file(string _path)
    {
        path = _path;
    }
    vector< particle > getData(void)
    {
        readFile();
        return data;
    }
private:
    bool fileExist(const string &fileName_in);
    void readFile(void);
    vector< particle > data;
    string path;
};

#endif // FILE_H
