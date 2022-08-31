#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 54321
#define BUFLEN 128
int main(){
	std::ofstream ofs("/tmp/daemon_log", std::ios::out |std::ios::app);
	ofs<<"daemon start"<<std::endl;
	
	struct sockaddr_in6 server_addr;
	int sock, i;
	socklen_t slen = sizeof(server_addr);
	char buf[BUFLEN];
	char message[BUFLEN];

	int count = 0;
	if( (sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		ofs<<"socket error"<<std::endl;
		exit(1);
	}
	ofs<<"socket ok"<<std::endl;

	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(PORT);
	server_addr.sin6_addr = in6addr_loopback;

	while(1){
		sleep(1);
		count++;
		sprintf(message, "count:%d\n",count);
		ofs<<"message:"<<message<<std::endl;

		if(sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, slen) == -1){
			ofs<<"sendto error"<<std::endl;
			exit(1);
		}
		ofs<<"recv wait"<<std::endl;
		memset(buf, '0', sizeof(buf));
		if(recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&server_addr, &slen) == -1){
			ofs<<"recvfrom error"<<std::endl;
		}

		ofs<<"buf:"<<buf<<std::endl;

	}

	ofs.close();
}
