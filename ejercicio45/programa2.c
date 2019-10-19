/* El programa2 es el destinado a generar un número, y verificar si es correcto o no */

#include <stdio.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/sem.h> 
#include <stdlib.h> 
#include <unistd.h>

#include "common.h"

int main(void){
    int semid; 
    struct sembuf operacion;
    int valor;
    int intento;

    int i;

    int shmid;
    datos *zona_comun;

    // creación de la memoria común
    shmid = shmget(CLAVE_SHM, sizeof(datos), IPC_CREAT|0666);

    if(shmid== -1){
	    perror("shmget");
	    exit(EXIT_FAILURE);
	}
    zona_comun = (datos*) shmat(shmid,0,0);

    // creacción del semaforo 
    semid = semget(CLAVE,2,IPC_CREAT|0600);
  
    if(semid == -1){ 
	    perror("semget");
	    exit(EXIT_FAILURE); 
	}

    /* Inicialización del semáforo */
    semctl(semid,SEM_2,SETVAL,0);

    printf("Valor semáforo: %d\n", semctl(semid,SEM_2,GETVAL,0));

    operacion.sem_flg=0;	/* No se activa ninguna opcion */ 

    
    /* Generación número aleatorio */
    srandom(getpid());   // inicializa la semilla para los numeros 
	valor=(int)(100*random()/RAND_MAX);   // x será un numero entre 0 y 100
    printf("El valor elegido es: %d\n",valor);
    
    zona_comun->valor = valor;
    zona_comun->criterio  = 1;
    sprintf(zona_comun->resolucion,"init");
    zona_comun->intento = -1;

    while(1){     
        // Espero semaforo 2
        operacion.sem_num = SEM_2; // que semaforo (puede haber más de 1!)
        operacion.sem_op = -1; // que operación: +1=signal, -1=wait
        semop(semid,&operacion,1);// envía el comando
        
        sleep(1);

        intento = zona_comun->intento;
        printf("El proceso 1 me lanza el valor: %d\n", intento);

        if(intento < valor){
            zona_comun->criterio = 1;
            sprintf(zona_comun->resolucion,"sube");
        }else if(intento > valor){
            zona_comun->criterio = -1;
            sprintf(zona_comun->resolucion,"baja");
        }else{
            zona_comun->criterio = 0;
            sprintf(zona_comun->resolucion,"done");
            printf ("El proceso 1 ha acertado\n");
            
            operacion.sem_num = SEM_1; // que semaforo (puede haber más de 1!)
            operacion.sem_op = +1; // que operación: +1=signal, -1=wait
            semop(semid,&operacion,1);// envía el comando

            break;
        }

        // Lanzo semáforo 1
        operacion.sem_num = SEM_1; // que semaforo (puede haber más de 1!)
        operacion.sem_op = +1; // que operación: +1=signal, -1=wait
        semop(semid,&operacion,1);// envía el comando


    }

printf("He acabado\n");
exit(EXIT_SUCCESS);
}