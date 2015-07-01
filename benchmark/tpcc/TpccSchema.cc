#include "TpccSchema.h"

using namespace IMDB::Benchmark::TPCC;
using namespace IMDB::StorageEngine;

Schema* TpccSchema::item_schema = NULL;
Schema* TpccSchema::warehouse_schema = NULL;
Schema* TpccSchema::district_schema = NULL;
Schema* TpccSchema::customer_schema = NULL;
Schema* TpccSchema::stock_schema = NULL;
Schema* TpccSchema::order_schema = NULL;
Schema* TpccSchema::new_order_schema = NULL;
Schema* TpccSchema::order_line_schema = NULL;
Schema* TpccSchema::history_schema = NULL;
Schema* TpccSchema::district_new_order_schema = NULL;

Schema* TpccSchema::getItemSchema() {
	if (item_schema == NULL) {
		item_schema = new Schema();
		item_schema->insertColumn(ColumnInfo("i_id", ValueType::INT32, true));
		item_schema->insertColumn(ColumnInfo("i_im_id", ValueType::INT32));
		item_schema->insertColumn(ColumnInfo("i_name", ValueType::VARCHAR, static_cast<size_t>(32)));
		item_schema->insertColumn(ColumnInfo("i_price", ValueType::DOUBLE));
		item_schema->insertColumn(ColumnInfo("i_data", ValueType::VARCHAR, static_cast<size_t>(64)));
	}
	return item_schema;
}

Schema* TpccSchema::getWarehouseSchema() {
	if (warehouse_schema == NULL) {
		warehouse_schema = new Schema();
		warehouse_schema->insertColumn(ColumnInfo("w_id", ValueType::INT32, true));
		warehouse_schema->insertColumn(ColumnInfo("w_name", ValueType::VARCHAR, static_cast<size_t>(16)));
		warehouse_schema->insertColumn(ColumnInfo("w_street_1", ValueType::VARCHAR, static_cast<size_t>(32)));
		warehouse_schema->insertColumn(ColumnInfo("w_street_2", ValueType::VARCHAR, static_cast<size_t>(32)));
		warehouse_schema->insertColumn(ColumnInfo("w_city", ValueType::VARCHAR, static_cast<size_t>(32)));
		warehouse_schema->insertColumn(ColumnInfo("w_state", ValueType::VARCHAR, static_cast<size_t>(2)));
		warehouse_schema->insertColumn(ColumnInfo("w_zip", ValueType::VARCHAR, static_cast<size_t>(9)));
		warehouse_schema->insertColumn(ColumnInfo("w_tax", ValueType::DOUBLE));
		warehouse_schema->insertColumn(ColumnInfo("w_ytd", ValueType::DOUBLE));
	}
	return warehouse_schema;
}

Schema* TpccSchema::getDistrictSchema() {
	if (district_schema == NULL) {
		district_schema = new Schema();
		district_schema->insertColumn(ColumnInfo("d_id", ValueType::INT32, true));
		district_schema->insertColumn(ColumnInfo("d_w_id", ValueType::INT32, true));
		district_schema->insertColumn(ColumnInfo("d_name", ValueType::VARCHAR, static_cast<size_t>(16)));
		district_schema->insertColumn(ColumnInfo("d_street_1", ValueType::VARCHAR, static_cast<size_t>(32)));
		district_schema->insertColumn(ColumnInfo("d_street_2", ValueType::VARCHAR, static_cast<size_t>(32)));
		district_schema->insertColumn(ColumnInfo("d_city", ValueType::VARCHAR, static_cast<size_t>(32)));
		district_schema->insertColumn(ColumnInfo("d_state", ValueType::VARCHAR, static_cast<size_t>(2)));
		district_schema->insertColumn(ColumnInfo("d_zip", ValueType::VARCHAR, static_cast<size_t>(9)));
		district_schema->insertColumn(ColumnInfo("d_tax", ValueType::DOUBLE));
		district_schema->insertColumn(ColumnInfo("d_ytd", ValueType::DOUBLE));
		district_schema->insertColumn(ColumnInfo("d_next_o_id", ValueType::INT32));
	}
	return district_schema;
}

