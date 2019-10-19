/* Programa de ejemplo del uso de pipes */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void){
	
	int FtS[2];
    int StF[2];

	int real;
	int intento;


	int status;
	int nread;
	char buffer1[100];
	char buffer2[100];
	char numero[3];
	/* Creacion de la pipe */

	if(pipe(FtS) == -1){
		perror("pipe 1");
		exit(EXIT_FAILURE);
	}
	printf("Pipe 1 OK!\n");

    if(pipe(StF) == -1){
		perror("pipe 2");
		exit(EXIT_FAILURE);
	}
	printf("Pipe 2 OK!\n");

	// Creacion de un proceso hijo
	switch(fork()){
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			// ESTE ES EL HIJO ¡

			srandom(getpid());   // inicializa la semilla para los numeros 
			real=(int)(256.0*random()/RAND_MAX);   // x será un numero entre 0 y 256
			printf("Hijo: El valor que genero es %d \n",real);

			// Cierre del descriptor de escritura y lectura de las pipes en el proceso hijo
			if(close(FtS[1]) == -1)
				perror("close");
				
			if(close(StF[0]) == -1)
				perror("close");
				

			while (1){
				// Lectura de la información contenida en la pipe.
				switch(nread = read(FtS[0],buffer1,sizeof(buffer1))){
					case -1:
						perror("read");
						break;
					case 0:
						perror("EOFs");
						break;
					default:
						printf("Hijo: Mi padre me da un valor de %s\n",buffer1);
						intento = atoi(buffer1);

						sleep(1);
						if(intento<real){
							/* El proceso padre escribe en la pipe */
							if(write(StF[1],"Sube",4) == -1)
								perror("write");	
						}else if (intento>real){
							if(write(StF[1],"Baja",4) == -1)
								perror("write");
						}else{
							if(write(StF[1],"Done",4) == -1)
								perror("write");
							break;	
						}

						
				}
				exit(EXIT_SUCCESS);
			}
	}

	// ESTE ES EL PADRE

	srandom(getpid());   // inicializa la semilla para los numeros 
	intento=(int)(256.0*random()/RAND_MAX);   // x será un numero entre 0 y 256
	printf("Padre: Genero un numero que es %d\n",intento);
	sprintf(numero,"%d",intento);

	// Cierre del descriptor de escritura y lectura de las pipes en el proceso hijo
	if(close(FtS[0]) == -1)
		perror("close");
				
	if(close(StF[1]) == -1)
		perror("close");

	while(1){
		
		if(write(FtS[1],numero,3) == -1)
			perror("write");
	
		sleep(1);
		switch(nread = read(StF[0],buffer2,sizeof(buffer2))){
			case -1:
				perror("read");
				break;
			case 0:
				perror("EOFf");
				break;
			default:
				printf("Padre: Mi hijo me dice ('%s')\n",buffer2);
				if(buffer2 == "Sube"){
			
					intento++;	
				}else if (buffer2 == "Baja"){
					intento--;
				}else{
					break;	
				}
		}
	

	}

	/* El proceso padre espera la finalizacion del proceso hijo */
	wait(&status);
	exit(EXIT_SUCCESS);
}