
volatile int isint = 0;

int isprime(int x) {
	if(x < 2) return 0;
	else if(x != 2 && x % 2 == 0) return 0;
	else {
		for(int d = 3; d * d <= x; d += 2) {
			if(x % d == 0) return 0;
		}
		return 1;
	}
}


void handler(int sig) {
	isint = 1;
}

int main(int argc, char* argv[]) {

	char sh_name[] = "/myshm";
	int shm_fd = shm_open(sh_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	signal(SIGINT, SIG_IGN);

	int n = strtol(argv[1], NULL, 10);
	int k = strtol(argv[2], NULL, 10);

	size_t shm_size = k * PAGE_SIZE;

	ftruncate(shm_fd, shm_size);
	
	int total = 0;
	
	for(int i = 0; i < k; ++i) {
		pid_t pid = fork();
		
		if(pid == 0) {
			signal(SIGINT, handler);
			char* shm_area = mmap(NULL, PAGE_SIZE, PROT_WRITE, 
							MAP_SHARED, shm_fd, i * PAGE_SIZE);
			int cnt = 0;
			isint = 0;

			
			printf("COPIL: %d, PARINTE: %d \n", getpid(), getppid());
			for(int y = i * (n / k) + 1; y <= (i + 1) * (n / k); ++y) {
				cnt += isprime(y)
				if(isint) {
					sprintf(shm_area, "%d copil %ld: [%ld, %ld] a gasit %ld numere prime si a fost oprit la %ld\n",
										cnt, i, i * (n / k) + 1, (i + 1) * (n / k), y);
					munmap(shm_area, PAGE_SIZE);
					exit(1);
				}
				
								
			}

			total += cnt;

			sprintf(shm_area, "copil %ld: [%ld, %ld] a gasit %ld numere prime\n",
								 i, i * (n / k) + 1, (i + 1) * (n / k), cnt);

			munmap(shm_area, PAGE_SIZE);
			exit(0);
		}
	}

	while(wait(NULL) > 0);

	for(int i=0;i < k; ++i) {
		total += atoi(shm_area);
		char* shm_area = mmap(NULL, PAGE_SIZE, PROT_READ, MAP_SHARED, 
								i * PAGE_SIZE);
		printf("%s", strchr(shm_area, ' ')  + 1);
		munmap(shm_area, PAGE_SIZE);
	}

	printf("in total %ld numere prime\n", total);

	shm_unlink(shm_name);

	return 0;

	
	
}
