#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

sem_t sem[4];
int na_stolu;
char naziv[3][20]={"duhan i sibice", "papir i sibice", "papir i duhan"};

void * pusac(void *br){
	switch(*((int *)br)){
	case 0:
		printf("Pusac 0: ima papir\n");
		break;
	case 1:
		printf("Pusac 1: ima duhan\n");
		break;
	case 2:
		printf("Pusac 2: ima sibice\n");
		break;	
	}
	while(1){
		sem_wait(&sem[*((int *)br)]);
		if(na_stolu==*((int *)br)){
			printf("Pusac %d uzima sastojke i ...\n",*((int *)br));
			sem_post(&sem[3]);
		}
	}
}

void * trgovac(void * br){
	sleep(1);
	while(1){
		sleep(1);
		na_stolu=rand()%3;
		printf("\nTrgovac stavlja: %s\n",naziv[na_stolu]);
		sem_post(&sem[0]);
		sem_post(&sem[1]);
		sem_post(&sem[2]);
		sem_wait(&sem[3]);
	}
}

int main(){
	
	pthread_t dretva[4];	
	int i,br[]={0,1,2};
	
	srand(time(NULL));
	for(i=0;i<4;i++)
		sem_init(&sem[i], 0, 0);

	pthread_create(&dretva[0], NULL, pusac, (void *)&br[0]);
	pthread_create(&dretva[1], NULL, pusac, (void *)&br[1]);
	pthread_create(&dretva[2], NULL, pusac, (void *)&br[2]);
	pthread_create(&dretva[3], NULL, trgovac,NULL);

	for(i=0;i<4;i++)
		pthread_join(dretva[i], NULL);
	for(i=0;i<4;i++)
		sem_destroy(&sem[i]);
	return 0;
}