Schema* TpccSchema::getCustomerSchema() {
	if (customer_schema == NULL) {
		customer_schema = new Schema();
		customer_schema->insertColumn(ColumnInfo("c_id", ValueType::INT32, true));
		customer_schema->insertColumn(ColumnInfo("c_d_id", ValueType::INT32, true));
		customer_schema->insertColumn(ColumnInfo("c_w_id", ValueType::INT32, true));
		customer_schema->insertColumn(ColumnInfo("c_first", ValueType::VARCHAR, static_cast<size_t>(32)));
		customer_schema->insertColumn(ColumnInfo("c_middle", ValueType::VARCHAR, static_cast<size_t>(2)));
		customer_schema->insertColumn(ColumnInfo("c_last", ValueType::VARCHAR, static_cast<size_t>(32)));
		customer_schema->insertColumn(ColumnInfo("c_street_1", ValueType::VARCHAR, static_cast<size_t>(32)));
		customer_schema->insertColumn(ColumnInfo("c_street_2", ValueType::VARCHAR, static_cast<size_t>(32)));
		customer_schema->insertColumn(ColumnInfo("c_city", ValueType::VARCHAR, static_cast<size_t>(32)));
		customer_schema->insertColumn(ColumnInfo("c_state", ValueType::VARCHAR, static_cast<size_t>(2)));
		customer_schema->insertColumn(ColumnInfo("c_zip", ValueType::VARCHAR, static_cast<size_t>(9)));
		customer_schema->insertColumn(ColumnInfo("c_phone", ValueType::VARCHAR, static_cast<size_t>(32)));
		customer_schema->insertColumn(ColumnInfo("c_since", ValueType::INT64));
		customer_schema->insertColumn(ColumnInfo("c_credit", ValueType::VARCHAR, static_cast<size_t>(2)));
		customer_schema->insertColumn(ColumnInfo("c_credit_lim", ValueType::DOUBLE));
		customer_schema->insertColumn(ColumnInfo("c_discount", ValueType::DOUBLE));
		customer_schema->insertColumn(ColumnInfo("c_balance", ValueType::DOUBLE));
		customer_schema->insertColumn(ColumnInfo("c_ytd_payment", ValueType::DOUBLE));
		customer_schema->insertColumn(ColumnInfo("c_payment_cnt", ValueType::INT32));
		customer_schema->insertColumn(ColumnInfo("c_delivery_cnt", ValueType::INT32));
		customer_schema->insertColumn(ColumnInfo("c_data", ValueType::VARCHAR, static_cast<size_t>(500)));
	}
	return customer_schema;
}

Schema* TpccSchema::getStockSchema() {
	if (stock_schema == NULL) {
		stock_schema = new Schema();
		stock_schema->insertColumn(ColumnInfo("s_i_id", ValueType::INT32, true));
		stock_schema->insertColumn(ColumnInfo("s_w_id", ValueType::INT32, true));
		stock_schema->insertColumn(ColumnInfo("s_quantity", ValueType::INT32));
		for (int i = 0; i < 10; ++i)
			stock_schema->insertColumn(ColumnInfo("s_dists" + std::to_string(i), ValueType::VARCHAR, static_cast<size_t>(32)));
		stock_schema->insertColumn(ColumnInfo("s_ytd", ValueType::INT32));
		stock_schema->insertColumn(ColumnInfo("s_order_cnt", ValueType::INT32));
		stock_schema->insertColumn(ColumnInfo("s_remote_cnt", ValueType::INT32));
		stock_schema->insertColumn(ColumnInfo("s_data", ValueType::VARCHAR, static_cast<size_t>(64)));
	}
	return stock_schema;
}

