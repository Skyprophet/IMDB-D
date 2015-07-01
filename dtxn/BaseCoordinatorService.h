#ifndef __BASE_COORDINATOR_SERVICE_H__
#define __BASE_COORDINATOR_SERVICE_H__

#include "NodeLinker.h"

namespace IMDB {
	namespace DTXN {
		class BaseCoordinatorService {
		public:
			BaseCoordinatorService() {}

			BaseCoordinatorService(std::vector<std::pair<std::string, std::string>> nodes) {
				for (size_t i = 0; i < nodes.size(); ++i)
					linkers.emplace_back(new NodeLinker(i, nodes[i].first, nodes[i].second));
			}

			virtual ~BaseCoordinatorService() {
				for (auto && linker: linkers)
					delete linker;
				linkers.clear();
			}

			int addNodeLinker(std::string host, std::string port) {
				int cnt = linkers.size();
				linkers.emplace_back(new NodeLinker(cnt, host, port));
				return cnt;
			}

			virtual std::future<std::pair<size_t, char*>> request(const int& node_no, const char* data, const size_t& length) {
				return linkers[node_no]->submitTask(data, length);
			}

		private:
			BaseCoordinatorService(const BaseCoordinatorService&);
			BaseCoordinatorService& operator=(const BaseCoordinatorService&);

		protected:
			std::vector<NodeLinker*> linkers;
		};
	}
}

#endif