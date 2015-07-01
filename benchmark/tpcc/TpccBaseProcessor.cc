#include "TpccBaseProcessor.h"

using namespace IMDB::Benchmark::TPCC;
using namespace IMDB::StorageEngine;

void TpccBaseProcessor::deliverySubTxn1(const DeliveryParam* param, std::unordered_map<int, int>& no_o_ids) {
	for (int no_d_id = 1; no_d_id <= DISTRICTS_PER_WAREHOUSE; ++no_d_id) {
		size_t d_no_cols[] = {0, 1};
		const char* d_no_values[] = {reinterpret_cast<const char*>(&param->w_id), reinterpret_cast<const char*>(&no_d_id)};
		ConstRecord district_new_order_record = storage_manager->selectRecord(DISTRICT_NEW_ORDER_TABLE_ID, ColumnPredicates(d_no_cols, d_no_values, 2));
		assert(district_new_order_record.fetchData() != NULL);
		int no_o_id = *(reinterpret_cast<const int*>(district_new_order_record.getColumn(2)));

		size_t no_cols[] = {0, 1, 2};
		const char* no_values[] = {reinterpret_cast<const char*>(&no_o_id), reinterpret_cast<const char*>(&no_d_id), reinterpret_cast<const char*>(&param->w_id)};
		size_t offset = kPartition ? scale_params->getOrderLineTableOffset(param->w_id, no_d_id) % kPartitionCnt : 0;
		ConstRecord new_order_record = storage_manager->selectRecord(NEW_ORDER_TABLE_ID + offset, ColumnPredicates(no_cols, no_values, 3));
		if (new_order_record.fetchData() != NULL) {
			no_o_ids[no_d_id] = no_o_id;
			storage_manager->deleteRecord(NEW_ORDER_TABLE_ID + offset, new_order_record);
			int next_o_id = no_o_id + 1;
			storage_manager->updateRecord(DISTRICT_NEW_ORDER_TABLE_ID, district_new_order_record, ColumnPredicate(2, reinterpret_cast<const char*>(&next_o_id)));
		}
	}
}

void TpccBaseProcessor::deliverySubTxn1Single(const DeliveryParam* param, const int& no_d_id, int& o_id) {
	size_t d_no_cols[] = {0, 1};
	const char* d_no_values[] = {reinterpret_cast<const char*>(&param->w_id), reinterpret_cast<const char*>(&no_d_id)};
	ConstRecord district_new_order_record = storage_manager->selectRecord(DISTRICT_NEW_ORDER_TABLE_ID, ColumnPredicates(d_no_cols, d_no_values, 2));
	assert(district_new_order_record.fetchData() != NULL);
	int no_o_id = *(reinterpret_cast<const int*>(district_new_order_record.getColumn(2)));
	
	size_t no_cols[] = {0, 1, 2};
	const char* no_values[] = {reinterpret_cast<const char*>(&no_o_id), reinterpret_cast<const char*>(&no_d_id), reinterpret_cast<const char*>(&param->w_id)};
	size_t offset = kPartition ? scale_params->getOrderLineTableOffset(param->w_id, no_d_id) % kPartitionCnt : 0;
	ConstRecord new_order_record = storage_manager->selectRecord(NEW_ORDER_TABLE_ID + offset, ColumnPredicates(no_cols, no_values, 3));
	if (new_order_record.fetchData() != NULL) {
		o_id = no_o_id;
		storage_manager->deleteRecord(NEW_ORDER_TABLE_ID + offset, new_order_record);
		int next_o_id = no_o_id + 1;
		storage_manager->updateRecord(DISTRICT_NEW_ORDER_TABLE_ID, district_new_order_record, ColumnPredicate(2, reinterpret_cast<const char*>(&next_o_id)));
	}
}

