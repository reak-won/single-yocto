#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define PORT 54321
void die(int fd, char *s){
	write(fd, s, strlen(s));
	exit(1);
}

int main(){
	struct sockaddr_in6 server_addr, client_addr;
	int sock, i;
	socklen_t slen = sizeof(client_addr), recv_len;

	char buf[128] = {0,};

	int fd = open("/tmp/core-start", O_CREAT|O_RDWR, 0777);
	write(fd,"hello\n",strlen("hello\n"));

	if((sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die(fd, "socket");
	write(fd, "socket ok\n",strlen("socket ok\n"));
	
	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_addr = in6addr_loopback;
	server_addr.sin6_port = htons(PORT);

	if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) die(fd, "bind");
	write(fd, "bind ok\n", strlen("bind ok\n"));
	while(1){
		memset(buf, 0, sizeof(buf));
		write(fd, "recv wait\n",strlen("recv wait\n"));
		if((recv_len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &slen)) == -1)
			die(fd, "recvfrom()");

		write(fd, buf, recv_len);
		write(fd, "send to\n",strlen("send to\n"));
		if(sendto(sock, buf, recv_len, 0, (struct sockaddr*) &client_addr, slen) == -1)
			die(fd, "sendto()");
	}
	close(fd);
}
