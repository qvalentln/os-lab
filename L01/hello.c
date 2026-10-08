#include <unistd.h>


#include <stdio.h> // printf
int main(int argc, char* argv[]) {
	char hlo[] = "Hello, World!\n";

	size_t written = 0;
	size_t to_write = sizeof(hlo) - 1;

	while(written < to_write) {
		ssize_t ret = write(1, hlo + written, to_write - written);
		if(ret == -1)
			return 1;
		written += ret;
	}	
	return 0;
	
}
