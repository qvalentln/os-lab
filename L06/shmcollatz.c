#include <stdio.h> // printf
#include <errno.h> // errno
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> // exit
#include <unistd.h>
#include <sys/wait.h>


#define PAGE_SIZE 4096

int main(int argc, char* argv[]) {

	// create some shared memory object

	char shm_name[] = "/myshm";
	int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(shm_fd < 0) {
		perror("shm_open");
		return errno;
	}
	
	int n = argc - 1;

	// define the shm obj size
	size_t shm_size = n * PAGE_SIZE;

	if(ftruncate(shm_fd, shm_size) < 0) {
		perror(NULL);
		shm_unlink(shm_name);
		return errno;
	}

	printf("Starting parent: %d \n", getpid());
	
	for(int i = 0; i < n; ++i) {
		pid_t pid = fork();
		if(pid < 0) {
			perror(NULL);
			return errno;
		}
		else if(pid == 0) {
			// child instructions

			char* shm_area = mmap(NULL, PAGE_SIZE, PROT_WRITE,
								MAP_SHARED, shm_fd, i * PAGE_SIZE);

			if(shm_area == MAP_FAILED) {
				perror(NULL);
				exit(errno);
			}

			long clz = strtol(argv[i + 1], NULL, 10);

			shm_area += sprintf(shm_area, "%ld: ", clz);
			shm_area += sprintf(shm_area, "%ld ", clz);
			while(clz > 1) {
				if(clz % 2 == 0) clz /= 2;
				else clz = clz * 3 + 1;
				shm_area += sprintf(shm_area, "%ld ", clz);
			}
			shm_area += sprintf(shm_area, "\n");
			printf("Done Parent %d Me %d \n", getppid(), getpid());
			munmap(shm_area, PAGE_SIZE);
			exit(0);
		}
		
		
	}
	
	while(wait(NULL) > 0);

	for(int i = 0; i < n; ++i) {
		char* shm_area = mmap(NULL, PAGE_SIZE, PROT_READ, 
							MAP_SHARED, shm_fd, i * PAGE_SIZE);
		if(shm_area == MAP_FAILED) {
			perror(NULL);
			shm_unlink(shm_name);
			return errno;
		}		

		printf("%s", shm_area);
		munmap(shm_area, PAGE_SIZE);
	}
	
	
	printf("Done Parent %d Me %d\n", getppid(), getpid());
	shm_unlink(shm_name);
	return 0;
}
