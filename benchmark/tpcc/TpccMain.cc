#include <boost/lockfree/spsc_queue.hpp>
#include "ScaleParams.h"
#include "TpccPopulator.h"
#include "TpccConstants.h"
#include "TpccSource.h"
#include "TpccTPBatchProcessor.h"
#include "TpccSerialProcessor.h"
#include "TpccBatchProcessor.h"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <cmath>

using namespace IMDB;
using namespace IMDB::StorageEngine;
using namespace IMDB::Benchmark::TPCC;

size_t IMDB::Benchmark::TPCC::kParallelism;
size_t IMDB::Benchmark::TPCC::kPartitionCnt;
size_t IMDB::Benchmark::TPCC::kBatchTime;
bool IMDB::Benchmark::TPCC::kPartition;

int main(int argc, char** argv) {
	if (argc != 5) {
		printf("Usage: tpcc_main processor_no[serial, batch, TPbatch] parallelism partition_num batch_time\n");
		return 1;
	}

	int processor_no = atoi(argv[1]);
	IMDB::Benchmark::TPCC::kParallelism = atoi(argv[2]);
	IMDB::Benchmark::TPCC::kPartitionCnt = atoi(argv[3]);
	IMDB::Benchmark::TPCC::kBatchTime = atoi(argv[4]);
	IMDB::Benchmark::TPCC::kPartition = atoi(argv[3]) == 1 ? false : true;

	srand(time(NULL));
	ScaleParams params(10, 1);
	StorageManager storage_manager;
	TpccPopulator populator(&params, &storage_manager);
	printf("Start Populating...\n");
	populator.start();
	printf("Population Complete...\n");

	boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> queue(2000000 + 1);

	printf("Starting Source...\n");
	TpccSource source(&queue, &params, 2000000);
	source.start();

	printf("Starting Processor...\n");
	if (processor_no == 0) {
		TpccSerialProcessor processor(&queue, &storage_manager, &params);
		processor.start();
	} else if (processor_no == 1) {
		TpccBatchProcessor processor(&queue, &storage_manager, &params);
		processor.start();
	} else {
		TpccTPBatchProcessor processor(&queue, &storage_manager, &params);
		processor.start();
	}

	printf("Finished!!!\n");

	return 0;
}
