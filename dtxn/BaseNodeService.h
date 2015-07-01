#ifndef __BASE_NODE_SERVICE__
#define __BASE_NODE_SERVICE__

#include <boost/asio.hpp>
#include "BaseNodeSession.h"

using boost::asio::ip::tcp;

namespace IMDB{
	namespace DTXN {
		template<class T>
		class BaseNodeService {
		public:
			BaseNodeService(int port) : acceptor(io_service, tcp::endpoint(tcp::v4(), port)), socket(io_service) {
				do_accept();
			}

			virtual ~BaseNodeService() {}

		protected:
			virtual void do_accept() {
				acceptor.async_accept(socket,
					[this](boost::system::error_code ec) {
						if (!ec) {
							std::make_shared<T>(std::move(socket))->start();
						}
						do_accept();
					});
			}

			tcp::acceptor acceptor;
			tcp::socket socket;
		protected:
			static boost::asio::io_service io_service;
		};

		template<class T>
		boost::asio::io_service BaseNodeService<T>::io_service;
	}
}

#endif
