#include <unistd.h> // fork
#include <stdio.h> // printf
#include <sys/wait.h> // wait
#include <errno.h> // errno
#include <signal.h> // signal

void handler() {
	printf("\ncalled the handler \n");
}

int main() {

	signal(SIGINT, SIG_IGN);	

	pid_t pid = fork();
	if(pid < 0) {
		return errno;
	}
	else if (pid == 0) {

		if(signal(SIGINT, handler) == SIG_ERR) {
			return errno;
		}		
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
