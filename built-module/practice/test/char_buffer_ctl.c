#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <char_buffer_ioctl.h>

#define DEVICE_PATH "/dev/char_buffer"

static void print_usage(const char *program)
{
	fprintf(stderr, "Usage: %s read | write <text> | clear | size\n",
		program);
}

int main(int argc, char **argv)
{
	char buffer[256];
	unsigned int size;
	ssize_t result;
	int fd;
	int status = 0;

	if (argc < 2) {
		print_usage(argv[0]);
		return 1;
	}

	fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	if (strcmp(argv[1], "read") == 0 && argc == 2) {
		result = read(fd, buffer, sizeof(buffer));
		if (result < 0) {
			perror("read");
			status = 1;
		} else if (result > 0) {
			fwrite(buffer, 1, result, stdout);
			if (buffer[result - 1] != '\n')
				putchar('\n');
		}
	} else if (strcmp(argv[1], "write") == 0 && argc == 3) {
		result = write(fd, argv[2], strlen(argv[2]));
		if (result < 0) {
			perror("write");
			status = 1;
		} else {
			printf("Wrote %zd bytes\n", result);
		}
	} else if (strcmp(argv[1], "clear") == 0 && argc == 2) {
		if (ioctl(fd, CHAR_BUFFER_CLEAR) < 0) {
			perror("ioctl clear");
			status = 1;
		} else {
			puts("Buffer cleared successfully");
		}
	} else if (strcmp(argv[1], "size") == 0 && argc == 2) {
		if (ioctl(fd, CHAR_BUFFER_GET_SIZE, &size) < 0) {
			perror("ioctl size");
			status = 1;
		} else {
			printf("Buffer size: %u bytes\n", size);
		}
	} else {
		print_usage(argv[0]);
		status = 1;
	}

	close(fd);
	return status;
}
