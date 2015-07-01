#include <boost/lockfree/spsc_queue.hpp>
#include "ScaleParams.h"
#include "TatpPopulator.h"
#include "TatpSource.h"
#include "TatpSerialProcessor.h"
#include "TatpBatchProcessor.h"
#include "TatpTPBatchProcessor.h"
#include "TatpConstants.h"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace IMDB;
using namespace IMDB::StorageEngine;
using namespace IMDB::Benchmark::TATP;

size_t IMDB::Benchmark::TATP::kParallelism;
size_t IMDB::Benchmark::TATP::kPartitionCnt;
size_t IMDB::Benchmark::TATP::kBatchTime;
bool IMDB::Benchmark::TATP::kPartition;

int main(int argc, char** argv) {
	if (argc != 5) {
		printf("Usage: tatp_main processor_no[serial, batch, TPbatch] parallelism partition_num batch_time\n");
		return 1;
	}

	int processor_no = atoi(argv[1]);
	IMDB::Benchmark::TATP::kParallelism = atoi(argv[2]);
	IMDB::Benchmark::TATP::kPartitionCnt = atoi(argv[3]);
	IMDB::Benchmark::TATP::kBatchTime = atoi(argv[4]);
	IMDB::Benchmark::TATP::kPartition = atoi(argv[3]) == 1 ? false : true;

	srand(time(NULL));
	ScaleParams params;
	StorageManager storage_manager;
	TatpPopulator populator(&params, &storage_manager);
	printf("Start Populating...\n");
	populator.start();
	printf("Population Finished...\n");

	boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> queue(100000001);

	printf("Starting Source...\n");
	TatpSource source(&queue, &params, 100000000);
	source.start();
	printf("Starting Processor...\n");
	
	if (processor_no == 0) {
		TatpSerialProcessor processor(&queue, &storage_manager, &params);
		processor.start();
	} else if (processor_no == 1) {
		TatpBatchProcessor processor(&queue, &storage_manager, &params);
		processor.start();
	} else {
		TatpTPBatchProcessor processor(&queue, &storage_manager, &params);
		processor.start();
	}
	printf("Finished!!!\n");

	return 0;
}
