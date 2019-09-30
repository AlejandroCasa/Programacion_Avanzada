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
	
	int i,res;
	
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
	
	intento = zona_comun->intento;
	printf("El intento fue %d\n",intento);	

	zona_comun->criterio = 0;
		
		sleep(1);
		
	/* Borrado de la zona de memoria compartida */ 
  shmctl(shmid, IPC_RMID,0);
  
  if(res==-1)
	{
	  perror("shmctl");
	  exit(EXIT_FAILURE);
	}
		

	exit(EXIT_SUCCESS);
}