#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>

int SemId;
int MemId;

struct t_data{
    int ULAZ, IZLAZ;
    char M[5];
};
struct t_data* podaci;

enum{ PISI = 0, PUN, PRAZAN, S1, S2 };

void SemGet( int *SemId, int N ){
    *SemId = semget( IPC_PRIVATE, N, 0600 );
    if( *SemId == -1 ){
        printf( "Semafori nisu kreirani!\n");
        exit(1);
    }
}

int SemOp( int SemId, int SemNum, int SemOp){
   struct sembuf SemBuf;
   SemBuf.sem_num = SemNum;
   SemBuf.sem_op  = SemOp;
   SemBuf.sem_flg = 0;
   return semop(SemId, &SemBuf, 1);
}


int SemSetVal( int SemId, int SemNum, int SemVal){
    union semun {
            int val;
            struct semid_ds *buf;
            ushort *array;
    } arg;
    arg.val = SemVal;
    return semctl( SemId, SemNum, SETVAL, arg);
}

void DeleteAll( int sig ){
    shmdt( (char*) podaci);
    shmctl( MemId, IPC_RMID, NULL);
    semctl( SemId, 0, IPC_RMID, 0);
}

void POTROSAC(){
    int i = 0, N = 0;
    char s[ 22 ];
    do{
        SemOp( SemId, PRAZAN, -1);
        s[ i ]=podaci->M[podaci->IZLAZ];
        printf("POTROSAC <- %c\n",s[ i ]);
        sleep(1);
        if( s[ i ] == '\0')
            N++;
        else
            i++;
        podaci->IZLAZ=(podaci->IZLAZ+1)%5;
        SemOp(SemId,PUN,1);
    }while( N < 2);
    s[ i ] = '\0';
    printf( "\nPrimljeno je %s\n",s);
}

void PROIZVODAC( int broj ){
    SemOp( SemId, S1, -1 );
    char s[ 10 ];
    printf( "Unesite znakove za proizvodjaca %d: ", broj );
    scanf("%s",s);
    SemOp( SemId, S1, 1);
    if( broj == 2)
        SemSetVal( SemId, S2, 2 );
    SemOp( SemId, S2, -1);
    int i = 0;
    do{
        SemOp( SemId, PUN, -1 );
        SemOp( SemId, PISI, -1 );
        podaci->M[podaci->ULAZ]=s[ i ];
        printf( "PROIZVODAC%d -> %c\n", broj, s[i]);
        podaci->ULAZ = ((podaci->ULAZ)+1)%5;
        SemOp( SemId, PISI, 1 );
        usleep(500000);
        SemOp( SemId, PRAZAN, 1);
    }while( s[i++ ] != '\0');
}

int main(){
    SemGet( &SemId, 5 );
    MemId = shmget( IPC_PRIVATE, sizeof( struct t_podaci* ), 0600);
    if( MemId == -1 ){
        printf( "Memorija nije kreirana!\n");
        exit(1);
    }
    podaci = (struct t_data*)shmat( MemId, NULL, 0 );
    podaci->ULAZ = 0;
    podaci->IZLAZ = 0;

    SemSetVal( SemId, S1, 1 );
    SemSetVal( SemId, PUN, 2);
    SemSetVal( SemId, PISI, 1);

    sigset( SIGINT, DeleteAll);
    int i;
    for( i = 0; i < 2; i++ ){
        if( fork()==0){
            PROIZVODAC(i+1);
            exit(0);
        }
    }

    if( fork() == 0 ){
        POTROSAC();
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);
    DeleteAll(0);
    return 0;
}

