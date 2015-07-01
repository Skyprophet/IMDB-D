#ifndef __TPCC_SCHEMA_H__
#define __TPCC_SCHEMA_H__

#include "../../storage/Schema.h"
#include <string>

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			using namespace IMDB::StorageEngine;
			class TpccSchema {
			public:
				static Schema* getItemSchema();
				static Schema* getWarehouseSchema();
				static Schema* getDistrictSchema();
				static Schema* getCustomerSchema();
				static Schema* getStockSchema();
				static Schema* getOrderSchema();
				static Schema* getNewOrderSchema();
				static Schema* getOrderLineSchema();
				static Schema* getHistorySchema();			
				static Schema* getDistrictNewOrderSchema();
			private:
				TpccSchema();
				TpccSchema(const TpccSchema&);
				TpccSchema& operator=(const TpccSchema&);

				static Schema* item_schema;
				static Schema* warehouse_schema;
				static Schema* district_schema;
				static Schema* customer_schema;
				static Schema* stock_schema;
				static Schema* order_schema;
				static Schema* new_order_schema;
				static Schema* order_line_schema;
				static Schema* history_schema;
				static Schema* district_new_order_schema;
			};
		}
	}
}

#endif
