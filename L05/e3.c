#include <unistd.h> // fork
#include <stdio.h> // perror
#include <errno.h> // errno
#include <sys/wait.h> //wait

#include <stdlib.h> // strtol
int main(int argc, char* argv[]) {

	printf("Starting parent %d\n", getpid());
	for(int i = 1; i < argc; ++i) {
		pid_t pid = fork();
		if(pid < 0) {
			return errno;
		
		}
		else if(pid == 0) {
			// child
			long int clz = strtol(argv[i], NULL, 10);
			printf("%d: ", clz);
			while(clz > 1) {
				printf("%d ", clz);
				if(clz % 2 == 0) clz /= 2;
				else clz = clz * 3 + 1;
			}
			printf("%d\n", clz);

			printf("Done Parent %d Child %d\n", getppid(), getpid());
			exit(0);
		}
	}

	for(int i=1;i<argc;++i) wait(NULL);
	
	printf("Done Parent %d\n Child", getppid(), getpid());

	return 0;
}
