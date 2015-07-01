#ifndef __TPCC_SOURCE_H__
#define  __TPCC_SOURCE_H__

#include <boost/lockfree/spsc_queue.hpp>
#include "../../common/EventTuple.h"
#include "TpccConstants.h"
#include "RandomGenerator.h"
#include "ScaleParams.h"
#include "TpccTuples.h"
#include "../../storage/MetaTypes.h"

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			using namespace IMDB::StorageEngine;
			class TpccSource {
			public:
				TpccSource(boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>>* const queue_ptr, ScaleParams* params, size_t num_transactions)
					: queue_ptr(queue_ptr), scale_params(params), num_transactions(num_transactions) {}
				~TpccSource() {}

				void start() {
					startExecution();
					while (!queue_ptr->push(std::make_pair(ENDEXECUTION, static_cast<EventTuple*>(NULL))));
				}

			private:
				void startExecution();

				DeliveryParam* generateDeliveryParam() const;
				NewOrderParam* generateNewOrderParam() const;
				PaymentParam* generatePaymentParam() const;
				OrderStatusParam* generateOrderStatusParam() const;
				StockLevelParam* generateStockLevelParam() const;

				TpccSource(const TpccSource&);
				TpccSource& operator=(const TpccSource&);

				boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>>* const queue_ptr;
				const ScaleParams* const scale_params;
				const size_t num_transactions;
			};
		}
	}
}

#endif