Schema* TpccSchema::getOrderSchema() {
	if (order_schema == NULL) {
		order_schema = new Schema();
		order_schema->insertColumn(ColumnInfo("o_id", ValueType::INT32, true));
		order_schema->insertColumn(ColumnInfo("o_c_id", ValueType::INT32));
		order_schema->insertColumn(ColumnInfo("o_d_id", ValueType::INT32, true));
		order_schema->insertColumn(ColumnInfo("o_w_id", ValueType::INT32, true));
		order_schema->insertColumn(ColumnInfo("o_entry_d", ValueType::INT64));
		order_schema->insertColumn(ColumnInfo("o_carrier_id", ValueType::INT32));
		order_schema->insertColumn(ColumnInfo("o_ol_cnt", ValueType::INT32));
		order_schema->insertColumn(ColumnInfo("o_all_local", ValueType::INT32));
	}
	return order_schema;
}

Schema* TpccSchema::getNewOrderSchema() {
	if (new_order_schema == NULL) {
		new_order_schema = new Schema();
		new_order_schema->insertColumn(ColumnInfo("o_id", ValueType::INT32, true));
		new_order_schema->insertColumn(ColumnInfo("d_id", ValueType::INT32, true));
		new_order_schema->insertColumn(ColumnInfo("w_id", ValueType::INT32, true));
	}
	return new_order_schema;
}

Schema* TpccSchema::getOrderLineSchema() {
	if (order_line_schema == NULL) {
		order_line_schema = new Schema();
		order_line_schema->insertColumn(ColumnInfo("ol_o_id", ValueType::INT32, true));
		order_line_schema->insertColumn(ColumnInfo("ol_d_id", ValueType::INT32, true));
		order_line_schema->insertColumn(ColumnInfo("ol_w_id", ValueType::INT32, true));
		order_line_schema->insertColumn(ColumnInfo("ol_number", ValueType::INT32, true));
		order_line_schema->insertColumn(ColumnInfo("ol_i_id", ValueType::INT32, kInt32Size));
		order_line_schema->insertColumn(ColumnInfo("ol_supply_w_id", ValueType::INT32, kInt32Size));
		order_line_schema->insertColumn(ColumnInfo("ol_delivery_d", ValueType::INT64, kInt64Size));
		order_line_schema->insertColumn(ColumnInfo("ol_quantity", ValueType::INT32, kInt32Size));
		order_line_schema->insertColumn(ColumnInfo("ol_amount", ValueType::DOUBLE, kDoubleSize));
		order_line_schema->insertColumn(ColumnInfo("ol_dist_info", ValueType::VARCHAR, static_cast<size_t>(32)));
	}

	return order_line_schema;
}

Schema* TpccSchema::getHistorySchema() {
	if (history_schema == NULL) {
		history_schema = new Schema();
		history_schema->insertColumn(ColumnInfo("h_c_id", ValueType::INT32));
		history_schema->insertColumn(ColumnInfo("h_c_d_id", ValueType::INT32));
		history_schema->insertColumn(ColumnInfo("h_c_w_id", ValueType::INT32));
		history_schema->insertColumn(ColumnInfo("h_d_id", ValueType::INT32));
		history_schema->insertColumn(ColumnInfo("h_w_id", ValueType::INT32));
		history_schema->insertColumn(ColumnInfo("h_date", ValueType::INT64));
		history_schema->insertColumn(ColumnInfo("h_amount", ValueType::DOUBLE));
		history_schema->insertColumn(ColumnInfo("h_data", ValueType::VARCHAR, static_cast<size_t>(32)));
	}
	return history_schema;
}

Schema* TpccSchema::getDistrictNewOrderSchema() {
	if (district_new_order_schema == NULL) {
		district_new_order_schema = new Schema();
		district_new_order_schema->insertColumn(ColumnInfo("w_id", ValueType::INT32, true));
		district_new_order_schema->insertColumn(ColumnInfo("d_id", ValueType::INT32, true));
		district_new_order_schema->insertColumn(ColumnInfo("o_id", ValueType::INT32));
	}
	return district_new_order_schema;
}
