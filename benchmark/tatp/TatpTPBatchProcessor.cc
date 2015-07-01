#include "TatpTPBatchProcessor.h"

using namespace IMDB::StorageEngine;
using namespace IMDB::Benchmark::TATP;

void TatpTPBatchProcessor::start() {
	printf("BatchProcessing...\n");
	Timer timer;
	timer.start();
	bool stopflag = false;
	size_t txn_cnt = 0;
	while (true) {
		std::pair<size_t, EventTuple*> tuple;
		Timer batch_timer;
		batch_timer.start();
		size_t batch_cnt = 0;
		while (true) {
			while (!queue_ptr->pop(tuple)) {}
			batch_params.emplace_back(tuple);
			if (tuple.first == TupleType::DELETE_CALL_FORWARDING) {
				dcf_params.emplace_back(static_cast<DeleteCallForwardingParam*>(tuple.second));
			} else if (tuple.first == TupleType::GET_ACCESS_DATA) {
				gad_params.emplace_back(static_cast<GetAccessDataParam*>(tuple.second));
			} else if (tuple.first == TupleType::GET_NEW_DESTINATION) {
				gnd_params.emplace_back(static_cast<GetNewDestinationParam*>(tuple.second));
			} else if (tuple.first == TupleType::GET_SUBSCRIBER_DATA) {
				gsd_params.emplace_back(static_cast<GetSubscriberDataParam*>(tuple.second));
			} else if (tuple.first == TupleType::INSERT_CALL_FORWARDING) {
				icf_params.emplace_back(static_cast<InsertCallForwardingParam*>(tuple.second));
			} else if (tuple.first == TupleType::UPDATE_LOCATION) {
				ul_params.emplace_back(static_cast<UpdateLocationParam*>(tuple.second));
			} else if (tuple.first == TupleType::UPDATE_SUBSCRIBER_DATA) {
				usd_params.emplace_back(static_cast<UpdateSubscriberDataParam*>(tuple.second));
			} else if (tuple.first == StorageEngine::ENDEXECUTION) {
				stopflag = true;
				break;
			}
			++txn_cnt; ++batch_cnt;
			batch_timer.stop();
			if (batch_timer.getElapsedMilliSeconds() >= kBatchTime) break;
		}
		
		processBatch();
		clearBatch();
		batch_timer.stop();
		long long elapsed_milliseconds = batch_timer.getElapsedMilliSeconds();
		printf("Batch Size: %ld\n", batch_cnt);
		printf("Time Elapsed MilliSeconds: %lld ms\n", elapsed_milliseconds);
		printf("Throughput: %.3f txn/s\n", (double)batch_cnt * 1000 / elapsed_milliseconds);
		if (stopflag) break;
	}

	timer.stop();
	long long elapsed_milliseconds = timer.getElapsedMilliSeconds();
	printf("Time Elapsed MilliSeconds: %lld ms\n", elapsed_milliseconds);
	printf("Throughput: %.3f txn/s\n", (double)txn_cnt * 1000 / elapsed_milliseconds);
}

void TatpTPBatchProcessor::clearBatch() {
	for (auto entry : batch_params)
		delete entry.second;
	dcf_params.clear();
	gad_params.clear();
	gnd_params.clear();
	gsd_params.clear();
	icf_params.clear();
	ul_params.clear();
	usd_params.clear();
	batch_params.clear();
}

uint32_t TatpTPBatchProcessor::hash_int(uint32_t a) {
	a = (a+0x7ed55d16) + (a<<12);
	a = (a^0xc761c23c) ^ (a>>19);
	a = (a+0x165667b1) + (a<<5);
	a = (a+0xd3a2646c) ^ (a<<9);
	a = (a+0xfd7046c5) + (a<<3);
	a = (a^0xb55a4f09) ^ (a>>16); 
	return a;
}

