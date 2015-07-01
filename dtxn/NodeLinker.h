#ifndef __NODE_LINKER_H__
#define __NODE_LINKER_H__

#include "../common/CharArray.h"

#include <boost/asio.hpp>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

using boost::asio::ip::tcp;

namespace IMDB {
	namespace DTXN {
		class NodeLinker {
		public:
			NodeLinker(int no, std::string host, std::string port);
			virtual ~NodeLinker();

			std::future<std::pair<size_t, char*>> submitTask(const char* request, const size_t& size);

		private:
			template<class F, class... Args>
			auto submitTask(F&& f, Args&&... args) 
				-> std::future<typename std::result_of<F(Args...)>::type>;

			int worker_id;
			std::string host, port;
			tcp::socket socket;

			std::thread* thd;
			std::queue< std::function<void()> > tasks;
			
			std::mutex queue_mutex;
			std::condition_variable condition;
			bool stop;

			static boost::asio::io_service io_service;
			static tcp::resolver resolver;
		};
	}
}

#endif