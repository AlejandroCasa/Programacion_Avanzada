/* El programa1 es el destinado a adivinar un numero */

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
    int intento;
    int criterio;

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
    semctl(semid,SEM_1,SETVAL,1); 

    printf("Valor semáforo: %d\n", semctl(semid,SEM_1,GETVAL,0));

    operacion.sem_flg=0;	/* No se activa ninguna opcion */ 

    printf("Los datos iniciales son:\n Valor: %d\n Intento: %d\n Criterio: %d\n Mensaje: %s\n", zona_comun->valor,zona_comun->intento,zona_comun->criterio,zona_comun->resolucion);


    intento = 50; 
    

    //




    while(1){

        sleep(1);

        //Espero valor de semaforo 1
        operacion.sem_num = SEM_1; // que semaforo (puede haber más de 1!)
        operacion.sem_op = -1; // que operación: +1=signal, -1=wait
        semop(semid,&operacion,1);// envía el comando
        
        printf("El programa2 dice: %s\n", zona_comun->resolucion);
        
        criterio = zona_comun->criterio;

        if(criterio <0 ){
            intento --;
            zona_comun->intento = intento;
            printf("El numero que intento es: %d\n",zona_comun->intento);
        }else if(criterio > 0){
            intento ++;
            printf("El numero que intento es: %d\n",intento);
            zona_comun->intento = intento;
        }else{
            
            printf("ACERTE CON EL NUMERO: %d\n",intento);
            
            /* Elimina el semáforo */ 
            semctl(semid,0,IPC_RMID,0);

            /* Borrado de la zona de memoria compartida */ 
            shmctl(shmid, IPC_RMID,0);
  
            printf("Fin !\n");

            exit(EXIT_SUCCESS);
            
        }

        //Aumento el valor del semaforo 2
        operacion.sem_num = SEM_2; // que semaforo (puede haber más de 1!)
        operacion.sem_op = +1; // que operación: +1=signal, -1=wait
        semop(semid,&operacion,1);// envía el comando


    }


    printf("Si se imprime esto es que algo ha ido mal");


    
}