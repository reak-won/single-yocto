#include <iostream>
#include <sec_ipc.h>
#include <stdlib.h>

int main(){
	int ret = 0;
	std::cout<<"IPC version :"<<getSecIPCVersion()<<std::endl;
	
	SecIPC *secipc = new SecIPC();

	ret = secipc->openIPCMQ(IPC_COMMON_MQ_NAME);
	
	if(ret == IPC_ERR_MQOPEN)
		perror("IPC mq open error");
	else
		std::cout<<"IPC mq open success"<<std::endl;
	
	uint8_t sdata[4]={97,98,99,100};
	uint8_t rdata[4]={0,};
	size_t size;
	ret = secipc->sendIPCMessage(sdata,4);
	
	if(ret == IPC_ERR_MQSEND)
		perror("IPC mq send error");
	else
		std::cout<<"IPC mq send success"<<std::endl;
	

	ret = secipc->receiveIPCMessage(rdata, &size);
	if(ret == IPC_ERR_MQRECV)
		perror("IPC mq recv error");
	else{
		std::cout<<"IPC mq recv success"<<std::endl;
		std::cout<<"size:"<<size<<std::endl;
		for(int i = 0; i < size; i++){
			std::cout<<rdata[i];
		}
		std::cout<<std::endl;
	}

}
