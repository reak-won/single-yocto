#include <iostream>
#include <mqueue.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../inc/sec_ipc.h"
#include <errno.h>

char* getSecIPCVersion(){
	//Warning removal : ISO C++ forbids converting a string constant to 'char*'
	return (char*)"1.0.0";
}

SecIPC::SecIPC(){
	attr.mq_maxmsg = 8;
	attr.mq_msgsize = 255;
}

int SecIPC::openIPCMQ(const char* mq_name){
	mq = mq_open(mq_name, O_RDWR| O_CREAT, 0666, &(this->attr));
	if(mq == -1)
		return IPC_ERR_MQOPEN;
	return IPC_ERR_NONE;
}

int SecIPC::sendIPCMessage(uint8_t *data, size_t data_size){
	int ret = IPC_ERR_NONE;
	ret = mq_send(mq, (char*)data, data_size, 1);
	if(ret == -1) 
		return IPC_ERR_MQSEND;
	return IPC_ERR_NONE;
}

int SecIPC::receiveIPCMessage(uint8_t *data, size_t *data_size){
	int ret = 0;
	ret = mq_receive(mq, (char*)data, this->attr.mq_msgsize, NULL);
	if(ret ==-1)
		return IPC_ERR_MQRECV;
	*data_size = ret;
	return IPC_ERR_NONE;
}

void SecIPC::closeIPC(){
	mq_close(mq);
}
