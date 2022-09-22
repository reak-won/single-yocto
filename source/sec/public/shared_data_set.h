#ifndef __SHARED_DATA_SET_H
#define __SHARED_DATA_SET_H
#include <stdint.h>

#define MAX_BUF_SIZE 4096
struct shared_data_set {
	uint32_t data_size;
	uint8_t data[MAX_BUF_SIZE];
};
#endif
