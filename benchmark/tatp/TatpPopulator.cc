#include "TatpPopulator.h"

using namespace IMDB::Benchmark::TATP;
using namespace IMDB::Benchmark;

void TatpPopulator::init() {
	size_t subscriber_column_ids[] = {1};
	IndexInfo* subscriber_index = new IndexInfo(IndexType::HASHMAP, subscriber_column_ids, 1);
	storage_manager->registerTable(SUBSCRIBER_TABLE_ID, TatpSchema::getSubscriberSchema(), subscriber_index, 1);
	delete subscriber_index;
	if (kPartition) {
		for (size_t i = 0; i < kPartitionCnt; ++i) {
			storage_manager->registerTable(ACCESS_INFO_TABLE_ID + i, TatpSchema::getAccessInfoSchema());
			size_t special_facility_column_ids[] = {0};
			IndexInfo* special_facility_index = new IndexInfo(IndexType::HASHMAP, special_facility_column_ids, 1);
			storage_manager->registerTable(SPECIAL_FACILITY_TABLE_ID + i, TatpSchema::getSpecialFacilitySchema(), special_facility_index, 1);
			delete special_facility_index;
			storage_manager->registerTable(CALL_FORWARDING_TABLE_ID + i, TatpSchema::getCallForwardingSchema());
		}
	} else {
		storage_manager->registerTable(ACCESS_INFO_TABLE_ID, TatpSchema::getAccessInfoSchema());
		size_t special_facility_column_ids[] = {0};
		IndexInfo* special_facility_index = new IndexInfo(IndexType::HASHMAP, special_facility_column_ids, 1);
		storage_manager->registerTable(SPECIAL_FACILITY_TABLE_ID, TatpSchema::getSpecialFacilitySchema(), special_facility_index, 1);
		delete special_facility_index;
		storage_manager->registerTable(CALL_FORWARDING_TABLE_ID, TatpSchema::getCallForwardingSchema());
	}
}

void TatpPopulator::startPopulate() {
	//Loading Subscriber
	for (int s_id = 1; s_id <= scale_params->subscriber_size; ++s_id) {
		SubscriberRecord* subscriber_record = generateSubscriberRecord(s_id);
		insertSubscriberRecord(subscriber_record);
		delete subscriber_record;
	}
	
	//Loading AccessInfo
	int arr[4] = {1, 2, 3, 4};
	int ai_types[4];
	int ai_types_len;
	TatpUtil::subArr(arr, 4, 1, 4, ai_types, ai_types_len);
	for (int s_id = 1; s_id <= scale_params->subscriber_size; ++s_id) {
		for (int j = 0; j < ai_types_len; ++j) {
			AccessInfoRecord* access_info_record = generateAccessInfoRecord(s_id, ai_types[j]);
			insertAccessInfoRecord(access_info_record);
			delete access_info_record;
		}
	}

	//Loading SpecialFacility & CallForwarding
	int spe_arr[4] = {1, 2, 3, 4};
	int cal_arr[3] = {0, 8, 6};
	for (int s_id  = 1; s_id <= scale_params->subscriber_size; ++s_id) {
		int sf_types[4];
		int sf_types_len;
		TatpUtil::subArr(spe_arr, 4, 1, 4, sf_types, sf_types_len);
		for (int j = 0; j < sf_types_len; ++j) {
			SpecialFacilityRecord* special_facility_record = generateSpecialFacilityRecord(s_id, sf_types[j]);
			insertSpecialFacilityRecord(special_facility_record);
			delete special_facility_record;

			int start_times[3];
			int start_times_length;
			TatpUtil::subArr(cal_arr, 3, 0, 3, start_times, start_times_length);
			for (int k = 0; k < start_times_length; ++k) {
				CallForwardingRecord* call_forwarding_record = generateCallForwardingRecord(s_id, sf_types[j], start_times[k]);
				insertCallForwardingRecord(call_forwarding_record);
				delete call_forwarding_record;
			}
		}
	}
}

SubscriberRecord* TatpPopulator::generateSubscriberRecord(const int& s_id) {
	SubscriberRecord* record = new SubscriberRecord();
	record->s_id = s_id;
	std::string tmp = TatpUtil::padWithZero(s_id);
	memcpy(record->sub_nbr, tmp.c_str(), tmp.size());
	for (int i = 0; i < 10; ++i)
		record->bit[i] = (int8_t)(RandomGenerator::generateInteger(0, 1));
	for (int i = 0; i < 10; ++i)
		record->hex[i] = (int8_t)(RandomGenerator::generateInteger(0, 15));
	for (int i = 0; i < 10; ++i)
		record->byte2[i] = (int16_t)(RandomGenerator::generateInteger(0, 255));
	record->msc_location = RandomGenerator::generateInteger(0, INT_MAX);
	record->vlr_location = RandomGenerator::generateInteger(0, INT_MAX);
	return record;
}

AccessInfoRecord* TatpPopulator::generateAccessInfoRecord(const int& s_id, const int& ai_type) {
	AccessInfoRecord* record = new AccessInfoRecord();
	record->s_id = s_id;
	record->ai_type = (int8_t)ai_type;
	record->data1 = (int16_t)(RandomGenerator::generateInteger(0, 255));
	record->data2 = (int16_t)(RandomGenerator::generateInteger(0, 255));
	std::string tmp1 = RandomGenerator::generateCString(3, 3);
	memcpy(record->data3, tmp1.c_str(), tmp1.size());
	std::string tmp2 = RandomGenerator::generateCString(5, 5);
	memcpy(record->data4, tmp2.c_str(), tmp2.size());
	return record;
}

