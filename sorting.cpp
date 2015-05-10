#include "sorting.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <cmath>

//TODO: modify merge sort algorithm (write myself?)

void sorting::merge(vector< float > &vt, int p, int r)
{
    int mid = floor((p+r)/2);
    int i1 = 0;
    int i2 = p;
    int i3 = mid + 1;

    vector< float > t_vt;
    t_vt.reserve(r-p+1);

    while( i2 <= mid && i3 <= r)
    {
        if ( vt[i2] < vt[i3] )
            t_vt[i1++] = vt[i2++];
        else
            t_vt[i1++] = vt[i3++];
    }

    while (i2 <= mid )
        t_vt[i1++] = vt[i2++];

    while ( i3 <= r )
        t_vt[i1++] = vt[i3++];

    for(int i = p; i <= r; i++)
        vt[i] = t_vt[i-p];

}

void sorting::merge_recursive(vector< float > &vt, int p, int r)
{
    if(p < r)
    {
        int mid = floor((p + r) / 2);
        merge_recursive(vt, p, mid);
        merge_recursive(vt, mid + 1, r);
        merge(vt, p, r);
    }
}

void sorting::merge_initial(vector< float > &vt)
{
    merge_recursive(vt, 0, vt.size()-1);
}

vector< float > sorting::mergeSort(void)
{
    vector< float > temp = data;
    merge_initial(temp);
    return temp;
}


