#ifndef __SEC_IPC_H_
#define __SEC_IPC_H_

#include <mqueue.h>
#include <stdint.h>

#define IPC_MQ_NAME "/sec-mq"

#define IPC_SHM_NAME "/sec-shm"
#define IPC_SHM_MAX_SIZE 512

#define IPC_ERR_NONE 0x0
#define IPC_ERR_MQOPEN 0x1
#define IPC_ERR_MQSEND 0x2
#define IPC_ERR_MQRECV 0x3

#define IPC_ERR_SHMOPEN 0x51
#define IPC_ERR_SHMSEND 0x52
#define IPC_ERR_SHMRECV 0x53
#define IPC_ERR_FTRUNC 0x54
#define IPC_ERR_MAP_FAILED 0x55
#define IPC_ERR_UNLINK 0x56


char *getSecIPCVersion();
class SecIPC{
private:
	/* MQ vars */
	mqd_t mq;
	struct mq_attr attr;

	/* Shm vars */
	int shmfd;
public:
	SecIPC();
	~SecIPC();

	/* IPC Message Queue */
	int openIPCMQ();
	int sendIPCMessageMQ(uint8_t* data,size_t data_size);
	int receiveIPCMessageMQ(uint8_t *data, size_t *data_size);
	void closeIPCMQ();
	/* IPC Message Queue End */

	/* IPC Shared Memory */
	int openIPCShm();
	int sendIPCMessageShm(uint8_t* data, size_t data_size);
	int receiveIPCMessageShm(uint8_t *data, size_t *data_size);
	int closeIPCShm();
	/* IPC Shared Memory End */
};
#endif
