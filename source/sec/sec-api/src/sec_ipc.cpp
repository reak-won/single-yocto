#include <iostream>
#include <mqueue.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../inc/sec_ipc.h"
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

char* getSecIPCVersion(){
	//Warning removal : ISO C++ forbids converting a string constant to 'char*'
	return (char*)"1.0.0";
}

SecIPC::SecIPC(){
}

SecIPC::~SecIPC(){
	
}
/* IPC Message Queue */
int SecIPC::openIPCMQ(){
	attr.mq_maxmsg = 8;
	attr.mq_msgsize = 255;
	char *mq_name = IPC_MQ_NAME;
	mq = mq_open(mq_name, O_RDWR| O_CREAT, 0666, &(this->attr));
	if(mq == -1)
		return IPC_ERR_MQOPEN;
	return IPC_ERR_NONE;
}

int SecIPC::sendIPCMessageMQ(uint8_t *data, size_t data_size){
	int ret = IPC_ERR_NONE;
	ret = mq_send(mq, (char*)data, data_size, 1);
	if(ret == -1) 
		return IPC_ERR_MQSEND;
	return IPC_ERR_NONE;
}

int SecIPC::receiveIPCMessageMQ(uint8_t *data, size_t *data_size){
	int ret = 0;
	ret = mq_receive(mq, (char*)data, this->attr.mq_msgsize, NULL);
	if(ret ==-1)
		return IPC_ERR_MQRECV;
	*data_size = ret;
	return IPC_ERR_NONE;
}

void SecIPC::closeIPCMQ(){
	mq_close(mq);
}
/* IPC Message Queue End */

/* IPC Shared Memory */
int SecIPC::openIPCShm(){
	int ret = IPC_ERR_NONE;
	shmfd = shm_open(IPC_SHM_NAME, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if(shmfd == -1)	return IPC_ERR_SHMOPEN;

	ret = ftruncate(shmfd, IPC_SHM_MAX_SIZE);
	if(ret == -1) return IPC_ERR_FTRUNC;

	return IPC_ERR_NONE;
}

int SecIPC::sendIPCMessageShm(uint8_t *data, size_t data_size){
	void *addr = mmap(NULL, IPC_SHM_MAX_SIZE, PROT_WRITE, MAP_SHARED, shmfd, 0);
	if(addr == MAP_FAILED)
		return IPC_ERR_MAP_FAILED;
	
	memcpy(addr, data, IPC_SHM_MAX_SIZE);
	return IPC_ERR_NONE;
}

int SecIPC::receiveIPCMessageShm(uint8_t *data, size_t *data_size){
	void *addr = mmap(NULL, IPC_SHM_MAX_SIZE, PROT_READ, MAP_SHARED, shmfd, 0);
	if(addr == MAP_FAILED)
		return IPC_ERR_MAP_FAILED;
	memcpy(data, addr, IPC_SHM_MAX_SIZE);
	return IPC_ERR_NONE;
}

int SecIPC::closeIPCShm(){
	if(shm_unlink(IPC_SHM_NAME) == -1)
		return IPC_ERR_UNLINK;
	return IPC_ERR_NONE;	
}

/* IPC Shared Memory End */
