// See ref: https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h
#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <future>
#include <functional>
#include <stdexcept>
#include <assert.h>

namespace fu
{
	class ThreadPool
	{
	public:
		explicit ThreadPool(std::size_t count = 4);

		template <class F, class...Args>
		auto enqueue(F&& f, Args&&...args) 
			-> std::future<typename std::result_of<F(Args...)>::type>;
		~ThreadPool();

	private:
		std::vector<std::thread> m_threads;
		std::queue<std::function<void()>> m_tasks;
		std::mutex m_queueMutex;
		std::condition_variable m_condition;
		bool m_stop;
	};

	inline ThreadPool::ThreadPool(size_t threadCount)
	: m_stop(false)
	{
		// assert(threadCount <= 0);
		for(size_t i = 0; i < threadCount; ++i){
			m_threads.emplace_back(
				[this]{
					for(;;){
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(m_queueMutex);
							m_condition.wait(lock,
								[this]{return m_stop || !m_tasks.empty(); });
							if(m_stop && m_tasks.empty())
								return;
							task = std::move(m_tasks.front());
							m_tasks.pop();
						}
						task();
					}
				}
			);
		}
	}

	template <class F, class...Args>
	auto ThreadPool::enqueue(F&& f, Args&&...args)
		-> std::future<typename std::result_of<F(Args...)>::type>
	{
		using return_type = typename std::result_of<F(Args...)>::type;

		auto task = std::make_shared< std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);

			// Don't allow enqueueing after stopping the pool
			if(m_stop)
				throw std::runtime_error("Enqueue on stopped ThreadPool");

			m_tasks.emplace([task](){ (*task)(); });
		}
		m_condition.notify_one();
		return res;
	}


	// Joins all threads
	inline ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_stop = true;
		}

		m_condition.notify_all();
		for(std::thread& thread : m_threads)
			thread.join();
	}
	template <typename T>
	using TaskResult = std::future<T>;
	template <typename T>
	using TaskResults = std::vector<TaskResult<T>>;


// Example:
// #include <iostream>

// int main(){
//     ThreadPool pool(4);
//     TaskResult<int> results;

//     for(unsigned int i = 0; i < 8; ++i){
//         results.emplace_back(
//             pool.enqueue([i]{
//                 std::cout << "hello " << i << std::endl;
//                 std::this_thread::sleep_for(std::chrono::seconds(i));
//                 std::cout << "world " << i << std::endl;
//                 return i*i;
//             })
//         )
//     }
// }


} // namespace fu
