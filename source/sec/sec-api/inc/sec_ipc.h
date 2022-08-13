#ifndef __SEC_IPC_H_
#define __SEC_IPC_H_

#include <mqueue.h>
#include <stdint.h>

#define IPC_ERR_NONE 0
#define IPC_ERR_MQOPEN 1
#define IPC_ERR_MQSEND 2
#define IPC_ERR_MQRECV 3

char *getSecIPCVersion();
class SecIPC{
private:
	mqd_t mq;
	struct mq_attr attr;
public:
	SecIPC();
	int openIPCMQ(const char* mq_name);
	int sendIPCMessage(uint8_t* data,size_t data_size);
	int receiveIPCMessage(uint8_t *data, size_t *data_size);
};
#endif
