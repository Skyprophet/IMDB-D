#ifndef __ECHO_NODE_SESSION_H_
#define __ECHO_NODE_SESSION_H_

#include "BaseNodeSession.h"
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace IMDB{
	namespace DTXN {
		class EchoNodeSession : public BaseNodeSession, public std::enable_shared_from_this<EchoNodeSession> {
		public:
			EchoNodeSession(tcp::socket socket) : BaseNodeSession(std::move(socket)) {}
			virtual ~EchoNodeSession() {}

			virtual void start() {
				do_read();
			}
			
		private:
			void do_read() {
				auto self(shared_from_this());
				socket.async_read_some(boost::asio::buffer(data, max_length),
					[this, self](boost::system::error_code ec, std::size_t length)
					{
					  if (!ec)
					  {
						do_write(length);
					  }
					});
			}

			void do_write(std::size_t length) {
				auto self(shared_from_this());
				boost::asio::async_write(socket, boost::asio::buffer(data, length),
					[this, self](boost::system::error_code ec, std::size_t /*length*/) {
						if (!ec) {
							do_read();
						}
					});
			}

			enum { max_length = 1024 };
			char data[max_length];
		};
	}
}

#endif
