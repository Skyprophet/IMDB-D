#include "TpccTPBatchProcessor.h"

using namespace IMDB::StorageEngine;
using namespace IMDB::Benchmark::TPCC;

void TpccTPBatchProcessor::start() {
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
			if (tuple.first == TupleType::DELIVERY) {
				delivery_params.emplace_back(static_cast<DeliveryParam*>(tuple.second));
			} else if (tuple.first == TupleType::NEW_ORDER) {
				new_order_params.emplace_back(static_cast<NewOrderParam*>(tuple.second));
			} else if (tuple.first == TupleType::PAYMENT) {
				payment_params.emplace_back(static_cast<PaymentParam*>(tuple.second));
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

void TpccTPBatchProcessor::clearBatch() {
	for (auto entry : batch_params)
		delete entry.second;
	delivery_params.clear();
	new_order_params.clear();
	payment_params.clear();
	batch_params.clear();
}

uint32_t TpccTPBatchProcessor::hash_int(uint32_t key) {
	key = ~key + (key << 15); // key = (key << 15) - key - 1;
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2057; // key = (key + (key << 3)) + (key << 11);
	key = key ^ (key >> 16);
	return key; 
}

void TpccTPBatchProcessor::processBatch() {
	std::vector<size_t> new_order_params_bkt[kParallelism];
	std::vector<std::pair<size_t, size_t>> new_order_params_i_id_bkt[kParallelism];
	std::vector<std::pair<size_t, int>> d_n_mixed_bkt[kParallelism];
	std::vector<std::vector<std::string>> s_dists_vector;
	std::vector<std::vector<double>> ol_amounts_vector;
	std::vector<std::vector<int>> no_o_ids_vector;

	size_t delivery_pos = 0;
	size_t new_order_pos = 0;
	for (size_t i = 0; i < batch_params.size(); ++i) {
		auto && tuple = batch_params.at(i);
		if (tuple.first == TupleType::DELIVERY) {
			DeliveryParam* param = static_cast<DeliveryParam*>(tuple.second);
			no_o_ids_vector.emplace_back(std::vector<int>(DISTRICTS_PER_WAREHOUSE + 1, -1));
			for (int no_d_id = 1; no_d_id <= DISTRICTS_PER_WAREHOUSE; ++no_d_id) {
				size_t offset = scale_params->getOrderLineTableOffset(param->w_id, no_d_id);
				d_n_mixed_bkt[offset % kParallelism].emplace_back(std::make_pair(delivery_pos, no_d_id));
			}
			++delivery_pos;
		} else if (tuple.first == TupleType::NEW_ORDER) {
			NewOrderParam* param = static_cast<NewOrderParam*>(tuple.second);
			size_t offset = scale_params->getOrderLineTableOffset(param->w_id, param->d_id);
			d_n_mixed_bkt[offset % kParallelism].emplace_back(std::make_pair(new_order_pos, -1));
			new_order_params_bkt[offset % kParallelism].emplace_back(new_order_pos);
			s_dists_vector.emplace_back(std::vector<std::string>(param->i_ids.size()));
			ol_amounts_vector.emplace_back(std::vector<double>(param->i_ids.size()));
			for (size_t i = 0; i < param->i_ids.size(); ++i) {
				int ol_i_id = scale_params->num_items * (param->i_w_ids.at(i) - 1) + param->i_ids.at(i) - 1;
				new_order_params_i_id_bkt[hash_int(ol_i_id) % kParallelism].emplace_back(std::make_pair(new_order_pos, i));
			}
			++new_order_pos;
		}
	}

	std::vector<std::future<void>> no4_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		no4_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			//printf("%ld\n", new_order_params_i_id_bkt[t].size());
			for (size_t i = 0; i < new_order_params_i_id_bkt[t].size(); ++i) {
				auto entry = new_order_params_i_id_bkt[t].at(i);
				neworderSubTxn4Single(new_order_params.at(entry.first), entry.second, s_dists_vector.at(entry.first).at(entry.second));
			}
			timer.stop();
			//printf("new_order_txn4_thread %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	std::vector<int> d_next_o_id_vector;
	d_next_o_id_vector.resize(new_order_params.size());
	std::vector<std::future<void>> no12_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		no12_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < new_order_params_bkt[t].size(); ++i) {
				size_t pos = new_order_params_bkt[t].at(i);
				d_next_o_id_vector[pos] = neworderSubTxn1(new_order_params.at(pos));
				neworderSubTxn2(new_order_params.at(pos), d_next_o_id_vector.at(pos));
			}
			timer.stop();
			//printf("new_order_txn12_thread %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	std::vector<std::future<void>> no5_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		no5_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < new_order_params_i_id_bkt[t].size(); ++i) {
				auto entry = new_order_params_i_id_bkt[t].at(i);
				neworderSubTxn5Single(new_order_params.at(entry.first), entry.second, ol_amounts_vector.at(entry.first).at(entry.second));
			}
			timer.stop();
			//printf("new_order_txn5_thread %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	std::future<void> payment_txn1_thread = thread_pool.enqueue([&]() {
		Timer timer;
		timer.start();
		for (size_t i = 0; i < payment_params.size(); ++i) {
			paymentSubTxn1(payment_params.at(i));
		}
		timer.stop();
		//printf("payment_txn1_thread: %lld ms\n", timer.getElapsedMilliSeconds());
	});

	std::future<void> payment_txn2_thread = thread_pool.enqueue([&]() {
		Timer timer;
		timer.start();
		for (size_t i = 0; i < payment_params.size(); ++i) {
			paymentSubTxn2(payment_params.at(i));
		}
		timer.stop();
		//printf("payment_txn2_thread: %lld ms\n", timer.getElapsedMilliSeconds());
	});

	std::future<void> payment_txn3_thread = thread_pool.enqueue([&]() {
		Timer timer;
		timer.start();
		for (size_t i = 0; i < payment_params.size(); ++i) {
			paymentSubTxn3(payment_params.at(i));
		}
		timer.stop();
		//printf("payment_txn3_thread: %lld ms\n", timer.getElapsedMilliSeconds());
	});


	for (auto && entry : no12_thds)
		entry.wait();

	std::vector<std::future<void>> neworder_conflict_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		neworder_conflict_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < d_n_mixed_bkt[t].size(); ++i) {
				auto entry = d_n_mixed_bkt[t].at(i);
				if (entry.second == -1) {
					neworderSubTxn3(new_order_params.at(entry.first), d_next_o_id_vector.at(entry.first));
				} else {
					deliverySubTxn1Single(delivery_params.at(entry.first), entry.second, no_o_ids_vector.at(entry.first).at(entry.second));
				}
			}
			timer.stop();
			//printf("new_order_conflict_txn_thread %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	for (auto && entry : neworder_conflict_thds)
		entry.wait();
	for (auto && entry : no4_thds)
		entry.wait();
	for (auto && entry : no5_thds)
		entry.wait();

	std::vector<std::vector<int>> c_ids_vector;
	std::vector<std::vector<double>> sums_vector;
	std::vector<std::pair<size_t, int>> d_p_mixed_bkt[kParallelism];
	std::vector<std::pair<size_t, int>> delivery_params_bkt[kParallelism];

	delivery_pos = 0;
	size_t payment_pos = 0;
	for (size_t i = 0; i < batch_params.size(); ++i) {
		auto && tuple = batch_params.at(i);
		if (tuple.first == TupleType::DELIVERY) {
			DeliveryParam* param = static_cast<DeliveryParam*>(tuple.second);
			c_ids_vector.emplace_back(std::vector<int>(DISTRICTS_PER_WAREHOUSE + 1, -1));
			sums_vector.emplace_back(std::vector<double>(DISTRICTS_PER_WAREHOUSE + 1, -1));
			for (int no_d_id = 1; no_d_id <= DISTRICTS_PER_WAREHOUSE; ++no_d_id) {
				if (no_o_ids_vector.at(delivery_pos).at(no_d_id) != -1) {
					size_t offset = scale_params->getOrderLineTableOffset(param->w_id, no_d_id);
					d_p_mixed_bkt[offset % kParallelism].emplace_back(std::make_pair(delivery_pos, no_d_id));
					delivery_params_bkt[offset % kParallelism].emplace_back(std::make_pair(delivery_pos, no_d_id));
				}
			}
			++delivery_pos;
		} else if (tuple.first == TupleType::PAYMENT) {
			PaymentParam* param = static_cast<PaymentParam*>(tuple.second);
			size_t offset = scale_params->getOrderLineTableOffset(param->w_id, param->d_id);
			d_p_mixed_bkt[offset % kParallelism].emplace_back(std::make_pair(payment_pos, -1));
			++payment_pos;
		}
	}

	std::vector<std::future<void>> del3_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		del3_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < delivery_params_bkt[t].size(); ++i) {
				auto && entry = delivery_params_bkt[t].at(i);
				deliverySubTxn3Single(delivery_params.at(entry.first), entry.second, no_o_ids_vector.at(entry.first).at(entry.second), c_ids_vector.at(entry.first).at(entry.second));
			}
			timer.stop();
			//printf("delivery_txn3_thread %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	std::vector<std::future<void>> no6_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		no6_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < new_order_params_bkt[t].size(); ++i) {
				size_t pos = new_order_params_bkt[t].at(i);
				neworderSubTxn6(new_order_params.at(pos), d_next_o_id_vector.at(pos), s_dists_vector.at(pos), ol_amounts_vector.at(pos));
			}
			timer.stop();
			//printf("new_order_txn6_thread %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	for (auto && entry : no6_thds)
		entry.wait();

	std::vector<std::future<void>> del2_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		del2_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < delivery_params_bkt[t].size(); ++i) {
				auto && entry = delivery_params_bkt[t].at(i);
				deliverySubTxn2Single(delivery_params.at(entry.first), entry.second, no_o_ids_vector.at(entry.first).at(entry.second), sums_vector.at(entry.first).at(entry.second));
			}
			timer.stop();
			//printf("delivery_txn2_thread: %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	for (auto && entry : del2_thds)
		entry.wait();
	for (auto && entry : del3_thds)
		entry.wait();


	std::vector<std::future<void>> conflict2_thds;
	for (size_t t = 0; t < kParallelism; ++t) {
		conflict2_thds.emplace_back(thread_pool.enqueue([&](const size_t& t) {
			Timer timer;
			timer.start();
			for (size_t i = 0; i < d_p_mixed_bkt[t].size(); ++i) {
				auto entry = d_p_mixed_bkt[t].at(i);
				if (entry.second == -1) {
					paymentSubTxn4(payment_params.at(entry.first));
				} else {
					deliverySubTxn4Single(delivery_params.at(entry.first), entry.second, sums_vector.at(entry.first).at(entry.second), c_ids_vector.at(entry.first).at(entry.second));
				}
			}
			timer.stop();
			//printf("conflict_txn_2 thread %ld: %lld ms\n", t, timer.getElapsedMilliSeconds());
		}, t));
	}

	payment_txn1_thread.wait();
	payment_txn2_thread.wait();
	payment_txn3_thread.wait();
	for (auto && entry : conflict2_thds)
		entry.wait();
}
