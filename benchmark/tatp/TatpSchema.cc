#include "TatpSchema.h"

using namespace IMDB::Benchmark::TATP;
using namespace IMDB::StorageEngine;

Schema* TatpSchema::subscriber_schema = NULL;
Schema* TatpSchema::access_info_schema = NULL;
Schema* TatpSchema::special_facility_schema = NULL;
Schema* TatpSchema::call_forwarding_schema = NULL;

Schema* TatpSchema::getSubscriberSchema() {
	if (subscriber_schema == NULL) {
		subscriber_schema = new Schema();
		subscriber_schema->insertColumn(ColumnInfo("s_id", ValueType::INT32, true));
		subscriber_schema->insertColumn(ColumnInfo("sub_nbr", ValueType::VARCHAR, static_cast<size_t>(15)));
		for (int i = 1; i <= 10; ++i) {
			subscriber_schema->insertColumn(ColumnInfo("bit_" + std::to_string(i), ValueType::INT8));
		}
		for (int i = 1; i <= 10; ++i) {
			subscriber_schema->insertColumn(ColumnInfo("hex_" + std::to_string(i), ValueType::INT8));
		}
		for (int i = 1; i <= 10; ++i) {
			subscriber_schema->insertColumn(ColumnInfo("byte2_" + std::to_string(i), ValueType::INT16));
		}
		subscriber_schema->insertColumn(ColumnInfo("msc_location", ValueType::INT32));
		subscriber_schema->insertColumn(ColumnInfo("vlr_location", ValueType::INT32));
	}
	return subscriber_schema;
}

Schema* TatpSchema::getAccessInfoSchema() {
	if (access_info_schema == NULL) {
		access_info_schema = new Schema();
		access_info_schema->insertColumn(ColumnInfo("s_id", ValueType::INT32, true));
		access_info_schema->insertColumn(ColumnInfo("ai_type", ValueType::INT8, true));
		access_info_schema->insertColumn(ColumnInfo("data1", ValueType::INT16));
		access_info_schema->insertColumn(ColumnInfo("data2", ValueType::INT16));
		access_info_schema->insertColumn(ColumnInfo("data3", ValueType::VARCHAR, static_cast<size_t>(3)));
		access_info_schema->insertColumn(ColumnInfo("data4", ValueType::VARCHAR, static_cast<size_t>(5)));
	}
	return access_info_schema;
}

Schema* TatpSchema::getSpecialFacilitySchema() {
	if (special_facility_schema == NULL) {
		special_facility_schema = new Schema();
		special_facility_schema->insertColumn(ColumnInfo("s_id", ValueType::INT32, true));
		special_facility_schema->insertColumn(ColumnInfo("sf_type", ValueType::INT8, true));
		special_facility_schema->insertColumn(ColumnInfo("is_active", ValueType::INT8));
		special_facility_schema->insertColumn(ColumnInfo("error_cntrl", ValueType::INT16));
		special_facility_schema->insertColumn(ColumnInfo("data_a", ValueType::INT16));
		special_facility_schema->insertColumn(ColumnInfo("data_b", ValueType::VARCHAR, static_cast<size_t>(5)));
	}
	return special_facility_schema;
}

Schema* TatpSchema::getCallForwardingSchema() {
	if (call_forwarding_schema == NULL) {
		call_forwarding_schema = new Schema();
		call_forwarding_schema->insertColumn(ColumnInfo("s_id", ValueType::INT32, true));
		call_forwarding_schema->insertColumn(ColumnInfo("sf_type", ValueType::INT8, true));
		call_forwarding_schema->insertColumn(ColumnInfo("start_time", ValueType::INT8, true));
		call_forwarding_schema->insertColumn(ColumnInfo("end_time", ValueType::INT8));
		call_forwarding_schema->insertColumn(ColumnInfo("numberx", ValueType::VARCHAR, static_cast<size_t>(15)));
	}
	return call_forwarding_schema;
}