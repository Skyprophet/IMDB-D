#ifndef __TATP_RECORDS_H__
#define __TATP_RECORDS_H__

#include <cstdint>
#include <string>
#include <sstream>

namespace IMDB {
	namespace Benchmark {
		namespace TATP {
			struct SubscriberRecord {
				int32_t s_id;
				char sub_nbr[15];
				int8_t bit[10];
				int8_t hex[10];
				int16_t byte2[10];
				int32_t msc_location;
				int32_t vlr_location;
			};

			struct AccessInfoRecord {
				int32_t s_id;
				int8_t ai_type;
				int16_t data1;
				int16_t data2;
				char data3[3];
				char data4[5];
			};

			struct SpecialFacilityRecord {
				int32_t s_id;
				int8_t sf_type;
				int8_t is_active;
				int16_t error_cntrl;
				int16_t data_a;
				char data_b[5];
			};

			struct CallForwardingRecord {
				int32_t s_id;
				int8_t sf_type;
				int8_t start_time;
				int8_t end_time;
				char numberx[15];
			};
		}
	}
}

#endif