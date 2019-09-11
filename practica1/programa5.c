#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main (void)
{
	int i;
	int n;
	int z;
	pid_t ppid;
	pid_t pid;
	int status;
	
	z=0;
	n=2;
	
	ppid = getpid() // Almaceno el PID del padre
	
	printf("Soy el padre con un valor de PID de %d, con un valor de Z de %d\n", ppid,z)
	
	for(i=0;i<=n;i++){
		if (ppid != getpid());
			printf("Soy el hijo con un valor de PID de %d, mi padre tiene un PID de %d y mi valor de Z es %d\n",getpid(),getppid(),z)
			break;
		}
		z++;
		pid = fork();
		waitpid(pid,&status,0);
	}
	
	exit(0);
	
}	