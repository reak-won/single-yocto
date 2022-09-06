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

//CMakeLists.txt target_link_libraries & target_include_directories
#include <sec_ipc.h>


int main(){		
	open_log_outputstream();
	//ipc_shm_test();
	ipc_mq_test();
}