void TpccBaseProcessor::deliverySubTxn2(const DeliveryParam* param, const std::unordered_map<int, int>& no_o_ids, std::unordered_map<int, double>& sums) {
	for (auto & entry : no_o_ids) {
		size_t ol_cols[] = {0, 1, 2};
		const char* ol_values[] = {reinterpret_cast<const char*>(&entry.second), reinterpret_cast<const char*>(&entry.first), reinterpret_cast<const char*>(&param->w_id)};
		size_t offset = kPartition ? scale_params->getOrderLineTableOffset(param->w_id, entry.first) % kPartitionCnt : 0;
		ConstRecords order_line_records = storage_manager->selectRecords(ORDER_LINE_TABLE_ID + offset, ColumnPredicates(ol_cols, ol_values, 3));
		assert(order_line_records.getRecordNum() != 0);
		double sum = 0;
		for (size_t i = 0; i < order_line_records.getRecordNum(); ++i) {
			storage_manager->updateRecord(ORDER_LINE_TABLE_ID + offset, order_line_records, i, ColumnPredicate(6, reinterpret_cast<const char*>(&param->ol_delivery_d)));
			sum += *(reinterpret_cast<const double*>(order_line_records.getColumn(i, 8)));
		}
		sums[entry.first] = sum;
	}
}

void TpccBaseProcessor::deliverySubTxn2Single(const DeliveryParam* param, const int& no_d_id, const int& no_o_id, double& sum) {
	size_t ol_cols[] = {0, 1, 2};
	const char* ol_values[] = {reinterpret_cast<const char*>(&no_o_id), reinterpret_cast<const char*>(&no_d_id), reinterpret_cast<const char*>(&param->w_id)};
	size_t offset = kPartition ? scale_params->getOrderLineTableOffset(param->w_id, no_d_id) % kPartitionCnt : 0;
	ConstRecords order_line_records = storage_manager->selectRecords(ORDER_LINE_TABLE_ID + offset, ColumnPredicates(ol_cols, ol_values, 3));
	assert(order_line_records.getRecordNum() != 0);
	sum = 0;
	for (size_t i = 0; i < order_line_records.getRecordNum(); ++i) {
		storage_manager->updateRecord(ORDER_LINE_TABLE_ID + offset, order_line_records, i, ColumnPredicate(6, reinterpret_cast<const char*>(&param->ol_delivery_d)));
		sum += *(reinterpret_cast<const double*>(order_line_records.getColumn(i, 8)));
	}
}

void TpccBaseProcessor::deliverySubTxn3(const DeliveryParam* param, const std::unordered_map<int, int>& no_o_ids, std::unordered_map<int, int>& c_ids) {
	for (auto& entry : no_o_ids) {
		size_t o_cols[] = {0, 2, 3};
		const char* o_values[] = {reinterpret_cast<const char*>(&entry.second), reinterpret_cast<const char*>(&entry.first), reinterpret_cast<const char*>(&param->w_id)};
		size_t offset = kPartition ? scale_params->getOrderLineTableOffset(param->w_id, entry.first) % kPartitionCnt : 0;
		ConstRecord order_record = storage_manager->selectRecord(ORDER_TABLE_ID + offset, ColumnPredicates(o_cols, o_values, 3));
		assert(order_record.fetchData() != NULL);
		storage_manager->updateRecord(ORDER_TABLE_ID + offset, order_record, ColumnPredicate(5, reinterpret_cast<const char*>(&param->o_carrier_id)));
		int c_id = *(reinterpret_cast<const int*>(order_record.getColumn(1)));
		c_ids[entry.first] = c_id;
	}
}

void TpccBaseProcessor::deliverySubTxn3Single(const DeliveryParam* param, const int& no_d_id, const int& no_o_id, int& c_id) {
	size_t o_cols[] = {0, 2, 3};
	const char* o_values[] = {reinterpret_cast<const char*>(&no_o_id), reinterpret_cast<const char*>(&no_d_id), reinterpret_cast<const char*>(&param->w_id)};
	size_t offset = kPartition ? scale_params->getOrderLineTableOffset(param->w_id, no_d_id) % kPartitionCnt : 0;
	ConstRecord order_record = storage_manager->selectRecord(ORDER_TABLE_ID + offset, ColumnPredicates(o_cols, o_values, 3));
	assert(order_record.fetchData() != NULL);
	storage_manager->updateRecord(ORDER_TABLE_ID + offset, order_record, ColumnPredicate(5, reinterpret_cast<const char*>(&param->o_carrier_id)));
	c_id = *(reinterpret_cast<const int*>(order_record.getColumn(1)));
}

