#ifndef __TATP_TP_BATCH_PROCESSOR_H__
#define __TATP_TP_BATCH_PROCESSOR_H__

#include "TatpBaseProcessor.h"
#include "TatpTuples.h"
#include "ScaleParams.h"
#include "TatpConstants.h"
#include <thread>
#include "../../common/ThreadPool.h"

namespace IMDB {
	namespace Benchmark {
		namespace TATP {
			typedef boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> lockfree_queue_t;
			using namespace IMDB::StorageEngine;

			class TatpTPBatchProcessor : public TatpBaseProcessor {
			public:
				TatpTPBatchProcessor(lockfree_queue_t* const queue_ptr, StorageManager* const sm, const ScaleParams* params)
				: TatpBaseProcessor(queue_ptr, sm, params), thread_pool(kParallelism) {
					dcf_params.clear();
					gad_params.clear();
					gnd_params.clear();
					gsd_params.clear();
					icf_params.clear();
					ul_params.clear();
					usd_params.clear();
					batch_params.clear();
				}

				virtual ~TatpTPBatchProcessor() {}

				virtual void start();
			private:
				void clearBatch();
				uint32_t hash_int(uint32_t);
				void processBatch();

				TatpTPBatchProcessor(const TatpTPBatchProcessor&);
				TatpTPBatchProcessor& operator=(const TatpTPBatchProcessor&);

				std::vector<DeleteCallForwardingParam*> dcf_params;
				std::vector<GetAccessDataParam*> gad_params;
				std::vector<GetNewDestinationParam*> gnd_params;
				std::vector<GetSubscriberDataParam*> gsd_params;
				std::vector<InsertCallForwardingParam*> icf_params;
				std::vector<UpdateLocationParam*> ul_params;
				std::vector<UpdateSubscriberDataParam*> usd_params;
				std::vector<std::pair<size_t, EventTuple*>> batch_params;
				ThreadPool thread_pool;
			};
		}
	}
}

#endif