#include "TpccPopulator.h"
#include "TpccRecords.h"
#include <iostream>

using namespace IMDB::Benchmark::TPCC;

void TpccPopulator::init() {
	storage_manager->registerTable(ITEM_TABLE_ID, TpccSchema::getItemSchema());
	storage_manager->registerTable(WAREHOUSE_TABLE_ID, TpccSchema::getWarehouseSchema());
	storage_manager->registerTable(DISTRICT_TABLE_ID, TpccSchema::getDistrictSchema());
	storage_manager->registerTable(CUSTOMER_TABLE_ID, TpccSchema::getCustomerSchema());
	if (kPartition) {
		for (size_t i = 0; i < kPartitionCnt; ++i) {
			storage_manager->registerTable(ORDER_TABLE_ID + i, TpccSchema::getOrderSchema());
			storage_manager->registerTable(NEW_ORDER_TABLE_ID + i, TpccSchema::getNewOrderSchema());
			size_t column_ids[] = {0, 1, 2};
			IndexInfo* order_line_index = new IndexInfo(IndexType::HASHMAP, column_ids, 3);
			storage_manager->registerTable(ORDER_LINE_TABLE_ID + i, TpccSchema::getOrderLineSchema(), order_line_index, 1);
			delete order_line_index;
		}
	} else {
		storage_manager->registerTable(ORDER_TABLE_ID, TpccSchema::getOrderSchema());
		storage_manager->registerTable(NEW_ORDER_TABLE_ID, TpccSchema::getNewOrderSchema());
		size_t column_ids[] = {0, 1, 2};
		IndexInfo* order_line_index = new IndexInfo(IndexType::HASHMAP, column_ids, 3);
		storage_manager->registerTable(ORDER_LINE_TABLE_ID, TpccSchema::getOrderLineSchema(), order_line_index, 1);
		delete order_line_index;
	}
	storage_manager->registerTable(HISTORY_TABLE_ID, TpccSchema::getHistorySchema());
	storage_manager->registerTable(STOCK_TABLE_ID, TpccSchema::getStockSchema());
	storage_manager->registerTable(DISTRICT_NEW_ORDER_TABLE_ID, TpccSchema::getDistrictNewOrderSchema());
}

void TpccPopulator::startPopulate() {
	std::unordered_set<int> original_rows;
	RandomGenerator::selectUniqueIds(scale_params->num_items/10, 1, scale_params->num_items, original_rows);
	for (int item_id = 1; item_id <= scale_params->num_items; ++item_id) {
		bool is_origin = (original_rows.find(item_id) != original_rows.end());
		ItemRecord* item_record = generateItemRecord(item_id, is_origin);
		insertItemRecord(item_record);
		delete item_record;
	}

	for (int w_id = 1; w_id <= scale_params->num_warehouses; ++w_id) {
		WarehouseRecord* warehouse_record = generateWarehouseRecord(w_id);
		insertWarehouseRecord(warehouse_record);
		delete warehouse_record;
		for (int d_id = 1; d_id <= scale_params->num_districts_per_warehouse; ++d_id) {
			int d_next_o_id = scale_params->num_customers_per_district + 1;
			DistrictRecord* district_record = generateDistrictRecord(w_id, d_id, d_next_o_id);
			insertDistrictRecord(district_record);
			delete district_record;

			std::unordered_set<int> selected_rows;
			RandomGenerator::selectUniqueIds(scale_params->num_customers_per_district/10, 1, scale_params->num_customers_per_district, selected_rows);
			for (int c_id = 1; c_id <= scale_params->num_customers_per_district; ++c_id) {
				bool bad_credit = (selected_rows.find(c_id) != selected_rows.end());
				CustomerRecord* customer_record = generateCustomerRecord(w_id, d_id, c_id, bad_credit);
				insertCustomerRecord(customer_record);
				delete customer_record;

				HistoryRecord* history_record = generateHistoryRecord(w_id, d_id, c_id);
				insertHistoryRecord(history_record);
				delete history_record;
			}

			int initial_new_order_id = scale_params->num_customers_per_district - scale_params->num_new_orders_per_district + 1;
			DistrictNewOrderRecord* district_new_order_record = generateDistrictNewOrderRecord(w_id, d_id, initial_new_order_id);
			insertDistrictNewOrderRecord(district_new_order_record);
			delete district_new_order_record;

			for (int o_id = 1; o_id <= scale_params->num_customers_per_district; ++o_id) {
				int o_ol_cnt = RandomGenerator::generateInteger(MIN_OL_CNT, MAX_OL_CNT);
				bool is_new_order = (o_id >= initial_new_order_id);

				OrderRecord* order_record = generateOrderRecord(w_id, d_id, o_id, o_id, o_ol_cnt, is_new_order);
				insertOrderRecord(order_record);
				delete order_record;

				for (int ol_number = 1; ol_number <= o_ol_cnt; ++ol_number) {
					OrderLineRecord* order_line_record = generateOrderLineRecord(w_id, d_id, o_id, ol_number, scale_params->num_items, is_new_order);
					insertOrderLineRecord(order_line_record);
					delete order_line_record;
				}

				if (is_new_order) {
					NewOrderRecord* new_order_record = generateNewOrderRecord(w_id, d_id, o_id);
					insertNewOrderRecord(new_order_record);
					delete new_order_record;
				}
			}
		}

		std::unordered_set<int> selected_rows;
		RandomGenerator::selectUniqueIds(scale_params->num_items/10, 1, scale_params->num_items, selected_rows);
		for (int i_id = 1; i_id <= scale_params->num_items; ++i_id) {
			bool original = (selected_rows.find(i_id) != selected_rows.end());
			StockRecord* stock_record = generateStockRecord(w_id, i_id, original);
			insertStockRecord(stock_record);
			delete stock_record;
		}
	}
}

