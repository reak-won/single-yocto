#include <stdio.h>
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
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <algorithm>
#include <vector>
#include <string>
#include <UDPComm.h>

#define PORT 54321
#define MAX_BUF_SIZE 4096

#define IPv6_ALEN 16
#define IPv6_ADDR_STR_MAX_LEN ((IPv6_ALEN *2) +7)

int logfd;
char *log_msg;

//UDP socket 
/*struct sockaddr_in6 server_addr, client_addr;
int sock;

void die(const char *s){
	write(logfd, s, strlen(s));
	exit(1);
}

void log(const char *l){
	write(logfd, l, strlen(l));
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

	char ipv6_addr_str[IPv6_ADDR_STR_MAX_LEN + 1];
	socklen_t slen, recv_len;
	memset(buf, 0, MAX_BUF_SIZE);
	if((recv_len = recvfrom(sock, buf, MAX_BUF_SIZE, 0, (struct sockaddr*)&client_addr, &slen)) == -1) die("recv from");
	
	log("recv : ");
	log(inet_ntop(AF_INET6, &client_addr.sin6_addr, ipv6_addr_str, sizeof(ipv6_addr_str)));
	log("\n");
	buf[recv_len] = '\n';
	log((const char*)buf);
	
	return recv_len;
}

ssize_t send_udp_msg(uint8_t *buf, size_t len){
	socklen_t slen = sizeof(client_addr);
	char ipv6_addr_str[IPv6_ADDR_STR_MAX_LEN + 1];
	ssize_t sent_bytes;
	if((sent_bytes = sendto(sock, buf, len, 0, (struct sockaddr*)&client_addr, slen)) == -1) 
		die("sendto() error\n");
	log("send : ");
	log(inet_ntop(AF_INET6, &client_addr.sin6_addr, ipv6_addr_str, sizeof(ipv6_addr_str)));
	log("\n");
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
	UDPCommServer *udpComm = new UDPCommServer();
	uint8_t buf[MAX_BUF_SIZE] = {0,};

	logfd = open("/tmp/core-start", O_CREAT|O_RDWR, 0777);
	create_udp_sock();
	if(ERR_SOCK_OPEN_FAIL == ret){
		log("SOCK OPEN FAIL ");
		log(strerror(errno));
		log("\n");
		exit(1);
	}
	bind_udp_sock();
	if(ERR_SOCK_BIND_FAIL == ret){
		log("BIND FAIL ");
		log(strerror(errno));
		log("\n");
		exit(1);
	}

	char logbuf[128]={0,};
	while(1){
		
		ret = recv_udp_msg(buf);
		if(ERR_SOCK_RECV_FAIL == ret){
			log("RECV FAIL ");
			log(strerror(errno));
			log("\n");
			exit(1);
		}
		
		// Length Log Insert
		memset(logbuf, 0, 128);
		sprintf(logbuf,"received len:%d\n",ret);
		log(logbuf);
		ret =  send_udp_msg(buf, sizeof(buf));
		if(ERR_SOCK_SEND_FAIL == ret){
			log("SEND FAIL");
			log(strerror(errno));
			log("\n");
			exit(1);
		}
	}

	close(logfd);
}*/
int main(){

	int ret = 0;
	UDPCommServer *udpComm = new UDPCommServer();
	uint8_t buf[MAX_BUF_SIZE] = {0,};

	udpComm->open_log_outputstream("/tmp/core-start");
	ret = udpComm->create_udp_sock(PORT);
	if(ERR_SOCK_OPEN_FAIL == ret){
		udpComm->log("SOCK OPEN FAIL\n");
		udpComm->log(strerror(errno));
		exit(1);
	}
	ret = udpComm->bind_udp_sock();
	if(ERR_SOCK_BIND_FAIL == ret){
		udpComm->log("BIND FAIL\n");
		udpComm->log(strerror(errno));
		exit(1);
	}

	char logbuf[128]={0,};
	while(1){
		
		ret = udpComm->recv_udp_msg(buf, sizeof(buf));
		if(ERR_SOCK_RECV_FAIL == ret){
			udpComm->log("RECV FAIL\n");
			exit(1);
		}
		
		// Length Log Insert
		memset(logbuf, 0, 128);
		buf[0]='A';
		//sprintf(logbuf,"msg:%s received len:%d\n",(char*)buf, ret);
		//udpComm->log(logbuf);
		//buf[0]='A';

		ret =  udpComm->send_udp_msg(buf, sizeof(buf));
		if(ERR_SOCK_SEND_FAIL == ret){
			udpComm->log("SEND FAIL\n");
			udpComm->log(strerror(errno));
			exit(1);
		}
	}

	close(logfd);
	udpComm->log("core end\n");
}
