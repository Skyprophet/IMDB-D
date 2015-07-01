#include "NodeLinker.h"

using namespace IMDB::DTXN;
using namespace IMDB;

boost::asio::io_service NodeLinker::io_service;
tcp::resolver NodeLinker::resolver(NodeLinker::io_service);

NodeLinker::NodeLinker(int no, std::string host, std::string port)
	: worker_id(no), host(host), port(port), socket(io_service), stop(false)
{
	boost::asio::connect(socket, resolver.resolve({host.c_str(), port.c_str()}));
	thd = new std::thread(
		[this]
		{
			for(;;)
			{
				std::function<void()> task;
					{
					std::unique_lock<std::mutex> lock(this->queue_mutex);
					this->condition.wait(lock,
						[this]{ return this->stop || !this->tasks.empty(); });
					if(this->stop && this->tasks.empty())
						return;
					task = std::move(this->tasks.front());
					this->tasks.pop();
				}
					task();
			}
		}
	);
}

NodeLinker::~NodeLinker() {
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();
	thd->join();
	delete thd;
}


std::future<std::pair<size_t, char*>> NodeLinker::submitTask(const char* request, const size_t& size) {
	return submitTask([this](const char* request, const size_t& size) {
		boost::asio::write(this->socket, boost::asio::buffer(request, size));
		char reply[1024];
		size_t length = this->socket.read_some(boost::asio::buffer(reply));
		char* ans;
		ans = new char[length];
		memcpy(ans, reply, length);
		return std::make_pair(length, ans);
	}, request, size);
}

template<class F, class... Args>
auto NodeLinker::submitTask(F&& f, Args&&... args) 
	-> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared< std::packaged_task<return_type()> >(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);
		
	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queue_mutex);

		// don't allow enqueueing after stopping the pool
		if(stop)
			throw std::runtime_error("enqueue on stopped ThreadPool");

		tasks.emplace([task](){ (*task)(); });
	}
	condition.notify_one();
	return res;
}