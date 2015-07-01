#include "NodeLinker.h"
#include "BaseCoordinatorService.h"
#include "../common/CharArray.h"
#include "../benchmark/tpcc-d/TpccTuples.h"
#include "../benchmark/tpcc-d/RandomGenerator.h"
#include <cstdio>
#include <cassert>

using namespace IMDB::DTXN;
using namespace IMDB::Benchmark::TPCCD;
using namespace IMDB;

std::vector<NodeLinker*> linkers;

int main() {
	BaseCoordinatorService cood_service;

	cood_service.addNodeLinker("localhost", "33333");

	NewOrderParam param;
	param.w_id = 10;
	param.d_id = 22;
	param.c_id = 333;
	param.o_entry_d = RandomGenerator::generateCurrentTime();
	for (int i = 0; i < 3; ++i) {
		param.i_ids.push_back(i);
		param.i_w_ids.push_back(i+2);
		param.i_qtys.push_back(i+3);
	}

	CharArray request;
	param.serialize(request);

	printf("Serialize Complete!!\n");
	printf("Request Data Length: %d\n", request.size);

	std::future<std::pair<size_t, char*>> reply = cood_service.request(0, request.data, request.size);

	std::pair<size_t, char*> reply_pair = reply.get();
	printf("Have got the reply!\n");
	printf("Reply Data Length: %ld\n", reply_pair.first);
	CharArray reply_data;
	reply_data.allocate(reply_pair.first);
	reply_data.data = reply_pair.second;
	assert(reply_data.data != NULL);

	NewOrderParam echo_param;
	printf("Deserialize Start!!\n");
	echo_param.deserialize(reply_data);
	printf("Deserialize Complete!!\n");

	assert(param.w_id == echo_param.w_id);
	assert(param.d_id == echo_param.d_id);
	assert(param.c_id == echo_param.c_id);
	assert(param.o_entry_d == echo_param.o_entry_d);
	assert(3 == echo_param.i_ids.size());
	assert(3 == echo_param.i_w_ids.size());
	assert(3 == echo_param.i_qtys.size());
	for (size_t i = 0; i < echo_param.i_ids.size(); ++i) {
		assert(param.i_ids[i] == echo_param.i_ids[i]);
		assert(param.i_w_ids[i] == echo_param.i_w_ids[i]);
		assert(param.i_qtys[i] == echo_param.i_qtys[i]);
	}

	printf("All test OK!\n");

	return 0;
}
