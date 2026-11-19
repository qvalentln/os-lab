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

	for(int i=0;i<n;++i) {
		pid_t pid = fork();
		if(pid < 0) {
			return errno;
		}
		else if(pid == 0) {
			// child

			char* shm_area = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
								MAP_SHARED, shm_fd, i * PAGE_SIZE);

			if(shm_area == MAP_FAILED) {
				perror(NULL);
				exit(errno);
			}

			long clz = strtol(argv[i + 1], NULL, 10);
			size_t pos = 0;
			pos += snprintf(shm_area + pos, PAGE_SIZE - pos, "%ld: ", clz);

			while(clz > 1 && pos < PAGE_SIZE) {
				pos += snprintf(shm_area + pos, PAGE_SIZE - pos, "%ld ", clz); 
				if(clz % 2 == 0) clz /= 2;
				else clz = clz * 3 + 1;
			}
			if(pos < PAGE_SIZE) {
				snprintf(shm_area + pos, PAGE_SIZE - pos, "%ld: ", clz);
			}
			munmap(shm_area, PAGE_SIZE);
			close(shm_fd);

			printf("Done Parent %d Me %d\n", getppid(), getpid());
			exit(0);
		}
		
	}
	while(wait(NULL) > 0);

	char* shm_ptr = mmap(NULL, shm_size, PROT_READ, MAP_SHARED, shm_fd, 0);
	if(shm_ptr == MAP_FAILED) {
		perror(NULL);
		shm_unlink(shm_name);
		return errno;
	}

	for(int i = 0; i < n; ++i) {
		printf("%s\n", shm_ptr + i * PAGE_SIZE);
	}
	
	munmap(shm_ptr, shm_size);
	return 0;
}
