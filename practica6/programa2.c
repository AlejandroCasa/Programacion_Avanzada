#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// varibles globales
pthread_mutex_t mut;
pthread_cond_t cond;

void* function1 (void *arg){
	int i = 0;

	printf("\n Hello, soy el thread 1 y me voy a dormir...\n");
	
	//PONER AQUI EL CODIGO NECESARIO PARA QUE 
	// ESTE THREAD SE SUSPENDA HASTA SER DESPERTADO POR EL OTRO
	
	pthread_mutex_lock(&mut);
	pthread_cond_wait(&cond,&mut);
	pthread_mutex_unlock(&mut);

	for (i=0;i<3;i++){
		printf("\n Hello, soy el thread 1\n");
		sleep(1);
	}
	
	printf("\n T1 says: bye bye !\n");
		
	
	return NULL;
}

void* function2(void *arg){
	int i;
	
	for (i=0;i<10;i++){
		printf("\n Hello, soy el thread 2\n");
		sleep(1);
		if(i==3){
			// PONER AQUI EL CODIGO PARA DESBLOQUEAR EL THREAD 1
			pthread_mutex_lock(&mut);
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mut);
		}
	}

	pthread_exit(NULL);
	printf("\n T2 says: Hasta luego Lucas !\n");
	
	return NULL;
}

int main(void){
	
	pthread_t t1_id, t2_id;
	int i,err,salida;
	
	pthread_cond_init(&cond,NULL);
	pthread_mutex_init(&mut,NULL);
	
	err = pthread_create(&t1_id, NULL, &function1, NULL);
	if (err != 0)
		printf("\ncan't create thread : [%s]",strerror(err));
	else
		printf("\n Thread created successfully\n");
	
	err = pthread_create(&t2_id, NULL, &function2, NULL);
	if (err != 0)
		printf("\ncan't create thread : [%s]",strerror(err));
	else
		printf("\n Thread created successfully\n");
	
	
	printf("\n Hello, soy el proceso principal \n");
	
	salida = pthread_join(t2_id, NULL);
	
	printf("\n ADIOS, soy el proceso principal \n");
	return 0;
}