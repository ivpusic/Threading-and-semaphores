#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

extern int random_prime();
int ttime;

void *dretva()
{
	int i;
	for(i=0;i<ttime;i++)
	{
		printf("\nDretva tid=%i broj=%i",pthread_self(),random_prime());
		fflush(stdout);
		usleep(1000000);
		fflush(stdout);
	}
	printf("\nDretva tid=%i zavrsila",pthread_self());
}	
int main(int argc, char *argv[])
{
	int i,threadsq,t;
	ttime=atoi(argv[2]);
	threadsq=atoi(argv[1]);
 	pthread_t threads[threadsq];
	for(i=0;i<threadsq;i++)
	{
		pthread_create(&threads[i], NULL,(void *)dretva, NULL);


	}
	for(i=0; i<threadsq; ++i) 
	{
		pthread_join(threads[i], NULL);
	}
	printf("\n");
return 0;
}

