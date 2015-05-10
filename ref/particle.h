#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <cmath>
using namespace std;

#include <typedefs.h>

/*!
\brief A representation of a 2D particle.
*/
class particle
{
public:

  /** The particles radius. */
  float radius;
  /** The particles position. */
  float pos[2];
  /** The particles velocity. */
  float vel[2];
  /** The particles acceleration. */
  float acc[2];

  /** Stationary particle. */
  particle()
    : radius(0.0) { pos[0]=pos[1]=vel[0]=vel[1]=acc[0]=acc[1]=0.0; }

  /** Initialize a particle. */
  particle
  (
    doublec p0,
    doublec p1,
    doublec v0,
    doublec v1,
    doublec r
  )
    { pos[0]=p0; pos[1]=p1, vel[0]=v0; vel[1]=v1; radius=r; }

  /** Do the two particles intersect? */
  boolc isIntersecting( particle const & p2 ) const
  {
    doublec dy(p2.pos[1]-pos[1]);
    doublec dx(p2.pos[0]-pos[0]);
    doublec r(radius+p2.radius);
    return dy*dy+dx*dx < r*r;
  }

  /** Increment the particle in time. */
  void step(doublec t)
  {
    vel[0] += t*acc[0];
    vel[1] += t*acc[1];
    pos[0] += t*vel[0];
    pos[1] += t*vel[1];
  }

  /** Set the acceleration to zero. */
  void acczero()
    { acc[0]=acc[1]=0.0; }

  ostream & print(ostream & os) const
  {
    os << pos[0] << " " << pos[1] << " ";
    os << vel[0] << " " << vel[1] << " ";
    os << acc[0] << " " << acc[1] << " ";
    os << radius;
    return os;
  }

/*
NOTE:  
  This code is to be directly put in and
  not called because there is a large
  penalty in calling it. Cache thrashing
  as a result of OO method call.

  boolc collision
  (
    particle & p2,
    doublec h
  )

  {
    if (isIntersecting(p2))
    {
      step(-h);
      p2.step(-h);
      collisionDynamics(p2);
      step(h);
      p2.step(h);

      return true;
    }

    return false;
  }
*/

  /** Collision resolution. */
  void collisionDynamics(particle & p2);
};


ostream & operator << (ostream & os, particle const & p);


#endif


