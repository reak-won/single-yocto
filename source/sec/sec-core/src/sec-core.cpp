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

int logfd;
void die(char *s){
	write(logfd, s, strlen(s));
	exit(1);
}

void udp_connect_test(){
	struct sockaddr_in6 server_addr, client_addr;
	int sock, i;
	socklen_t slen = sizeof(client_addr), recv_len;

	char buf[128] = {0,};

	write(logfd,"hello\n",strlen("hello\n"));

	if((sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket");
	write(logfd, "socket ok\n",strlen("socket ok\n"));
	
	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_addr = in6addr_loopback;
	server_addr.sin6_port = htons(PORT);

	if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) die("bind");
	write(logfd, "bind ok\n", strlen("bind ok\n"));
	while(1){
		memset(buf, 0, sizeof(buf));
		write(logfd, "recv wait\n",strlen("recv wait\n"));
		if((recv_len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &slen)) == -1)
			die("recvfrom()");

		write(logfd, buf, recv_len);
		write(logfd, "send to\n",strlen("send to\n"));
		if(sendto(sock, buf, recv_len, 0, (struct sockaddr*) &client_addr, slen) == -1)
			die("sendto()");
	}

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
	
	write(logfd, "encrypted\n",strlen("encrypted\n"));
	for(int i=0;i<UserDataSizePadded;i++){
		write(logfd, &EncryptedData[i], 1);
	}

	AES_KEY *AesDecryptKey = new AES_KEY();
	AES_set_decrypt_key(Key, 256, AesDecryptKey);

	unsigned char DecryptedData[512] = {0,};
	AES_cbc_encrypt(EncryptedData, DecryptedData, UserDataSizePadded, 
			(const AES_KEY*)AesDecryptKey, IVd, AES_DECRYPT);

	write(logfd, "\n",strlen("\n"));
	write(logfd, "decrypted\n",strlen("decrypted\n"));
	for(int i=0;i<UserDataSizePadded;i++)
		write(logfd, &DecryptedData[i], 1);
	
}
int main(){

	logfd = open("/tmp/core-start", O_CREAT|O_RDWR, 0777);
	write(logfd,"hello\n",strlen("hello\n"));
	//udp_connect_test(fd);
	aes_test();

	close(logfd);
}
