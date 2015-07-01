#ifndef __TPCC_SCHEDULE_PROCESSOR_H__
#define __TPCC_SCHEDULE_PROCESSOR_H__

#include "TpccBaseProcessor.h"
#include "ScaleParams.h"

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			typedef boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> lockfree_queue_t;
			using namespace IMDB::StorageEngine;

			class TpccSerialProcessor : public TpccBaseProcessor {
			public:
				TpccSerialProcessor(lockfree_queue_t* const queue_ptr, StorageManager* const sm, const ScaleParams* params)
					: TpccBaseProcessor(queue_ptr, sm, params) {}
				virtual ~TpccSerialProcessor() {}

				virtual void start();
			protected:
				void processDelivery(const DeliveryParam* param);
				void processNewOrder(const NewOrderParam* param);
				void processPayment(const PaymentParam* param);
			private:
				TpccSerialProcessor(const TpccSerialProcessor&);
				TpccSerialProcessor& operator=(const TpccSerialProcessor&);
			};
		}
	}
}

#endif