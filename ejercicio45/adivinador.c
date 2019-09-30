#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/sem.h> 

#include <sys/msg.h>

#include <unistd.h>



#include "common.h"
int main (void){
	
	int i;
	
	int shmid;
	int semid; 
	struct sembuf operacion; 
	
	int nmax,intento;
	
	datos *zona_comun;
	shmid = shmget(CLAVE_SHM, sizeof(datos), IPC_CREAT|0666);

	if(shmid== -1){
		perror("shmget");
	    exit(EXIT_FAILURE);
	}

	zona_comun = (datos*) shmat(shmid,0,0);
		
	nmax=100; //condicion enunciado
	
	intento = 0;
	
	
		
	zona_comun->intento = intento;
	printf("Mi intento es %d\n",intento);

	exit(EXIT_SUCCESS);
}