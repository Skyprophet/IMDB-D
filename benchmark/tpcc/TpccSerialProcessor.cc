#include "TpccSerialProcessor.h"
#include <cstdio>

using namespace IMDB::Benchmark::TPCC;
using namespace IMDB::StorageEngine;

void TpccSerialProcessor::start() {
	Timer timer;
	timer.start();

	std::pair<size_t, EventTuple*> tuple;
	size_t txn_cnt = 0;
	while (true) {
		while (!queue_ptr->pop(tuple)) {}
		if (tuple.first == TupleType::DELIVERY) {
			DeliveryParam* param = (DeliveryParam*) tuple.second;
			processDelivery(param);
		} else if (tuple.first == TupleType::NEW_ORDER) {
			NewOrderParam* param = (NewOrderParam*) tuple.second;
			processNewOrder(param);
		} else if (tuple.first == TupleType::PAYMENT) {
			PaymentParam* param = (PaymentParam*) tuple.second;
			processPayment(param);
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

void TpccSerialProcessor::processDelivery(const DeliveryParam* param) {
	std::unordered_map<int, int> no_o_ids;
	deliverySubTxn1(param, no_o_ids);
	std::unordered_map<int, double> sums;
	deliverySubTxn2(param, no_o_ids, sums);
	std::unordered_map<int, int> c_ids;
	deliverySubTxn3(param, no_o_ids, c_ids);
	deliverySubTxn4(param, no_o_ids, sums, c_ids);
}

void TpccSerialProcessor::processNewOrder(const NewOrderParam* param) {
	int d_next_o_id = neworderSubTxn1(param);
	neworderSubTxn2(param, d_next_o_id);
	neworderSubTxn3(param, d_next_o_id);
	std::vector<std::string> s_dists;
	neworderSubTxn4(param, s_dists);
	std::vector<double> ol_amounts;
	neworderSubTxn5(param, ol_amounts);
	neworderSubTxn6(param, d_next_o_id, s_dists, ol_amounts);
}

void TpccSerialProcessor::processPayment(const PaymentParam* param) {
	paymentSubTxn1(param);
	paymentSubTxn2(param);
	paymentSubTxn3(param);
	paymentSubTxn4(param);
}