ItemRecord* TpccPopulator::generateItemRecord(const int& item_id, bool original) const {
	ItemRecord* record = new ItemRecord();
	record->i_id = item_id;
	record->i_im_id = RandomGenerator::generateInteger(MIN_IM, MAX_IM);
	std::string name = RandomGenerator::generateCString(MIN_I_NAME, MAX_I_NAME);
	memcpy(record->i_name, name.c_str(), name.size());
	record->i_price = RandomGenerator::generateFixedPoint(MONEY_DECIMALS, MIN_PRICE, MAX_PRICE);
	std::string data = RandomGenerator::generateCString(MIN_I_DATA, MAX_I_DATA);
	memcpy(record->i_data, data.c_str(), data.size());
	return record;
}

WarehouseRecord* TpccPopulator::generateWarehouseRecord(const int& w_id) const {
	WarehouseRecord* record = new WarehouseRecord();
	record->w_id = w_id;
	std::string name = RandomGenerator::generateCString(MIN_NAME, MAX_NAME);
	memcpy(record->w_name, name.c_str(), name.size());
	RandomGenerator::generateAddress(record->w_street_1, record->w_street_2, record->w_city, record->w_state, record->w_zip);
	record->w_tax = RandomGenerator::generateTax();
	record->w_ytd = INITIAL_W_YTD;
	return record;
}

DistrictRecord* TpccPopulator::generateDistrictRecord(const int& d_w_id, const int& d_id, const int& d_next_o_id) const {
	DistrictRecord* record = new DistrictRecord();
	record->d_id = d_id;
	record->d_w_id = d_w_id;
	std::string name = RandomGenerator::generateCString(MIN_NAME, MAX_NAME);
	memcpy(record->d_name, name.c_str(), name.size());
	RandomGenerator::generateAddress(record->d_street_1, record->d_street_2, record->d_city, record->d_state, record->d_zip);
	record->d_tax = RandomGenerator::generateTax();
	record->d_ytd = INITIAL_D_YTD;
	record->d_next_o_id = d_next_o_id;
	return record;
}

