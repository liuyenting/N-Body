#ifndef PARTICLE_H
#define PARTICLE_H

#include "type.h"
#include "random.cpp"
#include <string>

class particle
{
public:
    particle(random generateRandom, string _name, Boundary boundary);
    particle(string _name, float _mass, DigitSet _position, DigitSet _velocity)
    {
        name = _name;
        mass = _mass;
        position = _position;
        velocity = _velocity;
        force = { 0, 0 };
        //TODO: initialize radius
    }
    void setCondition(float _localGravity, float _localDeltaT)
    {
        localGravity = _localGravity;
        localDeltaT = _localDeltaT;
    }
    float getMass(void) { return mass; }
    DigitSet getPosition(void) { return position; }
    DigitSet getVelocity(void) { return velocity; }
    void reciprocation(particle _target);
    void applyPosition(void) { position = temporaryPosition; }
    void updatePosition(void);
    SingleStep getSingleStep(void) { return singlestep; }
    string getName(void) { return name; }
private:
    string name;
    float mass;
    float localGravity, localDeltaT;
    //TODO: advance item for ellastic collision, use in n-body calculation module
    float radius;
    void writeTemporaryPosition(void);
    void applyRadius(void);
    bool isCollision(particle _target);
    DigitSet position, temporaryPosition;
    DigitSet velocity;
    DigitSet force;
    SingleStep singlestep;
};

#endif // PARTICLE_H
