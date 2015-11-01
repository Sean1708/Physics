#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int FIXED_SEED = 1;

#define ndim 3  // Dimensionality of physical space.
#define nparts 10000  // Number of particles.
const int nsteps = 2;  // Number of time-steps in the simulation.
const double mass = 1.0;
const double dt = 1.0e-4;  // Length of time-step.
const double box[ndim] = {10.0};  // Size of the simulation area

void init(double pos[nparts][ndim], double vel[nparts][ndim], double acc[nparts][ndim], const double box[ndim]);


int main(int argc, char* argv[]) {
    // Open the logfile.
    FILE* logfile = NULL;
    if (argc >= 2) {
        logfile = fopen(argv[1], "w");
    } else {
        logfile = fopen("simple_md.log", "w");
    }
    if (logfile == NULL) {
        fprintf(stderr, "Error: could not open logfile\n");
        return 1;
    }
    fprintf(logfile, "PE\tKE\tError\n");

    double energy = 0;
    double PE = 0;
    double KE = 0;
    double pos[nparts][ndim];
    double vel[nparts][ndim];
    double acc[nparts][ndim];
    double force[nparts][ndim];

    init(pos, vel, acc, box);
}


void init(double pos[nparts][ndim], double vel[nparts][ndim], double acc[nparts][ndim], const double box[ndim]) {
  /* random numbers for positions*/
  if (FIXED_SEED != 0) {
    srand48(0);
  } else {
    srand48(time(NULL));
  }

  /* set initial positions and velocities */
  for(int part = 0; part < nparts; part++)
  {
    for(int i = 0; i < ndim; i++)
    {
      pos[part][i] = drand48() * box[i];
      vel[part][i] = 0.0;
      acc[part][i] = 0.0;
    }
  }
}
