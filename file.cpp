#include "file.h"
#include "menu.cpp"

#include <fstream>

bool file::fileExist(const string& fileName_in)
{
    DWORD ftyp = GetFileAttributesA(fileName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return false;
    return true;
}

void file::readFile()
{
    ifstream inFile(path);

    string name;
    float mass;
    DigitSet position;
    DigitSet velocity;

    while(!inFile.eof())
    {
        name = toString(data.size() + 1);
        inFile >> mass >> position.x >> position.y >> velocity.x >> velocity.y;
        particle temp(name, mass, position, velocity);
        data.push_back(temp);
    }
}
