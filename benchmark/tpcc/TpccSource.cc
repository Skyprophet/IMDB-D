#include "TpccSource.h"

using namespace IMDB::Benchmark::TPCC;

void TpccSource::startExecution() {
	for (size_t i = 0; i < num_transactions; ++i) {
		int x = RandomGenerator::generateInteger(1, 100);
		if (x <= 0) {
			StockLevelParam* param = generateStockLevelParam();
			while (!queue_ptr->push(std::make_pair(TupleType::STOCK_LEVEL, param)));
		} else if (x <= 4 + 0) {
			DeliveryParam* param = generateDeliveryParam();
			while (!queue_ptr->push(std::make_pair(TupleType::DELIVERY, param)));
		} else if (x <= 4 + 0 + 0) {
			OrderStatusParam* param = generateOrderStatusParam();
			while (!queue_ptr->push(std::make_pair(TupleType::ORDER_STATUS, param)));
		} else if (x <= 43 + 4 + 0 + 0) {
			PaymentParam* param = generatePaymentParam();
			while (!queue_ptr->push(std::make_pair(TupleType::PAYMENT, param)));
		} else  {
			NewOrderParam* param = generateNewOrderParam();
			while (!queue_ptr->push(std::make_pair(TupleType::NEW_ORDER, param)));
		}
	}
}

DeliveryParam* TpccSource::generateDeliveryParam() const {
	DeliveryParam* param = new DeliveryParam();
	param->w_id = RandomGenerator::generateWarehouseId(scale_params->starting_warehouse, scale_params->ending_warehouse);
	param->o_carrier_id = RandomGenerator::generateInteger(MIN_CARRIER_ID, MAX_CARRIER_ID);
	param->ol_delivery_d = RandomGenerator::generateCurrentTime();
	return param;
}

NewOrderParam* TpccSource::generateNewOrderParam() const {
	NewOrderParam* param = new NewOrderParam();
	param->w_id = RandomGenerator::generateWarehouseId(scale_params->starting_warehouse, scale_params->ending_warehouse);
	param->d_id = RandomGenerator::generateDistrictId(scale_params->num_districts_per_warehouse);
	param->c_id = RandomGenerator::generateCustomerId(scale_params->num_customers_per_district);
	int ol_cnt = RandomGenerator::generateInteger(MIN_OL_CNT, MAX_OL_CNT);
	param->o_entry_d = RandomGenerator::generateCurrentTime();
	//bool roolback = (RandomGenerator::generateInteger(1, 100) == 1);
	for (int i = 0; i < ol_cnt; ++i) {
		param->i_ids.push_back(RandomGenerator::generateItemId(scale_params->num_items));
		bool remote = (RandomGenerator::generateInteger(1, 100) == 1);
		if (scale_params->num_warehouses > 1 &&  remote) {
			param->i_w_ids.push_back(RandomGenerator::generateIntegerExcluding(scale_params->starting_warehouse, scale_params->ending_warehouse, param->w_id));
		} else param->i_w_ids.push_back(param->w_id);
		param->i_qtys.push_back(RandomGenerator::generateInteger(1, MAX_OL_QUANTITY));
	}
	return param;
}

PaymentParam* TpccSource::generatePaymentParam() const {
	PaymentParam* param = new PaymentParam();
	int x = RandomGenerator::generateInteger(1, 100);

	param->w_id = RandomGenerator::generateWarehouseId(scale_params->starting_warehouse, scale_params->ending_warehouse);
	param->d_id = RandomGenerator::generateDistrictId(scale_params->num_districts_per_warehouse);
	param->c_w_id = -1;
	param->c_d_id = -1;
	//param->c_last = "";
	param->h_amount = RandomGenerator::generateFixedPoint(2, MIN_PAYMENT, MAX_PAYMENT);
	param->h_date = RandomGenerator::generateCurrentTime();

	if (scale_params->num_warehouses == 1 || x <= 85) {
		param->c_w_id = param->w_id;
		param->c_d_id = param->d_id;
	} else {
		param->c_w_id = RandomGenerator::generateIntegerExcluding(scale_params->starting_warehouse, scale_params->ending_warehouse, param->w_id);
		param->c_d_id = RandomGenerator::generateDistrictId(scale_params->num_districts_per_warehouse);
	}
	param->c_id = RandomGenerator::generateCustomerId(scale_params->num_customers_per_district);
	return param;
}

OrderStatusParam* TpccSource::generateOrderStatusParam() const {
	OrderStatusParam* param = new OrderStatusParam();
	param->w_id = RandomGenerator::generateWarehouseId(scale_params->starting_warehouse, scale_params->ending_warehouse);
	param->d_id = RandomGenerator::generateDistrictId(scale_params->num_districts_per_warehouse);
	param->c_last = "";
	param->c_id = RandomGenerator::generateCustomerId(scale_params->num_customers_per_district);
	return param;
}

StockLevelParam* TpccSource::generateStockLevelParam() const {
	StockLevelParam* param = new StockLevelParam();
	param->w_id = RandomGenerator::generateWarehouseId(scale_params->starting_warehouse, scale_params->ending_warehouse);
	param->d_id = RandomGenerator::generateDistrictId(scale_params->num_districts_per_warehouse);
	return param;
}
