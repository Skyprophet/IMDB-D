#include "BaseNodeService.h"
#include "EchoNodeSession.h"
#include <iostream>

using namespace IMDB::DTXN;

int main(int argc, char* argv[]) {
	try {
		if (argc != 2) {
			std::cerr << "Usage: server <port>" << std::endl;
			return 1;
		}

		BaseNodeService<EchoNodeSession> server(std::atoi(argv[1]));
		BaseNodeService<EchoNodeSession>::io_service.run();
	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
