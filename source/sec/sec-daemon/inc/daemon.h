#ifndef __daemon_H
#define __daemon_H

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>

#define PORT 54321

#define ERR_NONE 0
#define ERR_LOG_OPEN_FAIL 1
#define ERR_SOCK_OPEN_FAIL -1
#define ERR_SOCK_SEND_FAIL -2
#define ERR_SOCK_RECV_FAIL -3
class Daemon{
private:
	std::ofstream ofs;
	struct sockaddr_in6 server_addr;
	int sock;

public:
	Daemon();
	~Daemon();	
	void log(const std::string &msg);
	int open_log_outputstream();
	int create_udp_sock();
	int send_udp_msg(const char*);
	socklen_t recv_udp_msg(char*, size_t);
};
#endif
