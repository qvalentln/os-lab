#include <unistd.h> // fork
#include <stdio.h> // perror
#include <errno.h> // errno
#include <sys/wait.h> //wait

#include <stdlib.h> // strtol
int main(int argc, char* argv[]) {

	pid_t pid = fork();
	if(pid < 0) {
		return errno;
	}
	else if(pid == 0) {
		// child
		printf("my PID = %d, child PID = %d\n", getppid(), getpid());

		long int clz = strtol(argv[1], NULL, 10);
		
		while(clz > 1) {
			printf("%d ", clz);
			if(clz % 2 == 0) clz /= 2;
			else clz = clz * 3 + 1;
		}
		printf("%d\n", clz);
		// perror(NULL);		
		
	}
	else {
		// parent
		wait(NULL);
		// printf("Last\n");

		printf("child %d finished \n", pid);
	}


	return 0;
}