void TpccBaseProcessor::deliverySubTxn4(const DeliveryParam* param, const std::unordered_map<int, int>& no_o_ids, const std::unordered_map<int, double>& sums, const std::unordered_map<int, int>& c_ids) {
	for (auto& entry : no_o_ids) {
		size_t c_cols[] = {0, 1, 2};
		const char* c_values[] = {reinterpret_cast<const char*>(&c_ids.at(entry.first)), reinterpret_cast<const char*>(&entry.first), reinterpret_cast<const char*>(&param->w_id)};
		ConstRecord customer_record = storage_manager->selectRecord(CUSTOMER_TABLE_ID, ColumnPredicates(c_cols, c_values, 3));
		assert(customer_record.fetchData() != NULL);
		double balance = *(reinterpret_cast<const double*>(customer_record.getColumn(16))) + sums.at(entry.first);
		storage_manager->updateRecord(CUSTOMER_TABLE_ID, customer_record, ColumnPredicate(16, reinterpret_cast<const char*>(&balance)));
	}
}

void TpccBaseProcessor::deliverySubTxn4Single(const DeliveryParam* param, const int& no_d_id, const double& sum, const int& c_id) {
	size_t c_cols[] = {0, 1, 2};
	const char* c_values[] = {reinterpret_cast<const char*>(&c_id), reinterpret_cast<const char*>(&no_d_id), reinterpret_cast<const char*>(&param->w_id)};
	ConstRecord customer_record = storage_manager->selectRecord(CUSTOMER_TABLE_ID, ColumnPredicates(c_cols, c_values, 3));
	assert(customer_record.fetchData() != NULL);
	double balance = *(reinterpret_cast<const double*>(customer_record.getColumn(16))) + sum;
	storage_manager->updateRecord(CUSTOMER_TABLE_ID, customer_record, ColumnPredicate(16, reinterpret_cast<const char*>(&balance)));
}

int TpccBaseProcessor::neworderSubTxn1(const NewOrderParam* param) {
	size_t d_cols[] = {0, 1};
	const char* d_values[] = {reinterpret_cast<const char*>(&param->d_id), reinterpret_cast<const char*>(&param->w_id)};
	ConstRecord district_record = storage_manager->selectRecord(DISTRICT_TABLE_ID, ColumnPredicates(d_cols, d_values, 2));
	assert(district_record.fetchData() != NULL);
	int d_next_o_id = *(reinterpret_cast<const int*>(district_record.getColumn(10)));
	int o_id = d_next_o_id + 1;
	storage_manager->updateRecord(DISTRICT_TABLE_ID, district_record, ColumnPredicate(10, reinterpret_cast<const char*>(&o_id)));
	return d_next_o_id;
}

void TpccBaseProcessor::neworderSubTxn2(const NewOrderParam* param, const int& d_next_o_id) {
	bool all_local = true;
	for (auto & w_id : param->i_w_ids) {
		all_local = (all_local && (param->w_id == w_id));
	}
	int ol_cnt = static_cast<int>(param->i_ids.size());
	Record order_record(TpccSchema::getOrderSchema());
	char* order_data = new char[TpccSchema::getOrderSchema()->getSchemaSize()];
	order_record.transferData(order_data);
	order_record.setColumn(0, reinterpret_cast<const char*>(&d_next_o_id));
	order_record.setColumn(1, reinterpret_cast<const char*>(&param->c_id));
	order_record.setColumn(2, reinterpret_cast<const char*>(&param->d_id));
	order_record.setColumn(3, reinterpret_cast<const char*>(&param->w_id));
	order_record.setColumn(4, reinterpret_cast<const char*>(&param->o_entry_d));
	order_record.setColumn(5, reinterpret_cast<const char*>(&NULL_CARRIER_ID));
	order_record.setColumn(6, reinterpret_cast<const char*>(&ol_cnt));
	order_record.setColumn(7, reinterpret_cast<const char*>(&all_local));
	size_t offset = kPartition ? scale_params->getOrderLineTableOffset(param->w_id, param->d_id) % kPartitionCnt : 0;
	storage_manager->insertRecord(ORDER_TABLE_ID + offset, order_record);
	delete[] order_data;
}