void TatpTPBatchProcessor::processBatch() {
	std::vector<int> dcf_s_ids, icf_s_ids, ul_s_ids;
	dcf_s_ids.resize(dcf_params.size());
	icf_s_ids.resize(icf_params.size());
	ul_s_ids.resize(ul_params.size());
	std::vector<std::future<void>> dcf_sid_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		dcf_sid_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = t; i < dcf_params.size(); i += kParallelism) {
				dcf_s_ids.at(i) = getSubscriberId(dcf_params.at(i)->sub_nbr);
			}
			timer.stop();
			//printf("dcf_sid_thds %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	std::vector<std::future<void>> icf_sid_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		icf_sid_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = t; i < icf_params.size(); i += kParallelism) {
				icf_s_ids.at(i) = getSubscriberId(icf_params.at(i)->sub_nbr);
			}
			timer.stop();
			//printf("icf_sid_thds %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	std::vector<std::future<void>> ul_sid_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		ul_sid_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = t; i < ul_params.size(); i += kParallelism) {
				ul_s_ids.at(i) = getSubscriberId(ul_params.at(i)->sub_nbr);
			}
			timer.stop();
			//printf("ul_sid_thds %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	std::vector<std::future<void>> gad_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		gad_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = t; i < gad_params.size(); i += kParallelism) {
				GetAccessDataParam* param = gad_params.at(i);
				getAccessInfo(param->s_id, param->ai_type);
			}
			timer.stop();
			//printf("gad_thds %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}
	
	for (auto && entry : dcf_sid_thds)
		entry.wait();
	for (auto && entry : icf_sid_thds)
		entry.wait();
	for (auto && entry : ul_sid_thds)
		entry.wait();

	//need to resolve SPECIAL FACILITY &&  CALL FORWARDING && SUBSCRIBER
	std::vector<std::pair<uint8_t, size_t>> dcf_icf_bkt[kParallelism];
	std::vector<std::pair<uint8_t, size_t>> icf_usd_bkt[kParallelism];
	std::vector<std::pair<uint8_t, size_t>> gsd_ul_usd_bkt[kParallelism];
	
	size_t dcf_pos, gsd_pos, icf_pos, ul_pos, usd_pos;
	dcf_pos = gsd_pos = icf_pos = ul_pos = usd_pos = 0;
	for (size_t i = 0; i < batch_params.size(); ++i) {
		auto && tuple = batch_params.at(i);
		if (tuple.first == TupleType::DELETE_CALL_FORWARDING) {
			dcf_icf_bkt[hash_int(dcf_s_ids.at(dcf_pos)) % kParallelism].emplace_back(std::make_pair(0, dcf_pos));
			++dcf_pos;
		} else if (tuple.first == TupleType::GET_SUBSCRIBER_DATA) {
			GetSubscriberDataParam* param = static_cast<GetSubscriberDataParam*>(tuple.second);
			gsd_ul_usd_bkt[hash_int(param->s_id) % kParallelism].emplace_back(std::make_pair(0, gsd_pos));
			++gsd_pos;
		} else if (tuple.first == TupleType::INSERT_CALL_FORWARDING) {
			dcf_icf_bkt[hash_int(icf_s_ids.at(icf_pos)) % kParallelism].emplace_back(std::make_pair(1, icf_pos));
			icf_usd_bkt[hash_int(icf_s_ids.at(icf_pos)) % kParallelism].emplace_back(std::make_pair(0, icf_pos));
			++icf_pos;
		} else if (tuple.first == TupleType::UPDATE_LOCATION) {
			gsd_ul_usd_bkt[hash_int(ul_s_ids.at(ul_pos)) % kParallelism].emplace_back(std::make_pair(1, ul_pos));
			++ul_pos;
		} else if (tuple.first == TupleType::UPDATE_SUBSCRIBER_DATA) {
			UpdateSubscriberDataParam* param = static_cast<UpdateSubscriberDataParam*>(tuple.second);
			gsd_ul_usd_bkt[hash_int(param->s_id) % kParallelism].emplace_back(std::make_pair(2, usd_pos));
			icf_usd_bkt[hash_int(param->s_id) % kParallelism].emplace_back(std::make_pair(1, usd_pos));
			++usd_pos;
		}
	}

	std::vector<std::future<void>> dcf_icf_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		dcf_icf_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < dcf_icf_bkt[t].size(); ++i) {
				auto entry = dcf_icf_bkt[t].at(i);
				if (entry.first == 0) {
					DeleteCallForwardingParam* param = dcf_params.at(entry.second);
					deleteCallForwardingRecord(dcf_s_ids.at(entry.second), param->sf_type, param->start_time);
				} else {
					InsertCallForwardingParam* param = icf_params.at(entry.second);
					insertCallForwardingRecord(icf_s_ids.at(entry.second), param->sf_type, param->start_time, param->end_time, param->numberx);
				}
			}
			timer.stop();
			//printf("dcf_icf_thds %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	std::vector<std::future<void>> gsd_ul_usd_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		gsd_ul_usd_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < gsd_ul_usd_bkt[t].size(); ++i) {
				auto entry = gsd_ul_usd_bkt[t].at(i);
				if (entry.first == 0) {
					GetSubscriberDataParam* param = gsd_params.at(entry.second);
					getSubscriberData(param->s_id);
				} else if (entry.first == 1) {
					UpdateLocationParam* param = ul_params.at(entry.second);
					updateSubscriberVLRLocation(ul_s_ids.at(entry.second), param->vlr_location);
				} else {
					UpdateSubscriberDataParam* param = usd_params.at(entry.second);
					updateSubscriberBit1(param->s_id, param->bit_1);
				}
			}
			timer.stop();
			//printf("gsd_ul_usd_thds %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	std::vector<std::future<void>> icf_usd_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		icf_usd_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < icf_usd_bkt[t].size(); ++i) {
				auto entry = icf_usd_bkt[t].at(i);
				if (entry.first == 0) {
					getSpecialFacilitySFType(icf_s_ids.at(entry.second));
				} else {
					UpdateSubscriberDataParam* param = usd_params.at(entry.second);
					updateSpecialFacilityDataA(param->s_id, param->sf_type, param->data_a);
				}
			}
			timer.stop();
			//printf("icf_usd_thds %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	for (auto && entry : dcf_icf_thds)
		entry.wait();

	for (auto && entry : gsd_ul_usd_thds)
		entry.wait();

	for (auto && entry : icf_usd_thds)
		entry.wait();

	for (auto && entry : gad_thds)
		entry.wait();
}
