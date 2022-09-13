#include <iostream>
#include <sec_ipc.h>
#include <stdlib.h>
#include <thread>
#include <mutex>

void test_ipc(SecIPC *secipc, int &tid, int &count, std::mutex &m){
	int ret = 0, t=0, test_loop = 100;
	
	while(t<test_loop){
		std::lock_guard<std::mutex> lock(m);
		//2. send IPC message
		
		uint8_t sdata[IPC_SHM_MAX_SIZE]={tid, count};
		uint8_t rdata[IPC_SHM_MAX_SIZE]={0,};
		size_t size = 2;
	
		ret = secipc->sendIPCMessageMQ(sdata,size);
		if(ret == IPC_ERR_MQSEND)
			perror("IPC mq send error");

		//3. receive IPC message
		ret = secipc->receiveIPCMessageMQ(rdata, &size);
		if(ret == IPC_ERR_MQRECV)
			perror("IPC shm recv error");
		else{
			for(int i = 0; i < size; i++){
				std::cout<<(int)rdata[i]<<", ";
			}
			std::cout<<std::endl;
		}

		t++;
		count++;
	}

	std::cout<<tid<<" end"<<std::endl;

}

void thread_test_ipc(){
	int count = 0, ret = 0;
	int tids[3] = {1,2,3};
	std::mutex m;
	
	SecIPC *secipc = new SecIPC();
	//1. open 
	ret = secipc->openIPCMQ();
	if(ret == IPC_ERR_MQOPEN)
		perror("IPC mq open error");
	else
		std::cout<<"IPC mq open success"<<std::endl;



	std::thread t1(test_ipc, secipc, std::ref(tids[0]), std::ref(count), std::ref(m));
	std::thread t2(test_ipc, secipc, std::ref(tids[1]), std::ref(count), std::ref(m));
	std::thread t3(test_ipc, secipc, std::ref(tids[2]), std::ref(count), std::ref(m));

	t1.join();
	t2.join();
	t3.join();

	std::cout<<"count:"<<count<<std::endl;

	//4. close IPC
	secipc->closeIPCMQ(); 
}
int main(){
	
	thread_test_ipc();
}

