#ifndef __TATP_UTIL_H__
#define __TATP_UTIL_H__

#include "../distributions/RandomGenerator.h"
#include <string>
#include <cassert>
#include "TatpConstants.h"

namespace IMDB {
	namespace Benchmark {
		namespace TATP {
			class TatpUtil {
			public:
				static int isActive() {
					return (RandomGenerator::generateInteger(1, 100) < RandomGenerator::generateInteger(86, 100) ? 1 : 0);
				}

				static long getSubscriberId(const long& subscriber_size) {
					return RandomGenerator::generateLong(1, subscriber_size);
				}

				static std::string padWithZero(const long& n) {
					std::string str = std::to_string(n);
					std::string zeros(SUB_NBR_PADDING_SIZE - str.length(), '0');
					return zeros + str;
				}

				/**
				* Returns subarray of arr, with length in range [min_len, max_len];
				* Each element in arr appears at most once in sub array
				*/
				static void subArr(int* arr, int arr_len, const int& min_len, const int& max_len, int* sub, int& sub_len) {
					assert(min_len <= max_len && min_len >= 0);
					sub_len = RandomGenerator::generateInteger(min_len, max_len);
					assert(sub_len <= arr_len);

					sub = new int[sub_len];
					for (int i = 0; i < sub_len; ++i) {
						int j = RandomGenerator::generateInteger(0, arr_len - 1);
						sub[i] = arr[j];
						//Move arr[j] to tail;
						int tmp = arr[j];
						arr[j] = arr[arr_len - 1];
						arr[arr_len - 1] = tmp;
						arr_len--;
					}
				}
			};
		}
	}
}

#endif