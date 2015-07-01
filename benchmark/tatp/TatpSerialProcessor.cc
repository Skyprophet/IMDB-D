#include "TatpSerialProcessor.h"

using namespace IMDB::Benchmark::TATP;
using namespace IMDB::StorageEngine;

void TatpSerialProcessor::start() {
	Timer timer;
	timer.start();

	std::pair<size_t, EventTuple*> tuple;
	size_t txn_cnt = 0;
	while (true) {
		while (!queue_ptr->pop(tuple)) {}
		if (tuple.first == TupleType::DELETE_CALL_FORWARDING) {
			processDeleteCallForwarding(static_cast<DeleteCallForwardingParam*>(tuple.second));
		} else if (tuple.first == TupleType::GET_ACCESS_DATA) {
			processGetAccessData(static_cast<GetAccessDataParam*>(tuple.second));
		} else if (tuple.first == TupleType::GET_NEW_DESTINATION) {
			processGetNewDestination(static_cast<GetNewDestinationParam*>(tuple.second));
		} else if (tuple.first == TupleType::GET_SUBSCRIBER_DATA) {
			processGetSubscriberData(static_cast<GetSubscriberDataParam*>(tuple.second));
		} else if (tuple.first == TupleType::INSERT_CALL_FORWARDING) {
			processInsertCallForwarding(static_cast<InsertCallForwardingParam*>(tuple.second));
		} else if (tuple.first == TupleType::UPDATE_LOCATION) {
			processUpdateLocation(static_cast<UpdateLocationParam*>(tuple.second));
		} else if (tuple.first == TupleType::UPDATE_SUBSCRIBER_DATA) {
			processUpdateSubscriberData(static_cast<UpdateSubscriberDataParam*>(tuple.second));
		} else if (tuple.first == StorageEngine::ENDEXECUTION) {
			break;
		}
		++txn_cnt;
		delete tuple.second;
	}

	timer.stop();
	long long elapsed_milliseconds = timer.getElapsedMilliSeconds();
	printf("Time Elapsed MilliSeconds: %lld ms\n", elapsed_milliseconds);
	printf("Throughput: %.3f txn/s\n", (double)txn_cnt * 1000 / elapsed_milliseconds);
}

void TatpSerialProcessor::processDeleteCallForwarding(const DeleteCallForwardingParam* param) {
	int s_id = getSubscriberId(param->sub_nbr);
	deleteCallForwardingRecord(s_id, param->sf_type, param->start_time);
}

void TatpSerialProcessor::processGetAccessData(const GetAccessDataParam* param) {
	getAccessInfo(param->s_id, param->ai_type);
}

void TatpSerialProcessor::processGetNewDestination(const GetNewDestinationParam* param) {
	getNewDestination(param->s_id, param->sf_type, param->start_time, param->end_time);
}

void TatpSerialProcessor::processGetSubscriberData(const GetSubscriberDataParam* param) {
	getSubscriberData(param->s_id);
}

void TatpSerialProcessor::processInsertCallForwarding(const InsertCallForwardingParam* param) {
	int s_id = getSubscriberId(param->sub_nbr);
	getSpecialFacilitySFType(s_id);
	insertCallForwardingRecord(s_id, param->sf_type, param->start_time, param->end_time, param->numberx);
}

void TatpSerialProcessor::processUpdateLocation(const UpdateLocationParam* param) {
	int s_id = getSubscriberId(param->sub_nbr);
	updateSubscriberVLRLocation(s_id, param->vlr_location);
}

void TatpSerialProcessor::processUpdateSubscriberData(const UpdateSubscriberDataParam* param) {
	updateSubscriberBit1(param->s_id, param->bit_1);
	updateSpecialFacilityDataA(param->s_id, param->sf_type, param->data_a);
}