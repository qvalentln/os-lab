#include <unistd.h> // fork
#include <stdio.h> // perror
#include <errno.h> // errno
#include <sys/wait.h> //wait
int main() {

	pid_t pid = fork();

	if(pid < 0) {
		return errno;
	}
	else if(pid == 0) {
		// child
		printf("my PID = %d, child PID = %d\n", getppid(), getpid());
		char* argv[] = {"ls", NULL};
		execve("/bin/ls", argv, NULL);
			
		perror(NULL);
	}
	else {
		// parent
		
		wait(NULL);
		printf("child %d finished \n", pid);

		// printf("Last\n");
		
	}


	return 0;
}
