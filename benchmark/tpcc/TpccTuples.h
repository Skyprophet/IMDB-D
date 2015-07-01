#ifndef __TPCC_TUPLES_H__
#define __TPCC_TUPLES_H__

#include "../BenchmarkTuple.h"
#include "TpccRecords.h"
#include "TpccConstants.h"

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			TUPLE_CLASS(Item)
			TUPLE_CLASS(Warehouse)
			TUPLE_CLASS(District)
			TUPLE_CLASS(Customer)
			TUPLE_CLASS(Stock)
			TUPLE_CLASS(Order)
			TUPLE_CLASS(NewOrder)
			TUPLE_CLASS(OrderLine)
			TUPLE_CLASS(History)
			TUPLE_CLASS(DistrictNewOrder)


			class DeliveryParam : public EventTuple {
			public:
				DeliveryParam() : EventTuple(TupleType::DELIVERY) {}
				virtual ~DeliveryParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int w_id;
				int o_carrier_id;
				int64_t ol_delivery_d;
			};

			class NewOrderParam : public EventTuple {
			public:
				NewOrderParam() : EventTuple(TupleType::NEW_ORDER) {}
				virtual ~NewOrderParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int w_id;
				int d_id;
				int c_id;
				int64_t o_entry_d;
				std::vector<int> i_ids;
				std::vector<int> i_w_ids;
				std::vector<int> i_qtys;
			};

			class PaymentParam : public EventTuple {
			public:
				PaymentParam() : EventTuple(TupleType::PAYMENT) {}
				virtual ~PaymentParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int w_id;
				int d_id;
				double h_amount;
				int c_w_id;
				int c_d_id;
				int c_id;
				int64_t h_date;
			};

			class OrderStatusParam : public EventTuple {
			public:
				OrderStatusParam() {}
				virtual ~OrderStatusParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int w_id;
				int d_id;
				std::string c_last;
				int c_id;
			};

			class StockLevelParam : public EventTuple {
			public:
				StockLevelParam() {}
				virtual ~StockLevelParam() {}

				virtual int64_t getHashCode() const {
					return -1;
				}

				virtual void serialize(CharArray& serial_str) const {}
				virtual void deserialize(const CharArray& serial_str) {}
			public:
				int w_id;
				int d_id;
			};
		}
	}
}

#endif