CustomerRecord* TpccPopulator::generateCustomerRecord(const int& c_w_id, const int& c_d_id, const int& c_id, bool bad_credit) const {
	CustomerRecord* record = new CustomerRecord();
	record->c_id = c_id;
	record->c_d_id = c_d_id;
	record->c_w_id = c_w_id;
	std::string first = RandomGenerator::RandomGenerator::generateCString(MIN_FIRST, MAX_FIRST);
	memcpy(record->c_first, first.c_str(), first.size());
	std::string middle = MIDDLE;
	memcpy(record->c_middle, middle.c_str(), middle.size());
	std::string last;
	if (c_id < 1000) last = RandomGenerator::generateLastName(c_id - 1);
	else last = RandomGenerator::generateRandomLastName(CUSTOMERS_PER_DISTRICT);
	memcpy(record->c_last, last.c_str(), last.size());
	RandomGenerator::generateAddress(record->c_street_1, record->c_street_2, record->c_city, record->c_state, record->c_zip);
	std::string phone = RandomGenerator::generateNString(PHONE, PHONE);
	memcpy(record->c_phone, phone.c_str(), phone.size());
	record->c_since = RandomGenerator::generateCurrentTime();
	std::string credit = GOOD_CREDIT;
	if (bad_credit) credit = BAD_CREDIT;
	memcpy(record->c_credit, credit.c_str(), credit.size());
	record->c_credit_lim = INITIAL_CREDIT_LIM;
	record->c_discount = RandomGenerator::generateFixedPoint(DISCOUNT_DECIMALS, MIN_DISCOUNT, MAX_DISCOUNT);
	record->c_balance = INITIAL_BALANCE;
	record->c_ytd_payment = INITIAL_YTD_PAYMENT;
	record->c_payment_cnt = INITIAL_PAYMENT_CNT;
	record->c_delivery_cnt = INITIAL_DELIVERY_CNT;
	std::string data = RandomGenerator::generateCString(MIN_C_DATA, MAX_C_DATA);
	memcpy(record->c_data, data.c_str(), data.size());
	return record;
}

StockRecord* TpccPopulator::generateStockRecord(const int& s_w_id, const int& s_i_id, bool original) const {
	StockRecord* record = new StockRecord();
	record->s_i_id = s_i_id;
	record->s_w_id = s_w_id;
	record->s_quantity = RandomGenerator::generateInteger(MIN_QUANTITY, MAX_QUANTITY);
	for (int i = 0; i < DISTRICTS_PER_WAREHOUSE; ++i) {
		std::string dist = RandomGenerator::generateCString(DIST, DIST);
		memcpy(record->s_dists[i], dist.c_str(), dist.size());
	}
	record->s_ytd = 0;
	record->s_order_cnt = 0;
	record->s_remote_cnt = 0;
	std::string data = RandomGenerator::generateCString(MIN_I_DATA, MAX_I_DATA);
	memcpy(record->s_data, data.c_str(), data.size());
	return record;
}

OrderRecord* TpccPopulator::generateOrderRecord(const int& o_w_id, const int& o_d_id, const int& o_id, const int& o_c_id, const int& o_ol_cnt, bool new_order) const {
	OrderRecord* record = new OrderRecord();
	record->o_id = o_id;
	record->o_c_id = o_c_id;
	record->o_d_id = o_d_id;
	record->o_w_id = o_w_id;
	record->o_entry_d = RandomGenerator::generateCurrentTime();
	if (new_order) record->o_carrier_id = NULL_CARRIER_ID;
	else record->o_carrier_id = RandomGenerator::generateInteger(MIN_CARRIER_ID, MAX_CARRIER_ID);
	record->o_ol_cnt = o_ol_cnt;
	record->o_all_local = INITIAL_ALL_LOCAL;
	return record;
}

NewOrderRecord* TpccPopulator::generateNewOrderRecord(const int& w_id, const int& d_id, const int& o_id) const {
	NewOrderRecord* record = new NewOrderRecord();
	record->w_id = w_id;
	record->d_id = d_id;
	record->o_id = o_id;
	return record;
}

OrderLineRecord* TpccPopulator::generateOrderLineRecord(const int& ol_w_id, const int& ol_d_id, const int& ol_o_id, const int& ol_number, const int& max_items, bool new_order) const {
	OrderLineRecord* record = new OrderLineRecord();
	record->ol_o_id = ol_o_id;
	record->ol_d_id = ol_d_id;
	record->ol_w_id = ol_w_id;
	record->ol_number = ol_number;
	record->ol_i_id = RandomGenerator::generateInteger(1, max_items);
	record->ol_supply_w_id = ol_w_id;
	record->ol_quantity = INITIAL_QUANTITY;
	if (new_order) {
		record->ol_delivery_d = -1;
		record->ol_amount = RandomGenerator::generateFixedPoint(MONEY_DECIMALS, MIN_AMOUNT, MAX_PRICE * MAX_OL_QUANTITY);
	} else {
		record->ol_delivery_d = RandomGenerator::generateCurrentTime();
		record->ol_amount = 0.0;
	}
	std::string ol_dist_info = RandomGenerator::generateCString(DIST, DIST);
	memcpy(record->ol_dist_info, ol_dist_info.c_str(), ol_dist_info.size());
	return record;
}

