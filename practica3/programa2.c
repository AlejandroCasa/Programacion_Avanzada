/* Programa de ejemplo del uso de pipes */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>


/* Definición de la clave de la cola */
#define Clave_cola 1

/* Función auxiliar que crea la cola, retornando el identificador */ 
int crearcola(int key) 
{ 
  int msgqid;
  
  msgqid = msgget(key, IPC_CREAT|0666);
  switch(msgqid)
	{
	case -1:
	  perror("msgget");
	  return(-1);
	default:
	  return msgqid; 
	}
} /* Fin función auxiliar */

int main(void){
	
	int real;
	int intento;
    int i;
    int ret;

	int status;
	int nread;

    struct msgbuf{
	    long mtype;
	    char mtext[15];
    } mensaje;
    
	// Creacion de un proceso hijo
	switch(fork()){
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			// ESTE ES EL HIJO ¡

            id_cola=crearcola(Clave_cola);
            mensaje.mtype=1; 

			srandom(getpid());   // inicializa la semilla para los numeros 
			real=(int)(100.0*random()/RAND_MAX);   // x será un numero entre 0 y 256
			printf("Hijo: El valor que genero es %d \n",real);
				

			for(i=0;i<100;i++){

                sleep(1);

                ret=msgrcv(id_cola,&mensaje,sizeof(mensaje.mtext),1,0);
                if(ret == -1){
		            perror("msgrcv"); 
		            exit(EXIT_FAILURE); 
		        }
                intento = atoi(ret);
                printf("HIJO: He recibido el numero: %d\n",intento);

                if (intento != real){
                    mensaje.mtext = "False";
                    ret=msgsnd(id_cola,&mensaje,sizeof(mensaje.mtext),0);
                }
                else{
                    mensaje.mtext = "True";
                    ret=msgsnd(id_cola,&mensaje,sizeof(mensaje.mtext),0);
                    exit(EXIT_SUCCESS);
                }

				
			}
	}//default

	// ESTE ES EL PADRE

	id_cola=crearcola(Clave_cola);
    mensaje.mtype=2; 

	for(i=0;i<100;i++){

        

        sprintf(mensaje.mtext,"%d",i);
        ret=msgsnd(id_cola,&mensaje,sizeof(mensaje.mtext),0);
        if(ret == -1){
		    perror("msgsnd"); 
		    exit(EXIT_FAILURE); 
		}

        sleep(1);

        ret=msgrcv(id_cola,&mensaje,sizeof(mensaje.mtext),1,0);	  
	  
	    if(ret == -1){
		  perror("msgrcv"); 
		  exit(EXIT_FAILURE); 
		}

	    printf("PADRE: He recibido el mensaje: %s\n",mensaje.mtext);

        if (ret == "True"){
            /* El proceso padre espera la finalizacion del proceso hijo */
            wait(&status);
            exit(EXIT_SUCCESS);
        }

    }            


}