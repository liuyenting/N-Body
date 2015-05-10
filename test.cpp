#include <iostream>
#include <vector>

#include "type.h"
#include "random.cpp"

using namespace std;

class Test
{
public:
    Test()
    {
        random ran;
        data = ran.getInt();
    }
    int getResult()
    {
        return data;
    }
private:
    int data;
};

int main()
{
    Test inf[10];
    for(int i=0; i<10; i++)
    {
        cout << inf.getResult() << endl;
    }

    return 0;
}
