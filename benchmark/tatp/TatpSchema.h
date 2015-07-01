#ifndef __TATP_SCHEMA_H__
#define __TATP_SCHEMA_H__

#include "../../storage/Schema.h"

namespace IMDB {
	namespace Benchmark {
		namespace TATP {
			using namespace IMDB::StorageEngine;
			class TatpSchema {
			public:
				static Schema* getSubscriberSchema();
				static Schema* getAccessInfoSchema();
				static Schema* getSpecialFacilitySchema();
				static Schema* getCallForwardingSchema();
			private:
				TatpSchema();
				TatpSchema(const TatpSchema&);
				TatpSchema& operator=(const TatpSchema&);

				static Schema* subscriber_schema;
				static Schema* access_info_schema;
				static Schema* special_facility_schema;
				static Schema* call_forwarding_schema;
			};
		}
	}
}

#endif