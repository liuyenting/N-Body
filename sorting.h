#ifndef SORTING_H
#define SORTING_H

#include <vector>

using namespace std;

class sorting
{
public:
    sorting(vector< float > &_data)
    {
        data = _data;
    }
    vector< float > mergeSort(void);
private:
    vector< float > data;
    void merge(vector< float > &vt, int p, int r);
    void merge_recursive(vector< float > &vt, int p, int r);
    void merge_initial(vector< float > &vt);
};

#endif // SORTING_H
