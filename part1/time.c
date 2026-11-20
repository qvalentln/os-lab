

int main(int argc, char* argv[]) {
	

	
	FILE* f = fopen(argv[1], "r");

	char* line = NULL;
	size_t len = 0;
	ssize_t nread;
	while((nread = getline(&line, &len, f)) != -1) {


		
		if(nread > 0 && (line[nread - 1] == '\n' || line[nread - 1] == '\r')) {
			line[nread - 1] = '\0';
		}

		pid_t pid = fork();
		struct timeval start, end;

		char path[] = "/myshm";
		int shm_fd = shm_open(path, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
		int shm_size = PAGE_SIZE;
		ftruncate(shm_fd, shm_size);

		
		gettimeofday(&start, NULL);
		if(pid == 0) {
			close(1);
			dup(shm_fd);
			close(shm_fd);
			char args* = {line, NULL};
			execve(line, args, NULL);

			
		}
		int status;
		waitpid(pid, status, 0);

		gettimeofday(&end, NULL);

		char* shm_area = mmap(NULL, PAGE_SIZE, PROT_READ, MAP_SHARED, shm_fd ,0);

		printf("%s", shm_area);
		munmap(shm_area, PAGE_SIZE);
		
		double elapsed = (end.tv_sec - start_tv.sec) + (end.tv_usec - start.tv_usec) / 100000.0;

		printf("de executie: %.6f s\n", elapsed);

		if(WIFEXITED(status)) {
			printf("de iesire: %d\n", WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status)) {
			printf("terminat de semnalul %d\n", WTERMSIG(status));
		}

		close(shm_fd);
		shm_unlink(shm_name);
		
	}

	fclose(f);
	return 0;
}
