#include <unistd.h> // fork
#include <stdio.h>
#include <sys/wait.h> // wait
#include <errno.h> // errno
#include <signal.h> // signal

#define CH_NUM 10

void handler(int sig) {
	pid_t pid;
	int status;

	// pid_t waitpid(pid_t wpid, int *status, int options);

	while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		if(WIFEXITED(status)) {
			printf("pid %d exited with some status \n", pid);
		}
		else if(WIFSIGNALED(status)) {
			printf("pid %d killed by some signal %d \n", pid);
		}
	}
}



int main() {
	pid_t pid;

	struct sigaction sa = { 0 };
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);

	if(sigaction(SIGCHLD, &sa, NULL) == -1) {
		return errno;	
	}

	printf("launching %d children.. \n", CH_NUM);
	for(int i = 0; i < CH_NUM; ++i) {
		printf("fork %d ... \n", i);
		pid = fork();
		if(pid < 0) {
			return errno;
		}
		else if(pid == 0) {
			sleep(3);
			_exit(i + 1);
		}
	}

	printf("all children are launched ... \n");
	sleep(10);
	printf("done... \n");
	return 0;
	
	
}
