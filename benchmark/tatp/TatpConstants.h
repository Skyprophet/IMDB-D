#ifndef __TATP_CONSTANTS_H__
#define __TATP_CONSTANTS_H__

namespace IMDB {
	namespace Benchmark {
		namespace TATP {
			extern size_t kParallelism;
			extern size_t kPartitionCnt;
			extern bool kPartition;
			extern size_t kBatchTime;

			const size_t MAX_PARTITION_CNT = 100;

			enum TupleType : size_t {DELETE_CALL_FORWARDING, GET_ACCESS_DATA, GET_NEW_DESTINATION, GET_SUBSCRIBER_DATA, INSERT_CALL_FORWARDING, UPDATE_LOCATION, UPDATE_SUBSCRIBER_DATA};
			enum TableType : size_t {SUBSCRIBER_TABLE_ID = 0,
									 ACCESS_INFO_TABLE_ID = MAX_PARTITION_CNT,
									 SPECIAL_FACILITY_TABLE_ID = MAX_PARTITION_CNT * 2,
									 CALL_FORWARDING_TABLE_ID = MAX_PARTITION_CNT * 3};

			const long DEFAULT_NUM_SUBSCRIBERS = 1000000;

			const int SUB_NBR_PADDING_SIZE = 15;

			/**
			* STORED PROCEDURE EXECUTION FREQUENCIES (0 - 100)
			*/
			const int FREQUENCY_DELETE_CALL_FORWARDING = 2; //Multi
			const int FREQUENCY_GET_ACCESS_DATA = 35;		//Single
			const int FREQUENCY_GET_NEW_DESTINATION = 10;	//Single
			const int FREQUENCY_GET_SUBSCRIBER_DATA = 35;	//Single
			const int FREQUENCY_INSERT_CALL_FORWARDING = 2; //Multi
			const int FREQUENCY_UPDATE_LOCATION = 14;		//Multi
			const int FREQUENCY_UPDATE_SUBSCRIBER_DATA = 2;	//Single
		}
	}
}

#endif
