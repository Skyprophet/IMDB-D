#ifndef __TPCC_BATCH_PROCESSOR_H__
#define __TPCC_BATCH_PROCESSOR_H__

#include "TpccBaseProcessor.h"
#include "TpccTuples.h"
#include "ScaleParams.h"
#include "TpccConstants.h"
#include <thread>

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			typedef boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> lockfree_queue_t;
			using namespace IMDB::StorageEngine;

			class TpccBatchProcessor : public TpccBaseProcessor {
			public:
				TpccBatchProcessor(lockfree_queue_t* const queue_ptr, StorageManager* const sm, const ScaleParams* params)
				: TpccBaseProcessor(queue_ptr, sm, params) {
					delivery_params.clear();
					payment_params.clear();
					new_order_params.clear();

				}
				virtual ~TpccBatchProcessor() {}

				virtual void start();
			
			private:
				void clearBatch();
				uint32_t hash_int(uint32_t);
				void processBatch();

				TpccBatchProcessor(const TpccBatchProcessor&);
				TpccBatchProcessor& operator=(const TpccBatchProcessor&);
			
			private:
				std::vector<DeliveryParam*> delivery_params;
				std::vector<PaymentParam*> payment_params;
				std::vector<NewOrderParam*> new_order_params;
				//std::vector<size_t> new_order_params_bkt[kParallelism];
				//std::vector<std::pair<size_t, size_t>> new_order_params_i_id_bkt[kParallelism];
				std::vector<std::pair<size_t, EventTuple*>> batch_params;
				//std::vector<std::vector<std::string>> s_dists_vector;
				//std::vector<std::vector<double>> ol_amounts_vector;
			};
		}
	}
}

#endif