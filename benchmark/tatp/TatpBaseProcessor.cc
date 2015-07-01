#include "TatpBaseProcessor.h"

using namespace IMDB::Benchmark::TATP;
using namespace IMDB::StorageEngine;

int TatpBaseProcessor::getSubscriberId(const std::string& sub_nbr) {
	int ret = -1;
	ConstRecord subscriber_record = storage_manager->selectRecord(SUBSCRIBER_TABLE_ID, ColumnPredicate(1, sub_nbr.c_str()));
	if (subscriber_record.fetchData() != NULL)
		ret = *(reinterpret_cast<const int*>(subscriber_record.getColumn(0)));
	return ret;
}

void TatpBaseProcessor::deleteCallForwardingRecord(const int& s_id, const int8_t& sf_type, const int8_t& start_time) {
	size_t c_cols[] = {0, 1, 2};
	const char* c_values[] = {reinterpret_cast<const char*>(&s_id), reinterpret_cast<const char*>(&sf_type), reinterpret_cast<const char*>(&start_time)};
	size_t offset = kPartition ? s_id % kPartitionCnt : 0;
	ConstRecord call_forwarding_record = storage_manager->selectRecord(CALL_FORWARDING_TABLE_ID + offset, ColumnPredicates(c_cols, c_values, 3));
	if (call_forwarding_record.fetchData() != NULL)
		storage_manager->deleteRecord(CALL_FORWARDING_TABLE_ID + offset, call_forwarding_record);
}

void TatpBaseProcessor::getAccessInfo(const int& s_id, const int8_t& ai_type) {
	size_t a_cols[] = {0, 1};
	const char* a_values[] = {reinterpret_cast<const char*>(&s_id), reinterpret_cast<const char*>(&ai_type)};
	size_t offset = kPartition ? s_id % kPartitionCnt : 0;
	storage_manager->selectRecord(ACCESS_INFO_TABLE_ID + offset, ColumnPredicates(a_cols, a_values, 2));
}

void TatpBaseProcessor::getNewDestination(const int& s_id, const int8_t& sf_type, const int8_t& start_time, const int8_t& end_time) {
	// TODO Currently no join and scan
}

void TatpBaseProcessor::getSubscriberData(const int& s_id) {
	storage_manager->selectRecord(SUBSCRIBER_TABLE_ID, ColumnPredicate(0, reinterpret_cast<const char*>(&s_id)));
}

void TatpBaseProcessor::getSpecialFacilitySFType(const int& s_id) {
	size_t offset = kPartition ? s_id % kPartitionCnt : 0;
	storage_manager->selectRecord(SPECIAL_FACILITY_TABLE_ID + offset, ColumnPredicate(0, reinterpret_cast<const char*>(&s_id)));
}

void TatpBaseProcessor::insertCallForwardingRecord(const int& s_id, const int8_t& sf_type, const int8_t& start_time, const int8_t& end_time, const std::string numberx) {
	size_t c_cols[] = {0, 1, 2};
	const char* c_values[] = {reinterpret_cast<const char*>(&s_id), reinterpret_cast<const char*>(&sf_type), reinterpret_cast<const char*>(&start_time)};
	size_t offset = kPartition ? s_id % kPartitionCnt : 0;
	ConstRecord call_forwarding_record = storage_manager->selectRecord(CALL_FORWARDING_TABLE_ID + offset, ColumnPredicates(c_cols, c_values, 3));
	if (call_forwarding_record.fetchData() == NULL) {
		Record call_forwarding_record(TatpSchema::getCallForwardingSchema());
		char* data = new char[TatpSchema::getCallForwardingSchema()->getSchemaSize()];
		call_forwarding_record.transferData(data);
		call_forwarding_record.setColumn(0, reinterpret_cast<const char*>(&s_id));
		call_forwarding_record.setColumn(1, reinterpret_cast<const char*>(&sf_type));
		call_forwarding_record.setColumn(2, reinterpret_cast<const char*>(&start_time));
		call_forwarding_record.setColumn(3, reinterpret_cast<const char*>(&end_time));
		call_forwarding_record.setColumn(4, numberx.c_str(), 15);
		storage_manager->insertRecord(CALL_FORWARDING_TABLE_ID + offset, call_forwarding_record);
		delete[] data;
	}
}
void TatpBaseProcessor::updateSubscriberVLRLocation(const int& s_id, const int& vlr_location) {
	ConstRecord subscriber_record = storage_manager->selectRecord(SUBSCRIBER_TABLE_ID, ColumnPredicate(0, reinterpret_cast<const char*>(&s_id)));
	storage_manager->updateRecord(SUBSCRIBER_TABLE_ID, subscriber_record, ColumnPredicate(33, reinterpret_cast<const char*>(&vlr_location)));
}

void TatpBaseProcessor::updateSubscriberBit1(const int& s_id, const int8_t& bit_1) {
	ConstRecord subscriber_record = storage_manager->selectRecord(SUBSCRIBER_TABLE_ID, ColumnPredicate(0, reinterpret_cast<const char*>(&s_id)));
	if (subscriber_record.fetchData() != NULL)
		storage_manager->updateRecord(SUBSCRIBER_TABLE_ID, subscriber_record, ColumnPredicate(2, reinterpret_cast<const char*>(&bit_1)));
}

void TatpBaseProcessor::updateSpecialFacilityDataA(const int& s_id, const int8_t& sf_type, const int16_t& data_a) {
	size_t s_cols[] = {0, 1};
	const char* s_values[] = {reinterpret_cast<const char*>(&s_id), reinterpret_cast<const char*>(&sf_type)};
	size_t offset = kPartition ? s_id % kPartitionCnt : 0;
	ConstRecord special_facility_record = storage_manager->selectRecord(SPECIAL_FACILITY_TABLE_ID + offset, ColumnPredicates(s_cols, s_values, 2));
	if (special_facility_record.fetchData() != NULL)
		storage_manager->updateRecord(SPECIAL_FACILITY_TABLE_ID + offset, special_facility_record, ColumnPredicate(4, reinterpret_cast<const char*>(&data_a)));
}
