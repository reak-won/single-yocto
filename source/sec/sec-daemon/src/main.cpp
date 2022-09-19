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
#include "../inc/daemon.h"


/** main **/
int main(){		
	char msg[4096]={0, };
	Daemon *daemon = new Daemon();
	daemon->open_log_outputstream();
	if(ERR_SOCK_OPEN_FAIL == daemon->create_udp_sock())
		daemon->log("sock open fail\n");

	int count = 0;
	while(1){
		sleep(1);
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "count:%d\n",count++);
		daemon->log(msg);

		if(ERR_SOCK_SEND_FAIL == daemon->send_udp_msg(msg)){
			daemon->log("SEND FAIL\n");
			exit(1);
		}

		if(ERR_SOCK_RECV_FAIL == daemon->recv_udp_msg(msg, sizeof(msg))){
			daemon->log("RECV FAIL\n");
			exit(1);
		}
	}
	//ipc_shm_test();
	//ipc_mq_test();
}

