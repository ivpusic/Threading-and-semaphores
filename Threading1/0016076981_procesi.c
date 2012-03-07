#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <math.h>

void 
izracun (int n, float a)
{
  printf ("%d-kut duzine a = %f ima povrsinu: %f\n", n, a, (float) 1 / (float) 4 * (float) n * a * a / tan (3.14159265358979 / n));
  sleep (1);
  printf ("%d-kut duzine a = %f ima opseg: %f\n", n, a, n * a);
  _exit (0);
}


int
main (int argc, char *argv[])
{
  int i, n;
  float a;
  pid_t pid;
  
  if (argc > 11)
    {
      printf ("Visak argumenata! :(");
      return 0;
    }
  
  for (i = 1; i < argc;)
    {
      sscanf (argv[i], "%d", &n);
      if (i + 1 < argc)
  	sscanf (argv[i + 1], "%f", &a);
      
      pid = fork ();
      if (!pid)
	izracun (n, a);
      else if (pid > 0)
	  i += 2;

      else
	printf ("Ne moze se stvoriti proces!\n");
    }
  (void) wait (NULL);
  
  return 0;
}
