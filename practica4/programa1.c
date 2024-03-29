/* Creación de una zona de memoria compartida */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

/* Fichero que contiene la información común (clave y estrucura de datos) */
#include "common.h"


int main(void)
{
  int shmid;
  datos *zona_comun;

  shmid = shmget(CLAVE_SHM, sizeof(datos), IPC_CREAT|0666);

  if(shmid== -1)
	{
	  perror("shmget");
	  exit(EXIT_FAILURE);
	}

  zona_comun = (datos*) shmat(shmid,0,0);

  zona_comun->unEntero = 123;
  zona_comun->unFloat  = 1.23;
  strcpy(zona_comun->unArray,"hola mundo");
  
  exit(EXIT_SUCCESS);
  
}