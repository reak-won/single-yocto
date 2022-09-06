#include <iostream>
#include <sec_ipc.h>
#include <stdlib.h>

int main(){
	int ret = 0;
	std::cout<<"IPC version :"<<getSecIPCVersion()<<std::endl;
	
	SecIPC *secipc = new SecIPC();
	
	//1. open 
	ret = secipc->openIPCMQ();
	
	if(ret == IPC_ERR_MQOPEN)
		perror("IPC mq open error");
	else
		std::cout<<"IPC mmq open success"<<std::endl;
	
	uint8_t sdata[IPC_SHM_MAX_SIZE]={97,98,99,100};
	uint8_t rdata[IPC_SHM_MAX_SIZE]={0,};
	size_t size;

	//2. send IPC message
	ret = secipc->sendIPCMessageMQ(sdata,4);
	if(ret == IPC_ERR_MQSEND)
		perror("IPC mq send error");
	else
		std::cout<<"IPC mq send success"<<std::endl;
	
	//3. receive IPC message
	/*ret = secipc->receiveIPCMessageShm(rdata, &size);
	if(ret == IPC_ERR_SHMRECV)
		perror("IPC shm recv error");
	else{
		std::cout<<"IPC shm recv success"<<std::endl;
		std::cout<<"size:"<<size<<std::endl;
		for(int i = 0; i < 4; i++){
			std::cout<<rdata[i];
		}
		std::cout<<std::endl;
	}*/

	//4. close IPC
	//secipc->closeIPCShm(); 
	std::cout<<"end"<<std::endl;
}
