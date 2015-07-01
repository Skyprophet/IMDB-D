#ifndef __BASE_NODE_SESSION__
#define __BASE_NODE_SESSION__

#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace IMDB{
	namespace DTXN {
		class BaseNodeSession {
		public:
			BaseNodeSession(tcp::socket socket) : socket(std::move(socket)) {}

			virtual ~BaseNodeSession() {}

			virtual void start() = 0;

		protected:
			tcp::socket socket;
		};
	}
}

#endif
