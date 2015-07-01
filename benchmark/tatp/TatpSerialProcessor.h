#ifndef __TATP_SERIAL_PROCESSOR_H__
#define __TATP_SERIAL_PROCESSOR_H__

#include "TatpBaseProcessor.h"
#include "ScaleParams.h"

namespace IMDB {
	namespace Benchmark {
		namespace TATP {
			typedef boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> lockfree_queue_t;
			using namespace IMDB::StorageEngine;
			
			class TatpSerialProcessor : public TatpBaseProcessor {
			public:
				TatpSerialProcessor(lockfree_queue_t* const queue_ptr, StorageManager* const sm, const ScaleParams* params)
				: TatpBaseProcessor(queue_ptr, sm, params) {}
				virtual ~TatpSerialProcessor() {}

				virtual void start();
			protected:
				void processDeleteCallForwarding(const DeleteCallForwardingParam* param);
				void processGetAccessData(const GetAccessDataParam* param);
				void processGetNewDestination(const GetNewDestinationParam* param);
				void processGetSubscriberData(const GetSubscriberDataParam* param);
				void processInsertCallForwarding(const InsertCallForwardingParam* param);
				void processUpdateLocation(const UpdateLocationParam* param);
				void processUpdateSubscriberData(const UpdateSubscriberDataParam* param);

				TatpSerialProcessor(const TatpSerialProcessor&);
				TatpSerialProcessor& operator=(const TatpSerialProcessor&);
			};
		}
	}
}

#endif
