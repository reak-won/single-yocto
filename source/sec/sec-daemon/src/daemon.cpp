#include "../inc/daemon.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sec_ipc.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

Daemon::Daemon(){

}

Daemon::~Daemon(){
	if(sock	> 0) close(sock);
	//if(ofs != nullptr) ofs.close();
}
int Daemon::open_log_outputstream(){
	ofs = std::ofstream("/tmp/daemon_log", std::ios::out| std::ios::app);
	ofs<<"daemon start"<<std::endl;
	return ERR_NONE;
}
void Daemon::log(const std::string &msg){
	ofs<<msg<<std::endl;
}
int Daemon::create_udp_sock(){
	if((sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		return ERR_SOCK_OPEN_FAIL;

	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(PORT);
	server_addr.sin6_addr = in6addr_loopback;
	return ERR_NONE;
}

int Daemon::send_udp_msg(const char *msg){
	socklen_t slen = sizeof(server_addr);
	ssize_t sent_bytes;
	if((sent_bytes = sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&server_addr, slen)) == -1){
		return ERR_SOCK_SEND_FAIL;
	}
	return sent_bytes;

}

socklen_t Daemon::recv_udp_msg(char *msg, size_t len){
	
	socklen_t slen,  recv_len;
	if((recv_len = recvfrom(sock, msg, len, 0, (struct sockaddr*)&server_addr, &slen)) ==-1)
		return ERR_SOCK_RECV_FAIL;

	log("received");
	log(msg);
	return recv_len;
}
