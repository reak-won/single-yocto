#ifndef __THREAD_H_
#define __THREAD_H_

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>

const size_t DEFAULT_THREAD_SIZE = 3;
class ThreadPool{
public:
	~ThreadPool();
	static ThreadPool* getInstance(){
		if(instance == NULL)
			return instance = new ThreadPool(DEFAULT_THREAD_SIZE);
		return instance;
	}

private:
	static ThreadPool* instance;
	ssize_t thread_size;
	std::vector<std::thread> worker_threads;
	std::queue<std::function<void()>> jobs();
	std::mutex mtx;
	bool stop_all;

	ThreadPool(size_t thread_size);
	void workterThread();
};

#endif
