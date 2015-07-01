#include "TpccRecords.h"

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			std::string itemToString(const ItemRecord& item) {
				std::stringstream buf;
				buf << item.i_id << ";" << item.i_im_id << ";";
				buf << item.i_name << ";" << item.i_price << ";";
				buf << item.i_data;
				return buf.str();
			}

			std::string warehouseToString(const WarehouseRecord& warehouse) {
				std::stringstream buf;
				buf << warehouse.w_id << ";" << warehouse.w_name << ";";
				buf << warehouse.w_street_1 << ";" << warehouse.w_street_2 << ";";
				buf << warehouse.w_city << ";" <<warehouse. w_state << ";";
				buf << warehouse.w_zip << ";" << warehouse.w_tax << ";";
				buf << warehouse.w_ytd;
				return buf.str();
			}

			std::string districtToString(const DistrictRecord& district) {
				std::stringstream buf;
				buf << district.d_id << ";" << district.d_w_id << ";";
				buf << district.d_name << ";" << district.d_street_1 << ";";
				buf << district.d_street_2 << ";" << district.d_city << ";";
				buf << district.d_state << ";" << district.d_zip << ";";
				buf << district.d_tax << ";" << district.d_ytd << ";";
				buf << district.d_next_o_id;
				return buf.str();
			}

			std::string customerToString(const CustomerRecord& customer){
				std::stringstream buf;
				buf << customer.c_id << ";" << customer.c_d_id << ";";
				buf << customer.c_w_id << ";" << customer.c_first << ";";
				buf << customer.c_middle << ";" << customer.c_last << ";";
				buf << customer.c_street_1 << ";" << customer.c_street_2 << ";";
				buf << customer.c_city << ";" << customer.c_state << ";";
				buf << customer.c_zip << ";" << customer.c_phone << ";";
				buf << customer.c_since << ";" << customer.c_credit << ";";
				buf << customer.c_credit_lim << ";" << customer.c_discount << ";";
				buf << customer.c_balance << ";" << customer.c_ytd_payment << ";";
				buf << customer.c_payment_cnt << ";" << customer.c_delivery_cnt << ";";
				buf << customer.c_data;
				return buf.str();
			}

			std::string stockToString(const StockRecord& stock){
				std::stringstream buf;
				buf << stock.s_i_id << ";" << stock.s_w_id << ";" << stock.s_quantity << ";";
				for (int i = 0; i < 10; ++i){
					buf << stock.s_dists[i] << ";";
				}
				buf << stock.s_ytd << ";" << stock.s_order_cnt << ";";
				buf << stock.s_remote_cnt << ";" << stock.s_data;
				return buf.str();
			}

			std::string orderToString(const OrderRecord& order){
				std::stringstream buf;
				buf << order.o_id << ";" << order.o_c_id << ";";
				buf << order.o_d_id << ";" << order.o_w_id << ";";
				buf << order.o_entry_d << ";" << order.o_carrier_id << ";";
				buf << order.o_ol_cnt << ";" << order.o_all_local;
				return buf.str();
			}

			std::string newOrderToString(const NewOrderRecord& new_order){
				std::stringstream buf;
				buf << new_order.o_id << ";" << new_order.d_id << ";";
				buf << new_order.w_id;
				return buf.str();
			}

			std::string orderLineToString(const OrderLineRecord& order_line){
				std::stringstream buf;
				buf << order_line.ol_o_id << ";" << order_line.ol_d_id << ";";
				buf << order_line.ol_w_id << ";" << order_line.ol_number << ";";
				buf << order_line.ol_i_id << ";" << order_line.ol_supply_w_id << ";";
				buf << order_line.ol_delivery_d << ";" << order_line.ol_quantity << ";";
				buf << order_line.ol_amount << ";" << order_line.ol_dist_info;
				return buf.str();
			}

			std::string historyToString(const HistoryRecord& history){
				std::stringstream buf;
				buf << history.h_c_id << ";" << history.h_c_d_id << ";";
				buf << history.h_c_w_id << ";" << history.h_d_id << ";";
				buf << history.h_w_id << ";" << history.h_date << ";";
				buf << history.h_amount << ";" << history.h_data;
				return buf.str();
			}

			std::string districtNewOrderToString(const DistrictNewOrderRecord& district_new_order) {
				std::stringstream buf;
				buf << district_new_order.w_id << ";";
				buf << district_new_order.d_id << ";";
				buf << district_new_order.o_id;
				return buf.str();
			}
		}
	}
}
