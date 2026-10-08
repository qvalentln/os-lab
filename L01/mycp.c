#include <fcntl.h> // open

#include <sys/stat.h> // stat

#include <stdio.h> // perror
#include <errno.h> // errno

#include <stdlib.h> // malloc, free
#include <unistd.h> // write, read

#define BLK_SIZE 4096 // standard block size

int main(int argc, char* argv[]) {
	
	// argv[1], argv[2]


	if(argc != 3) {
		fprintf(stderr, "usage: %s [input_file] [output_file]\n", argv[0]);
		return 1;
	}

	int input_fd = open(argv[1], O_RDONLY);

	if(input_fd == -1) {
		perror(argv[1]);
		return errno;
	}

	int output_fd = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

	if(output_fd == -1) {
		perror(argv[2]);
		return errno;
	}



	ssize_t nread;

	char buff[BLK_SIZE];

	while((nread = read(input_fd, buff, sizeof(buff))) > 0) {
		ssize_t written = 0;
		while(written < nread) {
			ssize_t nwritten = write(output_fd, buff + written, nread - written);
			if(nwritten == -1) {
				perror("Writing error");
				close(input_fd);
				close(output_fd);
				return errno;
			}
			written += nwritten;
			
		}
	}
	if(nread == -1) {
		perror("Reading error");

		close(input_fd);
		close(output_fd);
		return errno;
	}

	close(input_fd);
	close(output_fd);
	return errno;
	

	return 0;
	
}
