#include <unistd.h> // fork
#include <stdio.h> // printf
#include <sys/wait.h> // wait
#include <errno.h> // errno

int main() {
	pid_t pid = fork();
	if(pid < 0) {
		return errno;
	}
	else if (pid == 0) {
	
		printf("my PID = %d\n", getpid());
		
		while(1) {
	
			;
		}
		perror(NULL);
	}
	else {
		wait(NULL);
		
	}

	return 0;
	
}
