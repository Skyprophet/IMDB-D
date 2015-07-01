#ifndef __TATP_POPULATOR_H__
#define __TATP_POPULATOR_H__

#include "../../storage/StorageManager.h"
#include "TatpRecords.h"
#include "ScaleParams.h"
#include "TatpSchema.h"
#include "TatpUtil.h"
#include "TatpConstants.h"
#include "../distributions/RandomGenerator.h"

namespace IMDB{
	namespace Benchmark {
		namespace TATP {
			using namespace IMDB::StorageEngine;

			class TatpPopulator {
			public:
				TatpPopulator(const ScaleParams* params, StorageManager* sm)
				: scale_params(params), storage_manager(sm) {}
				~TatpPopulator() {}

				void start() {
					init();
					startPopulate();
				}

			private:
				void init();
				void startPopulate();

				SubscriberRecord* generateSubscriberRecord(const int& s_id);
				AccessInfoRecord* generateAccessInfoRecord(const int& s_id, const int& sf_type);
				SpecialFacilityRecord* generateSpecialFacilityRecord(const int& s_id, const int& sf_type);
				CallForwardingRecord* generateCallForwardingRecord(const int& s_id, const int& sf_type, const int& start_time);

				void insertSubscriberRecord(const SubscriberRecord*);
				void insertAccessInfoRecord(const AccessInfoRecord*);
				void insertSpecialFacilityRecord(const SpecialFacilityRecord*);
				void insertCallForwardingRecord(const CallForwardingRecord*);

				TatpPopulator(const TatpPopulator&);
				TatpPopulator& operator=(const TatpPopulator&);

				const ScaleParams* const scale_params;
				StorageManager* const storage_manager;
			};
		}
	}
}

#endif