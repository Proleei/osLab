#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV_NAME "/dev/osLab4"
#define BUF_SIZE 64

int main()
{
	int fd, a, b;
	char buf[BUF_SIZE];
	fd = open(DEV_NAME, O_RDWR | O_CREAT);
	if (fd < 0) { // open device failed.
		printf("open device %s failded\n", DEV_NAME);
		return -1;
	}
    printf("请输入两个整数：");
    scanf("%d %d", &a, &b);
    sprintf(buf,"%d %d", a, b);
    write(fd, buf, BUF_SIZE);
	read(fd, buf, BUF_SIZE);
	close(fd);
	return 0;
}
