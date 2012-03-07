#include <stdio.h>
#include <signal.h>
#include <sys/shm.h>

struct struktura{
  int ULAZ[30];
  int ZADNJI_BROJ;
  int BROJ[30];
} *Zajednicka_Varijabla;

int Id;

void Izlaz(int s) {
  shmdt((char *) Zajednicka_Varijabla);
  shmctl(Id, IPC_RMID, NULL);

}

int main(int argn, char *polje[]) {
  int i = 0, N = 0, br1, br2, br3;;

  N = atoi(polje[1]);
 
   Id = shmget(IPC_PRIVATE, sizeof(*Zajednicka_Varijabla)*100, 0600); 
         
   Zajednicka_Varijabla = shmat(Id, NULL, 0);
   sigset(SIGINT, Izlaz);

   Zajednicka_Varijabla->ZADNJI_BROJ = 0;
   for (i = 0; i < N; i++) {
     Zajednicka_Varijabla->ULAZ[i] = 0;
     Zajednicka_Varijabla->BROJ[i] = 0;
   }
  
  for (i = 0; i < N; i++) {
    if (fork() == 0) {
      for (br1 = 0; br1 < 5; br1++) {
	      Zajednicka_Varijabla->ULAZ[i] = 1;
	      Zajednicka_Varijabla->BROJ[i] = Zajednicka_Varijabla->ZADNJI_BROJ + 1;
	      Zajednicka_Varijabla->ZADNJI_BROJ = Zajednicka_Varijabla->BROJ[i];
	      Zajednicka_Varijabla->ULAZ[i] = 0;	
	      for (br3 = 0; br3 < N; br3++) {
	        while (Zajednicka_Varijabla->ULAZ[br3] == 1);
	        while ((Zajednicka_Varijabla->BROJ[br3] != 0) && ((Zajednicka_Varijabla->BROJ[br3] < Zajednicka_Varijabla->BROJ[i]) || ((Zajednicka_Varijabla->BROJ[br3] == Zajednicka_Varijabla->BROJ[i]) && (br3 < i)))) ;
	      }
	
	      for (br2 = 0; br2 < 5; br2++) {
         sleep(1);
         printf("Proces: %d,  K.O. br: %d (%d/5)\n", i + 1, br1 + 1, br2 + 1);
	      }
	      printf("\n");

	      Zajednicka_Varijabla->BROJ[i] = 0;
	      sleep(1);
      }
    
    }
  }
  for (i = 0; i < N; i++) {
    wait(NULL);
  }
  
  return 0;
}
