#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>// ukljucujemo neke posebne tipove podataka
#include <sys/ipc.h> // ukljucujemo poruke semafore.......
#include <sys/shm.h> // // upravljanje signalima?

int mem;
int *seg;

void brisi_mem()
{
        shmdt((int *) seg); // otpusta segment 
        shmctl(mem, IPC_RMID, NULL); // unistava segment, prima 3. argumenta
        // 1. id memorije
        // 2. IPC_RMID remove id...
        // 3. NULL jer nam tu nije potreban, ....adresa na strukturu na koju mozemo neke podatke proslijedit, no to nam ovde nije potrebno
        printf("\nMemorija slobodna\n");
        exit(0);
}

int main(int arg, char *argc[]) // kao i u dretvama, moramo iz komandne linije uzet neke parametre...tj, u ovom slucaju broj procesa
                  // 
{
        mem=shmget(IPC_PRIVATE,sizeof(int)*100,0600); // identifikacijski broj, pretvara kljuc segmenta u njegov indetifikacijski broj, a ako on ne postoji 
        // onda stvori novi segment
        // 1. arg kljuc, identifikacijski...IPC_PRIVATE
        // 2. velicina segmenta, u ovom slucaju, velicine int, i to 100
        // 3. zastavice, kontroliramo kontrolu prostupa tom segmentu, 0600 - dozvola pristupa, da korisnik moze citati i pisati
        // shmget vraca -1 ako nije uspio, i 0 ako je uspio
        if(mem==-1)
        {
                printf("\nNema zajednicke memorije");
                exit(1);
        }

        int k = arg/2; // broj argumenata
        if(k>=5){ // zadano u zadatku... max je moguce 5 mnogokuta
	    printf("\nMaksimalno je moguce 5 mnogokuta!\n");
	    exit(0);		
	}
	int n[k];
        float a[k], P, O;
	int j=1, z=0;

	do{
	    n[z] = atoi(argc[j]); // zapisivanje argumenata u vsrijable
            a[z] = atof(argc[j+1]);
            j = j+2;
	    z++;	
	}while(j<arg);
	
	// n - broj stranica mnogokuta,
	// a - duljina stranice

        seg=(int *) shmat(mem, NULL, 0); // shmat funkcija, kazemo procesu gdje je ta  memorija koju smo gore stvorili
        // aeg adresa memorijskoj prostora....valjda..., to nam treba da bismo kasnije mogli obrisati taj dio memorije....valjda
          // ako uspijelo je gore, u mem ce bit zapisan id tog dijela memorije
          
        sigset(SIGINT,brisi_mem); // na pritisak ctrl+c pozvat ce se funkcija brisi memoriju....
        
	int i;
        for(i=0;i<k;i++)
        {
	    sleep(1);		
            if(fork()==0) // sa funkcijom fork stvaramo procese, ako fork uspije vraca NULL
            {
		P=4*n[i]*a[i]*a[i]/(3.14159265358979/n[i]); // tu fali 'tan', kaze da njemu nije radio 'tan' 
		printf("n=%d    a=%f    P=%f \n", n[i], a[i], P); 
              	sleep(1);
                O=n[i]*a[i];
		printf("n=%d    a=%f    O=%f \n", n[i], a[i], O);
                    
                exit(0);
            }
        }
        for(i=0;i<k;i++)
                wait (NULL); // proces roditelj (cijeli program) sa fork stvara procese dijecu....
                //procesu roditelju smo morali reci da ceka na tu djecu, i to radimo sa wait, slicno i kod dretva, ona tamo fukcije na kraju

        brisi_mem(0); // funkcija za brisanje memorije...
        printf("\n");
        return 0;
}

