#ifndef __daemon_H
#define __daemon_H

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>

#define ERR_NONE 0
#define ERR_LOG_OPEN_FAIL 1
#define ERR_SOCK_OPEN_FAIL -1
#define ERR_SOCK_SEND_FAIL -2
#define ERR_SOCK_RECV_FAIL -3
#define ERR_SOCK_BIND_FAIL -4
class UDPCommClient{
private:
	std::ofstream ofs;
	struct sockaddr_in6 server_addr;
	int sock;

public:
	UDPCommClient();
	~UDPCommClient();	
	void log(const std::string &msg);
	int open_log_outputstream(const char *);
	int create_udp_sock(const int port);
	int send_udp_msg(uint8_t *, uint32_t );
	socklen_t recv_udp_msg(uint8_t *, uint32_t);
};


class UDPCommServer{
private:
	std::ofstream ofs;
	struct sockaddr_in6 server_addr;
	struct sockaddr_in6 client_addr;
	int sock;

	//clen must be member. why? 
	socklen_t clen;
public:
	UDPCommServer();
	~UDPCommServer();
	void log(const std::string &msg);
	int open_log_outputstream(const char *);
	int create_udp_sock(const int port);
	int bind_udp_sock();
	int send_udp_msg(uint8_t *, uint32_t);
	socklen_t recv_udp_msg(uint8_t *, uint32_t);
};
#endif
