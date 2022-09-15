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
#include "../inc/test.h"
#include <string>
//CMakeLists.txt target_link_libraries & target_include_directories
#include <sec_ipc.h>

#define PORT 54321
extern std::ofstream ofs;

struct sockaddr_in6 server_addr;
int sock;

void die(const std::string &);
void create_udp_sock();
ssize_t send_udp_msg(const char*);
socklen_t recv_udp_msg(char*, size_t);

void log(const std::string &msg){
	ofs<<"[DAEMON] "<<msg<<std::endl;
}


int main(){		
	char msg[4096]={0, };
	open_log_outputstream();
	create_udp_sock();

	int count = 0;
	while(1){
		sleep(1);
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "count:%d\n",count++);

		send_udp_msg(msg);
		recv_udp_msg(msg, sizeof(msg));
	}
	//ipc_shm_test();
	//ipc_mq_test();
}


void die(const std::string &m){
	log(m);
	exit(1);
}
void create_udp_sock(){
	if((sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket error");

	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(PORT);
	server_addr.sin6_addr = in6addr_loopback;

}

ssize_t send_udp_msg(const char *msg){
	socklen_t slen = sizeof(server_addr);
	ssize_t sent_bytes;
	if((sent_bytes = sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&server_addr, slen)) == -1){
		die("sendto error\n");
	}
	return sent_bytes;

}

socklen_t recv_udp_msg(char *msg, size_t len){
	
	socklen_t slen,  recv_len;
	if((recv_len = recvfrom(sock, msg, len, 0, (struct sockaddr*)&server_addr, &slen)) ==-1){
		ofs<<"recvfrom error"<<std::endl;
	}
	log("received");
	log(msg);
	return recv_len;
}
