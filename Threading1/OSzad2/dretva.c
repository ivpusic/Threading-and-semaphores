#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int polje[30];
// dretva 1. tipa generira n slucajnih brojeva, i zapisuje ih u polje....tako je zadanu u zadatku...
void *dretvePolje () {     
  int i;
  for (i=0; i<30; i++) {
     polje[i] = rand() % 1000;
  }  
}
// pogledati zadatak, dretva 2. tipa, sta je zadano
// svcaka dretva ce za svoje polje racunati sumu...
void *dretveIzracun (void *x) {
  int j;
  int p = *((int*)x); // funkcije koje dretve izvrsavaju moraju biti pokazivaci na funkciju, jer je to tako definirano
      // int treba prepisat u broj, integer, pokazivac.....nesto....., uglavnom da se to valjda pretvori u broj
      // moglo je i jednostavnije valjda, tako kaze...
      
  int suma = 0;
  float s = 0;
  float aritm = 0;
  
  for (j=0; j<30; j++) {
    suma += polje[j];   // zbrajamo elemente polja, i rez spremamo u sumu...
  } 	
  printf("polje%d suma=%d\n", p, suma); // ispisujemo sumu
  s = suma; // s je tima float, a trebat ce mu folat za aritm sredinu...
  sleep(1); // zadano u zadatku nakon 1s ispisemo aritm sredinu...
  aritm = s/30;
  printf("polje%d aritm_sredina= %f\n", p, aritm); // ispis...
}

int main (int arg, char *argc[]) {//./dretva 3 3 je broj kojim definiramo broj dretvi
                        // int arg = broj dretvi
                        //char *arg[c] - njemu su zapisani argumenti, [0] naziv programa, a u iducem je zapisana 3
  int m, n;  
  m = atoi(argc[1]);// u m zapisivano 3 , atoi char > int
  n = m*2; // ukupno dretvi ima m*2, zadano u zadatku valjda...
  pthread_t tid[n]; // moramo znati nekako s kojom dretvom radimo, ovo nam stvara id dretve

  int i = 0;
  int p = 0;
  do{
    if (pthread_create (&tid[i], NULL, dretvePolje, NULL) != 0) { // ptred create stvara dretve, argumenti: 1. pokazuje nam mjesto u mem gdje se nalazi id dretve
                       // 2. NULL, to su neki atributi koje mozemo postavitit za neku dretvu koju stvaramo
                       // 3. navodimo naziv funkcije koju ce ta dretva izvrsavati
                       // 4. navodimo argument koji zelimo proslijediti iz tu funkciju, valjda dretvaPolje
                       
                       // ukoliko dretva nije uspjedno stvoreno, vrati se -1, a ako je stvorena, onda se valjda vraca NULL
       printf("\nPogreska dretva tip 1!\n"); // ako je doslo do pogreske onda se ovo ispisuje
    exit(1); // izlazimo van, ako je doslo do greske
    }
    
    sleep(1);
    
    if (pthread_create (&tid[i+1], NULL, dretveIzracun, &p) != 0) {// kreiranje dretvi kao i gore (u zadatku zadano da treba bit 2 tipa dretvi!!!
                                   // razlika u odnosu na gore je u 4. argumentu, ovdje imamo referencu na mjesto u memoriji gdje je ta varijabla p, a p je obicni broj,
                                   // i uvecava se u svakom koraku
                                   
       printf("\nPogreska dretva tip 2!\n"); // kao i gore provjeravamo da li je doslo do greske.... 
    exit(1);
    }
    p++;
    i = i + 2; // i ide do ukupnog broja dretvi, tj do n 
  }while(i<n);

  for (i=0; i<n; i++) {
    pthread_join(tid[i], NULL); // dretva ceka na zavrsetak druge dretve, konkretno, kad pokrenemo program on ima svoju dretvu, 
    // i sad program ide stvaran nove fdretve, i glavna dretva, njoj moramo rec da ne zavrsi prije dok sve druge ne zavrse
    // ako glavna dretva ne ceka na njih, onda te dretve postaju zombiji 
    // 2. arugent se kotisti ako nam treba vratit neko stanje, a sad nam to ne treba pa smo stavili NULL
  }
  
  return 0;
}
