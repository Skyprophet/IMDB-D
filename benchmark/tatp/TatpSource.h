#ifndef __TATP_SOURCE_H__
#define __TATP_SOURCE_H__

#include <boost/lockfree/spsc_queue.hpp>
#include <string>
#include <climits>

#include "TatpConstants.h"
#include "TatpUtil.h"
#include "ScaleParams.h"
#include "../distributions/RandomGenerator.h"
#include "TatpTuples.h"

namespace IMDB{
	namespace Benchmark {
		namespace TATP {
			typedef boost::lockfree::spsc_queue<std::pair<size_t, EventTuple*>> lockfree_queue_t;
			using namespace IMDB::Benchmark;

			class TatpSource {
			public:
				TatpSource(lockfree_queue_t* const queue_ptr, ScaleParams* params, size_t num_transactions)
				: queue_ptr(queue_ptr), scale_params(params), num_transactions(num_transactions) {}
				~TatpSource() {}

				void start() {
					startExecution();
					while (!queue_ptr->push(std::make_pair(ENDEXECUTION, static_cast<EventTuple*>(NULL))));
				}

			private:
				void startExecution() {
					for (size_t i = 0; i < num_transactions; ++i) {
						size_t x = RandomGenerator::generateInteger(1, 100);
						int s_id = TatpUtil::getSubscriberId(scale_params->subscriber_size);
						if (x <= scale_params->workload_weight[0]) {
							DeleteCallForwardingParam* param = new DeleteCallForwardingParam();
							param->sub_nbr = TatpUtil::padWithZero(s_id);
							param->sf_type = (int8_t)(RandomGenerator::generateInteger(1, 4));
							param->start_time = (int8_t)(RandomGenerator::generateInteger(0, 2) * 8);
							while (!queue_ptr->push(std::make_pair(TupleType::DELETE_CALL_FORWARDING, param)));
						} else if (x <= scale_params->workload_weight[1]) {
							GetAccessDataParam* param = new GetAccessDataParam();
							param->s_id = s_id;
							param->ai_type = (int8_t)(RandomGenerator::generateInteger(1, 4));
							while (!queue_ptr->push(std::make_pair(TupleType::GET_ACCESS_DATA, param)));
						} else if (x <= scale_params->workload_weight[2]) {
							GetNewDestinationParam* param = new GetNewDestinationParam();
							param->s_id = s_id;
							param->sf_type = (int8_t)(RandomGenerator::generateInteger(1, 4));
							param->start_time = (int8_t)(RandomGenerator::generateInteger(0, 2) * 8);
							param->end_time = (int8_t)(RandomGenerator::generateInteger(1, 24));
							while (!queue_ptr->push(std::make_pair(TupleType::GET_NEW_DESTINATION, param)));
						} else if (x <= scale_params->workload_weight[3]) {
							GetSubscriberDataParam* param = new GetSubscriberDataParam();
							param->s_id = s_id;
							while (!queue_ptr->push(std::make_pair(TupleType::GET_SUBSCRIBER_DATA, param)));
						} else if (x <= scale_params->workload_weight[4]) {
							InsertCallForwardingParam* param = new InsertCallForwardingParam();
							param->sub_nbr = TatpUtil::padWithZero(s_id);
							param->sf_type = (int8_t)(RandomGenerator::generateInteger(1, 4));
							param->start_time = (int8_t)(RandomGenerator::generateInteger(0, 2) * 8);
							param->end_time = (int8_t)(RandomGenerator::generateInteger(1, 24));
							param->numberx = TatpUtil::padWithZero(s_id);
							while (!queue_ptr->push(std::make_pair(TupleType::INSERT_CALL_FORWARDING, param)));
						} else if (x <= scale_params->workload_weight[5]) {
							UpdateLocationParam* param = new UpdateLocationParam();
							param->vlr_location = RandomGenerator::generateInteger(0, INT_MAX);
							param->sub_nbr = TatpUtil::padWithZero(s_id);
							while (!queue_ptr->push(std::make_pair(TupleType::UPDATE_LOCATION, param)));
						} else if (x <= scale_params->workload_weight[6]) {
							UpdateSubscriberDataParam* param = new UpdateSubscriberDataParam();
							param->s_id = s_id;
							param->bit_1 = (int8_t)(RandomGenerator::generateInteger(0, 1));
							param->data_a = (int16_t)(RandomGenerator::generateInteger(0, 255));
							param->sf_type = (int8_t)(RandomGenerator::generateInteger(1, 4));
							while (!queue_ptr->push(std::make_pair(TupleType::UPDATE_SUBSCRIBER_DATA, param)));
						}
					}
				}

				TatpSource(const TatpSource&);
				TatpSource& operator=(const TatpSource&);

				lockfree_queue_t* const queue_ptr;
				const ScaleParams* const scale_params;
				const size_t num_transactions;
			};
		}
	}
}

#endif