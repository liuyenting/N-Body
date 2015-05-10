#include "particle.h"

particle::particle(random generateRandom, string _name, Boundary boundary)
{
    //random generateRandom;

    name = _name;

    generateRandom.setBoundary(boundary.positionLower, boundary.positionUpper);
    position.x = generateRandom.getFloat();
    position.y = generateRandom.getFloat();

    generateRandom.setBoundary(boundary.velocityLower, boundary.velocityUpper);
    velocity.x = generateRandom.getFloat();
    velocity.y = generateRandom.getFloat();

    generateRandom.setBoundary(boundary.massLower, boundary.massUpper);
    mass = generateRandom.getInt();

    force = { 0, 0 };
    radius = pow(mass, 1/3.0);
}

void particle::reciprocation(particle _target)
{
    float dx = _target.getPosition().x - position.x;
    float dy = _target.getPosition().y - position.y;
    float r2 = pow(dx, 2) + pow(dy, 2);
    float _force = localGravity * mass * _target.getMass() / r2;

    force.x += _force * dx / sqrt(r2);
    force.y += _force * dy / sqrt(r2);
}

void particle::updatePosition(void)
{
    velocity.x += ( force.x / mass ) * localDeltaT;
    velocity.y += ( force.y / mass ) * localDeltaT;

    singlestep.x.push_back(velocity.x * localDeltaT);
    singlestep.y.push_back(velocity.y * localDeltaT);

    position.x += singlestep.x[singlestep.x.size() - 1];
    position.y += singlestep.y[singlestep.y.size() - 1];
}

void particle::applyRadius(void)
{

}

bool particle::isCollision(particle _target)
{

}
