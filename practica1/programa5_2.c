#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
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
			
	
	//El usuario puede introducir el número de hijos
	n = 2;			
	
	//Se inicia el contador del ejercicio. El valor es arbitrario
	z = 0;

	printf("Hola, yo soy el padre, y tengo PID %d. Mi valor de z es %d\n",getpid(),z);

	for(i=0;i<n;i++){
		z++;
		/* Creación de un proceso hijo */
		switch(pid = fork()){
			case (pid_t) -1:
				/* void perror(const char *message): imprime un
				mensaje de error asociado a la cadena que tiene como
				argumento */
				perror("fork");
				exit(-1);
			
			case (pid_t) 0:
				printf("Hola, yo soy el hijo, y tengo PID %d. Mi padre es el PID %d y mi valor de Z es %d\n",getpid(),getppid(),z);
				exit(EXIT_SUCCESS);
				break;
			default:
				waitpid(0,&status,0);
				break;
		}
	}
printf("Padre: mis hijos ya terminaron'!\n");
exit(0);
}
