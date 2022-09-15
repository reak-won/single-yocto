#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include <openssl/aes.h>
#include <inttypes.h>
#include <algorithm>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include <vector>
#include <string>

#define PORT 54321
#define MAX_BUF_SIZE 4096

int logfd;
char *log_msg;

//UDP socket 
struct sockaddr_in6 server_addr, client_addr;
int sock;

void die(const char *s){
	write(logfd, s, strlen(s));
	exit(1);
}

void log(const char *l){
	write(logfd,"[CORE] ", 7);
	write(logfd, l, strlen(l));
}

void log(uint8_t *l, size_t len){
	write(logfd,"[CORE] ", 7);
	write(logfd, l, len);
}
void udp_connect_test(){
	struct sockaddr_in6 server_addr, client_addr;
	int sock, i;
	socklen_t slen = sizeof(client_addr), recv_len;

	char buf[128] = {0,};
	if((sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket\n");
	
	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_addr = in6addr_loopback;
	server_addr.sin6_port = htons(PORT);

	if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) die("bind error\n");
	
	while(1){
		memset(buf, 0, sizeof(buf));
		if((recv_len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &slen)) == -1)
			die("recvfrom()\n");
		log("received\n");
		log(buf);
		
		if(sendto(sock, buf, recv_len, 0, (struct sockaddr*) &client_addr, slen) == -1)
			die("sendto()\n");
	}

}

void create_udp_sock(){
	int i;
	socklen_t slen = sizeof(client_addr), recv_len; 
	
	if((sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket error\n");
	
	log("socket ok\n");
	
	memset((char*)&server_addr, 0, sizeof(server_addr));

	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_addr = in6addr_loopback;
	server_addr.sin6_port = htons(PORT);

}

void bind_udp_sock(){
	if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		die("bind error\n");
	log("bind ok\n");
}

socklen_t recv_udp_msg(uint8_t *buf){

	socklen_t slen, recv_len;
	memset(buf, 0, MAX_BUF_SIZE);
	log("recv wait\n");
	if((recv_len = recvfrom(sock, buf, MAX_BUF_SIZE, 0, (struct sockaddr*)&client_addr, &slen)) == -1) die("recv from");

	log(buf, recv_len);
	
	return recv_len;
}

ssize_t send_udp_msg(uint8_t *buf, size_t len){
	socklen_t slen = sizeof(client_addr);
	ssize_t sent_bytes;
	if((sent_bytes = sendto(sock, buf, len, 0, (struct sockaddr*)&client_addr, slen)) == -1) 
		die("sendto() error\n");
	log("sendto\n");
	return sent_bytes;	
}
void aes_test(){

	uint8_t Key[32];
	uint8_t IV[AES_BLOCK_SIZE];
	RAND_bytes(Key, sizeof(Key));
	RAND_bytes(IV, sizeof(IV));
	uint8_t IVd[AES_BLOCK_SIZE];

	for(int i=0; i<AES_BLOCK_SIZE; i++)
		IVd[i] = IV[i];
	
	AES_KEY *AesKey = new AES_KEY();
	AES_set_encrypt_key(Key, 256, AesKey);

	std::string txt("this is a test\n");
	const int UserDataSize = (const int)txt.length();
	int RequiredPadding = (AES_BLOCK_SIZE - (txt.length() % AES_BLOCK_SIZE));
	std::vector<unsigned char> PaddedTxt(txt.begin(), txt.end());
	for(int i=0;i<RequiredPadding;i++)
		PaddedTxt.push_back(0);

	unsigned char *UserData = &PaddedTxt[0];
	const int UserDataSizePadded = (const int)PaddedTxt.size();

	unsigned char EncryptedData[512]={0,};
	AES_cbc_encrypt(UserData, EncryptedData, UserDataSizePadded, (const AES_KEY*)AesKey,
			IV, AES_ENCRYPT);
	
	log("encrypted\n");
	
	for(int i=0;i<UserDataSizePadded;i++){
		write(logfd, &EncryptedData[i], 1);
	}

	AES_KEY *AesDecryptKey = new AES_KEY();
	AES_set_decrypt_key(Key, 256, AesDecryptKey);

	unsigned char DecryptedData[512] = {0,};
	AES_cbc_encrypt(EncryptedData, DecryptedData, UserDataSizePadded, 
			(const AES_KEY*)AesDecryptKey, IVd, AES_DECRYPT);

	log("\ndecrypted");
	
	for(int i=0;i<UserDataSizePadded;i++)
		write(logfd, &DecryptedData[i], 1);
	
}
int main(){

	int ret = 0;
	uint8_t buf[MAX_BUF_SIZE] = {0,};
	logfd = open("/tmp/core-start", O_CREAT|O_RDWR, 0777);

	log("core started\n");

	create_udp_sock();
	bind_udp_sock();
	
	char logbuf[128]={0,};
	while(1){
		ssize_t len = recv_udp_msg(buf);
		memset(logbuf, 0, 128);
		sprintf(logbuf,"received len:%d\n",len);
		log(logbuf);
		buf[0]= 97;
		send_udp_msg(buf,sizeof(buf));
	}

	close(logfd);
}