HistoryRecord* TpccPopulator::generateHistoryRecord(const int& h_c_w_id, const int& h_c_d_id, const int& h_c_id) const {
	HistoryRecord* record = new HistoryRecord();
	record->h_c_id = h_c_id;
	record->h_c_d_id = h_c_d_id;
	record->h_c_w_id = h_c_w_id;
	record->h_d_id = h_c_d_id;
	record->h_w_id = h_c_w_id;
	record->h_date = RandomGenerator::generateCurrentTime();
	record->h_amount = INITIAL_AMOUNT;
	std::string data = RandomGenerator::generateCString(MIN_DATA, MAX_DATA);
	memcpy(record->h_data, data.c_str(), data.size());
	return record;
}

DistrictNewOrderRecord* TpccPopulator::generateDistrictNewOrderRecord(const int& w_id, const int& d_id, const int& o_id) const {
	DistrictNewOrderRecord* record = new DistrictNewOrderRecord();
	record->w_id = w_id;
	record->d_id = d_id;
	record->o_id = o_id;
	return record;
}

void TpccPopulator::insertItemRecord(const ItemRecord* record) {
	Record item_record(TpccSchema::getItemSchema());
	char* data = new char[TpccSchema::getItemSchema()->getSchemaSize()];
	item_record.transferData(data);
	item_record.setColumn(0, reinterpret_cast<const char*>(&record->i_id));
	item_record.setColumn(1, reinterpret_cast<const char*>(&record->i_im_id));
	item_record.setColumn(2, record->i_name, 32);
	item_record.setColumn(3, reinterpret_cast<const char*>(&record->i_price));
	item_record.setColumn(4, record->i_data, 64);
	storage_manager->insertRecord(ITEM_TABLE_ID, item_record);
	delete[] data;
}

void TpccPopulator::insertWarehouseRecord(const WarehouseRecord* record) {
	Record warehouse_record(TpccSchema::getWarehouseSchema());
	char* data = new char[TpccSchema::getWarehouseSchema()->getSchemaSize()];
	warehouse_record.transferData(data);
	warehouse_record.setColumn(0, reinterpret_cast<const char*>(&(record->w_id)));
	warehouse_record.setColumn(1, record->w_name, 16);
	warehouse_record.setColumn(2, record->w_street_1, 32);
	warehouse_record.setColumn(3, record->w_street_2, 32);
	warehouse_record.setColumn(4, record->w_city, 32);
	warehouse_record.setColumn(5, record->w_state, 2);
	warehouse_record.setColumn(6, record->w_zip, 9);
	warehouse_record.setColumn(7, reinterpret_cast<const char*>(&(record->w_tax)));
	warehouse_record.setColumn(8, reinterpret_cast<const char*>(&(record->w_ytd)));
	storage_manager->insertRecord(WAREHOUSE_TABLE_ID, warehouse_record);
	delete[] data;
}

void TpccPopulator::insertDistrictRecord(const DistrictRecord* record) {
	Record district_record(TpccSchema::getDistrictSchema());
	char* data = new char[TpccSchema::getDistrictSchema()->getSchemaSize()];
	district_record.transferData(data);
	district_record.setColumn(0, reinterpret_cast<const char*>(&(record->d_id)));
	district_record.setColumn(1, reinterpret_cast<const char*>(&(record->d_w_id)));
	district_record.setColumn(2, record->d_name, 16);
	district_record.setColumn(3, record->d_street_1, 32);
	district_record.setColumn(4, record->d_street_2, 32);
	district_record.setColumn(5, record->d_city, 32);
	district_record.setColumn(6, record->d_state, 2);
	district_record.setColumn(7, record->d_zip, 9);
	district_record.setColumn(8, reinterpret_cast<const char*>(&(record->d_tax)));
	district_record.setColumn(9, reinterpret_cast<const char*>(&(record->d_ytd)));
	district_record.setColumn(10, reinterpret_cast<const char*>(&(record->d_next_o_id)));
	storage_manager->insertRecord(DISTRICT_TABLE_ID, district_record);
	delete[] data;
}