void TpccBaseProcessor::neworderSubTxn3(const NewOrderParam* param, const int& d_next_o_id) {
	Record new_order_record(TpccSchema::getNewOrderSchema());
	char* new_order_data = new char[TpccSchema::getNewOrderSchema()->getSchemaSize()];
	new_order_record.transferData(new_order_data);
	new_order_record.setColumn(0, reinterpret_cast<const char*>(&d_next_o_id));
	new_order_record.setColumn(1, reinterpret_cast<const char*>(&param->d_id));
	new_order_record.setColumn(2, reinterpret_cast<const char*>(&param->w_id));
	size_t offset = kPartition ? scale_params->getOrderLineTableOffset(param->w_id, param->d_id) % kPartitionCnt : 0;
	storage_manager->insertRecord(NEW_ORDER_TABLE_ID + offset, new_order_record);
	delete[] new_order_data;
}

void TpccBaseProcessor::neworderSubTxn4(const NewOrderParam* param, std::vector<std::string>& s_dists) {
	for (size_t i = 0; i < param->i_ids.size(); ++i) {
		int ol_supply_w_id = param->i_w_ids.at(i);
		int ol_i_id = param->i_ids.at(i);
		int ol_quantity = param->i_qtys.at(i);

		size_t s_cols[] = {0, 1};
		const char* s_values[] = {reinterpret_cast<const char*>(&ol_i_id), reinterpret_cast<const char*>(&ol_supply_w_id)};
		ConstRecord stock_record = storage_manager->selectRecord(STOCK_TABLE_ID, ColumnPredicates(s_cols, s_values, 2));
		assert(stock_record.fetchData() != NULL);
		int quantity = *(reinterpret_cast<const int*>(stock_record.getColumn(2)));
		if (quantity >= ol_quantity + 10) {
			quantity -= ol_quantity;
		} else {
			quantity = quantity + 91 - ol_quantity;
		}
		int ytd = *(reinterpret_cast<const int*>(stock_record.getColumn(13))) + ol_quantity;
		int order_cnt = *(reinterpret_cast<const int*>(stock_record.getColumn(14))) + 1;
		
		size_t update_cols[] = {2, 13, 14};
		const char* update_values[] = {reinterpret_cast<const char*>(&quantity), reinterpret_cast<const char*>(&ytd), reinterpret_cast<const char*>(&order_cnt)};
		storage_manager->updateRecord(STOCK_TABLE_ID, stock_record, ColumnPredicates(update_cols, update_values, 3));
		if (ol_supply_w_id != param->w_id) {
			int remote_cnt = *(reinterpret_cast<const int*>(stock_record.getColumn(15))) + 1;
			storage_manager->updateRecord(STOCK_TABLE_ID, stock_record, ColumnPredicate(15, reinterpret_cast<const char*>(&remote_cnt)));
		}
		std::string s_dist_data;
		int dist_column = param->d_id + 2;
		stock_record.getColumn(dist_column, s_dist_data);
		s_dists.emplace_back(s_dist_data);
	}
}

