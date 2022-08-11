#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fstream>
#include <iostream>

int main(){
	std::ofstream ofs("/tmp/lock", std::ios::out |std::ios::app);
	ofs<<"Hello!"<<std::endl;
	ofs.close();
	while(1);
}
