#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* Variables globales */


/* Función principal */
int main (void){
	
	int i;
	int n;
	int z;
	int status;
	pid_t pid; /* pid_t es un entero que representa el
				identificador ID de un
				proceso */
				
	n = 2;			
	
	for(i=0;i<n;i++){
	
		/* Creación de un proceso hijo */
		switch(pid = fork()){
			case (pid_t) -1:
				/* void perror(const char *message): imprime un
				mensaje de error asociado a la cadena que tiene como
				argumento */
				perror("fork");
				exit(-1);
			
			case (pid_t) 0:
				z++;
				printf("Hola, yo soy el hijo, y tengo PID %d. Mi padre es el PID %d y mi valor de Z es %d\n",getpid(),getppid(),z);
				exit(EXIT_SUCCESS);
				break;
			default:
				printf("Hola, yo soy el padre, y tengo PID %d. Mi valor de z es %d\n",getpid(),z);
				waitpid(pid+i+1,&status,0);
		


		printf("Padre: mis hijos ya terminaron'!\n");
		break;
	}
 exit(0);
}