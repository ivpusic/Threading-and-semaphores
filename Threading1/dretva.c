#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int polje[30];

void *dretvePolje () {     
  int i;
  for (i=0; i<30; i++) {
     polje[i] = rand() % 1000;
  }  
}

void *dretveIzracun (void *x) {
  int j;
  int p = *((int*)x);
  int suma = 0;
  float s = 0;
  float aritm = 0;
  
  for (j=0; j<30; j++) {
    suma += polje[j];   
  } 	
  printf("polje%d suma=%d\n", p, suma);
  s = suma;
  sleep(1);
  aritm = s/30;
  printf("polje%d aritm_sredina= %f\n", p, aritm);
}

int main (int arg, char *argc[]) {
  int m, n;  
  m = atoi(argc[1]);
  n = m*2;
  pthread_t tid[n];

  int i = 0;
  int p = 0;
  do{
    if (pthread_create (&tid[i], NULL, dretvePolje, NULL) != 0) {
       printf("\nPogreska dretva tip 1!\n");
    exit(1);
    }
    
    sleep(1);
    
    if (pthread_create (&tid[i+1], NULL, dretveIzracun, &p) != 0) {
       printf("\nPogreska dretva tip 2!\n");
    exit(1);
    }
    p++;
    i = i + 2;
  }while(i<n);

  for (i=0; i<n; i++) {
    pthread_join(tid[i], NULL);
  }
  
  return 0;
}