void TpccBaseProcessor::neworderSubTxn4Single(const NewOrderParam* param, const size_t& i, std::string& s_dist) {
	int ol_supply_w_id = param->i_w_ids.at(i);
	int ol_i_id = param->i_ids.at(i);
	int ol_quantity = param->i_qtys.at(i);

	size_t s_cols[] = {0, 1};
	const char* s_values[] = {reinterpret_cast<const char*>(&ol_i_id), reinterpret_cast<const char*>(&ol_supply_w_id)};
	ConstRecord stock_record = storage_manager->selectRecord(STOCK_TABLE_ID, ColumnPredicates(s_cols, s_values, 2));
	assert(stock_record.fetchData() != NULL);
	int quantity = *(reinterpret_cast<const int*>(stock_record.getColumn(2)));
	if (quantity >= ol_quantity + 10) {
		quantity -= ol_quantity;
	} else {
		quantity = quantity + 91 - ol_quantity;
	}
	int ytd = *(reinterpret_cast<const int*>(stock_record.getColumn(13))) + ol_quantity;
	int order_cnt = *(reinterpret_cast<const int*>(stock_record.getColumn(14))) + 1;
	
	size_t update_cols[] = {2, 13, 14};
	const char* update_values[] = {reinterpret_cast<const char*>(&quantity), reinterpret_cast<const char*>(&ytd), reinterpret_cast<const char*>(&order_cnt)};
	storage_manager->updateRecord(STOCK_TABLE_ID, stock_record, ColumnPredicates(update_cols, update_values, 3));
	if (ol_supply_w_id != param->w_id) {
		int remote_cnt = *(reinterpret_cast<const int*>(stock_record.getColumn(15))) + 1;
		storage_manager->updateRecord(STOCK_TABLE_ID, stock_record, ColumnPredicate(15, reinterpret_cast<const char*>(&remote_cnt)));
	}

	int dist_column = param->d_id + 2;
	stock_record.getColumn(dist_column, s_dist);
}

void TpccBaseProcessor::neworderSubTxn5(const NewOrderParam* param, std::vector<double>& ol_amounts) {
	for (size_t i = 0; i < param->i_ids.size(); ++i) {
		ConstRecord record = storage_manager->selectRecord(ITEM_TABLE_ID, ColumnPredicate(0, reinterpret_cast<const char*>(&param->i_ids.at(i))));
		assert(record.fetchData() != NULL);
		int ol_quantity = param->i_qtys.at(i);
		double price = *(reinterpret_cast<const double*>(record.getColumn(3)));
		ol_amounts.push_back(ol_quantity * price);
	}
}

void TpccBaseProcessor::neworderSubTxn5Single(const NewOrderParam* param, const size_t& i, double& ol_amount) {
	ConstRecord record = storage_manager->selectRecord(ITEM_TABLE_ID, ColumnPredicate(0, reinterpret_cast<const char*>(&param->i_ids.at(i))));
	assert(record.fetchData() != NULL);
	int ol_quantity = param->i_qtys.at(i);
	double price = *(reinterpret_cast<const double*>(record.getColumn(3)));
	ol_amount = ol_quantity * price;
}

void TpccBaseProcessor::neworderSubTxn6(const NewOrderParam* param, const int& d_next_o_id, const std::vector<std::string>& s_dists, const std::vector<double>& ol_amounts) {
	for (size_t i = 0; i < param->i_ids.size(); ++i) {
		int ol_number = i + 1;
		int ol_i_id = param->i_ids.at(i);
		int ol_supply_w_id = param->i_w_ids.at(i);
		int ol_quantity = param->i_qtys.at(i);

		Record order_line_record(TpccSchema::getOrderLineSchema());
		char* order_line_data = new char[TpccSchema::getOrderLineSchema()->getSchemaSize()];
		order_line_record.transferData(order_line_data);
		order_line_record.setColumn(0, reinterpret_cast<const char*>(&d_next_o_id));
		order_line_record.setColumn(1, reinterpret_cast<const char*>(&param->d_id));
		order_line_record.setColumn(2, reinterpret_cast<const char*>(&param->w_id));
		order_line_record.setColumn(3, reinterpret_cast<const char*>(&ol_number));
		order_line_record.setColumn(4, reinterpret_cast<const char*>(&ol_i_id));
		order_line_record.setColumn(5, reinterpret_cast<const char*>(&ol_supply_w_id));
		order_line_record.setColumn(6, reinterpret_cast<const char*>(&param->o_entry_d));
		order_line_record.setColumn(7, reinterpret_cast<const char*>(&ol_quantity));
		order_line_record.setColumn(8, reinterpret_cast<const char*>(&ol_amounts.at(i)));
		order_line_record.setColumn(9, s_dists.at(i));
		size_t offset = kPartition ? scale_params->getOrderLineTableOffset(param->w_id, param->d_id) % kPartitionCnt : 0;
		storage_manager->insertRecord(ORDER_LINE_TABLE_ID + offset, order_line_record);
		delete[] order_line_data;
	}
}

