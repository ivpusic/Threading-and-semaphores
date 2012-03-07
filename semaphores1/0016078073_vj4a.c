#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int SemId;
void SemGet(int n){
   SemId = semget(IPC_PRIVATE, n, 0600);
   if (SemId == -1) {
      printf("Pogreska...!\n");
      exit(1);
   }
}

void SemRemove(void){
   (void) semctl(SemId, 0, IPC_RMID, 0);
}

int SemOp(int SemNum, int SemOp){
   struct sembuf SemBuf;
   SemBuf.sem_num = SemNum;
   SemBuf.sem_op  = SemOp;
   SemBuf.sem_flg = 0;
   return semop(SemId, & SemBuf, 1);
}

int broj,zastitna_suma=0,kraj=0,txt_datoteka=0;
char red_iz_datoteke[255];

void *dohvati(void *naziv_datoteke){
	FILE *dat;
	dat = fopen(naziv_datoteke,"r");
	while(1){
		fgets(red_iz_datoteke,255,dat);
		if(feof(dat))break;
		
		SemOp(1,1);
		SemOp(2,1);
		SemOp(0,-1);
		SemOp(0,-1);
		
	}
	kraj=1;
	SemOp(1,1);
	SemOp(2,1);
}

void *suma(){
	int i;
	while(1){
		i=0;
		SemOp(2,-1);
		if(kraj)break;
		while(red_iz_datoteke[i]!='\n' && i<256)
			zastitna_suma ^= red_iz_datoteke[i++];
		SemOp(0,1);
	}
}

int broji(){
	int broj=0,i=0;
	
	do{
		if(txt_datoteka){
			if(red_iz_datoteke[i]==' ' || red_iz_datoteke[i]=='\n')
				broj++;
		}else{
			if(red_iz_datoteke[i]=='<' && red_iz_datoteke[i+1]=='/')
				broj++;
		}
	}while(red_iz_datoteke[i++]!='\n' && i<256);
	return broj;
}

void *broji_rijeci_tagove(){
	while(1){
		SemOp(1,-1);
		if(kraj)break;
		broj+=broji();
		SemOp(0,1);
	}
}

void analiza(char *naziv_datoteke){
	int i=0,ispravna=1;
	pthread_t dretva[3];
	
	while(naziv_datoteke[i]!='\0' && naziv_datoteke[i++]!='.');
	
	switch(naziv_datoteke[i]){
	case 't':
		if(naziv_datoteke[i+1]!='x' || naziv_datoteke[i+2]!='t'){
			ispravna=0;
		}else{
			txt_datoteka=1;
		}
		break;		
	case 'h':
		if(naziv_datoteke[i+1]!='t' || naziv_datoteke[i+2]!='m' || naziv_datoteke[i+3]!='l')
			ispravna=0;
		break;
	default:
		ispravna=0;
	}
	if(ispravna){
		
		pthread_create(&dretva[0], NULL, dohvati,(void *)naziv_datoteke);
		pthread_create(&dretva[1], NULL, suma, NULL);
		pthread_create(&dretva[2], NULL, broji_rijeci_tagove, NULL);
		
		for(i=0;i<3;i++)
			pthread_join(dretva[i], NULL);
		if(txt_datoteka){	
			printf("Datoteka \"%s\" ima %d rijeci; zastitna suma je: %d\n",naziv_datoteke,broj, zastitna_suma);
		}else{
			printf("Datoteka \"%s\" ima %d tagova; zastitna suma je: %d\n",naziv_datoteke,broj, zastitna_suma);
		}
	}else{		
		printf("Analiza obustavljena za datoteku \"%s\"\n",naziv_datoteke);
	}	
}

int main(int argc, char** argv){
	int i;
	for(i=1;i<argc;i++){
		if(fork()==0){
			SemGet(3);
			analiza(argv[i]);
			SemRemove();
			exit(0);
		}
	}
	while(i--) wait(NULL);
	return 0;
}
