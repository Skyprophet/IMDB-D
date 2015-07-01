#ifndef __TATP_BASE_PROCESSOR_H__
#define __TATP_BASE_PROCESSOR_H__

#include <boost/lockfree/spsc_queue.hpp>
#include "../../common/Timer.h"
#include "../../storage/StorageManager.h"
#include "ScaleParams.h"
#include "TatpSchema.h"
#include "TatpTuples.h"
#include "TatpConstants.h"

namespace IMDB{
	namespace Benchmark {
		namespace TATP {
			typedef boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> lockfree_queue_t;

			class TatpBaseProcessor {
			public:
				TatpBaseProcessor(lockfree_queue_t* const queue_ptr, StorageManager* const sm, const ScaleParams* params)
				: queue_ptr(queue_ptr), storage_manager(sm), scale_params(params) {}
				virtual ~TatpBaseProcessor() {}
				
				virtual void start() = 0;
			protected:
				int getSubscriberId(const std::string& sub_nbr);
				void deleteCallForwardingRecord(const int& s_id, const int8_t& sf_type, const int8_t& start_time);
				void getAccessInfo(const int& s_id, const int8_t& ai_type);
				void getNewDestination(const int& s_id, const int8_t& sf_type, const int8_t& start_time, const int8_t& end_time);
				void getSubscriberData(const int& s_id);
				void getSpecialFacilitySFType(const int& s_id);
				void insertCallForwardingRecord(const int& s_id, const int8_t& sf_type, const int8_t& start_time, const int8_t& end_time, const std::string numberx);
				void updateSubscriberVLRLocation(const int& s_id, const int& vlr_location);
				void updateSubscriberBit1(const int& s_id, const int8_t& bit_1);
				void updateSpecialFacilityDataA(const int& s_id, const int8_t& sf_type, const int16_t& data_a);

				TatpBaseProcessor(const TatpBaseProcessor&);
				TatpBaseProcessor& operator=(const TatpBaseProcessor&);

				lockfree_queue_t* const queue_ptr;
				StorageManager* const storage_manager;
				const ScaleParams* const scale_params;
			};
		}
	}
}

#endif