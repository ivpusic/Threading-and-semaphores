#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <math.h>
int memor;
int *segm;

void brisiMemoriju()
{
        shmdt((int *) segm);
        shmctl(memor, IPC_RMID, NULL);
        printf("\nMemorija slobodna...\n");
        exit(0);
}

int main(int arg, char *argc[])
{
        memor=shmget(IPC_PRIVATE,sizeof(int)*100,0600);
        if(memor==-1)
        {
                printf("\nPogreska....Nema zajednicke memorije");
                exit(1);
        }

        int ka = arg/2;
        if(ka>=5){
            printf("\nPogreska...Maksimalno je moguce 5 mnogokuta!\n");
            exit(0);
        }
        int n[ka];
        float a[ka], Pov, Ops;
        int j=1, z=0;

        do{
            n[z] = atoi(argc[j]);
            a[z] = atof(argc[j+1]);
            j = j+2;
            z++;
        }while(j<arg);

        segm=(int *) shmat(memor, NULL, 0);
        sigset(SIGINT,brisiMemoriju);

        int i;
        for(i=0;i<ka;i++)
        {
            sleep(1);
            if(fork()==0)
            {
                Pov=0.25*n[i]*a[i]*a[i]/tan(3.14159265358979/n[i]);
                printf("n=%d  a=%f  Povrsina=%f \n", n[i], a[i], Pov);
                sleep(1);
                Ops=n[i]*a[i];
                printf("n=%d  a=%f  Opseg=%f \n", n[i], a[i], Ops);
                exit(0);
            }
        }
        for(i=0;i<ka;i++)
                wait (NULL);

        brisiMemoriju();
        return 0;
}
