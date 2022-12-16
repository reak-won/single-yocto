#include "../inc/UDPComm.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

#define IPv6_ALEN 16
#define IPv6_ADDR_STR_MAX_LEN ((IPv6_ALEN *2) +7)
/**
 * UDP Client 
 */
UDPCommClient::UDPCommClient(){

}

UDPCommClient::~UDPCommClient(){
	if(sock	> 0) close(sock);
	//if(ofs != nullptr) ofs.close();
}
int UDPCommClient::open_log_outputstream(const char* filename){
	ofs = std::ofstream(filename, std::ios::out| std::ios::app);
	ofs<<filename<<" start"<<std::endl;
	return ERR_NONE;
}
void UDPCommClient::log(const std::string &msg){
	ofs<<">> "<<msg<<std::endl;
}
int UDPCommClient::create_udp_sock(const int port){
	if((sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		return ERR_SOCK_OPEN_FAIL;

	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(port);
	server_addr.sin6_addr = in6addr_loopback;
	return ERR_NONE;
}

int UDPCommClient::send_udp_msg(uint8_t *msg, uint32_t len){
	socklen_t slen = sizeof(server_addr);
	ssize_t sent_bytes;
	char ipv6_addr_str[IPv6_ADDR_STR_MAX_LEN + 1];
	if((sent_bytes = sendto(sock, msg, len, 0, (struct sockaddr *)&server_addr, slen)) == -1){
		return ERR_SOCK_SEND_FAIL;
	}
	log("[CLIENT] send message");
	log((char*)msg);
	//log("[CLIENT] sendto : ");
	//log(inet_ntop(AF_INET6, &server_addr.sin6_addr, ipv6_addr_str, sizeof(ipv6_addr_str)));
	return sent_bytes;

}

socklen_t UDPCommClient::recv_udp_msg(uint8_t *msg, uint32_t len){
	
	socklen_t slen,  recv_len;
	char ipv6_addr_str[IPv6_ADDR_STR_MAX_LEN + 1];
	if((recv_len = recvfrom(sock, msg, len, 0, (struct sockaddr*)&server_addr, &slen)) ==-1){\
		return ERR_SOCK_RECV_FAIL;
	}

	log("[CLIENT] received : ");
	//log(inet_ntop(AF_INET6, &server_addr.sin6_addr, ipv6_addr_str, sizeof(ipv6_addr_str)));
	log((char*)msg);
	return recv_len;
}


/*
 * UDP Server
 */


UDPCommServer::UDPCommServer(){
	clen = sizeof(client_addr);
}

UDPCommServer::~UDPCommServer(){
	if(sock	> 0) close(sock);
	//if(ofs != nullptr) ofs.close();
}
int UDPCommServer::open_log_outputstream(const char *filename){
	ofs = std::ofstream(filename, std::ios::out| std::ios::app);
	ofs<<filename<<" start"<<std::endl;
	return ERR_NONE;
}
void UDPCommServer::log(const std::string &msg){
	ofs<<">> "<<msg<<std::endl;
}
int UDPCommServer::create_udp_sock(const int port){
	if((sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		return ERR_SOCK_OPEN_FAIL;

	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port = htons(port);
	server_addr.sin6_addr = in6addr_loopback;
	return ERR_NONE;
}
int UDPCommServer::bind_udp_sock(){
	if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		return ERR_SOCK_BIND_FAIL;
	return ERR_NONE;
}

#define IPv6_ALEN 16
#define IPv6_ADDR_STR_MAX_LEN ((IPv6_ALEN *2) +7)
int UDPCommServer::send_udp_msg(uint8_t *msg, uint32_t len){
	ssize_t sent_bytes;
	
	//for printing address 
	char ipv6_addr_str[IPv6_ADDR_STR_MAX_LEN + 1];

	if((sent_bytes = sendto(sock, msg, len, 0, (struct sockaddr *)&client_addr, clen)) == -1){
		log("[SERVER] send message ");
		log((char*)msg);
		//log(inet_ntop(AF_INET6, &client_addr.sin6_addr, ipv6_addr_str, sizeof(ipv6_addr_str)));
		return ERR_SOCK_SEND_FAIL;
	}
	return sent_bytes;

}

socklen_t UDPCommServer::recv_udp_msg(uint8_t *msg, uint32_t len){
	socklen_t recv_len;
	char ipv6_addr_str[IPv6_ADDR_STR_MAX_LEN + 1];
	if((recv_len = recvfrom(sock, msg, len, 0, (struct sockaddr*)&client_addr, &clen)) ==-1){
		return ERR_SOCK_RECV_FAIL;
	}
	log("[SERVER] received ");
	//log(inet_ntop(AF_INET6, &client_addr.sin6_addr, ipv6_addr_str, sizeof(ipv6_addr_str)));

	log((char*)msg);
	return recv_len;
}

