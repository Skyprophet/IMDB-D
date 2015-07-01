#ifndef __TPCC_SCALE_PARAMS_H__
#define __TPCC_SCALE_PARAMS_H__

#include "TpccConstants.h"

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			struct ScaleParams {
				ScaleParams(const int& num_warehouses, const double& scalefactor) {
					this->num_warehouses = num_warehouses;
					this->scalefactor = scalefactor;

					starting_warehouse = 1;
					ending_warehouse = starting_warehouse + num_warehouses - 1;

					num_items = (int)(NUM_ITEMS / scalefactor);
					num_districts_per_warehouse = DISTRICTS_PER_WAREHOUSE;
					num_customers_per_district = (int)(CUSTOMERS_PER_DISTRICT / scalefactor);
					num_new_orders_per_district = (int)(INITIAL_NEW_ORDERS_PER_DISTRICT / scalefactor);
				}

				size_t getOrderLineTableOffset(const int& w_id, const int& d_id) const {
					return ((w_id - 1) * num_districts_per_warehouse + d_id - 1);
				}
				
				int num_warehouses;
				double scalefactor;
				int starting_warehouse;
				int ending_warehouse;
				int num_items;
				int num_districts_per_warehouse;
				int num_customers_per_district;
				int num_new_orders_per_district;
			};
		}
	}
}

#endif
