#include <particle.h>

void particle::collisionDynamics(particle &p2)
{
  float n[2], t[2], n_mag, n_mag_sq;
  float vi_nt[2], vj_nt[2], tmp;

  /* Normal vector between centres. */
  n[0] = p2.pos[0] - pos[0];
  n[1] = p2.pos[1] - pos[1];
  
  /* Normalise normal vector. */
  n_mag_sq = n[0] * n[0] + n[1] * n[1];
  n_mag = sqrt(n_mag_sq);
  n[0] /= n_mag;
  n[1] /= n_mag;

  /* Tangent vector. */
  t[0] = -n[1];
  t[1] = n[0];

  /* 
   * Resolve particle velocities into components in the 
   * normal and tangential direction. 
   */
  vi_nt[0] = p2.vel[0] * n[0] + p2.vel[1] * n[1];
  vi_nt[1] = p2.vel[0] * t[0] + p2.vel[1] * t[1];
  vj_nt[0] = vel[0] * n[0] + vel[1] * n[1];
  vj_nt[1] = vel[0] * t[0] + vel[1] * t[1];

  /* Interchange particle velocities along normal direction. */
  tmp = vi_nt[0];
  vi_nt[0] = vj_nt[0];
  vj_nt[0] = tmp;

  /* Resolve back. */
  p2.vel[0] = vi_nt[0] * n[0] + vi_nt[1] * t[0];
  p2.vel[1] = vi_nt[0] * n[1] + vi_nt[1] * t[1];
  vel[0] = vj_nt[0] * n[0] + vj_nt[1] * t[0];
  vel[1] = vj_nt[0] * n[1] + vj_nt[1] * t[1];

}

ostream & operator << (ostream & os, particle const & p)
{
  return p.print(os);
}

