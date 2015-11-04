/* Simple container for MD data and routines for operating thereupon,
   e.g. force calculatio and position updating.
 */

/* Force gcc to define srand48, drand48, and M_PI - avoids implicit definition warnings. */
#define _XOPEN_SOURCE 500

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "md_stuff.h"


/* Simulation parameters */
#define ndim 3 /* Dimensionality of physical space */
#define nparts 10000 /* Number of particles */
const int nsteps = 2; /* Number of time-steps in the simulation */
const int fixed_seed = 1; /* true (non-zero) => repeatable rands for testing */
const double mass = 1.0;
const double dt = 1.0e-4;

/* Simulation variables */
double box[ndim] = {10, 10, 10};
double pos[nparts][ndim];
double vel[nparts][ndim];
double force[nparts][ndim];
double accel[nparts][ndim];
double PE;
double KE;

double min(double a, double b)
{
  if( a < b )
  {
    return a;
  }
  else
  {
    return b;
  }
}


/*
   ! Initialise positions, velocities and accelerations of all particles.    !
   ! Use system random number generator and either fixed or random seed.     !
   !-------------------------------------------------------------------------!
   ! References:                                                             !
   !   none                                                                  !
   !-------------------------------------------------------------------------!
   ! Arguments:                                                              !
   !   none                                                                  !
   !-------------------------------------------------------------------------!
   ! Return value:                                                           !
   !   none                                                                  !
   !-------------------------------------------------------------------------!
   ! Parent module variables used:                                           !
   !   pos, vel, accel all updated.                                          !
   !   fixed_seed: if set to .true. then get a repeatable sequence of rands  !
   !-------------------------------------------------------------------------!
   ! Modules used:                                                           !
   !   none                                                                  !
   !-------------------------------------------------------------------------!
   ! Key Internal Variables:                                                 !
   !   rand_seed: set to clock-based value if fixed_seed=.false.             !
   !-------------------------------------------------------------------------!
   ! Necessary conditions:                                                   !
   !   none                                                                  !
   !-------------------------------------------------------------------------!
   ! Written by Matt Probert, v1.0, 14/02/2005                               !

 */
void init()
{
  int i, p;

  /* random numbers for positions*/
  if( fixed_seed != 0 )
  {
    srand48(0);
  }
  else
  {
    srand48(time(NULL));
  }

  /* set initial positions and velocities */
  for(p=0 ; p < nparts ; p++)
  {
    for(i=0 ; i < ndim ; i++)
    {
      pos[p][i] = drand48() * box[i]; /* 0 <= drand48() < 1 */
      vel[p][i] = 0.0;
      accel[p][i] = 0.0;
    }
  }
}

const double pi_2 = M_PI/2.0;

/* Potential of each particle */
double v(double x){
  double sin_min = sin(min(x, pi_2));
  return sin_min*sin_min;
}

/* Derivative of potential function */
double dv(double x){
  double x_pi_min = min(x,pi_2);
  double sin_cos = sin(x_pi_min)*cos(x_pi_min);
  return sin_cos+sin_cos;
}

/*  ***************************************************************************
    ! Compute the forces on the particles using a simple model potential:     !
    !   V(dr)=sin(dr)^2 if dr<pi/2, 1.0 otherwise                             !
    !   where dr=|r_i-r_j| separation of particles i and j                    !
    !   Hence dV=2*sin(dr)*cos(dr) if dr<pi/2, 0.0 otherwise                  !
    !-------------------------------------------------------------------------!
    ! References:                                                             !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Arguments:                                                              !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Return value:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Parent module variables used:                                           !
    !   pos unchanged, force, PE and KE updated                               !
    !-------------------------------------------------------------------------!
    ! Modules used:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Key Internal Variables:                                                 !
    !   v(x) and dv(x) are macros                                             !
    !-------------------------------------------------------------------------!
    ! Necessary conditions:                                                   !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Written by Matt Probert, v1.0, 14/02/2005                               !
    ***************************************************************************
 */
void compute()
{
  int i,j,k;
  double rij[ndim];
  double d, d2;
  double dvd;

  PE = 0;
  KE = 0;

  /* For each particle: */
  for(i=0 ; i < nparts ; i++)
  {
    for(k=0 ; k < ndim ; k++)
    {
      /* Initialise forces to zero */
      force[i][k] = 0.0;
      /* compute kinetic energy */
      KE = KE + vel[i][k]*vel[i][k];
    }

    /* Loop over all other particles */
    for(j=0 ; j < nparts ; j++)
    {
      /* Ignoring self interactions */
      if( i != j )
      {
        /* d2 as the squared distance between the particles */
        d2 = 0;
        for(k=0 ; k < ndim ; k++)
        {
          rij[k] = pos[i][k] - pos[j][k];
          d2 += rij[k]*rij[k];
        }
        d = sqrt(d2);

        /* attribute half of the potential energy to particle 'j'*/
        PE = PE + 0.5*v(d);

        dvd = dv(d)/d;
        /* Update the force on particle i*/
        for(k=0 ; k < ndim ; k++)
        {
          force[i][k] = force[i][k] - rij[k]*dvd;
        }
      }
    }
  }
  KE = KE * 0.5 * mass;
}

/*  !=========================================================================!
    ! Simple Velocity-Verlet MD update of positions                           !
    ! i.e. r(t+dt) = r(t) + v(t)*dt + f(t)*dt**2/(2*m) +O(dt**4)              !
    !      v(t+dt) = v(t) + ( f(t+dt)+f(t) )*dt/(2*m)  +O(dt**3)              !
    !-------------------------------------------------------------------------!
    ! References:                                                             !
    !   Swope et al, J.Chem.Phys. v75, p637-649 (1982)                        !
    !-------------------------------------------------------------------------!
    ! Arguments:                                                              !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Return value:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Parent module variables used:                                           !
    !   pos, vel and force used,                                              !
    !   pos, vel and accel updated                                            !
    !-------------------------------------------------------------------------!
    ! Modules used:                                                           !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Key Internal Variables:                                                 !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Necessary conditions:                                                   !
    !   none                                                                  !
    !-------------------------------------------------------------------------!
    ! Written by Matt Probert, v1.0, 14/02/2005                               !
    !=========================================================================!
 */
void update()
{
  int i,j;

  for(i=0 ; i < nparts ; i++)
  {
    for(j=0 ; j < ndim ; j++)
    {
      pos[i][j] = pos[i][j] + vel[i][j]*dt + 0.5*dt*dt*accel[i][j];
      vel[i][j] = vel[i][j] + 0.5*dt*force[i][j]/mass + 0.5*dt*accel[i][j];
      accel[i][j] = force[i][j] / mass;
    }
  }
}
