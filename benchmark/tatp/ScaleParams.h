#ifndef __TATP_SCALE_PARAMS_H__
#define __TATP_SCALE_PARAMS_H__

#include <cstring>
#include <cassert>
#include "TatpConstants.h"

namespace IMDB {
	namespace Benchmark {
		namespace TATP {
			struct ScaleParams {
				ScaleParams() {
					memset(workload_weight, 0, sizeof(workload_weight));
					scale_factor = 1;
					subscriber_size = DEFAULT_NUM_SUBSCRIBERS * scale_factor;
					workload_weight[0]  = 2;
					workload_weight[1] = workload_weight[0] + 40;
					workload_weight[2] = workload_weight[1] + 0;
					workload_weight[3] = workload_weight[2] + 40;
					workload_weight[4] = workload_weight[3] + 2;
					workload_weight[5] = workload_weight[4] + 14;
					workload_weight[6] = workload_weight[5] + 2;
				}

				ScaleParams(const double& scale_factor, const size_t* weight) : scale_factor(scale_factor) {
					size_t sum = 0;
					for (size_t i = 0; i < 7; ++i) {
						sum += weight[i];
						workload_weight[i] = sum;
					}
					subscriber_size = DEFAULT_NUM_SUBSCRIBERS * scale_factor;
					assert(sum == 100);
				}

				double scale_factor;
				//0 : DELETE_CALL_FORWARDING	1 : GET_ACCESS_DATA
				//2 : GET_NEW_DESTINATION		3 : GET_SUBSCRIBER_DATA
				//4 : INSERT_CALL_FORWARDING	5 : UPDATE_LOCATION
				//6 : UPDATE_SUBSCRIBER_DATA
				size_t workload_weight[7];
				int subscriber_size;
			};
		}
	}
}

#endif