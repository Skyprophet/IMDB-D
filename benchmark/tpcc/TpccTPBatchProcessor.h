#ifndef __TPCC_TPBATCH_PROCESSOR_H__
#define __TPCC_TPBATCH_PROCESSOR_H__

#include "TpccBaseProcessor.h"
#include "TpccTuples.h"
#include "ScaleParams.h"
#include "TpccConstants.h"
#include <thread>
#include "../../common/ThreadPool.h"

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			typedef boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> lockfree_queue_t;
			using namespace IMDB::StorageEngine;

			class TpccTPBatchProcessor : public TpccBaseProcessor {
			public:
				TpccTPBatchProcessor(lockfree_queue_t* const queue_ptr, StorageManager* const sm, const ScaleParams* params)
				: TpccBaseProcessor(queue_ptr, sm, params), thread_pool(kParallelism) {
					delivery_params.clear();
					payment_params.clear();
					new_order_params.clear();
					batch_params.clear();
					//thread_pool.start();

				}
				virtual ~TpccTPBatchProcessor() {}

				virtual void start();
			
			private:
				void clearBatch();
				uint32_t hash_int(uint32_t);
				void processBatch();

				TpccTPBatchProcessor(const TpccTPBatchProcessor&);
				TpccTPBatchProcessor& operator=(const TpccTPBatchProcessor&);
			
			private:
				std::vector<DeliveryParam*> delivery_params;
				std::vector<PaymentParam*> payment_params;
				std::vector<NewOrderParam*> new_order_params;
				std::vector<std::pair<size_t, EventTuple*>> batch_params;
				ThreadPool thread_pool;
			};
		}
	}
}

#endif
