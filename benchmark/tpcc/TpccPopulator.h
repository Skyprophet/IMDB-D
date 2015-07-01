#ifndef __TPCC_POPULATOR_H__
#define __TPCC_POPULATOR_H__

#include "../../storage/StorageManager.h"
#include "TpccRecords.h"
#include "RandomGenerator.h"
#include "TpccConstants.h"
#include "ScaleParams.h"
#include "TpccSchema.h"

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			using namespace IMDB::StorageEngine;

			class TpccPopulator {
			public:
				TpccPopulator(const ScaleParams* params, StorageManager* sm)
					: scale_params(params), storage_manager(sm) {}
				~TpccPopulator() {}

				void start() {
					init();
					startPopulate();
				}

			private:
				void init();
				void startPopulate();

				ItemRecord* generateItemRecord(const int& item_id, bool original) const;
				WarehouseRecord* generateWarehouseRecord(const int& w_id) const;
				DistrictRecord* generateDistrictRecord(const int& d_w_id, const int& d_id, const int& d_next_o_id) const;
				CustomerRecord* generateCustomerRecord(const int& c_w_id, const int& c_d_id, const int& c_id, bool bad_credit) const;
				StockRecord* generateStockRecord(const int& s_w_id, const int& s_i_id, bool original) const;
				OrderRecord* generateOrderRecord(const int& o_w_id, const int& o_d_id, const int& o_id, const int& o_c_id, const int& o_ol_cnt, bool new_order) const;
				NewOrderRecord* generateNewOrderRecord(const int& w_id, const int& d_id, const int& o_id) const;
				OrderLineRecord* generateOrderLineRecord(const int& ol_w_id, const int& ol_d_id, const int& ol_o_id, const int& ol_number, const int& max_items, bool new_order) const;
				HistoryRecord* generateHistoryRecord(const int& h_c_w_id, const int& h_c_d_id, const int& h_c_id) const;
				DistrictNewOrderRecord* generateDistrictNewOrderRecord(const int& w_id, const int& d_id, const int& o_id) const;

				void insertItemRecord(const ItemRecord*);
				void insertWarehouseRecord(const WarehouseRecord*);
				void insertDistrictRecord(const DistrictRecord*);
				void insertCustomerRecord(const CustomerRecord*);
				void insertStockRecord(const StockRecord*);
				void insertOrderRecord(const OrderRecord*);
				void insertNewOrderRecord(const NewOrderRecord*);
				void insertOrderLineRecord(const OrderLineRecord*);
				void insertHistoryRecord(const HistoryRecord*);
				void insertDistrictNewOrderRecord(const DistrictNewOrderRecord*);

				TpccPopulator(const TpccPopulator&);
				TpccPopulator& operator=(const TpccPopulator&);

				const ScaleParams* const scale_params;
				StorageManager * const storage_manager;
			};
		}
	}
}

#endif