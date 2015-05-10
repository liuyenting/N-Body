#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#define ARR_SIZE 15

using namespace std;

void print(vector<int> &vt)
{
    copy(vt.begin(), vt.end(), ostream_iterator<int>(cout, "  "));
    cout<<endl;
}

void merge(vector<int> &vt, int p, int r)
{
    int mid = floor((p+r)/2);
    int i1 = 0;
    int i2 = p;
    int i3 = mid + 1;

    vector<int> t_vt;
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

/*Algorithm
    1> Split the list into two equal parts at the middle
    2> Apply merge_sort() on each list
    3> Merge both the lists
*/
void merge_recursive(vector<int> &vt, int p, int r)
{
    if(p<r)
    {
        int mid = floor((p+r)/2);
        merge_recursive(vt, p, mid);
        merge_recursive(vt, mid+1, r);
        merge(vt, p, r);
    }
}

void merge_sort(vector< int > &vt)
{
    merge_recursive(vt, 0, vt.size()-1);
}

int main()
{
    vector<int> vt;
    vt.reserve(ARR_SIZE);

    srand(time(NULL));

    for(int i=0; i<ARR_SIZE; i++)
        vt.push_back(rand()%1000);

    cout<<"Unsorted : "<<endl;
    print(vt);

    merge_sort(vt);

    cout<<"Sorted : "<<endl;
    print(vt);

    return 0;
}
