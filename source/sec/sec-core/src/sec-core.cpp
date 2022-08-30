#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	int fd = open("/tmp/core-start", O_CREAT, 0777);
	close(fd);
}
