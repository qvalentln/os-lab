#include <unistd.h> // write, read
#include <sys/stat.h> // S_IRUSR, S_IWUSR, stat, chmod
#include <fcntl.h> // open
#include <stdio.h> // perror
#include <errno.h> // errno

#define BLK_SIZE 4096 // standard block size

int main(int argc, char* argv[]) {

	// argv[1], argv[2]

	if(argc != 3) {
		fprintf(stderr, "usage: %s [input_file] [output_file]", argv[0]);
		return 1;
	}

	int input_fd = open(argv[1], O_RDONLY);

	if(input_fd == -1) {
		perror(argv[1]);
		return errno;
		
	}

	int output_fd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

	if(output_fd == -1) {
		perror(argv[2]);
		return errno;
	}


	char buff[BLK_SIZE];
	ssize_t nread;
	
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



	// copied the entire file, now i want to "copy"  the perms

	struct stat input_st;
	if(stat(argv[1], &input_st)) {
		perror(argv[1]);

		close(input_fd);
		close(output_fd);
		return errno;
	}


	mode_t perms = input_st.st_mode & 07777777;

	if(chmod(argv[2], perms) < 0) {
		perror(argv[2]);

		return errno;
	}

	// ret 0, perms changed
	
	close(input_fd);
	close(output_fd);

	return 0;

}
