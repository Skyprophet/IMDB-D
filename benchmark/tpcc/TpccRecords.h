#ifndef __TPCC_RECORDS_H__
#define __TPCC_RECORDS_H__

#include <sstream>
#include <cstdint>

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			struct ItemRecord {
				int i_id;
				int i_im_id;
				char i_name[32];
				double i_price;
				char i_data[64];
			};

			struct WarehouseRecord {
				int w_id;
				char w_name[16];
				char w_street_1[32];
				char w_street_2[32];
				char w_city[32];
				char w_state[2];
				char w_zip[9];
				double w_tax;
				double w_ytd;
			};

			struct DistrictRecord {
				int d_id;
				int d_w_id;
				char d_name[16];
				char d_street_1[32];
				char d_street_2[32];
				char d_city[32];
				char d_state[2];
				char d_zip[9];
				double d_tax;
				double d_ytd;
				int d_next_o_id;
			};

			struct CustomerRecord {
				int c_id;
				int c_d_id;
				int c_w_id;
				char c_first[32];
				char c_middle[2];
				char c_last[32];
				char c_street_1[32];
				char c_street_2[32];
				char c_city[32];
				char c_state[2];
				char c_zip[9];
				char c_phone[32];
				int64_t c_since;
				char c_credit[2];
				double c_credit_lim;
				double c_discount;
				double c_balance;
				double c_ytd_payment;
				int c_payment_cnt;
				int c_delivery_cnt;
				char c_data[500];
			};

			struct StockRecord{
				int s_i_id;
				int s_w_id;
				int s_quantity;
				char s_dists[10][32];
				int s_ytd;
				int s_order_cnt;
				int s_remote_cnt;
				char s_data[64];
			};

			struct OrderRecord{
				int o_id;
				int o_c_id;
				int o_d_id;
				int o_w_id;
				int64_t o_entry_d;
				int o_carrier_id;
				int o_ol_cnt;
				int o_all_local;
			};

			struct NewOrderRecord{
				int o_id;
				int d_id;
				int w_id;
			};

			struct OrderLineRecord{
				int ol_o_id;
				int ol_d_id;
				int ol_w_id;
				int ol_number;
				int ol_i_id;
				int ol_supply_w_id;
				int64_t ol_delivery_d;
				int ol_quantity;
				double ol_amount;
				char ol_dist_info[32];
			};

			struct HistoryRecord{
				int h_c_id;
				int h_c_d_id;
				int h_c_w_id;
				int h_d_id;
				int h_w_id;
				int64_t h_date;
				double h_amount;
				char h_data[32];
			};

			struct DistrictNewOrderRecord {
				int w_id;
				int d_id;
				int o_id;
			};
		}
	}
}

#endif
