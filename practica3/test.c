/* Programa de ejemplo del uso de colas de mensajes -- consumidor*/

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


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


/* Función principal */ 
int main(void) { 
    int id_cola, i, ret;
    int status,valmax, valmin;
    pid_t pid;

    int nmax;

    int real,intento,crit;

    struct msgbuf{
	    long mtype;
	    char mtext[15];
    } mensaje;

    nmax = 100; //Condicion enunciado

    switch(pid = fork()){
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
            //Este es el hijo
            id_cola=crearcola(Clave_cola);
            


            srandom(getpid());   // inicializa la semilla para los numeros 
			real=(int)(nmax*random()/RAND_MAX);   // x será un numero entre 0 y 256
			printf("HIJO: El valor que genero es %d \n",real);
            sleep(1);

            while(1){
                sleep(1);


                /* Lectura de datos de la cola */ 
	            ret=msgrcv(id_cola,&mensaje,sizeof(mensaje.mtext),2,0);	  
	  
	            if(ret == -1){
		            perror("HIJO: msgrcv"); 
		            exit(EXIT_FAILURE); 
		        }

                
                intento = atoi(mensaje.mtext);


                if(intento<real){
                    printf("HIJO: Sube\n");
                    sprintf(mensaje.mtext,"%d",1);
                }else if (intento>real){
                    printf("HIJO: Baja\n");
                    sprintf(mensaje.mtext,"%d",-1);
                }else{
                    printf("HIJO: Exacto\n");
                    sprintf(mensaje.mtext,"%d",0);
                }


                mensaje.mtype=1; //Envio mensajes por 1, leo por 2
                ret=msgsnd(id_cola,&mensaje,sizeof(mensaje.mtext),0);
                if(ret == -1){
		            perror("HIJO: msgsnd"); 
		            exit(EXIT_FAILURE); 


		        }

                if (intento == real){
                    break;
                }
            
            }    

            break;
        default:
            //Este es el padre
            
             
            valmax = nmax;
            valmin = 0;
            id_cola=crearcola(Clave_cola);
            
            intento = (int) (valmax + valmin)/2;


            printf("PADRE: El valor que intento es %d\n",intento);
            sleep(1);
            while(1){
                

                sprintf(mensaje.mtext,"%d",intento);
                mensaje.mtype=2; //Envio mensajes por 2, leo por 1

	            /* Envío del mensaje (introducción en la cola) */ 
	            ret=msgsnd(id_cola,&mensaje,sizeof(mensaje.mtext),0);

	            if(ret == -1){
		            perror("PADRE: msgsnd"); 
		            exit(EXIT_FAILURE); 
		        }
                sleep(1);

                ret=msgrcv(id_cola,&mensaje,sizeof(mensaje.mtext),1,0);	  
	  
	            if(ret == -1){
		            perror("PADRE: msgrcv"); 
		            exit(EXIT_FAILURE); 
		        }

                crit = atoi(mensaje.mtext);
                

                /*El criterio de resolución se hace probando todas las soluciones
                ya que es más sencillo y no se especifica un limite de intentos
                como en la practica 2*/

                if (crit == -1){
                    intento--;
                }else if(crit == 0){
                    printf("PADRE: Acerte\n");
                    break;
                }else{
                    intento++; //Por defecto suma en caso de retardo en la respuesta 
                }

                printf("PADRE: El valor que intento es %d\n",intento);
                sleep(1);
            }    







            /* El proceso padre espera la finalizacion del proceso hijo */
            waitpid(pid,&status,0);
            
            // elimina la cola del sistema
            ret=msgctl(id_cola,IPC_RMID,(struct msqid_ds*)0);
            
    }


    exit(EXIT_SUCCESS);

}