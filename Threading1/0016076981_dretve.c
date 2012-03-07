#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define VEL_POLJA 30

int array [30][30];

void * 
generator (void *p)
{
  int n = * (int *) p, i;
  for (i = 0; i < VEL_POLJA; ++i)
    array [n][i] = rand () % 100000;
}

void *
adder (void *p)
{
  int n = * (int *) p, i;
  int sum = 0;
  for (i = 0; i < VEL_POLJA; ++i)
    sum += array [n][i];
        
  printf ("Polje %d = %d\n", n + 1, sum);
  sleep (1);
  printf ("Polje %d artimeticka sredina: %f\n", n + 1, (double) sum / VEL_POLJA);
}


int
main (int argc, char *argv[])
{
  int i, m;
  pthread_t threadsGenerator [30];
  pthread_t threadsAdder [30];

  srand (time(0));
  rand ();

  if (argc < 2)
    {
      printf ("Premalo argumenata!\n");
      return 0;
    }
  
  sscanf (argv[1], "%d", &m);
  
  for (i = 0; i < m; ++i)
    pthread_create (&threadsGenerator [i], NULL, generator, (void *) &i);
  

  for (i = 0; i < m; ++i)
    pthread_create (&threadsAdder [i], NULL, adder, (void *) &i);
  
  for (i = 0; i < m; ++i)
    {
      pthread_join (threadsGenerator [i], NULL);
      pthread_join (threadsAdder [i], NULL);
    }

  return 0;
}
