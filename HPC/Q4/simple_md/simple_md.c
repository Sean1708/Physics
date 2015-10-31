/* This is a C translation of the program simple_md.f90 originally written
   by Matt Probert.
*/

#include <stdio.h>
#include <stdlib.h>
#include "md_stuff.h"

int main(void)
{
  int i;
  double Etot;
  FILE* log_file;

  init();
  compute();
  
  Etot = PE + KE;
  
  log_file = fopen("simple_md.log", "w");
  if ( log_file == NULL )
  {
    fprintf(stderr, "Error in initialising simple_md.log\n");
    exit(1);
  }

  fprintf(log_file, "PE\tKE\tError\n");

  // TODO: don't keep closing and opening the log file.
  fclose(log_file);

  for(i=0 ; i < nsteps ; i++)
  {
    compute();

    log_file = fopen("simple_md.log", "a");
    fprintf(log_file, "%3.20f\t%3.20f\t%3.20f\n", PE, KE, (PE+KE-Etot)/Etot);
    fclose(log_file);

    update();
  }
  return 0;
}
