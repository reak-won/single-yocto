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

#include "../inc/UDPComm.h"


/** main **/
int main(){		
	uint8_t msg[MAX_BUF_SIZE]={0, };
	UDPComm *udpComm = new UDPComm();

	udpComm->open_log_outputstream();
	if(ERR_SOCK_OPEN_FAIL == udpComm->create_udp_sock())
		udpComm->log("sock open fail\n");

	int count = 0;
	while(1){
		sleep(1);
		memset(msg, 0, sizeof(msg));
		//udpComm->log((char*)msg);
		
		uint8_t data[]={'h','e','l','l','o','\0'};
		
		if(ERR_SOCK_SEND_FAIL == udpComm->send_udp_msg(data,sizeof(data))){
			udpComm->log("SEND FAIL\n");
			exit(1);
		}

		if(ERR_SOCK_RECV_FAIL == udpComm->recv_udp_msg(msg, sizeof(msg))){
			udpComm->log("RECV FAIL\n");
			exit(1);
		}
	}
	//ipc_shm_test();
	//ipc_mq_test();
}