void TpccBaseProcessor::paymentSubTxn1(const PaymentParam* param) {
	ConstRecord warehouse_record = storage_manager->selectRecord(WAREHOUSE_TABLE_ID, ColumnPredicate(0, reinterpret_cast<const char*>(&param->w_id)));
	double w_ytd = *(reinterpret_cast<const double*>(warehouse_record.getColumn(8))) + param->h_amount;
	storage_manager->updateRecord(WAREHOUSE_TABLE_ID, warehouse_record, ColumnPredicate(8, reinterpret_cast<const char*>(&w_ytd)));
}

void TpccBaseProcessor::paymentSubTxn2(const PaymentParam* param) {
	size_t d_cols[] = {0, 1};
	const char* d_values[] = {reinterpret_cast<const char*>(&param->d_id), reinterpret_cast<const char*>(&param->w_id)};
	ConstRecord district_record = storage_manager->selectRecord(DISTRICT_TABLE_ID, ColumnPredicates(d_cols, d_values, 2));
	double d_ytd = *(reinterpret_cast<const double*>(district_record.getColumn(9))) + param->h_amount;
	storage_manager->updateRecord(DISTRICT_TABLE_ID, district_record, ColumnPredicate(9, reinterpret_cast<const char*>(&d_ytd)));
}

void TpccBaseProcessor::paymentSubTxn3(const PaymentParam* param) {
	std::string h_data = RandomGenerator::generateCString(MIN_DATA, MAX_DATA);
	Record history_record(TpccSchema::getHistorySchema());
	char* history_data = new char[TpccSchema::getHistorySchema()->getSchemaSize()];
	history_record.transferData(history_data);
	history_record.setColumn(0, reinterpret_cast<const char*>(&param->c_id));
	history_record.setColumn(1, reinterpret_cast<const char*>(&param->c_d_id));
	history_record.setColumn(2, reinterpret_cast<const char*>(&param->c_w_id));
	history_record.setColumn(3, reinterpret_cast<const char*>(&param->d_id));
	history_record.setColumn(4, reinterpret_cast<const char*>(&param->w_id));
	history_record.setColumn(5, reinterpret_cast<const char*>(&param->h_date));
	history_record.setColumn(6, reinterpret_cast<const char*>(&param->h_amount));
	history_record.setColumn(7, h_data);
	storage_manager->insertRecord(HISTORY_TABLE_ID, history_record);
	delete[] history_data;
}

void TpccBaseProcessor::paymentSubTxn4(const PaymentParam* param) {
	size_t c_cols[] = {0, 1, 2};
	const char* c_values[] = {reinterpret_cast<const char*>(&param->c_id), reinterpret_cast<const char*>(&param->d_id), reinterpret_cast<const char*>(&param->w_id)};
	ConstRecord customer_record = storage_manager->selectRecord(CUSTOMER_TABLE_ID, ColumnPredicates(c_cols, c_values, 3));
	double balance = *(reinterpret_cast<const double*>(customer_record.getColumn(16))) - param->h_amount;
	double ytd_payment = *(reinterpret_cast<const double*>(customer_record.getColumn(17))) + param->h_amount;
	int payment_cnt = *(reinterpret_cast<const int*>(customer_record.getColumn(18))) + 1;
	size_t update_cols[] = {16, 17, 18};
	const char* update_values[] = {reinterpret_cast<const char*>(&balance), reinterpret_cast<const char*>(&ytd_payment), reinterpret_cast<const char*>(&payment_cnt)};
	storage_manager->updateRecord(CUSTOMER_TABLE_ID, customer_record, ColumnPredicates(update_cols, update_values, 3));

}
