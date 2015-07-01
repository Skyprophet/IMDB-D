#ifndef __TPCC_BASE_PROCESSOR_H__
#define __TPCC_BASE_PROCESSOR_H__

#include <boost/lockfree/spsc_queue.hpp>
#include "TpccSchema.h"
#include "TpccConstants.h"
#include "TpccTuples.h"
#include "RandomGenerator.h"
#include "ScaleParams.h"
#include "../../storage/StorageManager.h"
#include "../../common/Timer.h"

namespace IMDB {
	namespace Benchmark {
		namespace TPCC {
			typedef boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> lockfree_queue_t;
			using namespace IMDB::StorageEngine;

			class TpccBaseProcessor{
			public:
				TpccBaseProcessor(lockfree_queue_t* const queue_ptr, StorageManager* const sm, const ScaleParams* params)
					: queue_ptr(queue_ptr), storage_manager(sm), scale_params(params) {}
				virtual ~TpccBaseProcessor() {}

				virtual void start() = 0;
			protected:
				void deliverySubTxn1(const DeliveryParam* param, std::unordered_map<int, int>& no_o_ids);
				void deliverySubTxn2(const DeliveryParam* param, const std::unordered_map<int, int>& no_o_ids, std::unordered_map<int, double>& sums);
				void deliverySubTxn3(const DeliveryParam* param, const std::unordered_map<int, int>& no_o_ids, std::unordered_map<int, int>& c_ids);
				void deliverySubTxn4(const DeliveryParam* param, const std::unordered_map<int, int>& no_o_ids, const std::unordered_map<int, double>& sums, const std::unordered_map<int, int>& c_ids);

				//vector version
				void deliverySubTxn1Single(const DeliveryParam* param, const int& no_d_id, int& o_id);
				void deliverySubTxn2Single(const DeliveryParam* param, const int& no_d_id, const int& no_o_id, double& sum);
				void deliverySubTxn3Single(const DeliveryParam* param, const int& no_d_id, const int& no_o_id, int& c_id);
				void deliverySubTxn4Single(const DeliveryParam* param, const int& no_d_id, const double& sum, const int& c_id);

				int neworderSubTxn1(const NewOrderParam* param);
				void neworderSubTxn2(const NewOrderParam* param, const int& d_next_o_id);
				void neworderSubTxn3(const NewOrderParam* param, const int& d_next_o_id);
				void neworderSubTxn4(const NewOrderParam* param, std::vector<std::string>& s_dists);
				void neworderSubTxn4Single(const NewOrderParam* param, const size_t& i, std::string& s_dist);
				void neworderSubTxn5(const NewOrderParam* param, std::vector<double>& ol_amounts);
				void neworderSubTxn5Single(const NewOrderParam* param, const size_t& i, double& ol_amount);
				void neworderSubTxn6(const NewOrderParam* param, const int& d_next_o_id, const std::vector<std::string>& s_dists, const std::vector<double>& ol_amounts);

				void paymentSubTxn1(const PaymentParam* param);
				void paymentSubTxn2(const PaymentParam* param);
				void paymentSubTxn3(const PaymentParam* param);
				void paymentSubTxn4(const PaymentParam* param);
			private:
				TpccBaseProcessor(const TpccBaseProcessor&);
				TpccBaseProcessor& operator=(const TpccBaseProcessor&);
			protected:
				lockfree_queue_t* const queue_ptr;
				StorageManager* const storage_manager;
				const ScaleParams* const scale_params;
			};
		}
	}
}

#endif