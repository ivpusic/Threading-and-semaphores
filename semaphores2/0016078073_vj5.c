#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int stapic[5];
char filozofi[5];
pthread_mutex_t monitor;
pthread_cond_t uvjet[5];

void ispis(){
    int br;
	for(br=0;br<5;br++)
		printf(" %c",filozofi[br]);
	printf("\n");
}

void jesti(int ID){
     
	pthread_mutex_lock(&monitor);
	filozofi[ID]='o';
	while(stapic[ID]==0 || stapic[(ID+1)%5]==0)
		pthread_cond_wait(&uvjet[ID],&monitor);
	stapic[ID] = stapic[(ID+1)%5]=0;
	filozofi[ID]='X';
	ispis();	
	pthread_mutex_unlock(&monitor);
	
	sleep(2);
	
	pthread_mutex_lock(&monitor);
	filozofi[ID]='0';
	stapic[ID] = stapic[(ID+1)%5]=1;
	pthread_cond_signal(&uvjet[(ID-1)%5]);
	pthread_cond_signal(&uvjet[(ID+1)%5]);
	ispis();	
	pthread_mutex_unlock(&monitor);
}

void misliti(){
	sleep(3);
}

void *filozof(void *ID){
	while(1){
		jesti(*((int*)ID));
		misliti();
	}
};
int main(){
	int j[5];
	int br;
	for(br=0;br<5;br++){		
		pthread_cond_init(&uvjet[br],NULL);
		stapic[br]=1;
		filozofi[br]='0';
	}
	
	pthread_mutex_init(&monitor,NULL);
	pthread_t *dretve;
	dretve=malloc(5*sizeof(pthread_t));
	
	for(br=0;br<5;br++){
		j[br]=br;
		pthread_create(&dretve[br], NULL, filozof, &j[br]);
	}
	
	for(br=0;br<5;br++)
		pthread_join(dretve[br], NULL);
		
	for(br=0;br<5;br++)
		pthread_mutex_destroy(&monitor);
		
	return 0;
}