void TpccPopulator::insertCustomerRecord(const CustomerRecord* record) {
	Record customer_record(TpccSchema::getCustomerSchema());
	char* data = new char[TpccSchema::getCustomerSchema()->getSchemaSize()];
	customer_record.transferData(data);
	customer_record.setColumn(0, reinterpret_cast<const char*>(&(record->c_id)));
	customer_record.setColumn(1, reinterpret_cast<const char*>(&(record->c_d_id)));
	customer_record.setColumn(2, reinterpret_cast<const char*>(&(record->c_w_id)));
	customer_record.setColumn(3, record->c_first, 32);
	customer_record.setColumn(4, record->c_middle, 2);
	customer_record.setColumn(5, record->c_last, 32);
	customer_record.setColumn(6, record->c_street_1, 32);
	customer_record.setColumn(7, record->c_street_2, 32);
	customer_record.setColumn(8, record->c_city, 32);
	customer_record.setColumn(9, record->c_state, 2);
	customer_record.setColumn(10, record->c_zip, 9);
	customer_record.setColumn(11, record->c_phone, 32);
	customer_record.setColumn(12, reinterpret_cast<const char*>(&(record->c_since)));
	customer_record.setColumn(13, record->c_credit, 2);
	customer_record.setColumn(14, reinterpret_cast<const char*>(&(record->c_credit_lim)));
	customer_record.setColumn(15, reinterpret_cast<const char*>(&(record->c_discount)));
	customer_record.setColumn(16, reinterpret_cast<const char*>(&(record->c_balance)));
	customer_record.setColumn(17, reinterpret_cast<const char*>(&(record->c_ytd_payment)));
	customer_record.setColumn(18, reinterpret_cast<const char*>(&(record->c_payment_cnt)));
	customer_record.setColumn(19, reinterpret_cast<const char*>(&(record->c_delivery_cnt)));
	customer_record.setColumn(20, record->c_data, 500);
	storage_manager->insertRecord(CUSTOMER_TABLE_ID, customer_record);
	delete[] data;
}

void TpccPopulator::insertStockRecord(const StockRecord* record) {
	Record stock_record(TpccSchema::getStockSchema());
	char* data = new char[TpccSchema::getStockSchema()->getSchemaSize()];
	stock_record.transferData(data);
	stock_record.setColumn(0, reinterpret_cast<const char*>(&(record->s_i_id)));
	stock_record.setColumn(1, reinterpret_cast<const char*>(&(record->s_w_id)));
	stock_record.setColumn(2, reinterpret_cast<const char*>(&(record->s_quantity)));
	for (int i = 3; i < 13; ++i)
		stock_record.setColumn(i, record->s_dists[i - 3], 32);
	stock_record.setColumn(13, reinterpret_cast<const char*>(&(record->s_ytd)));
	stock_record.setColumn(14, reinterpret_cast<const char*>(&(record->s_order_cnt)));
	stock_record.setColumn(15, reinterpret_cast<const char*>(&(record->s_remote_cnt)));
	stock_record.setColumn(16, record->s_data, 64);
	storage_manager->insertRecord(STOCK_TABLE_ID, stock_record);
	delete[] data;
}

void TpccPopulator::insertOrderRecord(const OrderRecord* record) {
	Record order_record(TpccSchema::getOrderSchema());
	char* data = new char[TpccSchema::getOrderSchema()->getSchemaSize()];
	order_record.transferData(data);
	order_record.setColumn(0, reinterpret_cast<const char*>(&(record->o_id)));
	order_record.setColumn(1, reinterpret_cast<const char*>(&(record->o_c_id)));
	order_record.setColumn(2, reinterpret_cast<const char*>(&(record->o_d_id)));
	order_record.setColumn(3, reinterpret_cast<const char*>(&(record->o_w_id)));
	order_record.setColumn(4, reinterpret_cast<const char*>(&(record->o_entry_d)));
	order_record.setColumn(5, reinterpret_cast<const char*>(&(record->o_carrier_id)));
	order_record.setColumn(6, reinterpret_cast<const char*>(&(record->o_ol_cnt)));
	order_record.setColumn(7, reinterpret_cast<const char*>(&(record->o_all_local)));
	size_t offset = kPartition ? scale_params->getOrderLineTableOffset(record->o_w_id, record->o_d_id) % kPartitionCnt : 0;
	storage_manager->insertRecord(ORDER_TABLE_ID + offset, order_record);
	delete[] data;
}

