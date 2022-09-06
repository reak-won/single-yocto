
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
#include <sec_ipc.h>


#define PORT 54321
#define BUFLEN 128
std::ofstream ofs;
void open_log_outputstream(){
	ofs= std::ofstream("/tmp/daemon_log", std::ios::out |std::ios::app);
	ofs<<"daemon start"<<std::endl;
}

void sock_udp_test(){
	
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

void ipc_shm_test(){
	int ret = 0;
	SecIPC *ipc = new SecIPC();
	if((ret = ipc->openIPCShm()) != IPC_ERR_NONE){
		ofs<<"ipc open error :"<<ret<<std::endl;
		return;
	}
	ofs<<"ipc open "<<ret<<std::endl;
	uint8_t data[IPC_SHM_MAX_SIZE]={0,};
	size_t data_size; 
	sleep(40);
	if((ret = ipc->receiveIPCMessageShm(data, &data_size)) != IPC_ERR_NONE){
		ofs<<"ipc receive error:"<<ret<<std::endl;
		ipc->closeIPCShm();
		return;
	}

	ofs<<"ipc receive "<<ret<<std::endl;
	ofs<<"received from ipc : "<<data[0]<<", "<<data[1]<<std::endl;
	
	ipc->closeIPCShm();
}

void ipc_mq_test(){
	int ret = 0;
	SecIPC *ipc = new SecIPC();
	if((ret = ipc->openIPCMQ()) != IPC_ERR_NONE){
		ofs<<"ipc open error :"<<ret<<std::endl;
		return;
	}

	ofs<<"ipc open "<<ret<<std::endl;
	uint8_t data[512] = {0,};
	size_t data_size;
	if((ret = ipc->receiveIPCMessageMQ(data, &data_size)) != IPC_ERR_NONE){
		ofs<<"ipc receive error:"<<ret<<std::endl;
		return; 
	}
	ofs<<"ipc receive "<<ret<<std::endl;
	ofs<<"received from ipc : "<<data[0]<<", "<<data[1]<<std::endl;
	
	ipc->closeIPCMQ();
}
