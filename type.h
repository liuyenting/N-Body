#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#include <string>
#include <vector>

using namespace std;
/*
typedef struct
{
    float x, y;
} Coordinate;

typedef struct
{
    float x, y;
} Velocity;

typedef struct
{
    float x, y;
} Force;
*/

typedef struct
{
    float x, y;
} DigitSet;

typedef struct
{
    vector< float > x;
    vector< float > y;
} SingleStep;
/*
typedef struct
{
    string name;
    int mass;
    //TODO: advance item for ellastic collision, use in n-body calculation module
    float radius;
    Coordinate position;
    Velocity velocity;
    Force force;
    SingleStep singlestep;
} Star;
*/
typedef struct
{
    float positionUpper, positionLower;
    float velocityUpper, velocityLower;
    int massUpper, massLower;
} Boundary;

#endif // TYPE_H_INCLUDED
