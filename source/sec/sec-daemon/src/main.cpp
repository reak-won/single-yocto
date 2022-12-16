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
#include <shared_data_set.h>

#include <UDPComm.h>

#define PORT 54321
/** main **/
int main(){		
	uint8_t msg[MAX_BUF_SIZE]={0, };
	int ret = 0;
	UDPCommClient *udpComm = new UDPCommClient();

	udpComm->open_log_outputstream("/tmp/daemon_log");
	if(ERR_SOCK_OPEN_FAIL == udpComm->create_udp_sock(PORT))
		udpComm->log("sock open fail\n");

	int count = 0;
	while(1){
		sleep(1);
		memset(msg, 0, sizeof(msg));
		//udpComm->log((char*)msg);
		
		uint8_t data[]={'h','e','l','l','o','\0'};
		ret = udpComm->send_udp_msg(data, sizeof(data));
		if(ERR_SOCK_SEND_FAIL == ret){
			udpComm->log("SEND FAIL\n");
			udpComm->log(strerror(errno));
			exit(1);
		}

		ret = udpComm->recv_udp_msg(msg, sizeof(msg));
		if(ERR_SOCK_RECV_FAIL == ret){
			udpComm->log("RECV FAIL\n");
			udpComm->log(strerror(errno));
			exit(1);
		}
	}

	udpComm->log("daemon end\n");
	//ipc_shm_test();
	//ipc_mq_test();
}

