#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

int ZASTAVICA[] = {0, 0}, PRAVO = 0;

void Izlaz(int arg) {
  exit(0);
}

void *Ispis (void *x)
{
  int i, j;  
  int prva = *((int*)x);
  int druga = (prva + 1) % 2;
  
  for (i = 0; i < 5; i++) {
    ZASTAVICA[prva] = 1;
    while (ZASTAVICA[druga] != 0) {
      if (PRAVO == druga) {
        ZASTAVICA[prva] = 0;
        while (PRAVO != prva);
        ZASTAVICA[prva] = 1;
       }
    }

    for (j = 0; j < 5; j++) {
     sleep(1);
     printf("Dretva: %d,  K.O. br: %d (%d/5)\n", prva + 1, i + 1, j + 1);
    }
    printf("\n");

    PRAVO = druga;
    ZASTAVICA[prva] = 0;
    sleep(1);
  }
}

int main() {
   sigset(SIGINT, Izlaz);
   
   int i, POLJE[] = {0, 1};

   pthread_t thr_id[2];
   for (i = 0; i < 2; i++) {
    if (pthread_create(&thr_id[i], NULL, Ispis, &POLJE[i]) != 0) {
      printf("Pogreska!\n");
      return -1;
     }
   }
   for (i = 0; i < 2; i++) {
     pthread_join(thr_id[i], NULL);
   }

   return 0;
}