void TpccPopulator::insertNewOrderRecord(const NewOrderRecord* record) {
	Record new_order_record(TpccSchema::getNewOrderSchema());
	char* data = new char[TpccSchema::getNewOrderSchema()->getSchemaSize()];
	new_order_record.transferData(data);
	new_order_record.setColumn(0, reinterpret_cast<const char*>(&(record->o_id)));
	new_order_record.setColumn(1, reinterpret_cast<const char*>(&(record->d_id)));
	new_order_record.setColumn(2, reinterpret_cast<const char*>(&(record->w_id)));
	size_t offset = kPartition ? scale_params->getOrderLineTableOffset(record->w_id, record->d_id) % kPartitionCnt : 0;
	storage_manager->insertRecord(NEW_ORDER_TABLE_ID + offset, new_order_record);
	delete[] data;
}

void TpccPopulator::insertOrderLineRecord(const OrderLineRecord* record) {
	Record order_line_record(TpccSchema::getOrderLineSchema());
	char* data = new char[TpccSchema::getOrderLineSchema()->getSchemaSize()];
	order_line_record.transferData(data);
	order_line_record.setColumn(0, reinterpret_cast<const char*>(&(record->ol_o_id)));
	order_line_record.setColumn(1, reinterpret_cast<const char*>(&(record->ol_d_id)));
	order_line_record.setColumn(2, reinterpret_cast<const char*>(&(record->ol_w_id)));
	order_line_record.setColumn(3, reinterpret_cast<const char*>(&(record->ol_number)));
	order_line_record.setColumn(4, reinterpret_cast<const char*>(&(record->ol_i_id)));
	order_line_record.setColumn(5, reinterpret_cast<const char*>(&(record->ol_supply_w_id)));
	order_line_record.setColumn(6, reinterpret_cast<const char*>(&(record->ol_delivery_d)));
	order_line_record.setColumn(7, reinterpret_cast<const char*>(&(record->ol_quantity)));
	order_line_record.setColumn(8, reinterpret_cast<const char*>(&(record->ol_amount)));
	order_line_record.setColumn(9, record->ol_dist_info, 32);
	size_t offset = kPartition ? scale_params->getOrderLineTableOffset(record->ol_w_id, record->ol_d_id) % kPartitionCnt : 0;
	storage_manager->insertRecord(ORDER_LINE_TABLE_ID + offset, order_line_record);
	delete[] data;
}

void TpccPopulator::insertHistoryRecord(const HistoryRecord* record) {
	Record history_record(TpccSchema::getHistorySchema());
	char* data = new char[TpccSchema::getHistorySchema()->getSchemaSize()];
	history_record.transferData(data);
	history_record.setColumn(0, reinterpret_cast<const char*>(&(record->h_c_id)));
	history_record.setColumn(1, reinterpret_cast<const char*>(&(record->h_c_d_id)));
	history_record.setColumn(2, reinterpret_cast<const char*>(&(record->h_c_w_id)));
	history_record.setColumn(3, reinterpret_cast<const char*>(&(record->h_d_id)));
	history_record.setColumn(4, reinterpret_cast<const char*>(&(record->h_w_id)));
	history_record.setColumn(5, reinterpret_cast<const char*>(&(record->h_date)));
	history_record.setColumn(6, reinterpret_cast<const char*>(&(record->h_amount)));
	history_record.setColumn(7, record->h_data, 32);
	storage_manager->insertRecord(HISTORY_TABLE_ID, history_record);
	delete[] data;
}

void TpccPopulator::insertDistrictNewOrderRecord(const DistrictNewOrderRecord* record) {
	Record district_new_order_record(TpccSchema::getDistrictNewOrderSchema());
	char* data = new char[TpccSchema::getDistrictNewOrderSchema()->getSchemaSize()];
	district_new_order_record.transferData(data);
	district_new_order_record.setColumn(0, reinterpret_cast<const char*>(&(record->w_id)));
	district_new_order_record.setColumn(1, reinterpret_cast<const char*>(&(record->d_id)));
	district_new_order_record.setColumn(2, reinterpret_cast<const char*>(&(record->o_id)));
	storage_manager->insertRecord(DISTRICT_NEW_ORDER_TABLE_ID, district_new_order_record);
	delete[] data;
}