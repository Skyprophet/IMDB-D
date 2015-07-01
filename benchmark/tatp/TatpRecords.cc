#include "TatpRecords.h"

namespace IMDB {
	namespace Benchmark {
		namespace TATP {
			std::string subscriberToString(const SubscriberRecord& record) {
				std::stringstream buf;
				buf << record.s_id << ";" << record.sub_nbr << ";";
				for (int i = 0; i < 10; ++i)
					buf << record.bit[i] << ";";
				for (int i = 0; i < 10; ++i)
					buf << record.hex[i] << ";";
				for (int i = 0; i < 10; ++i)
					buf << record.byte2[i] << ";";
				buf << record.msc_location << ";" << record.vlr_location;
				return buf.str();
			}

			std::string accessInfoToString(const AccessInfoRecord& record) {
				std::stringstream buf;
				buf << record.s_id << ";" << record.ai_type << ";";
				buf << record.data1 << ";" << record.data2 << ";";
				buf << record.data3 << ";" << record.data4;
				return buf.str();
			}

			std::string specialFacilityToString(const SpecialFacilityRecord& record) {
				std::stringstream buf;
				buf << record.s_id << ";" << record.sf_type << ";";
				buf << record.is_active << ";" << record.error_cntrl << ";";
				buf << record.data_a << ";" << record.data_b;
				return buf.str();
			}

			std::string callForwardToString(const CallForwardingRecord& record) {
				std::stringstream buf;
				buf << record.s_id << ";" << record.sf_type << ";";
				buf << record.start_time << ";" << record.end_time << ";";
				buf << record.numberx;
				return buf.str();
			}
		}
	}
}