SpecialFacilityRecord* TatpPopulator::generateSpecialFacilityRecord(const int& s_id, const int& sf_type) {
	SpecialFacilityRecord* record = new SpecialFacilityRecord();
	record->s_id = s_id;
	record->sf_type = (int8_t)sf_type;
	record->is_active = (int8_t)TatpUtil::isActive();
	record->error_cntrl = (int16_t)(RandomGenerator::generateInteger(0, 255));
	record->data_a = (int16_t)(RandomGenerator::generateInteger(0, 255));
	std::string tmp = RandomGenerator::generateCString(5, 5);
	memcpy(record->data_b, tmp.c_str(), tmp.size());
	return record;
}

CallForwardingRecord* TatpPopulator::generateCallForwardingRecord(const int& s_id, const int& sf_type, const int& start_time) {
	CallForwardingRecord* record = new CallForwardingRecord();
	record->s_id = s_id;
	record->sf_type = (int8_t)sf_type;
	record->start_time = (int8_t)start_time;
	record->end_time = (int8_t)(start_time + RandomGenerator::generateInteger(1, 8));
	std::string tmp = RandomGenerator::generateNString(15, 15);
	memcpy(record->numberx, tmp.c_str(), tmp.size());
	return record;
}

void TatpPopulator::insertSubscriberRecord(const SubscriberRecord* record) {
	Record subscriber_record(TatpSchema::getSubscriberSchema());
	char* data = new char[TatpSchema::getSubscriberSchema()->getSchemaSize()];
	subscriber_record.transferData(data);
	subscriber_record.setColumn(0, reinterpret_cast<const char*>(&(record->s_id)));
	subscriber_record.setColumn(1, record->sub_nbr, 15);
	for (int i = 0; i < 10; ++i)
		subscriber_record.setColumn(i + 2, reinterpret_cast<const char*>(&(record->bit[i])));
	for (int i = 0; i < 10; ++i)
		subscriber_record.setColumn(i + 12, reinterpret_cast<const char*>(&(record->hex[i])));
	for (int i = 0; i < 10; ++i)
		subscriber_record.setColumn(i + 22, reinterpret_cast<const char*>(&(record->byte2[i])));
	subscriber_record.setColumn(32, reinterpret_cast<const char*>(&(record->msc_location)));
	subscriber_record.setColumn(33, reinterpret_cast<const char*>(&(record->vlr_location)));
	storage_manager->insertRecord(SUBSCRIBER_TABLE_ID, subscriber_record);
	delete[] data;
}

void TatpPopulator::insertAccessInfoRecord(const AccessInfoRecord* record) {
	Record access_info_record(TatpSchema::getAccessInfoSchema());
	char* data = new char[TatpSchema::getAccessInfoSchema()->getSchemaSize()];
	access_info_record.transferData(data);
	access_info_record.setColumn(0, reinterpret_cast<const char*>(&(record->s_id)));
	access_info_record.setColumn(1, reinterpret_cast<const char*>(&(record->ai_type)));
	access_info_record.setColumn(2, reinterpret_cast<const char*>(&(record->data1)));
	access_info_record.setColumn(3, reinterpret_cast<const char*>(&(record->data2)));
	access_info_record.setColumn(4, record->data3, 3);
	access_info_record.setColumn(5, record->data4, 5);
	size_t offset = kPartition ? record->s_id % kPartitionCnt : 0;
	storage_manager->insertRecord(ACCESS_INFO_TABLE_ID + offset, access_info_record);
	delete[] data;
}

void TatpPopulator::insertSpecialFacilityRecord(const SpecialFacilityRecord* record) {
	Record special_facility_record(TatpSchema::getSpecialFacilitySchema());
	char* data = new char[TatpSchema::getSpecialFacilitySchema()->getSchemaSize()];
	special_facility_record.transferData(data);
	special_facility_record.setColumn(0, reinterpret_cast<const char*>(&(record->s_id)));
	special_facility_record.setColumn(1, reinterpret_cast<const char*>(&(record->sf_type)));
	special_facility_record.setColumn(2, reinterpret_cast<const char*>(&(record->is_active)));
	special_facility_record.setColumn(3, reinterpret_cast<const char*>(&(record->error_cntrl)));
	special_facility_record.setColumn(4, reinterpret_cast<const char*>(&(record->data_a)));
	special_facility_record.setColumn(5, record->data_b, 5);
	size_t offset = kPartition ? record->s_id % kPartitionCnt : 0;
	storage_manager->insertRecord(SPECIAL_FACILITY_TABLE_ID + offset, special_facility_record);
	delete[] data;
}

void TatpPopulator::insertCallForwardingRecord(const CallForwardingRecord* record) {
	Record call_forwarding_record(TatpSchema::getCallForwardingSchema());
	char* data = new char[TatpSchema::getCallForwardingSchema()->getSchemaSize()];
	call_forwarding_record.transferData(data);
	call_forwarding_record.setColumn(0, reinterpret_cast<const char*>(&(record->s_id)));
	call_forwarding_record.setColumn(1, reinterpret_cast<const char*>(&(record->sf_type)));
	call_forwarding_record.setColumn(2, reinterpret_cast<const char*>(&(record->start_time)));
	call_forwarding_record.setColumn(3, reinterpret_cast<const char*>(&(record->end_time)));
	call_forwarding_record.setColumn(4, record->numberx, 15);
	size_t offset = kPartition ? record->s_id % kPartitionCnt : 0;
	storage_manager->insertRecord(CALL_FORWARDING_TABLE_ID + offset, call_forwarding_record);
	delete[] data;
}